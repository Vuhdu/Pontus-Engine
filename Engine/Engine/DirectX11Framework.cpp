#include "stdafx.h"
#include "DirectX11Framework.h"

CDirectX11Framework::CDirectX11Framework()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
	myBackBuffer = nullptr;
	myDepthBuffer = nullptr;

	myEditorCameraTexture = nullptr;
	myEditorCameraResourceView = nullptr;
	myEditorCameraRenderTarget = nullptr;
	myEditorCameraDepthBuffer = nullptr;

	myMainCameraTexture = nullptr;
	myMainCameraResourceView = nullptr;
	myMainCameraRenderTarget = nullptr;
	myMainCameraDepthBuffer = nullptr;

	myWireFrameRasterizer = nullptr;
	mySolidRasterizer = nullptr;
}

CDirectX11Framework::~CDirectX11Framework()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
	myBackBuffer = nullptr;
	myDepthBuffer = nullptr;

	myEditorCameraTexture = nullptr;
	myEditorCameraResourceView = nullptr;
	myEditorCameraRenderTarget = nullptr;
	myEditorCameraDepthBuffer = nullptr;

	myMainCameraTexture = nullptr;
	myMainCameraResourceView = nullptr;
	myMainCameraRenderTarget = nullptr;
	myMainCameraDepthBuffer = nullptr;

	myWireFrameRasterizer = nullptr;
	mySolidRasterizer = nullptr;
}

bool CDirectX11Framework::Init(CWindowHandler* aWindowHandler)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapchainDescription = {};
	swapchainDescription.BufferCount = 1;
	swapchainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDescription.OutputWindow = aWindowHandler->GetHWND();
	swapchainDescription.SampleDesc.Count = 1;
	swapchainDescription.Windowed = true;

	result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
		&swapchainDescription, &mySwapChain, &myDevice, nullptr, &myContext);

	if (FAILED(result))
	{
		return false;
	}
	
	ID3D11Texture2D* backbufferTexture;

	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture);
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreateRenderTargetView(backbufferTexture, nullptr, &myBackBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = backbufferTexture->Release();
	if (FAILED(result))
	{
		return false;
	}
	
	// Temporary
	ID3D11Texture2D* depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthbufferDescription = { 0 };
	depthbufferDescription.Width = static_cast<unsigned int>(aWindowHandler->GetWidth());
	depthbufferDescription.Height = static_cast<unsigned int>(aWindowHandler->GetHeight());
	depthbufferDescription.ArraySize = 1;
	depthbufferDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthbufferDescription.SampleDesc.Count = 1;
	depthbufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	result = myDevice->CreateTexture2D(&depthbufferDescription, nullptr, &depthBufferTexture);
	if (FAILED(result))
	{
		return false;
	}
	
	result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &myDepthBuffer);
	if (FAILED(result))
	{
		return false;
	}

	myContext->OMSetRenderTargets(1, &myBackBuffer, myDepthBuffer);

	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(aWindowHandler->GetWidth());
	viewport.Height = static_cast<float>(aWindowHandler->GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);
	// Temporary
	
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	result = myDevice->CreateRasterizerState(&desc, &myWireFrameRasterizer);
	if (FAILED(result))
	{
		return false;
	}

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	result = myDevice->CreateRasterizerState(&desc, &mySolidRasterizer);
	if (FAILED(result))
	{
		return false;
	}
	SetRenderMode(eRenderMode::Solid);
	
	if (!InitEditorCameraRenderTarget(*aWindowHandler))
	{
		return false;
	}

	if (!InitMainCameraRenderTarget(*aWindowHandler))
	{
		return false;
	}
	return true;
}

void CDirectX11Framework::BeginFrame(std::array<float, 4> aClearColor)
{
	myContext->OMSetRenderTargets(1, &myBackBuffer, myDepthBuffer);
	myContext->ClearRenderTargetView(myBackBuffer, &aClearColor[0]);
	myContext->ClearRenderTargetView(myEditorCameraRenderTarget, &aClearColor[0]);
	myContext->ClearRenderTargetView(myMainCameraRenderTarget, &aClearColor[0]);
}

void CDirectX11Framework::EndFrame()
{
	mySwapChain->Present(1, 0);
}

ID3D11DeviceContext* CDirectX11Framework::GetContext()
{
	return myContext;
}

ID3D11Device* CDirectX11Framework::GetDevice()
{
	return myDevice;
}

ID3D11DepthStencilView* CDirectX11Framework::GetDepthBuffer()
{
	return myDepthBuffer;
}

ID3D11Texture2D* CDirectX11Framework::GetEditorCameraTexture2D()
{
	return myEditorCameraTexture;
}

ID3D11ShaderResourceView* CDirectX11Framework::GetEditorCameraRenderResourceView()
{
	return myEditorCameraResourceView;
}

ID3D11RenderTargetView* CDirectX11Framework::GetEditorCameraRenderTargetView()
{
	return myEditorCameraRenderTarget;
}

ID3D11Texture2D* CDirectX11Framework::GetMainCameraTexture2D()
{
	return myMainCameraTexture;
}

ID3D11ShaderResourceView* CDirectX11Framework::GetMainCameraRenderResourceView()
{
	return myMainCameraResourceView;
}

ID3D11RenderTargetView* CDirectX11Framework::GetMainCameraRenderTargetView()
{
	return myMainCameraRenderTarget;
}

void CDirectX11Framework::SetRenderMode(const eRenderMode& aRenderMode)
{
	switch (aRenderMode)
	{
	case eRenderMode::Solid:
		myContext->RSSetState(mySolidRasterizer);
		break;
	case eRenderMode::Wireframe:
		myContext->RSSetState(myWireFrameRasterizer);
		break;
	default:
		ERROR_PRINT("No valid rendermode was entered.");
		break;
	}
}

bool CDirectX11Framework::InitEditorCameraRenderTarget(const CWindowHandler& aWindowHandler)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = aWindowHandler.GetWidth();
	textureDesc.Height = aWindowHandler.GetHeight();
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = myDevice->CreateTexture2D(&textureDesc, nullptr, &myEditorCameraTexture);
	if (FAILED(result))
	{
		ERROR_PRINT("EditorCamera CreateTexture2D failed.");
		return false;
	}

	result = myDevice->CreateShaderResourceView(myEditorCameraTexture, nullptr, &myEditorCameraResourceView);
	if (FAILED(result))
	{
		ERROR_PRINT("EditorCamera CreateShaderResourceView failed.");
		return false;
	}

	result = myDevice->CreateRenderTargetView(myEditorCameraTexture, nullptr, &myEditorCameraRenderTarget);
	if (FAILED(result))
	{
		ERROR_PRINT("EditorCamera CreateRenderTargetView failed.");
		return false;
	}
	return true;
}

bool CDirectX11Framework::InitMainCameraRenderTarget(const CWindowHandler& aWindowHandler)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = aWindowHandler.GetWidth();
	textureDesc.Height = aWindowHandler.GetHeight();
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = myDevice->CreateTexture2D(&textureDesc, nullptr, &myMainCameraTexture);
	if (FAILED(result))
	{
		ERROR_PRINT("MainCamera CreateTexture2D failed.");
		return false;
	}

	result = myDevice->CreateShaderResourceView(myMainCameraTexture, nullptr, &myMainCameraResourceView);
	if (FAILED(result))
	{
		ERROR_PRINT("MainCamera CreateShaderResourceView failed.");
		return false;
	}

	result = myDevice->CreateRenderTargetView(myMainCameraTexture, nullptr, &myMainCameraRenderTarget);
	if (FAILED(result))
	{
		ERROR_PRINT("MainCamera CreateRenderTargetView failed.");
		return false;
	}
	return true;
}
