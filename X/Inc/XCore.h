//====================================================================================================
// Filename:	XCore.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_CORE_H
#define INCLUDED_XENGINE_CORE_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

//----------------------------------------------------------------------------------------------------

#if defined(_DEBUG)
	#define XLOG(format, ...)\
		do {\
			char _buffer[4096];\
			int ret = _snprintf_s(_buffer, std::size(_buffer), _TRUNCATE, "%s(%d) "##format, __FILE__, __LINE__, __VA_ARGS__);\
			OutputDebugStringA(_buffer);\
			if (ret == -1) OutputDebugStringA("** message truncated **\n");\
			OutputDebugStringA("\n");\
		} while (false)

	#define XASSERT(condition, format, ...)\
		do {\
			if (!(condition))\
			{\
				XLOG(format, __VA_ARGS__);\
				DebugBreak();\
			}\
		} while (false)

	#define XVERIFY(condition, format, ...)\
		do {\
			if (!(condition))\
			{\
				XLOG(format, __VA_ARGS__);\
				DebugBreak();\
			}\
		} while (false)
#else
	#define XLOG(format, ...)
	#define XASSERT(condition, format, ...)
	#define XVERIFY(condition, format, ...) condition;
#endif

//----------------------------------------------------------------------------------------------------

namespace X {

using WindowMessageHandler = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeDelete(T*& ptr)
{
	delete ptr;
	ptr = nullptr;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeDeleteArray(T*& ptr)
{
	delete[] ptr;
	ptr = nullptr;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeRelease(T*& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_CORE_H