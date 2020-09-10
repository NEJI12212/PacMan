//====================================================================================================
// Filename:	Vertex.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"
#include "Vertex.h"

using namespace X;

//====================================================================================================
// Statics
//====================================================================================================

const uint32_t VertexP::Format		= VE_Position;
const uint32_t VertexPX::Format		= VE_Position | VE_Texcoord;
const uint32_t VertexPC::Format		= VE_Position | VE_Color;
const uint32_t VertexPNX::Format	= VE_Position | VE_Normal | VE_Texcoord;
const uint32_t VertexPNTX::Format	= VE_Position | VE_Normal | VE_Tangent | VE_Texcoord;
const uint32_t VertexPNTXB::Format	= VE_Position | VE_Normal | VE_Tangent | VE_Texcoord | VE_BIndices | VE_BWeights;
