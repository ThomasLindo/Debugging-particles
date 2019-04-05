#include "DefaultScene.h"
#include "PauseEffect.h"
#include "Game.h"
#include "ResourceManager.h"
#include "PauseEffect.h"
#include "IO.h"
#include "Input.h"
#include "UI.h"
#include <iostream>

DefaultScene::DefaultScene(Game * game) : Scene(game)
{
	initializeScene();
}

DefaultScene::~DefaultScene()
{

}

constexpr float forceDistance = 5.0f;
std::vector<vec3> forceLocations = 
{ 
	vec3(-forceDistance, -forceDistance, -forceDistance), vec3(forceDistance, -forceDistance, -forceDistance), 
	vec3(-forceDistance,  forceDistance, -forceDistance), vec3(forceDistance,  forceDistance, -forceDistance),
	vec3(-forceDistance, -forceDistance,  forceDistance), vec3(forceDistance, -forceDistance,  forceDistance), 
	vec3(-forceDistance,  forceDistance,  forceDistance), vec3(forceDistance,  forceDistance,  forceDistance) 
};

void updateParticleForce(ParticleEmitter* particleSystem)
{	
	//revered loops

	vec3 *diffValue;
	float *diffValue2 = 0;
	//diffValue2 = &particleSystem->m_pParticles[particleSystem->m_pNumParticles].position.x;
	//moved force locations out
	//vec3* vec = forceLocations.data()+1;
	for (unsigned int idx = particleSystem->m_pNumParticles; idx > 0;--idx) {
		/*
		vec3 pos = particleSystem->m_pParticles[idx].position;
		if (counter > (particleSystem->m_pNumParticles*7)) {
				//diffValue2 prints out each memory address and the value assigned to each variable
				diffValue2 = &particleSystem->m_pParticles[idx].position.x;

				std::cout << "Particle #" << idx << ": " << &particleSystem->m_pParticles[idx] << std::endl;
				std::cout << "Pointer pointing to Address: " << diffValue2 << std::endl;
				std::cout << "Position: " << &particleSystem->m_pParticles[idx].position << " Pointing to Value: " << particleSystem->m_pParticles[idx].position << std::endl;
				std::cout << "Velocity: " << &particleSystem->m_pParticles[idx].velocity << " Pointing to Value: " << particleSystem->m_pParticles[idx].velocity << std::endl;
				std::cout << "Acceleration: " << &particleSystem->m_pParticles[idx].acceleration << " Pointing to Value: " << particleSystem->m_pParticles[idx].acceleration << std::endl;
				std::cout << "Mass: " << &particleSystem->m_pParticles[idx].mass << " Pointing to Value: " << particleSystem->m_pParticles[idx].mass << std::endl;
				std::cout << "Size: " << &particleSystem->m_pParticles[idx].size << " Pointing to Value: " << particleSystem->m_pParticles[idx].size << std::endl;
				std::cout << "Life: " << &particleSystem->m_pParticles[idx].life << " Pointing to Value: " << particleSystem->m_pParticles[idx].life << std::endl;
				std::cout << "Lifetime: " << &particleSystem->m_pParticles[idx].lifetime << " Pointing to Value: " << particleSystem->m_pParticles[idx].lifetime << std::endl;
				std::cout << "Force: " << &particleSystem->m_pParticles[idx].force << " Pointing to Value: " << particleSystem->m_pParticles[idx].force << std::endl;
				
				//diffValue2 prints out each memory address and the value assigned to each variable
				std::cout << "PointerPx: " << diffValue2 << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerPy: " << diffValue2 << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerPz: " << diffValue2 << "Pointer*: " << *diffValue2 << std::endl;
				//diffValue2 += 14;

				//3
				++diffValue2;
				std::cout << "PointerVx: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerVy: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerVz: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;

				//6
				++diffValue2;
				std::cout << "PointerAx: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerAy: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerAz: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;

				//9
				++diffValue2;
				std::cout << "PointerFx: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerFy: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerFz: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;

				//12
				++diffValue2;
				std::cout << "PointerM: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerS: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerL: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
				std::cout << "PointerLt: " << diffValue2 << std::endl;
				std::cout << "Pointer*: " << *diffValue2 << std::endl;
				++diffValue2;
			}
		else { counter++; }
		/**/
		//vec3* forceLoc = forceLocations.data() + k;
		//vec3 force = vec3(1.0f / (forceLoc->x - *diffValue2), 1.0f / (forceLoc->y - (*diffValue2 + 1)), 1.0f / (forceLoc->z - (*diffValue2 + 2)));
		//vec3 force = vec3(1.0f / *diffValue2, 1.0f / (*diffValue2 + 1), 1.0f / (*diffValue2 + 2));
		//diffValue2 += 16;


		diffValue2 = &particleSystem->m_pParticles[idx].position.x;
		//Force
		vec3 force = vec3();
		diffValue = &force;
		*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / ( forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / ( forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / (-forceDistance - *diffValue2), 1.0f / ( forceDistance - (*diffValue2 + 1)), 1.0f / ( forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / ( forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / ( forceDistance - *diffValue2), 1.0f / ( forceDistance - (*diffValue2 + 1)), 1.0f / (-forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / ( forceDistance - *diffValue2), 1.0f / (-forceDistance - (*diffValue2 + 1)), 1.0f / ( forceDistance - (*diffValue2 + 2)));
		*diffValue += vec3(1.0f / ( forceDistance - *diffValue2), 1.0f / ( forceDistance - (*diffValue2 + 1)), 1.0f / ( forceDistance - (*diffValue2 + 2)));
		particleSystem->m_pParticles[idx].force += *diffValue;

		//velocity
		//diffValue2 += 3;
		//*diffValue2 = random(velocityMin, velocityMax)
	}
}

void DefaultScene::initializeScene()
{
	shaderParticle = ResourceManager::addShaderProgramVertGeomFrag("Particle.vert", "Particle.geom", "Particle.frag"); 
	particles.initialize(1000);
	particles.lifeRange = vec2(1.0f, 5.0f);
	particles.sizeRange = vec2(0.1f);
	particles.velocityMin = vec3(-2.0f);
	particles.velocityMax = vec3(2.0f);
	particles.emitPosMin = vec3(-1.0f);
	particles.emitPosMax = vec3(1.0f);
	particles.massRange = vec2(1.0f, 10.0f);
	particles.setTexture(rm::addTexture("SunAlpha1.png"));
	particles.playing = true;
	
	shaderUnlit = ResourceManager::addShaderProgramVertFrag("shader.vert", "shaderUnlit.frag");
	shaderUnlitAlphaColor = ResourceManager::addShaderProgramVertFrag("shader.vert", "shaderUnlitAlphaColor.frag");

	// Whoops, this is really bad
	Mesh* plane = new Mesh();
	plane->initMeshPlane(16, 16);
	goPlane.setMesh(plane);
	goPlane.setTexture(rm::addTexture("grass.jpg"));
	goPlane.setScale(vec3(10.0f, 1.0f, 10.0f));
	goPlane.setLocalPos(vec3(0.0f, 0.0f, 0.0f));
	particles.CollisionPlane = goPlane.getLocalPos();
	particles.normal = vec3(0.0f, 1.0f, 0.0f);
	goPlane.update(1);


	mainCamera = new Camera();
	addChild(&cameraPivot);
	cameraPivot.addChild(mainCamera);
	cameraPivot.setLocalRot(vec3(0.0f, 0.0f, 0.0f));

	mainCamera->perspective(60.0f, Window::getAspect(), 0.05f, 1000.0f);
	//mainCamera->orthographic(-10.0f*getAspect, 10.0f*getAspect, -10.0f, 10.0f, -10.0f, 10.0f);
	mainCamera->setLocalPos(vec3(0.0f, 0.0f, 10.0f));
}

void DefaultScene::update(float dt)
{
	// Update particles
	//updateParticleForce(&particles);
	particles.update(dt);

	Scene::update(dt);
}

void DefaultScene::draw()
{
	mainCamera->render();

	glEnable(GL_DEPTH_TEST);
	
	shaderUnlit->bind();
	shaderUnlit->sendUniform("uModel", goPlane.getLocalToWorld());
	shaderUnlit->sendUniform("uColor", vec4(vec3(0.4906105f, 0.6732029f, 1.0f)*2.0f, 1));
	//shaderUnlit->sendUniform("uTexST", vec4(3, 1, 0, 0));
	goPlane.draw();

	glDisable(GL_DEPTH_TEST);
	
	shaderParticle->bind();
	shaderParticle->sendUniform("uModel", particles.m_pLocalToWorld);
	//particles.draw takes up a lo of cpu
	particles.draw();
	shaderParticle->unbind();


	glDisable(GL_BLEND);


#if ENABLE_GUI
	ImGui::Begin("Particles Scene");	
	
	ParticleEmitter* p = &particles;
	// Some temp parameters just so I can change one value instead of the following 7 lines
	float numDragSpeed = 0.01f;
	const char* numPrecision = "%.3f";
	float numPower = f_e;
	
	vec3 cameraRotation = cameraPivot.m_pLocalRotationEuler;
	if (ImGui::DragFloat3("Camera Rotation", &cameraRotation.x, 0.25f))
	{
		cameraPivot.m_pLocalRotationEuler = cameraRotation;
	}

	if (ImGui::TreeNode("Particle Settings"))
	{
		int numOfParticles = (int) p->m_pNumParticles;
		if (ImGui::InputInt("Number of Particles", &numOfParticles))
		{
			numOfParticles = clamp(numOfParticles, 1, 10000000);
			p->initialize((unsigned int)numOfParticles);
		}

		ImGui::DragFloat2("Lifetime Range", p->lifeRange.data,	numDragSpeed, 0.0f, 1000.0f, numPrecision, numPower);
		ImGui::DragFloat2("Size Range",		p->sizeRange.data,	numDragSpeed, 0.0f, 100.0f,  numPrecision, numPower);
		ImGui::DragFloat3("Velocity Min",	p->velocityMin.data,numDragSpeed, -50.0f, 50.0f, numPrecision, numPower);
		ImGui::DragFloat3("Velocity Max",	p->velocityMax.data,numDragSpeed, -50.0f, 50.0f, numPrecision, numPower);
		ImGui::ColorEdit4("Color Start",	p->colorStart.data, numDragSpeed);
		ImGui::ColorEdit4("Color End",		p->colorEnd.data,	numDragSpeed);
		ImGui::DragFloat2("Mass Range",		p->massRange.data,	numDragSpeed,  0.0f, 100.0f,  numPrecision, numPower);
		ImGui::DragFloat3("Emit Range Min",	p->emitPosMin.data,	numDragSpeed, -10.0f, 10.0f,  numPrecision, numPower);
		ImGui::DragFloat3("Emit Range Max",	p->emitPosMax.data,	numDragSpeed, -10.0f, 10.0f,  numPrecision, numPower);

		ImGui::TreePop();
	}
	ImGui::End();
#endif
}

void DefaultScene::reshape(int width, int height)
{
}
