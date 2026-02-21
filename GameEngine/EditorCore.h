#pragma once
#include "Scene.h"
#include "Action.h"
#include <vector>
#include <stack>
#include "EditorCamera.h"

class EditorCore
{
public: 
	std::vector<Scene*> scenes;
	std::stack<Action*> changes;
	EditorCamera* camera;
};

