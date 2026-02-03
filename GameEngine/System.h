#pragma once
#include "engine.h"
#include "Component.h"

class System
{
public:
	virtual void Update() = 0;
protected:
	virtual void Commit(Component& component) = 0;
};

