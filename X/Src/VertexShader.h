//====================================================================================================
// Filename:	VertexShader.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_VERTEXSHADER_H
#define INCLUDED_XENGINE_VERTEXSHADER_H

namespace X {

class VertexShader
{
public:
	VertexShader();
	~VertexShader();

	void Initialize(const char* filename, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat);
	void Initialize(const void* buffer, uint32_t size, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat);
	void Terminate();

	void Bind();

private:
	void InitializeInternal(ID3DBlob* shaderBlob, uint32_t vertexFormat);

	ID3D11VertexShader* mVertexShader;
	ID3D11InputLayout* mVertexLayout;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_VERTEXSHADER_H
