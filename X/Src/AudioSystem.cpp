//====================================================================================================
// Filename:	AudioSystem.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "AudioSystem.h"

#include <DirectXTK/Inc/Audio.h>

using namespace DirectX;
using namespace X;

namespace
{
	AudioSystem* sAudioSystem = nullptr;
}

void AudioSystem::StaticInitialize()
{
	XASSERT(sAudioSystem == nullptr, "[AudioSystem] System already initialized!");
	sAudioSystem = new AudioSystem();
	sAudioSystem->Initialize();
}

//----------------------------------------------------------------------------------------------------

void AudioSystem::StaticTerminate()
{
	if (sAudioSystem != nullptr)
	{
		sAudioSystem->Terminate();
		SafeDelete(sAudioSystem);
	}
}

//----------------------------------------------------------------------------------------------------

AudioSystem* AudioSystem::Get()
{
	XASSERT(sAudioSystem != nullptr, "[AudioSystem] No system registered.");
	return sAudioSystem;
}

//----------------------------------------------------------------------------------------------------

AudioSystem::AudioSystem()
	: mAudioEngine(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

AudioSystem::~AudioSystem()
{
	XASSERT(mAudioEngine == nullptr, "[AudioSystem] Terminate() must be called to clean up!");
}

//----------------------------------------------------------------------------------------------------

void AudioSystem::Initialize()
{
	XASSERT(mAudioEngine == nullptr, "[AudioSystem] System already initialized.");

	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#if defined(_DEBUG)
	flags = flags | AudioEngine_Debug;
#endif

	mAudioEngine = new DirectX::AudioEngine(flags);
}

//----------------------------------------------------------------------------------------------------

void AudioSystem::Terminate()
{
	SafeDelete(mAudioEngine);
}

//----------------------------------------------------------------------------------------------------

void AudioSystem::Update()
{
	if (mAudioEngine && !mAudioEngine->Update())
	{
		// No audio device is active
		if (mAudioEngine->IsCriticalError())
		{
			XLOG("[AudioSystem] Critical Error. Shutting down.");
			SafeDelete(mAudioEngine);
		}
	}
}