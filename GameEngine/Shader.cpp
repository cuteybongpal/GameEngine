#include "Shader.h"
#include <d3dcompiler.h>
#include "GraphicsDevice.h"

Shader::Shader(ShaderType type, LPCWSTR filename)
{
	ID3DBlob* blob = nullptr;
	D3DReadFileToBlob(filename, &blob);
	GraphicDeviceHandler<ID3D11Device> deviceHandler = Graphic::GetDevice();
	this->type = type;

	switch (type)
	{
	case ShaderType::VertexShader:
		deviceHandler->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, (ID3D11VertexShader**)&shader);
		break;
	case ShaderType::PixelShader:
		deviceHandler->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, (ID3D11PixelShader**)&shader);
		break;
	}
}

Shader::Shader(ShaderType type, LPCWSTR filename, D3D11_INPUT_ELEMENT_DESC* inputlayout_desc, int size)
{
	ID3DBlob* blob = nullptr;
	D3DReadFileToBlob(filename, &blob);
	GraphicDeviceHandler<ID3D11Device> deviceHandler = Graphic::GetDevice();
	this->type = type;

	switch (type)
	{
	case ShaderType::VertexShader:
		deviceHandler->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, (ID3D11VertexShader**)&shader);
		break;
	case ShaderType::PixelShader:
		deviceHandler->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, (ID3D11PixelShader**)&shader);
		break;
	}
	deviceHandler->CreateInputLayout(inputlayout_desc, size, blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
}

void Shader::SetConstantBuff(ID3D11Buffer* buff)
{
	GraphicDeviceHandler<ID3D11DeviceContext> contextHandler = Graphic::GetDeviceContext();

	switch (type)
	{
	case ShaderType::VertexShader:
		contextHandler->VSSetConstantBuffers(0, 1, &buff);
		break;
	case ShaderType::PixelShader:
		contextHandler->PSSetConstantBuffers(0, 1, &buff);
		break;
	}
}

void Shader::SetShader()
{
	GraphicDeviceHandler<ID3D11DeviceContext> contextHandler = Graphic::GetDeviceContext();
	switch (type)
	{
	case ShaderType::VertexShader:
		contextHandler->IASetInputLayout(inputLayout);
		contextHandler->VSSetShader((ID3D11VertexShader*)shader, nullptr, 0);
		break;
	case ShaderType::PixelShader:
		contextHandler->PSSetShader((ID3D11PixelShader*)shader, nullptr, 0);
		break;
	}
}
