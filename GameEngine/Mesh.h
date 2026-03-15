#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <cstdint>
#include <wrl/client.h>
#include "engine.h"

using Microsoft::WRL::ComPtr;

struct Material;

struct Vertex
{
	Vector3 pos;
	Vector2 uv;
};

struct Mesh
{
	std::vector<Vertex> vertexes;
	std::vector<UINT> indexes;
	Material** material;
};

struct MeshBuffer
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	UINT indexCount = 0;
	Material** material;
};

struct Material
{

};
MeshBuffer meshToMeshBuffer(Mesh& mesh);