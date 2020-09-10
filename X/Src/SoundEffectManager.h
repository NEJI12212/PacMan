//====================================================================================================
// Filename:	SoundEffectManager.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_SOUNDEFFECTMANAGER_H
#define INCLUDED_XENGINE_SOUNDEFFECTMANAGER_H

#include "XTypes.h"

namespace DirectX { class SoundEffect; class SoundEffectInstance; }

namespace X {

class SoundEffectManager
{
public:
	static void StaticInitialize(const char* root);
	static void StaticTerminate();
	static SoundEffectManager* Get();

public:
	SoundEffectManager();
	~SoundEffectManager();

	SoundEffectManager(const SoundEffectManager&) = delete;
	SoundEffectManager& operator=(const SoundEffectManager&) = delete;

	void SetRootPath(const char* path);

	SoundId Load(const char* fileName);
	void Clear();

	void Play(SoundId id, bool loop = false);
	void Stop(SoundId id);
	bool IsPlaying(SoundId id) const;

private:
	struct Entry
	{
		std::unique_ptr<DirectX::SoundEffect> effect;
		std::unique_ptr<DirectX::SoundEffectInstance> instance;
	};

	std::string mRoot;
	std::unordered_map<std::size_t, std::unique_ptr<Entry>> mInventory;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_XENGINE_SOUNDEFFECTMANAGER_H