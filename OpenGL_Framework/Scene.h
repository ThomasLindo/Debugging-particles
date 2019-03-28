#pragma once
#include "Transform.h"
#include "Camera.h"

class Game;
class Scene : public Transform
{
public:
	Scene() = default;
	Scene(Game* game);
	~Scene();

	virtual void initializeScene();
	virtual void draw();
	void update(float dt);
	virtual void reshape(int width, int height);
	
	std::string getName();
	Camera* mainCamera;
	std::vector<Camera*> cameras;
	std::vector<Transform*> objects;
protected:
	float m_pTimeSinceLoad = 0.0f;
	Game* m_pGame;
	std::string m_pSceneName;
};