#pragma once
#include <cstdint>
#include <vector>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>
#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <string>
#include "Shader.h"

#pragma comment(lib, "Dbghelp.lib")

#define UNITSIZE 64
//x가 false라면 강제 중단함
#define CHECK(x) { if (!x) __debugbreak(); }

void PrintStackTrace(unsigned int skipFrames = 1);
#define STACK(x) { PrintStackTrace(x); }
typedef uint64_t uuid;

enum class AssetType
{ 
	image,
	sound,
	prefab,
	scene,
	script
};