#include "DefaultScene.h"
#include "PauseEffect.h"
#include "Game.h"
#include "ResourceManager.h"
#include "PauseEffect.h"
#include "IO.h"
#include "Input.h"
#include "UI.h"

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
	unsigned int k = 7;
	//moved force locations out
	for (unsigned int idx = particleSystem->m_pNumParticles; idx > 0; --k) {
		vec3 forceLoc = forceLocations[k];
		//split clamp into 3 instead of using vector
		vec3 pos = particleSystem->m_pParticles[idx].position;
		float x = forceLoc.x - pos.x;
		float y = forceLoc.y - pos.y;
		float z = forceLoc.z - pos.z;
		vec3 locMinPos = vec3(x, y, z);
		float fx = 1.0f / locMinPos.x;
		float fy = 1.0f / locMinPos.y;
		float fz = 1.0f / locMinPos.z;
		vec3 force = vec3(fx, fy,fz);

		if (k <= 0) { --idx; k = 7; }

		particleSystem->m_pParticles[idx].force += force;
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
	goPlane.setScale(vec3(10.0f, 10.0f, 10.0f));
	goPlane.update(1);

	mainCamera = new Camera();
	addChild(&cameraPivot);
	cameraPivot.addChild(mainCamera);
	cameraPivot.setLocalRot(vec3(-45.0f, 0.0f, 0.0f));

	mainCamera->perspective(60.0f, Window::getAspect(), 0.05f, 1000.0f);
	//mainCamera->orthographic(-10.0f*getAspect, 10.0f*getAspect, -10.0f, 10.0f, -10.0f, 10.0f);
	mainCamera->setLocalPos(vec3(0.0f, 0.0f, 10.0f));
}

void DefaultScene::update(float dt)
{
	// Update particles
	updateParticleForce(&particles);
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
