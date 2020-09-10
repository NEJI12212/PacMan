//====================================================================================================
// Filename:	PixelShader.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "PixelShader.h"

#include "GraphicsSystem.h"

using namespace X;

PixelShader::PixelShader()
	: mPixelShader(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

PixelShader::~PixelShader()
{
	XASSERT(mPixelShader == nullptr, "[PixelShader] Shader not released!");
}

//----------------------------------------------------------------------------------------------------

void PixelShader::Initialize(const char* filename, const char* entryPoint, const char* pixelShaderModel)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	
	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, filename, 1024);

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(wbuffer, nullptr, nullptr, entryPoint, pixelShaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr) && errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	SafeRelease(errorBlob);

	// Create pixel buffer
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	XASSERT(SUCCEEDED(hr), "[PixelShader] Failed to create pixel shader.");

	SafeRelease(shaderBlob);
}

//----------------------------------------------------------------------------------------------------

void PixelShader::Initialize(const void* buffer, uint32_t size, const char* entryPoint, const char* pixelShaderModel)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompile(buffer, size, nullptr, nullptr, nullptr, entryPoint, pixelShaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr) && errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	SafeRelease(errorBlob);

	// Create pixel buffer
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	XASSERT(SUCCEEDED(hr), "[PixelShader] Failed to create pixel shader.");

	SafeRelease(shaderBlob);
}

//----------------------------------------------------------------------------------------------------

void PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

//----------------------------------------------------------------------------------------------------

void PixelShader::Bind()
{
	GraphicsSystem::Get()->GetContext()->PSSetShader(mPixelShader, nullptr, 0);
}