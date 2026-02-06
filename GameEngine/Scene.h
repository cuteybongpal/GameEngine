#pragma once
#include "engine.h"
#include "GameObject.h"
#include "Component.h"
#include "System.h"
#include <vector>

class Scene
{
public:
	std::vector<GameObject> gameObjects;
	std::vector<Component> components;
	std::vector<System> systems;
	float dt;
	float timeSpeed=1;
	void init();
	void update();
	void dead();
	Component* getComponent(uint64_t t);
};

