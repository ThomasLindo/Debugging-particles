#pragma once
#include "GL/glew.h"
#include <string>
#include <vector>
#include "Resource.h"

/*
  //////////////
 // Textures // 2019 Stephen Thompson
//////////////

Textures are used to hold one or more images of the same format,
they can either be used for reading from inside a Shader, of used
as a render target for drawing (Done with the Framebuffer class)

Common types of textures are
GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, and GL_TEXTURE_CUBE_MAP

Mipmaps are pre-shrunk versions of a full-sized image, and help prevent aliasing
with far away/zoomed out textures. OpenGL will automatically select which mipmap
layer to use, and prevent the need to take multiple texture samples per fragment.

You can read more about Textures here
https://www.khronos.org/opengl/wiki/Texture
There are many parameters for Texture sampling, such as Filtering and Edge Wrapping
These parameters can even be shared across multiple textures using Sampler Objects
https://www.khronos.org/opengl/wiki/Sampler_Object

Limitations of Textures can be found using glGetIntegerv(GL_MACRO, &integer)
	-Textures have a limit on their resolution for any dimension
		GL_MAX_TEXTURE_SIZE
		GL_MAX_3D_TEXTURE_SIZE
		GL_MAX_ARRAY_TEXTURE_LAYERS

	-Texture sizes are advised to stick to powers of 2
		16, 32, 64, 128, 256, 512, 1024, etc.

*/

class TextureBind;

class Texture : public Resource
{
public:
	enum wrapType
	{
		Clamp = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT,
		MirrorRepeat = GL_MIRRORED_REPEAT,
		MirrorClamp = GL_MIRROR_CLAMP_TO_EDGE
	};
	enum filterType
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		LinearMipLinear = GL_LINEAR_MIPMAP_LINEAR
	};
	struct TextureFilter
	{
		GLenum mag = Linear;
		GLenum min = LinearMipLinear;
	};

	Texture() = default;
	Texture(const std::string &file, GLenum wrapX = Repeat, GLenum wrapY = Repeat, GLenum filterMag = Linear, GLenum filterMin = LinearMipLinear);
	~Texture();

	static Texture* add(const std::string &file, GLenum wrapX = Repeat, GLenum wrapY = Repeat, GLenum filterMag = Linear, GLenum filterMin = LinearMipLinear);

	// Loads the texture
	//bool load(const std::string &file);
	bool load(const std::string &file, GLenum wrapX = Repeat, GLenum wrapY = Repeat, GLenum filterMag = Linear, GLenum filterMin = LinearMipLinear);
	bool load3D(const std::vector<std::string> &files);
	// Unloads the texture
	bool unload();

	// Description:
	// Creates the texture, allocates memory and uploads data to GPU
	// If you do not want to upload data to the GPU pass in a nullptr for the dataPtr.
	// For a description on filtering and edgeBehaviour see https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	// For a description on internalFormat, textureFormat and dataType see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
	void createTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void* newDataPtr);


	// Binds the texture to the last active slot
	void bind() const;
	// Binds the texture to a specific slot
	void bind(int textureSlot) const;
	// Unbinds the last active texture slot
	void unbind() const;
	// Unbinds a specific texture slot
	void unbind(int textureSlot) const;
	GLuint getID();

protected:
	static float anisotropyAmount;

	int countMipMapLevels(bool mipmap);
	void generateMipMaps();

	GLuint _TexHandle;

	std::string filename;
	std::string filetype;

	GLenum _InternalFormat;
	GLenum _Target;
	TextureFilter _Filter;
	GLenum _WrapX = Repeat; // Per axis Wrap parameter
	GLenum _WrapY = Repeat;
	GLenum _WrapZ = Clamp;

	int sizeX;
	int sizeY;
	int channels;
};

class TextureBind
{
public:
	TextureBind(Texture* tex, int slot) { texture = tex; bindSlot = slot; }
	Texture* texture;
	int bindSlot;
};