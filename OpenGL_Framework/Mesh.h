#pragma once
#include "GL/glew.h"
#include "Transform.h"
#include <vector>
#include "Resource.h"
#include "VertexBufferObject.h"
#include "Particle.h"

class Mesh : public Resource
{
public:
	void initMeshSphere(const unsigned int xSlices, const unsigned int ySlices);
	void initMeshCylinder(const unsigned int xSlices, const unsigned int ySlices, bool invert);
	void initMeshCylinder(const unsigned int xSlices, const unsigned int ySlices, bool invert, const float degrees); //Sliced version
	void initMeshPlane(const unsigned int xDivisions, const unsigned int yDivisions);

	bool loadFromObj(const std::string &file);
	void initParticles(Particle* particle, void* position);
	void draw() const;
	void Mesh::bind() const;
	static void Mesh::unbind();

	std::vector<vec4> dataVertex;
	std::vector<vec4> dataTexture;
	std::vector<vec4> dataNormal;
	std::vector<vec4> dataColor;

	void updateVAO();
	VertexArrayObject vao;
protected:
	
	static std::string _ModelDirectory;

	void initUploadToGPU();
};