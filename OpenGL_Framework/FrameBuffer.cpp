#include "FrameBuffer.h"
#include "IO.h"
#include <iostream>

#define GL_DONT_GIVE_A_FUCK GL_DONT_CARE

#define BUFFER_OFFSET(i) ((char *)0 + (i))


int Window::width = 1280;
int Window::height = 720;
vec2 Window::size = vec2(static_cast<float>(Window::width), static_cast<float>(Window::height));
float Window::aspectRatio = size.x / size.y;


GLuint FrameBuffer::FullScreenQuadVAO = GL_NONE;
GLuint FrameBuffer::FullScreenQuadVBO = GL_NONE;

FrameBuffer::FrameBuffer(unsigned numColorAttachments)
{
	_NumColorAttachments = numColorAttachments;

	glGenFramebuffers(1, &_FBO);
	//glGenFramebuffersEXT(1, &_FBO);

	_ColorAttachments = new GLuint[_NumColorAttachments];
	//_Bufs is required as parameter for glDrawBuffers().
	_Bufs = new GLenum[_NumColorAttachments];
	_ColorWidth = new unsigned[_NumColorAttachments];
	_ColorHeight = new unsigned[_NumColorAttachments];
	_ColorFormat = new GLint[_NumColorAttachments];
	_ColorFilter = new GLint[_NumColorAttachments];
	_ColorWrap = new GLint[_NumColorAttachments];

	for (unsigned int i = 0; i < _NumColorAttachments; i++)
	{
		_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

FrameBuffer::~FrameBuffer()
{
	Unload();
}

void FrameBuffer::InitDepthTexture(unsigned width, unsigned height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	//create a depth texture
	glGenTextures(1, &_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	DepthTestActive = true;

	_DepthWidth = width;
	_DepthHeight = height;
	_DepthFilter = GL_NEAREST;
	_DepthWrap = GL_CLAMP_TO_EDGE;
}

void FrameBuffer::InitDepthTexture(unsigned width, unsigned height, GLint filter, GLint wrap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	//create a depth texture
	glGenTextures(1, &_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	//bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	DepthTestActive = true;

	_DepthWidth = width;
	_DepthHeight = height;
	_DepthFilter = filter;
	_DepthWrap = wrap;
}

void FrameBuffer::InitColorTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	//create a color texture
	glGenTextures(1, &_ColorAttachments[index]);
	glBindTexture(GL_TEXTURE_2D, _ColorAttachments[index]);
	glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	//bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, _ColorAttachments[index], 0);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	_ColorWidth[index] = width;
	_ColorHeight[index] = height;

	_ColorFormat[index] = internalFormat;
	_ColorFilter[index] = filter;
	_ColorWrap[index] = wrap;

}

bool FrameBuffer::CheckFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Unload();
		return false;
	}
	else
	{
		//PrintInfo();
		return true;
	}
}

//Clears all openGL memory
void FrameBuffer::Unload()
{
	if (_Bufs != nullptr)
	{
		delete[] _Bufs;
		_Bufs = nullptr;

		delete[] _ColorWidth;
		delete[] _ColorHeight;
		delete[] _ColorFormat;
		delete[] _ColorFilter;
		delete[] _ColorWrap;

		_ColorWidth = nullptr;
		_ColorHeight = nullptr;
		_ColorFormat = nullptr;
		_ColorFilter = nullptr;
		_ColorWrap = nullptr;
	}
	if (_ColorAttachments != nullptr) {
		for (unsigned int i = 0; i < _NumColorAttachments; i++) {
			glDeleteTextures(1, &_ColorAttachments[i]);
		}
		delete[] _ColorAttachments;
		_ColorAttachments = nullptr;
	}
	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	_NumColorAttachments = 0;
}

//Clears all attached textures
void FrameBuffer::Clear()
{
	GLbitfield temp = 0;
	if (_DepthAttachment != GL_NONE)
	{
		temp = temp | GL_DEPTH_BUFFER_BIT;


	}
	if (_ColorAttachments != nullptr)
	{
		temp = temp | GL_COLOR_BUFFER_BIT;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(temp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

}


void FrameBuffer::Reset(unsigned numColorAttachments)
{
	if (_Bufs != nullptr)
	{
		delete[] _Bufs;
		_Bufs = nullptr;

		delete[] _ColorWidth;
		delete[] _ColorHeight;
		delete[] _ColorFormat;
		delete[] _ColorFilter;
		delete[] _ColorWrap;

		_ColorWidth = nullptr;
		_ColorHeight = nullptr;
		_ColorFormat = nullptr;
		_ColorFilter = nullptr;
		_ColorWrap = nullptr;
	}
	if (_ColorAttachments != nullptr) {
		for (unsigned int i = 0; i < _NumColorAttachments; i++) {
			glDeleteTextures(1, &_ColorAttachments[i]);
		}
		delete[] _ColorAttachments;
		_ColorAttachments = nullptr;
	}
	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	_NumColorAttachments = numColorAttachments;

	_ColorAttachments = new GLuint[_NumColorAttachments];
	//_Bufs is required as parameter for glDrawBuffers().
	_Bufs = new GLenum[_NumColorAttachments];

	_ColorWidth = new unsigned[_NumColorAttachments];
	_ColorHeight = new unsigned[_NumColorAttachments];
	_ColorFormat = new GLint[_NumColorAttachments];
	_ColorFilter = new GLint[_NumColorAttachments];
	_ColorWrap = new GLint[_NumColorAttachments];

	for (unsigned int i = 0; i < _NumColorAttachments; i++)
	{
		_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

void FrameBuffer::Reset()
{
	if (_Bufs != nullptr)
	{
		delete[] _Bufs;
		_Bufs = nullptr;

		delete[] _ColorWidth;
		delete[] _ColorHeight;
		delete[] _ColorFormat;
		delete[] _ColorFilter;
		delete[] _ColorWrap;

		_ColorWidth = nullptr;
		_ColorHeight = nullptr;
		_ColorFormat = nullptr;
		_ColorFilter = nullptr;
		_ColorWrap = nullptr;
	}
	if (_ColorAttachments != nullptr) {
		for (unsigned int i = 0; i < _NumColorAttachments; i++) {
			glDeleteTextures(1, &_ColorAttachments[i]);
		}
		delete[] _ColorAttachments;
		_ColorAttachments = nullptr;
	}
	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	//_NumColorAttachments = numColorAttachments;

	//glGenFramebuffers(1, &_FBO);

	_ColorAttachments = new GLuint[_NumColorAttachments];
	//_Bufs is required as parameter for glDrawBuffers().
	_Bufs = new GLenum[_NumColorAttachments];

	_ColorWidth = new unsigned[_NumColorAttachments];
	_ColorHeight = new unsigned[_NumColorAttachments];
	_ColorFormat = new GLint[_NumColorAttachments];
	_ColorFilter = new GLint[_NumColorAttachments];
	_ColorWrap = new GLint[_NumColorAttachments];

	for (unsigned int i = 0; i < _NumColorAttachments; i++)
	{
		_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

void FrameBuffer::reshape(int width, int height)
{
	if (_Bufs != nullptr)
	{
		delete[] _Bufs;
		_Bufs = nullptr;
	}
	if (_ColorAttachments != nullptr) 
	{
		for (unsigned int i = 0; i < _NumColorAttachments; i++)
		{
			glDeleteTextures(1, &_ColorAttachments[i]);
		}
		delete[] _ColorAttachments;
		_ColorAttachments = nullptr;
	}
	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	_ColorAttachments = new GLuint[_NumColorAttachments];
	_Bufs = new GLenum[_NumColorAttachments];

	for (unsigned int i = 0; i < _NumColorAttachments; i++)
	{
		_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	for (unsigned int i = 0; i < _NumColorAttachments; i++)
	{
		InitColorTexture(i, width, height, _ColorFormat[i], _ColorFilter[i], _ColorWrap[i]);
	}

	if (DepthTestActive)
	{
		InitDepthTexture(width, height, _DepthFilter, _DepthWrap);
	}
}



void FrameBuffer::DrawFromSource(unsigned textureHandle)
{
	bind();
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	unbind();
}

void FrameBuffer::DrawFromSource(unsigned textureHandle1, unsigned textureHandle2)
{
	bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureHandle2);

	DrawFullScreenQuad();

	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	unbind();
}

void FrameBuffer::DrawToBackFromSource(unsigned textureHandle)
{
	unbind();
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void FrameBuffer::DrawToBackFromSource(unsigned textureHandle1, unsigned textureHandle2)
{
	unbind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureHandle2);

	DrawFullScreenQuad();

	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void FrameBuffer::InitFullScreenQuad()
{
	float VBO_DATA[] = {
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		-1.f, 1.f, 0.f,

		1.f, 1.f, 0.f,
		-1.f, 1.f, 0.f,
		1.f, -1.f, 0.f,

		0.f, 0.f,	//one half of screen (triangle coordinates)
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,	//other half of screen
		0.f, 1.f,
		1.f, 0.f
	};

	int vertexSize = 6 * 3 * sizeof(float);
	int uvSize = 6 * 2 * sizeof(float);

	glGenVertexArrays(1, &FullScreenQuadVAO);
	glBindVertexArray(FullScreenQuadVAO);

	glEnableVertexAttribArray(0); //< Vertices
	glEnableVertexAttribArray(1); //< UVs

	glGenBuffers(1, &FullScreenQuadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, FullScreenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize + uvSize, VBO_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize));


	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void FrameBuffer::DrawFullScreenQuad()
{
	glBindVertexArray(FullScreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}






void FrameBuffer::bind() const
{
	glViewport(0, 0, _ColorWidth[0], _ColorHeight[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glDrawBuffers(_NumColorAttachments, _Bufs);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void FrameBuffer::MoveToBackBuffer(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::CopyFromFramebuffer(unsigned index, FrameBuffer *source, unsigned sourceIndex)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source->_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _FBO);

	glBlitFramebuffer(
		0, 0, source->_ColorWidth[sourceIndex], source->_ColorHeight[sourceIndex],
		0, 0, source->_ColorWidth[sourceIndex], source->_ColorHeight[sourceIndex], GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::CopyDepthFromFramebuffer(FrameBuffer *source)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source->_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _FBO);

	glBlitFramebuffer(
		0, 0, source->_DepthWidth, source->_DepthHeight,
		0, 0, source->_DepthWidth, source->_DepthHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
}

GLuint FrameBuffer::GetDepthHandle() const
{
	return _DepthAttachment;
}

unsigned FrameBuffer::GetDepthWidth() const
{
	return _DepthWidth;
}

unsigned FrameBuffer::GetDepthHeight() const
{
	return _DepthHeight;
}

GLuint FrameBuffer::GetDepthFilter() const
{
	return _DepthFilter;
}

GLuint FrameBuffer::GetDepthWrap() const
{
	return _DepthWrap;
}


GLuint FrameBuffer::GetColorHandle(unsigned index) const
{
	return _ColorAttachments[index];
}


unsigned FrameBuffer::GetColorWidth(unsigned index) const
{
	return _ColorWidth[index];
}

unsigned FrameBuffer::GetColorHeight(unsigned index) const
{
	return _ColorHeight[index];
}

GLuint FrameBuffer::GetColorFormat(unsigned index) const
{
	return _ColorFormat[index];
}

GLuint FrameBuffer::GetColorFilter(unsigned index) const
{
	return _ColorFilter[index];
}

GLuint FrameBuffer::GetColorWrap(unsigned index) const
{
	return _ColorWrap[index];
}

void FrameBuffer::bindAsTex(int textureSlot, unsigned int index) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GetColorHandle(index));
}










void FrameBuffer::PrintInfo()
{
	SAT_DEBUG_LOG("Number of Attachments: %u", _NumColorAttachments);
	SAT_DEBUG_LOG("Depth Buffer Active: %u", DepthTestActive);
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

float Window::getAspect()
{
	return aspectRatio;
}

vec2 Window::getSize()
{
	return size;
}

void Window::setWidth(int w)
{
	size.x = static_cast<float>(width = w);
	recalculateAspectRatio();
}

void Window::setHeight(int h)
{
	size.y = static_cast<float>(height = h);
	recalculateAspectRatio();
}

void Window::setResolution(int w, int h)
{
	size.x = static_cast<float>(width = w);
	size.y = static_cast<float>(height = h);
	recalculateAspectRatio();
}

void Window::recalculateAspectRatio()
{
	aspectRatio = size.x / size.y;
}
