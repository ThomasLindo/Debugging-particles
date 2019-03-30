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
	for (size_t k = forceLocations.size()-1; k > 0; --k)
	{
		//moved force locations out
		vec3 forceLoc = forceLocations[k];
		for (unsigned int idx = particleSystem->m_pNumParticles -1; idx >0; --idx)
		{
			//split clamp into 3 instead of using vector
			vec3 pos = particleSystem->m_pParticles[idx].position;
			float x = forceLoc.x - pos.x;
			float y = forceLoc.y - pos.y;
			float z = forceLoc.z - pos.z;
			vec3 locMinPos = vec3(x, y, z);
			float fx = 1.0f / locMinPos.x;
			float fy = 1.0f / locMinPos.y;
			float fz = 1.0f / locMinPos.z;
			vec3 force = vec3(fx,fy,fz);
			/*clamp(force.x, -10.0f, 10.0f);
			clamp(force.y, -10.0f, 10.0f);
			clamp(force.z, -10.0f, 10.0f);*/
			particleSystem->m_pParticles[idx].force += force;
		}
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
	particles.setTexture(rm::addTexture("SunAlpha.png"));
	particles.playing = true;
	
	shaderUnlit = ResourceManager::addShaderProgramVertFrag("shader.vert", "shaderUnlit.frag");
	shaderUnlitAlphaColor = ResourceManager::addShaderProgramVertFrag("shader.vert", "shaderUnlitAlphaColor.frag");

	fontCommodore = FontManager::initNewFont("CommodoreAngled.ttf", 48);
	//fontCommodore = FontManager::initNewFont("impact.ttf", 64);
	textTest.fontface = fontCommodore;
	textTest.text = std::string("Here are some particles");
	textTest.origin = vec2(500.0f, 100.0f);

	textTest.shake.amount = vec2(5.0f, 15.0f);
	textTest.shake.frequency = vec2(0.02f, 0.005f);
	textTest.shake.speed = vec2(8.0f);
	//textTest.shake.amount = vec2(2.0f);
	//textTest.shake.speed = vec2(23.21f, 30.0f);
	//textTest.shake.frequency = vec2(10.5f, 10.33f);

	textTest.init();

	// Whoops, this is really bad
	/*Mesh* plane = new Mesh();
	plane->initMeshPlane(16, 16);
	goPlane.setMesh(plane);
	goPlane.setTexture(rm::addTexture("grass.jpg"));*/

	mainCamera = new Camera();
	addChild(&cameraPivot);
	cameraPivot.addChild(mainCamera);

	mainCamera->perspective(60.0f, Window::getAspect(), 0.05f, 1000.0f);
	//mainCamera->orthographic(-10.0f*getAspect, 10.0f*getAspect, -10.0f, 10.0f, -10.0f, 10.0f);
	mainCamera->setLocalPos(vec3(0.0f, 0.0f, 10.0f));
}

void DefaultScene::update(float dt)
{
	// Update particles
	updateParticleForce(&particles);
	particles.update(dt);

	float rainbowSpeed = 2.5f;
	float rainbowFrequency = -0.01f;
	textTest.update(dt);
	//alpha made
	float alpha = rainbowFrequency + m_pTimeSinceLoad * rainbowSpeed;
	for (size_t i = 0; i < textTest.data.size(); ++i)
	{
		float offsetAmount = (textTest.data[i].x + textTest.data[i].y) * alpha;
		textTest.data[i].color = vec4(
			sinf(offsetAmount), sinf(offsetAmount + f_pi / 3.0f * 2.0f), sinf(offsetAmount + f_pi / 3.0f * 4.0f),
			1.0f);
		textTest.data[i].color += vec4(1.0f);
		textTest.data[i].color *= 0.5f;
	}

	Scene::update(dt);
}

void DefaultScene::draw()
{
	mainCamera->render();

	glEnable(GL_DEPTH_TEST);
	
	//shaderUnlit->bind();
	//shaderUnlit->sendUniform("uModel", goPlane.getLocalToWorld());
	//shaderUnlit->sendUniform("uColor", vec4(vec3(0.4906105f, 0.6732029f, 1.0f) * 0.5f, 1));
	//shaderUnlit->sendUniform("uTexST", vec4(3, 1, 0, 0));
	//goPlane.draw();

	glDisable(GL_DEPTH_TEST);
	
	shaderParticle->bind();
	shaderParticle->sendUniform("uModel", particles.m_pLocalToWorld);
	//particles.draw takes up a lo of cpu
	particles.draw();
	shaderParticle->unbind();

	textTest.draw();

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
