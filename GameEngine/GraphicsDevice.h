#pragma once
#include "engine.h"

//IUnknown만 받을 수 있게 즉, COM객체만 받을 수 있게 타입 제한
template<typename T,typename = std::enable_if_t<std::is_base_of<IUnknown, T>::value>>
class GraphicDeviceHandler
{
public:
	T* operator->() const
	{
		//COM 객체가 유효하지 않다면 __debugbreak()호출함
		CHECK((obj != nullptr));
		CHECK((*obj != nullptr));
		return *obj;
	}
	GraphicDeviceHandler(T** d)
	{
		obj = d;
	}
private:
	//COM객체를 직접 소유 하지 않음
	T** obj = nullptr;
};

class Graphic
{
public:
	static void SetDevice(ID3D11Device* device);
	static void SetDeviceContext(ID3D11DeviceContext* deviceContext);
	static void SetSwapChain(IDXGISwapChain* swapChain);

	static GraphicDeviceHandler<ID3D11Device> GetDevice();
	static GraphicDeviceHandler<ID3D11DeviceContext> GetDeviceContext();
	static GraphicDeviceHandler<IDXGISwapChain> GetSwapChain();

	static void CleanUp();
private:
	static ID3D11Device* g_pd3dDevice;
	static ID3D11DeviceContext* g_pd3dDeviceContext;
	static IDXGISwapChain* g_pSwapChain;
};
