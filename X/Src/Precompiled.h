//====================================================================================================
// Filename:	Precompiled.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_PRECOMPILED_H
#define INCLUDED_XENGINE_PRECOMPILED_H

#define DIRECTINPUT_VERSION 0x0800

#include <algorithm>
#include <codecvt>
#include <list>
#include <locale>
#include <map>
#include <string>
#include <vector>

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <dinput.h>

#include "XCore.h"
#include "Forward.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#endif // #ifndef INCLUDED_XENGINE_PRECOMPILED_H
