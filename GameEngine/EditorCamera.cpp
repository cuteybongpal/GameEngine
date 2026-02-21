#include "EditorCamera.h"
#include <iostream>

void EditorCamera::Draw(ID3D11DeviceContext& context, IDXGISwapChain& swapchain)
{
	//회색 배경으로 초기화
	float clearViewport[4] = { 0.35f, 0.35f, 0.35f, 1 };
	context.OMSetRenderTargets(1, &rtv, nullptr);
	context.ClearRenderTargetView(rtv, clearViewport);
	//todo: 그리드
}
EditorCamera::EditorCamera(ID3D11Device& device, ID3D11DeviceContext& context, IDXGISwapChain& swapchain)
{
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

	HRESULT hr = device.CreateTexture2D(&desc, nullptr, &this->texture);
	if (FAILED(hr) || !this->texture)
	{
		std::cout << "Editor Camera texture generate failed!!" << std::endl;
		return;
	}
	hr = device.CreateRenderTargetView(this->texture, nullptr, &this->rtv);
	if (FAILED(hr) || !this->rtv)
	{
		std::cout << "Editor Camera RTV generate failed!!" << std::endl;
		return;
	}
	hr = device.CreateShaderResourceView(this->texture, nullptr, &this->srv);
	if (FAILED(hr) || !this->srv) 
	{
		std::cout << "Editor Camera SRV generate failed!!" << std::endl;
		return;
	}

}