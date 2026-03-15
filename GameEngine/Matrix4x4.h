#pragma once
#include "Vector3.h"

class Matrix4x4
{
public:
	float m[4][4];
};

Matrix4x4 position2Matrix4x4(Vector3& position);
Matrix4x4 rotation2Matrix4x4(Vector3& rotation);
Matrix4x4 scale2Matrix4x4(Vector3& scale);
Matrix4x4 getIdentityMatrix4x4();
