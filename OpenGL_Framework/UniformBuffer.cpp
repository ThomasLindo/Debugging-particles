#include "UniformBuffer.h"
#include <assert.h>

UniformBuffer::UniformBuffer()
{

}

UniformBuffer::~UniformBuffer()
{
	if (_UBO)
	{
		glDeleteBuffers(1, &_UBO);
	}
}

void UniformBuffer::allocateMemory(unsigned int bytes)
{
	glGenBuffers(1, &_UBO);
	bind();
	glBufferData(GL_UNIFORM_BUFFER, bytes, GL_NONE, GL_DYNAMIC_DRAW);
	unbind();
	_IsInit = true;
}

void UniformBuffer::sendMatrix(mat4 matrix, int offset)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(mat4), &matrix);
	unbind();
}

void UniformBuffer::sendVector(vec3 vector, int offset)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec3), &vector);
	unbind();
}

void UniformBuffer::sendFloat(float scalar, int offset)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &scalar);
	unbind();
}

void UniformBuffer::bind(GLuint slot)
{
	assert(_IsInit);

	glBindBufferBase(GL_UNIFORM_BUFFER, slot, _UBO);
	_BindLocation = slot;
}

bool UniformBuffer::isInit()
{
	return _IsInit;
}

void UniformBuffer::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _UBO);
}

void UniformBuffer::unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}
