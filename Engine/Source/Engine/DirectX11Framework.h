#pragma once
#include "WindowHandler.h"

struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11DepthStencilView;

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

struct IDXGISwapChain;
struct ID3D11RasterizerState;

enum class eRenderMode 
{
	Solid,
	Wireframe
};

class CDirectX11Framework
{
public:
	CDirectX11Framework();
	~CDirectX11Framework();

	bool Init(CWindowHandler* aWindowHandler);

	void BeginFrame(std::array<float, 4> aClearColor);
	void EndFrame();

	ID3D11DeviceContext*	GetContext();
	ID3D11Device*			GetDevice();
	ID3D11DepthStencilView* GetDepthBuffer();

	ID3D11Texture2D*			GetEditorCameraTexture2D();
	ID3D11ShaderResourceView*	GetEditorCameraRenderResourceView();
	ID3D11RenderTargetView*		GetEditorCameraRenderTargetView();

	ID3D11Texture2D*			GetMainCameraTexture2D();
	ID3D11ShaderResourceView*	GetMainCameraRenderResourceView();
	ID3D11RenderTargetView*		GetMainCameraRenderTargetView();

	void SetRenderMode(const eRenderMode& aRenderMode);

public:
	ID3D11RenderTargetView* myBackBuffer;

private:
	bool InitEditorCameraRenderTarget(const CWindowHandler& aWindowHandler);
	bool InitMainCameraRenderTarget(const CWindowHandler& aWindowHandler);

private:
	IDXGISwapChain* mySwapChain;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;

	ID3D11Texture2D* myEditorCameraTexture;
	ID3D11ShaderResourceView* myEditorCameraResourceView;
	ID3D11RenderTargetView* myEditorCameraRenderTarget;
	ID3D11DepthStencilView* myEditorCameraDepthBuffer;

	ID3D11Texture2D* myMainCameraTexture;
	ID3D11ShaderResourceView* myMainCameraResourceView;
	ID3D11RenderTargetView* myMainCameraRenderTarget;
	ID3D11DepthStencilView* myMainCameraDepthBuffer;

	ID3D11RasterizerState* myWireFrameRasterizer;
	ID3D11RasterizerState* mySolidRasterizer;
	
	ID3D11DepthStencilView* myDepthBuffer;

};
