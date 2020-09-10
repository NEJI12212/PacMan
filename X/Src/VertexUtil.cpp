//====================================================================================================
// Filename:	VertexUtil.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "VertexUtil.h"

using namespace X;

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexUtil::CreateVertexLayout(uint32_t vertexFormat)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

	uint32_t offset = 0;

	if (HasElement(vertexFormat, VE_Position))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "POSITION";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_Position);
	}
	if (HasElement(vertexFormat, VE_Normal))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "NORMAL";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_Normal);
	}
	if (HasElement(vertexFormat, VE_Tangent))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "TANGENT";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_Tangent);
	}
	if (HasElement(vertexFormat, VE_Texcoord))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "TEXCOORD";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_Texcoord);
	}
	if (HasElement(vertexFormat, VE_Color))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "COLOR";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_Color);
	}
	if (HasElement(vertexFormat, VE_BIndices))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "BLENDINDICES";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_BIndices);
	}
	if (HasElement(vertexFormat, VE_BWeights))
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC());
		D3D11_INPUT_ELEMENT_DESC& desc = layout.back();
		desc.SemanticName = "BLENDWEIGHT";
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		offset += GetElementSize(VE_BWeights);
	}

	return layout;
}