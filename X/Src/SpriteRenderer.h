//====================================================================================================
// Filename:	SpriteRenderer.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_SPRITERENDERER_H
#define INCLUDED_XENGINE_SPRITERENDERER_H

#include "XMath.h"
#include "XTypes.h"

namespace DirectX {	class CommonStates; class SpriteBatch; }

namespace X {

class Texture;

class SpriteRenderer
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SpriteRenderer* Get();

public:
	SpriteRenderer();
	~SpriteRenderer();

	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	void Initialize();
	void Terminate();

	void SetTransform(const Math::Matrix4& transform);

	void BeginRender();
	void EndRender();

	void Draw(const Texture& texture, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
	void Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center, Flip flip = Flip::None);

private:
	friend class Font;

	DirectX::CommonStates* mCommonStates;
	DirectX::SpriteBatch* mSpriteBatch;

	Math::Matrix4 mTransform;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_XENGINE_SPRITERENDERER_H