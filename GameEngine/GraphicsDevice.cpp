#include "GraphicsDevice.h"
#include <windows.h>

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

GraphicDeviceHandler<ID3D11Device> Graphic::GetDevice()
{
	GraphicDeviceHandler<ID3D11Device> gdh = GraphicDeviceHandler<ID3D11Device>(&g_pd3dDevice);
	return gdh;
}

GraphicDeviceHandler<ID3D11DeviceContext> Graphic::GetDeviceContext()
{
	GraphicDeviceHandler<ID3D11DeviceContext> gdh = GraphicDeviceHandler<ID3D11DeviceContext>(&g_pd3dDeviceContext);
	return gdh;
}

GraphicDeviceHandler<IDXGISwapChain> Graphic::GetSwapChain()
{
	GraphicDeviceHandler<IDXGISwapChain> gdh = GraphicDeviceHandler<IDXGISwapChain>(&g_pSwapChain);
	return gdh;
}

void Graphic::CleanUp()
{
	STACK(1);
	g_pd3dDevice->Release();
	g_pd3dDeviceContext->Release();
	g_pSwapChain->Release();
}
