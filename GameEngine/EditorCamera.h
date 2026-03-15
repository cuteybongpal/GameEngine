#pragma once
#include "engine.h"
#include "Vector3.h"
#include "Mesh.h"
#include "Matrix4x4.h"

class EditorCamera
{
public:
	//에디터 화면 텍스쳐
	ID3D11Texture2D* texture;
	//텍스쳐 보기 전용 객체
	ID3D11ShaderResourceView* srv;
	//텍스쳐 그리기 전용 객체
	ID3D11RenderTargetView* rtv;
	
	//화면에 그려지는 사각형 메쉬
	MeshBuffer quadMeshBuffer;
	//그리는 함수
	void Draw();
	EditorCamera();
	//그리드 그리기용 vs, ps
	ID3D11VertexShader* gridVertexShader;
	ID3D11PixelShader* gridPixelShader;

	//inputLayout vs에 들어갈
	ID3D11InputLayout* gridInputLayout;
	ID3D11Buffer* editorConstantBuff;
	//viewport
	D3D11_VIEWPORT viewport;
	//상수 버퍼에 들어갈 값
	Vector2 size;
	Vector3 pos;
	float zoom;
};

//그리드 그릴 때 필요한 상수 버퍼 그리기
struct alignas(16) EditorConstantBuff
{
	Matrix4x4 proj;
	Matrix4x4 cameraPosMatrix;
	float cameraZoom;
	Vector2 size;
	float unitsize;
};