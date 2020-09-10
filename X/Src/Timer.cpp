//====================================================================================================
// Filename:	Timer.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "Timer.h"

using namespace X;

//----------------------------------------------------------------------------------------------------

Timer::Timer()
	: mElapsedTime(0.0f)
	, mTotalTime(0.0f)
	, mLastUpdateTime(0.0f)
	, mFrameSinceLastSecond(0.0f)
	, mFramesPerSecond(0.0f)
{
	mTicksPerSecond.QuadPart = 0;
	mLastTick.QuadPart = 0;
	mCurrentTick.QuadPart = 0;
}

//----------------------------------------------------------------------------------------------------

void Timer::Initialize()
{
	// Get the system clock frequency and current tick
	QueryPerformanceFrequency(&mTicksPerSecond);
	QueryPerformanceCounter(&mCurrentTick);

	mLastTick = mCurrentTick;
	
	// Reset
	mElapsedTime = 0.0f;
	mTotalTime = 0.0f;
	mLastUpdateTime = 0.0f;
	mFrameSinceLastSecond = 0.0f;
	mFramesPerSecond = 0.0f;
}

//----------------------------------------------------------------------------------------------------

void Timer::Update()
{
	// Get the current tick count
	QueryPerformanceCounter(&mCurrentTick);

	// Calculate the total time and elapsed time
	mElapsedTime = static_cast<float>(mCurrentTick.QuadPart - mLastTick.QuadPart) / mTicksPerSecond.QuadPart;
	mTotalTime += mElapsedTime;

	// Update the last tick count
	mLastTick = mCurrentTick;

	// Calculate the FPS
	mFrameSinceLastSecond += 1.0f;
	if (mTotalTime >= mLastUpdateTime + 1.0f)
	{
		mFramesPerSecond = mFrameSinceLastSecond / (mTotalTime - mLastUpdateTime);
		mFrameSinceLastSecond = 0.0f;
		mLastUpdateTime = mTotalTime;
	}
}

//----------------------------------------------------------------------------------------------------

float Timer::GetElapsedTime() const
{
	return mElapsedTime;
}

//----------------------------------------------------------------------------------------------------

float Timer::GetTotalTime() const
{
	return mTotalTime;
}

//----------------------------------------------------------------------------------------------------

float Timer::GetFramesPerSecond() const
{
	return mFramesPerSecond;
}