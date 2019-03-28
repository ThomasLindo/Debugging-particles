#pragma once
#include "MeshDynamic.h"
#include "Texture.h"
#include "AniMath.h"

class PauseMenuMesh
{
public:
	~PauseMenuMesh();

	MeshDynamic gridMesh;
	MeshDynamic shineMesh;
	vec4 color = vec4(vec3(0.4906105f, 0.6732029f, 1.0f), 1.0f);
	vec2 lensScale = vec2(0.25f, 0.25f);


	const unsigned int width = 10; 
	const unsigned int height = 10;

	void initMeshPauseGrids(Texture* gridTex, Texture* bloomTex);
	void updateMeshPauseGrids(float deltaTime);
	bool toggle();
	bool getState();

	void draw();

private:
	bool m_pActive = false;
	float m_pTransitionSpeed = 1.0f;
	float timer = 0.0f;	
	float interp = 0.0f;

	Texture* m_pGridTex;
	Texture* m_pBloomTex;

	class GridShine
	{
	public:
		float offset;
		float offsetMax = 5.0f;
		float speed;

		void Init()
		{
			SetSpeed();
			offset = random(0.0f, offsetMax);
		}
	
		void ReInit()
		{
			SetSpeed();
			offset = 0.0f;
		}

		void update(float deltaTime)
		{
			offset += speed * deltaTime;
			if (offset > offsetMax)
			{
				ReInit();
			}
		}

	private:
		void SetSpeed()
		{
			speed = random(0.45f, 0.85f);
		}
	};

	GridShine gridShine[10];

	void distortVertex(vec4 &position);
};