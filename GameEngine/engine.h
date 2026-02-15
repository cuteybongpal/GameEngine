#pragma once
#include <cstdint>
#include <vector>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>

typedef uint64_t uuid;

typedef enum AssetType
{
	image,
	sound,
	prefab,
	scene,
	script
} assetType;