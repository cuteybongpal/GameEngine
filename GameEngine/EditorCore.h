#pragma once
#include "Scene.h"
#include "Action.h"
#include <vector>
#include <stack>

class EditorCore
{
public: 
	std::vector<Scene*> scenes;
	std::stack<Action*> changes;
};

