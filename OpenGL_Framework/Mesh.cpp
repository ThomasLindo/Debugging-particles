#define _CRT_SECURE_NO_WARNINGS //Remove warnings from deprecated functions. Shut up, Microsoft.
#define CHAR_BUFFER_SIZE 128 //Size of the character buffer.

#include "Mesh.h"
#include <vector>
#include <gmtk/gmtk.h>
#include "AniMath.h"
#include "IO.h"
#include <fstream>

std::string Mesh::_ModelDirectory = "../assets/models/";
struct MeshFace
{
	MeshFace()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned u1, unsigned u2, unsigned u3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = u1;
		textureUVs[1] = u2;
		textureUVs[2] = u3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
	unsigned tangent[3];
	unsigned bitangent[3];
};

void Mesh::initMeshSphere(const unsigned int xSlices, const unsigned int ySlices)
{
	float x = (float)xSlices;
	float y = (float)ySlices;
	dataVertex.reserve(xSlices*ySlices * 6);
	dataTexture.reserve(xSlices*ySlices * 6);
	dataNormal.reserve(xSlices*ySlices * 6);
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			float s0 = sin(((j + 0) / y)* f_pi);
			float s1 = sin(((j + 1) / y)* f_pi);
		
			float c0 = cos((2.0f * (j + 0) / y - 1.0f)* f_pi * 0.5f + f_pi * 0.5f);
			float c1 = cos((2.0f * (j + 1) / y - 1.0f)* f_pi * 0.5f + f_pi * 0.5f);

			dataVertex.push_back(vec4(s0 * sin((i + 0) * f_pi*2.0f / x), c0, s0 * cos((i + 0) * f_pi*2.0f / x), 0.0f));
			dataVertex.push_back(vec4(s1 * sin((i + 0) * f_pi*2.0f / x), c1, s1 * cos((i + 0) * f_pi*2.0f / x), 0.0f));
			dataVertex.push_back(vec4(s0 * sin((i + 1) * f_pi*2.0f / x), c0, s0 * cos((i + 1) * f_pi*2.0f / x), 0.0f));
			dataVertex.push_back(vec4(s1 * sin((i + 0) * f_pi*2.0f / x), c1, s1 * cos((i + 0) * f_pi*2.0f / x), 0.0f));
			dataVertex.push_back(vec4(s1 * sin((i + 1) * f_pi*2.0f / x), c1, s1 * cos((i + 1) * f_pi*2.0f / x), 0.0f));
			dataVertex.push_back(vec4(s0 * sin((i + 1) * f_pi*2.0f / x), c0, s0 * cos((i + 1) * f_pi*2.0f / x), 0.0f));

			vec4 bl = vec4((i    ) / x, 1.f - (j    ) / y, 0.f, 0.f);
			vec4 br = vec4((i + 1) / x, 1.f - (j    ) / y, 0.f, 0.f);	
			vec4 tl = vec4((i    ) / x, 1.f - (j + 1) / y, 0.f, 0.f);
			vec4 tr = vec4((i + 1) / x, 1.f - (j + 1) / y, 0.f, 0.f);

			dataTexture.push_back(bl);
			dataTexture.push_back(tl);
			dataTexture.push_back(br);
			dataTexture.push_back(tl);
			dataTexture.push_back(tr);
			dataTexture.push_back(br);
			
			dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 6]));
			dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 5]));
			dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 4]));
			dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 3]));
			dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 2]));
			dataNormal.push_back(normalize(dataVertex[dataVertex.size() - 1]));
		}
	}

	initUploadToGPU();
}

void Mesh::initMeshCylinder(const unsigned int xSlices, const unsigned int ySlices, bool invert)
{
	//std::vector<vec4> vertexData;
	//std::vector<vec4> textureData;
	//std::vector<vec4> normalData;

	float x = (float)xSlices;
	float y = (float)ySlices;
	dataVertex.reserve(xSlices*ySlices * 6);
	dataTexture.reserve(xSlices*ySlices * 6);
	dataNormal.reserve(xSlices*ySlices * 6);
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			float c0 = (j + 0) / y * 2 - 1;
			float c1 = (j + 1) / y * 2 - 1;
						
			vec4 blVert, blNorm = blVert = vec4(sin((i + 0) * f_pi*2.0f / x), c0, cos((i + 0) * f_pi*2.0f / x), 0.0f);
			vec4 brVert, brNorm = brVert = vec4(sin((i + 1) * f_pi*2.0f / x), c0, cos((i + 1) * f_pi*2.0f / x), 0.0f);
			vec4 tlVert, tlNorm = tlVert = vec4(sin((i + 0) * f_pi*2.0f / x), c1, cos((i + 0) * f_pi*2.0f / x), 0.0f);
			vec4 trVert, trNorm = trVert = vec4(sin((i + 1) * f_pi*2.0f / x), c1, cos((i + 1) * f_pi*2.0f / x), 0.0f);
			
			if (invert)
			{
				dataVertex.push_back(blVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(brVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(trVert);
				dataVertex.push_back(brVert);
			}
			else
			{
				dataVertex.push_back(blVert);
				dataVertex.push_back(brVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(brVert);
				dataVertex.push_back(trVert);
			}

			vec4 blUV;
			vec4 brUV;
			vec4 tlUV;
			vec4 trUV;

			if (invert)
			{
				blUV = vec4(1 - (i) / x, (j) / y, 0.f, 0.f);
				brUV = vec4(1 - (i + 1) / x, (j) / y, 0.f, 0.f);
				tlUV = vec4(1 - (i) / x, (j + 1) / y, 0.f, 0.f);
				trUV = vec4(1 - (i + 1) / x, (j + 1) / y, 0.f, 0.f);
			}
			else
			{
				blUV = vec4((i) / x, (j) / y, 0.f, 0.f);
				brUV = vec4((i + 1) / x, (j) / y, 0.f, 0.f);
				tlUV = vec4((i) / x, (j + 1) / y, 0.f, 0.f);
				trUV = vec4((i + 1) / x, (j + 1) / y, 0.f, 0.f);
			}

			dataTexture.push_back(blUV);
			dataTexture.push_back(tlUV);
			dataTexture.push_back(brUV);
			dataTexture.push_back(tlUV);
			dataTexture.push_back(trUV);
			dataTexture.push_back(brUV);
			
			blNorm.y = 0.0f;
			brNorm.y = 0.0f;
			tlNorm.y = 0.0f;
			trNorm.y = 0.0f;

			blNorm = normalize(blNorm);
			brNorm = normalize(brNorm);
			tlNorm = normalize(tlNorm);
			trNorm = normalize(trNorm);

			dataNormal.push_back(blNorm);
			dataNormal.push_back(tlNorm);
			dataNormal.push_back(brNorm);
			dataNormal.push_back(tlNorm);
			dataNormal.push_back(trNorm);
			dataNormal.push_back(brNorm);
		}
	}

	initUploadToGPU();
}

void Mesh::initMeshCylinder(const unsigned int xSlices, const unsigned int ySlices, bool invert, const float degrees)
{
	float x = (float)xSlices;
	float y = (float)ySlices;
	dataVertex.reserve(xSlices*ySlices * 6);
	dataTexture.reserve(xSlices*ySlices * 6);
	dataNormal.reserve(xSlices*ySlices * 6);
	//float inv = invert * 2.f - 1.f;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			float c0 = (j + 0) / y * 2 - 1;
			float c1 = (j + 1) / y * 2 - 1;

			vec4 blVert, blNorm = blVert = vec4(sin((i + 0) * f_pi*2.0f / x * degrees / 360.f), c0, cos((i + 0) * f_pi*2.0f / x * degrees / 360.0f), 0.0f);
			vec4 brVert, brNorm = brVert = vec4(sin((i + 1) * f_pi*2.0f / x * degrees / 360.f), c0, cos((i + 1) * f_pi*2.0f / x * degrees / 360.0f), 0.0f);
			vec4 tlVert, tlNorm = tlVert = vec4(sin((i + 0) * f_pi*2.0f / x * degrees / 360.f), c1, cos((i + 0) * f_pi*2.0f / x * degrees / 360.0f), 0.0f);
			vec4 trVert, trNorm = trVert = vec4(sin((i + 1) * f_pi*2.0f / x * degrees / 360.f), c1, cos((i + 1) * f_pi*2.0f / x * degrees / 360.0f), 0.0f);

			if (invert)
			{
				dataVertex.push_back(blVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(brVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(trVert);
				dataVertex.push_back(brVert);
			}
			else
			{
				dataVertex.push_back(blVert);
				dataVertex.push_back(brVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(tlVert);
				dataVertex.push_back(brVert);
				dataVertex.push_back(trVert);
			}

			vec4 blUV;
			vec4 brUV;
			vec4 tlUV;
			vec4 trUV;

			if(invert)
			{
				blUV = vec4(1 - (i) / x, (j) / y, 0.f, 0.f);
				brUV = vec4(1 - (i + 1) / x, (j) / y, 0.f, 0.f);
				tlUV = vec4(1 - (i) / x, (j + 1) / y, 0.f, 0.f);
				trUV = vec4(1 - (i + 1) / x, (j + 1) / y, 0.f, 0.f);
			}
			else
			{
				blUV = vec4((i) / x, (j) / y, 0.f, 0.f);
				brUV = vec4((i + 1) / x, (j) / y, 0.f, 0.f);
				tlUV = vec4((i) / x, (j + 1) / y, 0.f, 0.f);
				trUV = vec4((i + 1) / x, (j + 1) / y, 0.f, 0.f);				
			}

			dataTexture.push_back(blUV);
			dataTexture.push_back(tlUV);
			dataTexture.push_back(brUV);
			dataTexture.push_back(tlUV);
			dataTexture.push_back(trUV);
			dataTexture.push_back(brUV);

			blNorm.y = 0.0f;
			brNorm.y = 0.0f;
			tlNorm.y = 0.0f;
			trNorm.y = 0.0f;

			blNorm = normalize(blNorm);
			brNorm = normalize(brNorm);
			tlNorm = normalize(tlNorm);
			trNorm = normalize(trNorm);

			dataNormal.push_back(blNorm);
			dataNormal.push_back(tlNorm);
			dataNormal.push_back(brNorm);
			dataNormal.push_back(tlNorm);
			dataNormal.push_back(trNorm);
			dataNormal.push_back(brNorm);
		}
	}

	initUploadToGPU();
}

void Mesh::initMeshPlane(const unsigned int xDivisions, const unsigned int yDivisions)
{
	//std::vector<vec4> vertexData;
	//std::vector<vec4> textureData;
	//std::vector<vec4> normalData;

	float x = (float)xDivisions;
	float y = (float)yDivisions;
	dataVertex.reserve(xDivisions*yDivisions * 6);
	dataTexture.reserve(xDivisions*yDivisions * 6);
	dataNormal.reserve(xDivisions*yDivisions * 6);
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			dataVertex.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 0) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 1) / y * 2 - 1, 0.0f));
			dataVertex.push_back(vec4((i + 1) / x * 2 - 1, 0.0f, (j + 0) / y * 2 - 1, 0.0f));

			vec4 bl = vec4((i) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 br = vec4((i + 1) / x, 1.f - (j) / y, 0.f, 0.f);
			vec4 tl = vec4((i) / x, 1.f - (j + 1) / y, 0.f, 0.f);
			vec4 tr = vec4((i + 1) / x, 1.f - (j + 1) / y, 0.f, 0.f);

			dataTexture.push_back(bl);
			dataTexture.push_back(tl);
			dataTexture.push_back(br);
			dataTexture.push_back(tl);
			dataTexture.push_back(tr);
			dataTexture.push_back(br);

			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
			dataNormal.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
		}
	}

	initUploadToGPU();
}

void Mesh::initParticles(Particle *particle, void* position)
{
	VertexBufferData vertexData;

	vertexData.attributeLocation = VERTEX;
	vertexData.elementType = GL_FLOAT;
	vertexData.numElementsPerAttribute = 3;
	vertexData.numElements = vertexData.numElementsPerAttribute;
	vertexData.sizeOfElement = sizeof(float);
	vertexData.data = position;

	vao.setPrimitiveType(GL_POINTS);
	vao.addVBO(vertexData);
	vao.createVAO(GL_DYNAMIC_DRAW);
}

void Mesh::bind() const
{
	//glBindVertexArray(VAO);
	vao.bind();
}

void Mesh::unbind()
{
	glBindVertexArray(GL_NONE);
}

void Mesh::updateVAO()
{
	//takes a lot of cpu
	vao.reuploadVAO();
}

bool Mesh::loadFromObj(const std::string & file)
{
	std::ifstream input;
	input.open(_ModelDirectory + file);

	if (!input)
	{
		SAT_ERROR_LOC("Error: Could not open file \"%s\"!\n", file.c_str());
		return false;
	}
	char line[CHAR_BUFFER_SIZE];

	vec3 topCorn; // Bounding box corner
	vec3 btmCorn; // Bounding box corner

	//Unique data
	std::vector<vec3> vertexDataLoad;
	std::vector<vec2> textureDataLoad;
	std::vector<vec3> normalDataLoad;

	// Index/Face data
	std::vector<MeshFace> faceData;

	float x, y, z;
	MeshFace faceTemp;

	while (!input.eof()) // Go to end of file
	{
		input.getline(line, CHAR_BUFFER_SIZE);

		switch (line[0])
		{
		case 'v':
			switch (line[1])
			{
			case ' ':
				std::sscanf(line, "v %f %f %f", &x, &y, &z);
				vertexDataLoad.push_back(vec3(x, y, z));
				break;
			case 't':
				std::sscanf(line, "vt %f %f", &x, &y);
				textureDataLoad.push_back(vec2(x, y));
				break;
			case 'n':
				std::sscanf(line, "vn %f %f %f", &x, &y, &z);
				normalDataLoad.push_back(vec3(x, y, z));
				break;
			}
			break;
		case 'f':
			std::sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&faceTemp.vertices[0], &faceTemp.textureUVs[0], &faceTemp.normals[0],
				&faceTemp.vertices[1], &faceTemp.textureUVs[1], &faceTemp.normals[1],
				&faceTemp.vertices[2], &faceTemp.textureUVs[2], &faceTemp.normals[2]);
			faceData.push_back(faceTemp);
			break;
		case 'o':
		case '#':
		case 'm':
		default:
			break;
		}
	}
	input.close();

	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			vec4 data = vec4(
				vertexDataLoad[faceData[i].vertices[j] - 1].x,
				vertexDataLoad[faceData[i].vertices[j] - 1].y,
				vertexDataLoad[faceData[i].vertices[j] - 1].z,
				1.0f);
			dataVertex.push_back(vec4(
				vertexDataLoad[faceData[i].vertices[j] - 1].x,
				vertexDataLoad[faceData[i].vertices[j] - 1].y,
				vertexDataLoad[faceData[i].vertices[j] - 1].z,
				1.0f));

			dataTexture.push_back(vec4(
				textureDataLoad[faceData[i].textureUVs[j] - 1].x,
				textureDataLoad[faceData[i].textureUVs[j] - 1].y,
				0.0f,
				1.0f));

			dataNormal.push_back(vec4(
				normalDataLoad[faceData[i].normals[j] - 1].x,
				normalDataLoad[faceData[i].normals[j] - 1].y,
				normalDataLoad[faceData[i].normals[j] - 1].z,
				1.0f));
		}



	}

	initUploadToGPU();

	vertexDataLoad.clear(); //Clear the vectors from RAM now that everything's in the GPU.
	textureDataLoad.clear();
	normalDataLoad.clear();
	faceData.clear();

	setOpenGLName(GL_VERTEX_ARRAY, vao.getVaoHandle(), "Mesh: " + file);
	return true;
}

void Mesh::draw() const
{
	vao.draw();
	//bind();
	//glDrawArrays(GL_TRIANGLES, 0, _NumVert);
	//unbind();
}

void Mesh::initUploadToGPU()
{
	unsigned int numTris = (unsigned int)(dataVertex.size()) / 3; 

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
	vao.createVAO(GL_STATIC_DRAW);
}

