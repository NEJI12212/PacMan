//====================================================================================================
// Filename:	GraphicsSystem.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_GRAPHICSSYSTEM_H
#define INCLUDED_XENGINE_GRAPHICSSYSTEM_H

#include "XColors.h"

namespace X {

class GraphicsSystem
{
public:
	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate();
	static GraphicsSystem* Get();

public:
	GraphicsSystem();
	~GraphicsSystem();

	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

	void BeginRender(const Color& clearColor = Colors::Black);
	void EndRender();

	void ToggleFullscreen();
	void ResetRenderTarget();
	void ResetViewport();

	void EnableDepthTesting(bool enable);

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	ID3D11Device* GetDevice()			{ return mD3DDevice; }
	ID3D11DeviceContext* GetContext()	{ return mImmediateContext; }

private:
	ID3D11Device* mD3DDevice;
	ID3D11DeviceContext* mImmediateContext;

	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11DepthStencilState* mDisableDepthStencil;

	D3D_DRIVER_TYPE mDriverType;
	D3D_FEATURE_LEVEL mFeatureLevel;

	DXGI_SWAP_CHAIN_DESC mSwapChainDesc;

	D3D11_VIEWPORT mViewport;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_GRAPHICSSYSTEM_H