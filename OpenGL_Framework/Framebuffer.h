#pragma once
#include <GL/glew.h>
#include <string>
#include <gmtk/gmtk.h>

/*
  /////////////////
 // Framebuffer // 2019 Stephen Thompson
/////////////////

FrameBuffers are a collection of buffers (usually textures) used as a method of
storing the results of rendering. OpenGL has 2 different kinds of framebuffers,
one that we've been using this whole time! The Default Framebuffer, used to
represent a window or display device, and user-created Framebuffer Objects.
"FBOs" reference images from either Textures or Renderbuffers, and are not
directly visible. In order to view them, we must draw/blit the framebuffer
to the Default Framebuffer. In order to draw it, we create a quad that covers
the entire screen, and sample from the FBO like a texture using a simple
passthrough shader. A Framebuffer can hold any number of color attachments
and an optional depth attachment. No color and only depth is also allowed.
They can also contain a stencil buffer, or even a merged depth/stencil.

More info about Framebuffers can be read here
https://www.khronos.org/opengl/wiki/Framebuffer

Limitations of Framebuffers can be found using glGetIntegerv(GL_MACRO, &integer)
	-Framebuffers have a maximum number of draw buffers
		GL_MAX_DRAW_BUFFERS
*/

class Window
{
	static int width;
	static int height;
	static vec2 size;
	static float aspectRatio;
public:
	static int getWidth();
	static int getHeight();
	static float getAspect();
	static vec2 getSize();
	static void setWidth(int w);
	static void setHeight(int h);
	static void setResolution(int w, int h);
private:
	static void recalculateAspectRatio();
};

class FrameBuffer
{
public:
	FrameBuffer() = delete; //Cause compile error if we use the default constructor.
	FrameBuffer(unsigned numColorAttachments);
	~FrameBuffer();

	void InitDepthTexture(unsigned width, unsigned height);
	void InitDepthTexture(unsigned width, unsigned height, GLint filter, GLint wrap = GL_CLAMP_TO_EDGE);
	void InitColorTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat = GL_RGBA8, GLint filter = GL_LINEAR, GLint wrap = GL_CLAMP_TO_EDGE);
	bool CheckFBO();

	//Clears all openGL memory
	void Unload();

	//Clears all attached textures
	void Clear();

	//Inbetween of previous two
	void Reset(unsigned numColorAttachments);
	void Reset();
	void reshape(int width, int height);

	void DrawFromSource(unsigned textureHandle);
	void DrawFromSource(unsigned textureHandle1, unsigned textureHandle2);

	static void DrawToBackFromSource(unsigned textureHandle);
	static void DrawToBackFromSource(unsigned textureHandle1, unsigned textureHandle2);

	void bind() const;
	static void unbind();

	void MoveToBackBuffer(int windowWidth, int windowHeight);

	void CopyFromFramebuffer(unsigned index, FrameBuffer *source, unsigned sourceIndex);
	void CopyDepthFromFramebuffer(FrameBuffer *source);

	GLuint GetDepthHandle() const;
	unsigned GetDepthWidth() const;
	unsigned GetDepthHeight() const;
	GLuint GetDepthFilter() const;
	GLuint GetDepthWrap() const;
	GLuint GetColorHandle(unsigned index) const;
	unsigned GetColorWidth(unsigned index) const;
	unsigned GetColorHeight(unsigned index) const;
	GLuint GetColorFormat(unsigned index) const;
	GLuint GetColorFilter(unsigned index) const;
	GLuint GetColorWrap(unsigned index) const;
	void bindAsTex(int textureSlot, unsigned int index = 0) const;
	void PrintInfo();

	//
	static void InitFullScreenQuad();
	static void DrawFullScreenQuad();

private:
	GLuint _FBO = GL_NONE;
	GLuint _DepthAttachment = GL_NONE;
	GLuint *_ColorAttachments = nullptr;
	GLenum *_Bufs = nullptr;

	unsigned _DepthWidth = 0;
	unsigned _DepthHeight = 0;
	GLint	_DepthFilter = GL_NONE;
	GLint	_DepthWrap = GL_NONE;

	unsigned *_ColorWidth = nullptr;
	unsigned *_ColorHeight = nullptr;
	GLint *	_ColorFormat = nullptr;
	GLint * _ColorFilter = nullptr;
	GLint * _ColorWrap = nullptr;

	static GLuint FullScreenQuadVAO;
	static GLuint FullScreenQuadVBO;

	bool IsLoaded_ = false;
	bool DepthTestActive = false;

	unsigned _NumColorAttachments = 0;

};

class GBuffer : public FrameBuffer
{

};