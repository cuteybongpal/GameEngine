#include "GraphicsDevice.h"
ID3D11Device* Graphic::g_pd3dDevice = nullptr;
ID3D11DeviceContext* Graphic::g_pd3dDeviceContext = nullptr;
IDXGISwapChain* Graphic::g_pSwapChain = nullptr;


void Graphic::SetDevice(ID3D11Device* device)
{
	g_pd3dDevice = device;
}

void Graphic::SetDeviceContext(ID3D11DeviceContext* deviceContext)
{
	g_pd3dDeviceContext = deviceContext;
}

void Graphic::SetSwapChain(IDXGISwapChain* swapChain)
{
	g_pSwapChain = swapChain;
}

ID3D11Device* Graphic::GetDevice()
{
	return g_pd3dDevice;
}

ID3D11DeviceContext* Graphic::GetDeviceContext()
{
	return g_pd3dDeviceContext;
}

IDXGISwapChain* Graphic::GetSwapChain()
{
	return g_pSwapChain;
}