#pragma once
struct Vector3
{
public:
	float x;
	float y;
	float z;
	bool operator==(Vector3& other);
};