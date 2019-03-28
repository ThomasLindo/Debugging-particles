#include "PauseEffect.h"
#include "AniMath.h"
#include "IO.h"

void PauseMenuMesh::distortVertex(vec4 &position)
{
	// Creates fish eye effect
	position.x = lerp(position.x, 1.3f* position.x / (1.0f + position.y * position.y * lensScale.x), interp);
	position.y = lerp(position.y, position.y * (1.0f + position.x * position.x * lensScale.y), interp);
}



PauseMenuMesh::~PauseMenuMesh()
{
	gridMesh.vao.destroy();
	SAT_DEBUG_LOG_WARNING("Pause Menu Deconstructor not finished");
}

void PauseMenuMesh::initMeshPauseGrids(Texture* gridTex, Texture* bloomTex)
{
	m_pGridTex = gridTex;
	m_pBloomTex = bloomTex;

	float xSize = (float)width;
	float ySize = (float)height;

	unsigned int size = static_cast<unsigned int>(width * height * 6);

	gridMesh.dataVertex = std::vector<vec4>(size, vec4(0.0f));
	gridMesh.dataTexture = std::vector<vec4>(size, vec4(0.0f));
	gridMesh.dataNormal = std::vector<vec4>(size, vec4(0.0f));
	gridMesh.dataColor = std::vector<vec4>(size, vec4(0.0f));

	std::vector<vec4>* vertexData = &gridMesh.dataVertex;
	std::vector<vec4>* textureData = &gridMesh.dataTexture;
	std::vector<vec4>* normalData = &gridMesh.dataNormal;
	std::vector<vec4>* colorData = &gridMesh.dataColor;

	for (int x = 0, i = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y, i += 6)
		{
			vec4 blVert = vec4((x + 0) / xSize * 2 - 1, (y + 0) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(blVert);
			vec4 brVert = vec4((x + 1) / xSize * 2 - 1, (y + 0) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(brVert);
			vec4 tlVert = vec4((x + 0) / xSize * 2 - 1, (y + 1) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(tlVert);
			vec4 trVert = vec4((x + 1) / xSize * 2 - 1, (y + 1) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(trVert);

			vertexData[0][i + 0] = (blVert);
			vertexData[0][i + 1] = (brVert);
			vertexData[0][i + 2] = (tlVert);
			vertexData[0][i + 3] = (tlVert);
			vertexData[0][i + 4] = (brVert);
			vertexData[0][i + 5] = (trVert);

			vec4 blUV = vec4((x) / xSize, (y) / ySize, 0.f, 0.f);
			vec4 brUV = vec4((x + 1) / xSize, (y) / ySize, 0.f, 0.f);
			vec4 tlUV = vec4((x) / xSize, (y + 1) / ySize, 0.f, 0.f);
			vec4 trUV = vec4((x + 1) / xSize, (y + 1) / ySize, 0.f, 0.f);

			textureData[0][i + 0] = (blUV);
			textureData[0][i + 1] = (brUV);
			textureData[0][i + 2] = (tlUV);
			textureData[0][i + 3] = (tlUV);
			textureData[0][i + 4] = (brUV);
			textureData[0][i + 5] = (trUV);

			vec4 norm = vec4(0.0f, 0.0f, 1.0f, 0.0f);

			normalData[0][i + 0] = (norm);
			normalData[0][i + 1] = (norm);
			normalData[0][i + 2] = (norm);
			normalData[0][i + 3] = (norm);
			normalData[0][i + 4] = (norm);
			normalData[0][i + 5] = (norm);

			vec4 colorOut = lerp(vec4(vec3(1.0f), 0.0f), color, interp);

			colorData[0][i + 0] = (colorOut);
			colorData[0][i + 1] = (colorOut);
			colorData[0][i + 2] = (colorOut);
			colorData[0][i + 3] = (colorOut);
			colorData[0][i + 4] = (colorOut);
			colorData[0][i + 5] = (colorOut);
		}
	}

	for (vec4 vertex : *vertexData)
	{
		vertex.x = vertex.x * 0.5f + 0.5f;
		vertex.y = vertex.y * 0.5f + 0.5f;
	}

	gridMesh.initUploadToGPU();
	
	shineMesh.dataVertex = std::vector<vec4>(size, vec4(0.0f));
	shineMesh.dataTexture = std::vector<vec4>(size, vec4(0.0f));
	shineMesh.dataNormal = std::vector<vec4>(size, vec4(0.0f));
	shineMesh.dataColor = std::vector<vec4>(size, vec4(0.0f));

	vertexData =		&shineMesh.dataVertex;
	textureData =	&shineMesh.dataTexture;
	normalData =		&shineMesh.dataNormal;
	colorData =		&shineMesh.dataColor;

	for (int x = 0, i = 0; x < xSize; ++x)
	{
		gridShine[x].Init();

		for (int y = 0; y < ySize; ++y, i += 6)
		{
			vertexData[0][i + 0] = (vec4((x + 0) / xSize, (y + 0) / ySize, 0.0f, 0.0f));
			vertexData[0][i + 1] = (vec4((x + 0) / xSize, (y + 1) / ySize, 0.0f, 0.0f));
			vertexData[0][i + 2] = (vec4((x + 1) / xSize, (y + 0) / ySize, 0.0f, 0.0f));
			vertexData[0][i + 3] = (vec4((x + 0) / xSize, (y + 1) / ySize, 0.0f, 0.0f));
			vertexData[0][i + 4] = (vec4((x + 1) / xSize, (y + 1) / ySize, 0.0f, 0.0f));
			vertexData[0][i + 5] = (vec4((x + 1) / xSize, (y + 0) / ySize, 0.0f, 0.0f));

			vec4 bl = vec4((x) / xSize, 1.f - (y) / ySize, 0.f, 0.f);
			vec4 br = vec4((x + 1) / xSize, 1.f - (y) / ySize, 0.f, 0.f);
			vec4 tl = vec4((x) / xSize, 1.f - (y + 1) / ySize, 0.f, 0.f);
			vec4 tr = vec4((x + 1) / xSize, 1.f - (y + 1) / ySize, 0.f, 0.f);

			textureData[0][i + 0] = (bl);
			textureData[0][i + 1] = (tl);
			textureData[0][i + 2] = (br);
			textureData[0][i + 3] = (tl);
			textureData[0][i + 4] = (tr);
			textureData[0][i + 5] = (br);

			normalData[0][i + 0] = (vec4(0.0f, 0.0f, 1.0f, 0.0f));
			normalData[0][i + 1] = (vec4(0.0f, 0.0f, 1.0f, 0.0f));
			normalData[0][i + 2] = (vec4(0.0f, 0.0f, 1.0f, 0.0f));
			normalData[0][i + 3] = (vec4(0.0f, 0.0f, 1.0f, 0.0f));
			normalData[0][i + 4] = (vec4(0.0f, 0.0f, 1.0f, 0.0f));
			normalData[0][i + 5] = (vec4(0.0f, 0.0f, 1.0f, 0.0f));

			colorData[0][i + 0] = (vec4(0.5f, 0.5f, 1.0f, 1.0f));
			colorData[0][i + 1] = (vec4(0.5f, 0.5f, 1.0f, 1.0f));
			colorData[0][i + 2] = (vec4(0.5f, 0.5f, 1.0f, 1.0f));
			colorData[0][i + 3] = (vec4(0.5f, 0.5f, 1.0f, 1.0f));
			colorData[0][i + 4] = (vec4(0.5f, 0.5f, 1.0f, 1.0f));
			colorData[0][i + 5] = (vec4(0.5f, 0.5f, 1.0f, 1.0f));
		}
	}

	shineMesh.initUploadToGPU();
}

void PauseMenuMesh::updateMeshPauseGrids(float dt)
{
	if (m_pActive)
		timer += m_pTransitionSpeed * dt;
	else
		timer -= m_pTransitionSpeed * dt;
	
	timer = saturate(timer);
	interp = interp::smoothstep(0.0f, 1.0f, timer);
	
	std::vector<vec4>* vertexData = &gridMesh.dataVertex;
	std::vector<vec4>* textureData = &gridMesh.dataTexture;
	std::vector<vec4>* normalData = &gridMesh.dataNormal;
	std::vector<vec4>* colorData = &gridMesh.dataColor;

	float xSize = (float)10;
	float ySize = (float)10;
	float yShineSize = (float)10;

	for (int x = 0, i = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y, i += 6)
		{
			vec4 blVert = vec4((x + 0) / xSize * 2 - 1, (y + 0) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(blVert);
			vec4 brVert = vec4((x + 1) / xSize * 2 - 1, (y + 0) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(brVert);
			vec4 tlVert = vec4((x + 0) / xSize * 2 - 1, (y + 1) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(tlVert);
			vec4 trVert = vec4((x + 1) / xSize * 2 - 1, (y + 1) / ySize * 2 - 1, 0.0f, 0.0f); distortVertex(trVert);

			vertexData[0][i + 0] = (blVert);
			vertexData[0][i + 1] = (brVert);
			vertexData[0][i + 2] = (tlVert);
			vertexData[0][i + 3] = (tlVert);
			vertexData[0][i + 4] = (brVert);
			vertexData[0][i + 5] = (trVert);

			vec4 colorOut = lerp(vec4(vec3(1.0f), 0.0f), color, interp);

			colorData[0][i + 0] = (colorOut);
			colorData[0][i + 1] = (colorOut);
			colorData[0][i + 2] = (colorOut);
			colorData[0][i + 3] = (colorOut);
			colorData[0][i + 4] = (colorOut);
			colorData[0][i + 5] = (colorOut);
		}
	}

	

	gridMesh.uploadToGPU();

	vertexData = &shineMesh.dataVertex;
	textureData = &shineMesh.dataTexture;
	normalData = &shineMesh.dataNormal;
	colorData = &shineMesh.dataColor;

	for (int x = 0, i = 0; x < xSize; ++x)
	{
		gridShine[x].update(dt);

		for (int y = 0; y < yShineSize; ++y, i += 6)
		{
			vec4 blVert = vec4((x + 0) / xSize * 2 - 1, ((y + 0) / ySize / 5.0f + (1 - gridShine[x].offset)) * 2 - 1, -0.1f, 0.0f);
			vec4 brVert = vec4((x + 1) / xSize * 2 - 1, ((y + 0) / ySize / 5.0f + (1 - gridShine[x].offset)) * 2 - 1, -0.1f, 0.0f);
			vec4 tlVert = vec4((x + 0) / xSize * 2 - 1, ((y + 1) / ySize / 5.0f + (1 - gridShine[x].offset)) * 2 - 1, -0.1f, 0.0f);
			vec4 trVert = vec4((x + 1) / xSize * 2 - 1, ((y + 1) / ySize / 5.0f + (1 - gridShine[x].offset)) * 2 - 1 , -0.1f, 0.0f);

			distortVertex(blVert);
			distortVertex(brVert);
			distortVertex(tlVert);
			distortVertex(trVert);

			vertexData[0][i + 0] = (blVert);
			vertexData[0][i + 1] = (brVert);
			vertexData[0][i + 2] = (tlVert);
			vertexData[0][i + 3] = (tlVert);
			vertexData[0][i + 4] = (brVert);
			vertexData[0][i + 5] = (trVert);

			vec4 bl = vec4((x + 0) / xSize, (y + 0) / ySize / 5.0f - gridShine[x].offset+1, 0.f, 0.f);
			vec4 br = vec4((x + 1) / xSize, (y + 0) / ySize / 5.0f - gridShine[x].offset+1, 0.f, 0.f);
			vec4 tl = vec4((x + 0) / xSize, (y + 1) / ySize / 5.0f - gridShine[x].offset+1, 0.f, 0.f);
			vec4 tr = vec4((x + 1) / xSize, (y + 1) / ySize / 5.0f - gridShine[x].offset+1, 0.f, 0.f);


			vec4 colorOut = lerp(vec4(vec3(1.0f), 0.0f), color, interp);

			colorData[0][i + 0] = (colorOut);
			colorData[0][i + 1] = (colorOut);
			colorData[0][i + 2] = (colorOut);
			colorData[0][i + 3] = (colorOut);
			colorData[0][i + 4] = (colorOut);
			colorData[0][i + 5] = (colorOut);

			float shineHeightT = static_cast<float>(y + 1) / yShineSize;
			float shineHeightB = static_cast<float>(y + 0) / yShineSize;

			vec4 colorB = colorOut * (interp::smoothStart4(interp::flip(shineHeightB)) * 4.0f + 1);
			vec4 colorT = colorOut * (interp::smoothStart4(interp::flip(shineHeightT)) * 4.0f + 1);
			
			colorT.w = interp; 
			colorB.w = interp;

			float texB = 0.0f;
			float texT = 0.0f;

			texB += interp::smoothStart3(interp::flip(shineHeightB)) * 0.1f * interp;
			texT += interp::smoothStart3(interp::flip(shineHeightT)) * 0.1f * interp;
			
			if (y == 0)
			{
				colorB = colorOut;
				texB = 0.0f;
			}
			if (y >= yShineSize - 1)
			{
				colorT = colorOut;
				texT = 0.0f;
			}

			colorData[0][i + 2] = lerp(colorOut, colorT, interp);
			colorData[0][i + 3] = lerp(colorOut, colorT, interp);
			colorData[0][i + 5] = lerp(colorOut, colorT, interp);
			colorData[0][i + 0] = lerp(colorOut, colorB, interp);
			colorData[0][i + 1] = lerp(colorOut, colorB, interp);
			colorData[0][i + 4] = lerp(colorOut, colorB, interp);
									
			textureData[0][i + 0] = (bl) + vec4(texB, 0, 0, 0);
			textureData[0][i + 1] = (br) + vec4(texB, 0, 0, 0);
			textureData[0][i + 2] = (tl) + vec4(texT, 0, 0, 0);
			textureData[0][i + 3] = (tl) + vec4(texT, 0, 0, 0);
			textureData[0][i + 4] = (br) + vec4(texB, 0, 0, 0);
			textureData[0][i + 5] = (tr) + vec4(texT, 0, 0, 0);

			//colorData[0][i + 0] = (colorOut);
			//colorData[0][i + 1] = (colorOut);
			//colorData[0][i + 2] = (colorOut);
			//colorData[0][i + 3] = (colorOut);
			//colorData[0][i + 4] = (colorOut);
			//colorData[0][i + 5] = (colorOut);


		}
	}

	shineMesh.uploadToGPU();
}

bool PauseMenuMesh::toggle()
{
	m_pActive = !m_pActive;
	return m_pActive;
}

bool PauseMenuMesh::getState()
{
	return m_pActive;
}

void PauseMenuMesh::draw()
{
	glDisable(GL_BLEND);
	m_pGridTex->bind(1);
	m_pBloomTex->bind(2);
	gridMesh.draw();
	shineMesh.draw();
	m_pBloomTex->unbind(2);
	m_pGridTex->unbind(1);
	glEnable(GL_BLEND);
}
