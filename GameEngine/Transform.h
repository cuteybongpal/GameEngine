#pragma once
#include "Vector3.h"
#include "Component.h"

class Transform : Component
{
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};