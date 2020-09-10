//====================================================================================================
// Filename:	VertexUtil.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_VERTEXUTIL_H
#define INCLUDED_XENGINE_VERTEXUTIL_H

#include "Vertex.h"

namespace X {
namespace VertexUtil {

inline int GetElementSize(VertexElement element)
{
#define Element(Name, Bit, Size)\
	if (element == VE_##Name) return Size;
#include "VertexElement.tup"
#undef Element
	return -1;
}

//----------------------------------------------------------------------------------------------------

inline bool HasElement(uint32_t vertexFormat, VertexElement element)
{
	return (vertexFormat & element) != 0;
}

//----------------------------------------------------------------------------------------------------

inline int GetElementOffset(uint32_t vertexFormat, VertexElement element)
{
	if (!HasElement(vertexFormat, element))
	{
		return -1;
	}

	int offset = 0;
#define Element(Name, Bit, Size)\
	if (element == VE_##Name)\
	{\
		return offset;\
	}\
	if (HasElement(vertexFormat, VE_##Name))\
	{\
		offset += Size;\
	}
#include "VertexElement.tup"
#undef Element
	return offset;
}

//----------------------------------------------------------------------------------------------------

inline uint32_t GetSize(uint32_t vertexFormat)
{
	uint32_t size = 0;
#define Element(Name, Bit, Size)\
	if (HasElement(vertexFormat, VE_##Name))\
	{\
		size += Size;\
	}
#include "VertexElement.tup"
#undef Element
	return size;
}

//----------------------------------------------------------------------------------------------------

std::vector<D3D11_INPUT_ELEMENT_DESC> CreateVertexLayout(uint32_t vertexFormat);

//----------------------------------------------------------------------------------------------------

template <typename T>
inline bool HasWeights(const T& vertex)
{
	return false;
}

template <>
inline bool HasWeights(const VertexPNTXB& vertex)
{
	return vertex.boneWeight[0] > 0.0f;
}

} // namespace VertexUtil
} // namespace X

#endif // #ifndef INCLUDED_XENGINE_VERTEXUTIL_H