//====================================================================================================
// Filename:	VertexShader.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "VertexShader.h"

#include "GraphicsSystem.h"
#include "VertexUtil.h"

using namespace X;

//====================================================================================================
// Class Definitions
//====================================================================================================

VertexShader::VertexShader()
	: mVertexShader(nullptr)
	, mVertexLayout(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

VertexShader::~VertexShader()
{
	XASSERT(mVertexShader == nullptr, "[VertexShader] Shader not released!");
	XASSERT(mVertexLayout == nullptr, "[VertexShader] Shader not released!");
}

//----------------------------------------------------------------------------------------------------

void VertexShader::Initialize(const char* filename, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat)
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
	HRESULT hr = D3DCompileFromFile(wbuffer, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr) && errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	SafeRelease(errorBlob);

	InitializeInternal(shaderBlob, vertexFormat);
	SafeRelease(shaderBlob);
}

//----------------------------------------------------------------------------------------------------

void VertexShader::Initialize(const void* buffer, uint32_t size, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat)
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
	HRESULT hr = D3DCompile(buffer, size, nullptr, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr) && errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	SafeRelease(errorBlob);
	
	InitializeInternal(shaderBlob, vertexFormat);
	SafeRelease(shaderBlob);
}

//----------------------------------------------------------------------------------------------------

void VertexShader::InitializeInternal(ID3DBlob* shaderBlob, uint32_t vertexFormat)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout = VertexUtil::CreateVertexLayout(vertexFormat);

	// Create vertex buffer
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mVertexShader);
	XASSERT(SUCCEEDED(hr), "[VertexShader] Failed to create vertex shader.");

	// Create the input layout
	hr = device->CreateInputLayout(&vertexLayout[0], (UINT)vertexLayout.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &mVertexLayout);
	XASSERT(SUCCEEDED(hr), "[VertexShader] Failed to create input layout.");
}

//----------------------------------------------------------------------------------------------------

void VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mVertexLayout);
}

//----------------------------------------------------------------------------------------------------

void VertexShader::Bind()
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->IASetInputLayout(mVertexLayout);
}