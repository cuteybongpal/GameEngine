#include "EditorCamera.h"
#include <iostream>
#include "Mesh.h"
#include "GraphicsDevice.h"
#include <d3dcompiler.h>
#include <filesystem>

//매 프레임마다 viewport에 그려지는 에디터 화면
void EditorCamera::Draw()
{
	//회색 배경으로 초기화
	float clearViewport[4] = { 0.35f, 0.35f, 0.35f, 1 };
	GraphicDeviceHandler<ID3D11DeviceContext> context = Graphic::GetDeviceContext();
	GraphicDeviceHandler<ID3D11Device> device = Graphic::GetDevice();

	context->OMSetRenderTargets(1, &this->rtv, nullptr);
	context->ClearRenderTargetView(this->rtv, clearViewport);
	//vs, ps 설정
	gridVertexShader.SetShader();
	gridPixelShader.SetShader();
	//inputLayout 설정
	//정점, 간선 설정
	context->IASetVertexBuffers(0, 1, this->quadMeshBuffer.vertexBuffer.GetAddressOf(), &this->quadMeshBuffer.stride, &this->quadMeshBuffer.offset);
	context->IASetIndexBuffer(this->quadMeshBuffer.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//상수 버퍼 설정
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = context->Map(this->editorConstantBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//실패 시, state 정리 후 리턴
	
	if (FAILED(hr))
	{
		std::cout << "editor constant buffer mapping failed in Draw() : EditorCamera" << std::endl;
		context->ClearState();
		return;
	}
	EditorConstantBuff* buff = (EditorConstantBuff*)mappedResource.pData;
	buff->cameraPosMatrix = position2Matrix4x4(this->pos);
	buff->cameraZoom = this->zoom;
	buff->proj = getIdentityMatrix4x4();
	buff->unitsize = UNITSIZE;
	buff->size = this->size;
	context->Unmap(this->editorConstantBuff, 0);
	gridVertexShader.SetConstantBuff(editorConstantBuff);
	gridPixelShader.SetConstantBuff(editorConstantBuff);
	//viewport 설정
	context->RSSetViewports(1, &this->viewport);
	context->DrawIndexed(this->quadMeshBuffer.indexCount, 0, 0);
	context->ClearState();
}

EditorCamera::EditorCamera()
{
	GraphicDeviceHandler<ID3D11Device> deviceHandler = Graphic::GetDevice();
	//에디터 뷰포트 텍스쳐의 기본적인 정보 초기화
	this->pos = { 0, 0, 0 };
	this->size = { 1920, 1080 };
	this->zoom = 1;

	D3D11_TEXTURE2D_DESC desc{};
	// 뷰포트 픽셀 너비
	desc.Width = 1920;
	// 뷰포트 픽셀 높이
	desc.Height = 1080;
	//밉맵 개수
	desc.MipLevels = 1;
	//텍스쳐 배열 크기
	desc.ArraySize = 1;
	//컬러 포맷
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//멀티 샘플링 설정
	//1이면 MSAA안 씀
	desc.SampleDesc.Count = 1;
	//GPU/CPU가 어떻게 사용할지
	//우리는 텍스쳐 그리고, 사용만 하기때문에, GPU전용으로 만들었음.
	desc.Usage = D3D11_USAGE_DEFAULT;
	//이 텍스쳐는 어떤 용도인가?
	//그리기 가능과 보기 가능(rtv, srv를 만들 수 있게 설정)
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // ⭐ 둘 다 필수
	//CPU가 직접 접근이 가능한가?
	//우린 ㄴㄴ임 그래서 0
	desc.CPUAccessFlags = 0;
	//특수 옵션
	desc.MiscFlags = 0;

	HRESULT hr = deviceHandler->CreateTexture2D(&desc, nullptr, &this->texture);
	if (FAILED(hr) || !this->texture)
	{
		std::cout << "Editor Camera texture generate failed!!" << std::endl;
		return;
	}

	hr = deviceHandler->CreateRenderTargetView(this->texture, nullptr, &this->rtv);
	if (FAILED(hr) || !this->rtv)
	{
		std::cout << "Editor Camera RTV generate failed!!" << std::endl;
		return;
	}

	hr = deviceHandler->CreateShaderResourceView(this->texture, nullptr, &this->srv);
	if (FAILED(hr) || !this->srv) 
	{
		std::cout << "Editor Camera SRV generate failed!!" << std::endl;
		return;
	}
	Mesh mesh{};
	mesh.vertexes =
	{
		{ {-1.f,  -1.f, 0.f}, {0.f, 0.f} }, // 왼쪽 아래
		{ { -1.f,  1.f, 0.f}, {1.f, 0.f} }, // 왼쪽 위
		{ { 1.f, 1.f, 0.f}, {1.f, 1.f} }, // 오른쪽 위
		{ {1.f, -1.f, 0.f}, {0.f, 1.f} }, // 오른쪽 아래
	};
	mesh.indexes = 
	{
		0, 1, 2,
		0, 2, 3
	};
	MeshBuffer mBuffer = meshToMeshBuffer(mesh);
	this->quadMeshBuffer = mBuffer;

	//vertexshader에 들어갈 매개변수에 대한 설명
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",                    // SemanticName
			0,                             // SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,   // float3
			0,                             // InputSlot
			0,                             // AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   // per-vertex
			0                              // InstanceDataStepRate
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,      // float2
			0,
			12,                            // pos 다음이니까 12바이트 뒤
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	//shader 생성하기
	gridVertexShader = Shader(ShaderType::VertexShader, L"GridVertexShader.cso", layout, ARRAYSIZE(layout));
	gridPixelShader = Shader(ShaderType::PixelShader, L"GridPixelShader.cso");
	
	//상수 버퍼 만들기
	
	D3D11_BUFFER_DESC cb_desc{};
	cb_desc.ByteWidth = sizeof(EditorConstantBuff);
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	deviceHandler->CreateBuffer(&cb_desc, nullptr, &this->editorConstantBuff);

	this->viewport.TopLeftX = 0;
	this->viewport.TopLeftY = 0;
	this->viewport.Width = size.x;
	this->viewport.Height = size.y;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
}