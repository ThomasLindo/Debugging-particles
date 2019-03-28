#pragma once
#include "Mesh.h"

class PauseMenuMesh;

class MeshDynamic : public Mesh
{
public:
	~MeshDynamic();


protected:
	void initUploadToGPU();

	void uploadToGPU();

	friend PauseMenuMesh;
};
