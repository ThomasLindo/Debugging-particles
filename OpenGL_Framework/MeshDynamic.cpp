#include "MeshDynamic.h"

MeshDynamic::~MeshDynamic()
{
	vao.destroy();
}

void MeshDynamic::initUploadToGPU()
{
	unsigned int numTris = (unsigned int)(dataVertex.size()) / 3; // Todo: non-triangulated meshes

	// Setup VBO

	// Set up position (vertex) attribute
	if (dataVertex.size() > 0)
	{
		VertexBufferData positionAttrib;
		positionAttrib.attributeLocation = AttributeLocations::VERTEX;
		positionAttrib.attributeName = "vertex";
		positionAttrib.data = &dataVertex[0];
		positionAttrib.sizeOfElement = sizeof(float);
		positionAttrib.elementType = GL_FLOAT;
		positionAttrib.numElementsPerAttribute = 4;
		positionAttrib.numElements = numTris * 3 * positionAttrib.numElementsPerAttribute;
		vao.addVBO(positionAttrib);

	}

	// Set up UV attribute
	if (dataTexture.size() > 0)
	{
		VertexBufferData uvAttrib;
		uvAttrib.attributeLocation = AttributeLocations::TEXCOORD;
		uvAttrib.attributeName = "uv";
		uvAttrib.data = &dataTexture[0];
		uvAttrib.sizeOfElement = sizeof(float);
		uvAttrib.elementType = GL_FLOAT;
		uvAttrib.numElementsPerAttribute = 4;
		uvAttrib.numElements = numTris * 3 * uvAttrib.numElementsPerAttribute;
		vao.addVBO(uvAttrib);
	}

	// Set up normal attribute
	if (dataNormal.size() > 0)
	{
		VertexBufferData normalAttrib;
		normalAttrib.attributeLocation = AttributeLocations::NORMAL;
		normalAttrib.attributeName = "normal";
		normalAttrib.data = &dataNormal[0];
		normalAttrib.sizeOfElement = sizeof(float);
		normalAttrib.elementType = GL_FLOAT;
		normalAttrib.numElementsPerAttribute = 4;
		normalAttrib.numElements = numTris * 3 * normalAttrib.numElementsPerAttribute;
		vao.addVBO(normalAttrib);
	}

	// Set up normal attribute
	if (dataColor.size() > 0)
	{
		VertexBufferData colorAttrib;
		colorAttrib.attributeLocation = AttributeLocations::COLOR;
		colorAttrib.attributeName = "color";
		colorAttrib.data = &dataColor[0];
		colorAttrib.sizeOfElement = sizeof(float);
		colorAttrib.elementType = GL_FLOAT;
		colorAttrib.numElementsPerAttribute = 4;
		colorAttrib.numElements = numTris * 3 * colorAttrib.numElementsPerAttribute;
		vao.addVBO(colorAttrib);
	}

	// set up other attributes...

	vao.createVAO(GL_DYNAMIC_DRAW);
}

void MeshDynamic::uploadToGPU()
{
	vao.reuploadVAO();
}
