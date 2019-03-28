#pragma comment(lib, "SOIL_ext.lib")

#include "IO.h"

#include "Texture.h"
#include "SOIL/SOIL.h"
#include "ResourceManager.h"

float Texture::anisotropyAmount = 32.0f;

GLenum filterModes[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};


Texture::Texture(const std::string & file, GLenum wrapX, GLenum wrapY, GLenum filterMag, GLenum filterMin) : Resource(file)
{
	this->load(file, wrapX, wrapY, filterMag, filterMin);
}

Texture::~Texture()
{
	this->unload();
}

Texture * Texture::add(const std::string & file, GLenum wrapX, GLenum wrapY, GLenum filterMag, GLenum filterMin)
{
	return ResourceManager::addTexture(file, wrapX, wrapY, filterMag, filterMin);
}

bool Texture::load(const std::string & file, GLenum wrapX, GLenum wrapY, GLenum filterMag, GLenum filterMin)
{
	SetID(file);
	this->filename = "../assets/textures/" + file;

	unsigned char* textureData;
	textureData = SOIL_load_image((this->filename).c_str(), 
		&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (textureData == 0 || this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		SAT_DEBUG_LOG_ERROR("TEXTURE BROKE: %s", this->filename.c_str());
		return false;
	}
	
	// If the texture is 2D, set it to be a 2D texture;
	this->_Target = GL_TEXTURE_2D;
	this->_InternalFormat = GL_RGBA8;

	glGenTextures(1, &this->_TexHandle);
	this->bind();
	glTextureStorage2D(this->_TexHandle, 1, this->_InternalFormat, this->sizeX, this->sizeY);
	
	glTextureSubImage2D(this->_TexHandle, 0, // We are editing the first and only layer in memory
		0, 0, // No offset
		this->sizeX, this->sizeY, // the dimensions of our image loaded
		GL_RGBA, GL_UNSIGNED_BYTE, // Data format and type
		textureData); // Pointer to the texture data

	this->_Filter.min = filterMin;
	this->_Filter.mag = filterMag;
	this->_WrapX = wrapX;
	this->_WrapY = wrapY;

	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_S, this->_WrapX);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_T, this->_WrapY);
	glTextureParameterf(this->_TexHandle, GL_TEXTURE_MAX_ANISOTROPY_EXT, Texture::anisotropyAmount);

	this->unbind();
	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::load3D(const std::vector<std::string>& files)
{
	std::vector<unsigned char*> textureDatas;

	for (unsigned int i = 0; i < files.size(); ++i)
	{
		std::string file = "../assets/textures/" + files[i];

		int sizeXOld = sizeX;
		int sizeYOld = sizeY;
		textureDatas.push_back(SOIL_load_image(file.c_str(), &this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA));

		if (i == 0)
		{
			sizeXOld = sizeX;
			sizeYOld = sizeY;
		}

		if (textureDatas.back() == 0 || this->sizeX == 0 || this->sizeY == 0 || this->channels == 0
			|| sizeXOld != sizeX || sizeYOld != sizeY)
		{
			SAT_DEBUG_LOG_ERROR("3D TEXTURE BROKE: %s", file.c_str());
		}
	}
	   	
	// set it to be a 3D texture;
	this->_Target = GL_TEXTURE_3D;
	this->_InternalFormat = GL_RGBA8;

	glGenTextures(1, &this->_TexHandle);
	this->bind();
	glTextureStorage3D(this->_TexHandle, 1, this->_InternalFormat, this->sizeX, this->sizeY, textureDatas.size());

	for (unsigned int i = 0; i < textureDatas.size(); ++i)
	{
		glTextureSubImage3D(this->_TexHandle, 0, // We are editing the first and only layer in memory
			0, 0, i, // No offset
			this->sizeX, this->sizeY, 1,  // the dimensions of our image loaded
			GL_RGBA, GL_UNSIGNED_BYTE, // Data format and type
			textureDatas[i]); // Pointer to the texture data
	}

	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MIN_FILTER, this->_Filter.min);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MAG_FILTER, this->_Filter.mag);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_S, this->_WrapX);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_T, this->_WrapY);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_R, this->_WrapZ);
	this->unbind();

	for (unsigned int i = 0; i < textureDatas.size(); ++i)
	{
		SOIL_free_image_data(textureDatas[i]);
		textureDatas[i] = nullptr;
	}
	return true;
}

bool Texture::unload()
{
	if (this->_TexHandle != 0)
	{
		glDeleteTextures(1, &this->_TexHandle);
		return true;
	}
	return false;
}

int Texture::countMipMapLevels(bool mipmap)
{
	int levels = 1;

	if (mipmap)
	{
		float largest = fmaxf(static_cast<float>(this->sizeX), static_cast<float>(this->sizeY));
		levels += static_cast<int>(std::floor(std::log2(largest)));
	}
	return levels;
}

void Texture::generateMipMaps()
{
	glGenerateTextureMipmap(this->_TexHandle);
	glTextureParameterf(this->_TexHandle, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyAmount);
}

void Texture::createTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void * newDataPtr)
{
	sizeX = w;
	sizeY = h;
	_Filter.mag = filtering;
	_WrapX = edgeBehaviour;
	_WrapY = edgeBehaviour;
	_InternalFormat = internalFormat;
	_Target = target;

	GLenum error = 0;

	// Not necessary to enable GL_TEXTURE_* in modern context.
//	glEnable(m_pTarget);
//	error = glGetError();

	unload();

	glGenTextures(1, &_TexHandle);
	glBindTexture(target, _TexHandle);
	error = glGetError();

	glTexParameteri(_Target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(_Target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_S, edgeBehaviour);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_T, edgeBehaviour);
	error = glGetError();

	glTexImage2D(_Target, 0, internalFormat, w, h, 0, textureFormat, dataType, newDataPtr);
	error = glGetError();

	if (error != 0)
	{
		SAT_DEBUG_LOG_ERROR("[Texture.cpp : createTexture] Error when creating texture. ");
	}

	glBindTexture(_Target, 0);
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_TexHandle);
}

void Texture::bind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->bind();
}

void Texture::unbind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unbind(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	this->unbind();
}

GLuint Texture::getID()
{
	return _TexHandle;
}