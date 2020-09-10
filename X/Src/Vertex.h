//====================================================================================================
// Filename:	Vertex.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_VERTEX_H
#define INCLUDED_XENGINE_VERTEX_H

#include "XColors.h"
#include "XMath.h"

namespace X {

enum VertexElement
{
#define Element(Name, Bit, Size) VE_##Name = 0x1 << Bit,
#include "VertexElement.tup"
#undef Element
};

//----------------------------------------------------------------------------------------------------

struct VertexP
{
	static const uint32_t Format;
	Math::Vector3 position;
};

//----------------------------------------------------------------------------------------------------

struct VertexPX
{
	static const uint32_t Format;
	Math::Vector3 position;
	Math::Vector2 texcoord;
};

//----------------------------------------------------------------------------------------------------

struct VertexPC
{
	static const uint32_t Format;
	Math::Vector3 position;
	Color color;
};

//----------------------------------------------------------------------------------------------------

struct VertexPNX
{
	static const uint32_t Format;
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector2 texcoord;
};

//----------------------------------------------------------------------------------------------------

struct VertexPNTX
{
	static const uint32_t Format;
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 texcoord;
};

//----------------------------------------------------------------------------------------------------

struct VertexPNTXB
{
	static const uint32_t Format;

	VertexPNTXB()
	{
		memset(boneIndex, 0, sizeof(boneIndex));
		memset(boneWeight, 0, sizeof(boneWeight));
	}
	
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 texcoord;
	int boneIndex[4];
	float boneWeight[4];
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_VERTEX_H
