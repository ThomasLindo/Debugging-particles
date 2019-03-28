#include "VertexBufferObject.h"

VertexArrayObject::VertexArrayObject()
{
	vaoHandle = 0;
	primitiveType = GL_TRIANGLES;
}

VertexArrayObject::~VertexArrayObject()
{
	destroy();
}

int VertexArrayObject::addVBO(VertexBufferData descriptor)
{
	vboData.push_back(descriptor);
	return 1;
}

VertexBufferData * VertexArrayObject::getVboData(AttributeLocations loc) 
{
	for (size_t i = 0; i < vboData.size(); ++i)
	{
		if (vboData[i].attributeLocation == loc)
		{
			return &vboData[i];
		}
	}
	return nullptr;
}

GLuint VertexArrayObject::getVaoHandle() const
{
	return vaoHandle;
}
GLuint VertexArrayObject::getVboHandle(AttributeLocations loc) const
{
	for (size_t i = 0; i < vboData.size(); ++i)
	{
		if (vboData[i].attributeLocation == loc)
		{
			return vboHandles[i];
		}
	}
	return 0;
}

GLenum VertexArrayObject::getPrimitiveType() const
{
	return primitiveType;
}

void VertexArrayObject::setPrimitiveType(GLenum type)
{
	primitiveType = type;
}

void VertexArrayObject::createVAO(GLenum vboUsage)
{
	if (vaoHandle)
	{
		destroy();
	}
	
	vboUsageType = vboUsage;
	glGenVertexArrays(1, &vaoHandle);
	this->bind();

	auto numBuffers = vboData.size();
	vboHandles.resize(numBuffers);

	glGenBuffers(numBuffers, &vboHandles[0]);

	for (size_t i = 0; i < numBuffers; ++i)
	{
		VertexBufferData* attrib = &vboData[i];

		attrib->numVertices = attrib->numElements / attrib->numElementsPerAttribute;

		glEnableVertexAttribArray(attrib->attributeLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
		glBufferData(GL_ARRAY_BUFFER, attrib->numElements * attrib->sizeOfElement,
			attrib->data, vboUsage);

		glVertexAttribPointer(attrib->attributeLocation, attrib->numElementsPerAttribute,
			attrib->elementType, GL_FALSE, 0, 0);

	}
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	this->unbind();
}

void VertexArrayObject::reuploadVAO()
{
	for (size_t i = 0; i < vboHandles.size(); ++i)
	{
		VertexBufferData* attrib = &vboData[i];

		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
		//takes a lot of cpu
		glBufferSubData(GL_ARRAY_BUFFER, 0, attrib->numElements * attrib->sizeOfElement, attrib->data);
	}

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void VertexArrayObject::draw() const
{
	if (vaoHandle)
	{
		this->bind();
		//glDrawArrays takes a lot of cpu
		glDrawArrays(primitiveType, 0, vboData[0].numVertices);this->unbind();
	}
}

void VertexArrayObject::bind() const
{
	glBindVertexArray(vaoHandle);
}

void VertexArrayObject::unbind() const
{
	glBindVertexArray(GL_NONE);
}

void VertexArrayObject::destroy()
{
	if (vaoHandle)
	{
		glDeleteVertexArrays(1, &vaoHandle);
		glDeleteBuffers((GLsizei)vboHandles.size(), &vboHandles[0]);
	}

	vboHandles.clear();
	vboData.clear();
}
