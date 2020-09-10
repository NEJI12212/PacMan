//====================================================================================================
// Filename:	PixelShader.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_PIXELSHADER_H
#define INCLUDED_XENGINE_PIXELSHADER_H

namespace X {

class PixelShader
{
public:
	PixelShader();
	~PixelShader();
	
	void Initialize(const char* filename, const char* entryPoint, const char* shaderModel);
	void Initialize(const void* buffer, uint32_t size, const char* entryPoint, const char* shaderModel);
	void Terminate();

	void Bind();

private:
	ID3D11PixelShader* mPixelShader;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_PIXELSHADER_H
