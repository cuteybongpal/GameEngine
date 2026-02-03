#pragma once
#include <string>
#include "engine.h"

class GameObject
{
public:
	uuid _uuid;
	std::string name;
	std::string tag;
	
private:
	//컴포넌트의 index를 저장함
	std::vector<uint64_t> components;
};

