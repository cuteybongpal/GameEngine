#pragma once
#include "engine.h"
#include "Scene.h"
#include "Asset.h"
#include "EditorCore.h"
class EngineCore
{
public:
	EditorCore editorCore;
	void AddAsset(char* path);
	Asset* getAsset(char* key);
private:
	std::vector<Asset> assets;
};

