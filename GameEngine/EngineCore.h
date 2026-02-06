#pragma once
#include "engine.h"
#include "Scene.h"
#include "Asset.h"
#include "EditorCore.h"
class EngineCore
{
public:
	std::vector<Asset> assets;
	EditorCore editorCore;
};

