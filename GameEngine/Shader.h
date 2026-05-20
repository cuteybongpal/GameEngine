#pragma once
#include "engine.h"

enum class ShaderType
{
	VertexShader,
	PixelShader
};

class Shader
{
public:
	Shader() = default;
	//vs를 제외한 여러 셰이더
	Shader(ShaderType type, LPCWSTR filename);
	//요거는 vs 전용 inputlayout을 생성함
	Shader(ShaderType type, LPCWSTR filename, D3D11_INPUT_ELEMENT_DESC* inputlayout_desc, int size);
	void SetConstantBuff(ID3D11Buffer* buff);
	void SetShader();
	//복사 방지
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	//이동 허용
	Shader(Shader&&) noexcept = default;
	Shader& operator=(Shader&&) noexcept = default;

	//
private:
	ID3D11DeviceChild* shader;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* constantBuffer;
	ShaderType type;
};

