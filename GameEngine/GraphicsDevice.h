#pragma once
#include "engine.h"
class Graphic
{
public:
	static void SetDevice(ID3D11Device* device);
	static void SetDeviceContext(ID3D11DeviceContext* deviceContext);
	static void SetSwapChain(IDXGISwapChain* swapChain);

	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();
	static IDXGISwapChain* GetSwapChain();      
private:
	static ID3D11Device* g_pd3dDevice;
	static ID3D11DeviceContext* g_pd3dDeviceContext;
	static IDXGISwapChain* g_pSwapChain;
};



