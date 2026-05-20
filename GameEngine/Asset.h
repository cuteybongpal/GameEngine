#pragma once
#include "engine.h"
#include <string>
class Asset
{
public:
	uuid _uuid;
	std::string name;
	AssetType type;
	void* meta;
};

