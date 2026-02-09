#pragma once
#include <cstdint>
#include <vector>
typedef uint64_t uuid;

typedef enum AssetType
{
	image,
	sound,
	prefab,
	scene,
	script
} assetType;