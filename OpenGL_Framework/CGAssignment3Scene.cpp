#include "CGAssignment3Scene.h"

CGAssignment3Scene::CGAssignment3Scene(Game * game) : Scene(game)
{
	initializeScene();
}

void CGAssignment3Scene::initializeScene()
{
	meshSphere.initMeshSphere(32U, 32U);
	meshPlane.initMeshPlane(256U, 256U);

	std::vector<std::string> tex3Dfilename;
	tex3Dfilename.push_back("sand.jpg");
	tex3Dfilename.push_back("grass.jpg");
	tex3Dfilename.push_back("rock.jpg");
	tex3D = new Texture();
	tex3D->load3D(tex3Dfilename);
	texWater = Texture::add("water.png");
	texWaterSpecular = Texture::add("waterSpecular.png");
	texNoise = Texture::add("noise.png");

	goTerrain.setMesh(&meshPlane);
	goWater.setMesh(&meshPlane);
	goTerrain.setTextureBinds({ TextureBind(tex3D, 11), TextureBind(texNoise, 10) });
	goWater.setTextureBinds({ TextureBind(texWater, 0), TextureBind(texWaterSpecular, 2), TextureBind(texNoise, 10) });

	addChild(&goTerrain);
	addChild(&goWater);

	goWater.setLocalPos(vec3(0.0f, 0.0f, 0.0f));
	goTerrain.setLocalPos(vec3(0.0f, 0.0f, 0.0f));
	goWater.setScale(vec3(10.0f));
	goTerrain.setScale(vec3(10.0f));

	shaderWater = ResourceManager::addShaderProgramVertFrag("water.vert", "water.frag");
	shaderTerrain = ResourceManager::addShaderProgramVertFrag("terrain.vert", "terrain.frag");

	mainCamera = new Camera();
	addChild(mainCamera);

	mainCamera->perspective(60.0f, Window::getAspect(), 0.05f, 1000.0f);
	mainCamera->setLocalRotX(-15.0f);
	mainCamera->setLocalPos(vec3(0.0f, 10.0f, 20.0f));
}

void CGAssignment3Scene::update(float dt)
{
	Scene::update(dt);
}

void CGAssignment3Scene::draw()
{
	mainCamera->render();
	glEnable(GL_DEPTH_TEST);

	shaderTerrain->bind();
	shaderTerrain->sendUniform("uModel", goTerrain.getLocalToWorld());
	goTerrain.draw();

	shaderWater->bind();
	shaderWater->sendUniform("uModel", goWater.getLocalToWorld());
	goWater.draw();
}

void CGAssignment3Scene::reshape(int width, int height)
{
}
