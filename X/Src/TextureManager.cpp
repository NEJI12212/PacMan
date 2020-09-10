//====================================================================================================
// Filename:	TextureManager.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "TextureManager.h"

#include "Texture.h"

using namespace X;

namespace
{
	TextureManager* sTextureManager = nullptr;
}

void TextureManager::StaticInitialize(const char* root)
{
	XASSERT(sTextureManager == nullptr, "[TextureManager] Manager already initialized!");
	sTextureManager = new TextureManager();
	sTextureManager->SetRootPath(root);
}

//----------------------------------------------------------------------------------------------------

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->Clear();
		SafeDelete(sTextureManager);
	}
}

//----------------------------------------------------------------------------------------------------

TextureManager* TextureManager::Get()
{
	XASSERT(sTextureManager != nullptr, "[TextureManager] No instance registered.");
	return sTextureManager;
}

//----------------------------------------------------------------------------------------------------

TextureManager::TextureManager()
{
}

//----------------------------------------------------------------------------------------------------

TextureManager::~TextureManager()
{
	XASSERT(mInventory.empty(), "[TextureManager] Clear() must be called to clean up.");
}

//----------------------------------------------------------------------------------------------------

void TextureManager::SetRootPath(const char* path)
{
	mRoot = path;
}

//----------------------------------------------------------------------------------------------------

TextureId TextureManager::Load(const char* fileName)
{
	std::string fullName = mRoot + "/" + fileName;

	std::hash<std::string> hasher;
	TextureId hash = hasher(fullName);

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		Texture* texture = new Texture();
		if (texture->Initialize(fullName.c_str()))
		{
			result.first->second = texture;
		}
		else
		{
			SafeDelete(texture);
			mInventory.erase(result.first);
			hash = 0;
		}
	}

	return hash;
}

//----------------------------------------------------------------------------------------------------

void TextureManager::Clear()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			SafeDelete(item.second);
		}
	}
	mInventory.clear();
}

//----------------------------------------------------------------------------------------------------

void TextureManager::BindVS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindVS(slot);
	}
}

//----------------------------------------------------------------------------------------------------

void TextureManager::BindPS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindPS(slot);
	}
}

//----------------------------------------------------------------------------------------------------

Texture* TextureManager::GetTexture(TextureId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second : nullptr;
}