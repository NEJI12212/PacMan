//====================================================================================================
// Filename:	Texture.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_TEXTURE_H
#define INCLUDED_XENGINE_TEXTURE_H

namespace X {

class Texture
{
public:
	Texture();
	~Texture();
	
	bool Initialize(const char* fileName);
	bool Initialize(const void* data, uint32_t width, uint32_t height);
	void Terminate();
	
	void BindVS(uint32_t index);
	void BindPS(uint32_t index);

	ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }

	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }

private:
	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mShaderResourceView;
	uint32_t mWidth;
	uint32_t mHeight;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_TEXTURE_H