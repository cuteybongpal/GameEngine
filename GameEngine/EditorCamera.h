#pragma once
#include "engine.h"

class EditorCamera
{
public:
	//에디터 화면 텍스쳐
	ID3D11Texture2D* texture;
	//텍스쳐 보기 전용 객체
	ID3D11ShaderResourceView* srv;
	//텍스쳐 그리기 전용 객체
	ID3D11RenderTargetView* rtv;
	//그리는 함수
	void Draw(ID3D11DeviceContext& context, IDXGISwapChain& swapchain);
	EditorCamera(ID3D11Device& device, ID3D11DeviceContext& context, IDXGISwapChain& swapchain);
};

