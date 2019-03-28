#include "Scene.h"

Scene::Scene(Game * game)
{
	m_pGame = game;
}

Scene::~Scene()
{
}

void Scene::initializeScene()
{
}

void Scene::draw()
{
}

void Scene::update(float dt)
{
	m_pTimeSinceLoad += dt;
	Transform::update(dt);
}

void Scene::reshape(int width, int height)
{
}

std::string Scene::getName()
{
	return m_pSceneName;
}
