#include "GraphicsApp.h"

#define SAFE_RELEASE(p) if(p != nullptr) p->Release();


GraphicsApp::GraphicsApp()
{

}


GraphicsApp::~GraphicsApp()
{
	Release();
}


int GraphicsApp::HandleInput(MSG msg)
{
	// When the user releases escape, exit
	if (msg.wParam == VK_ESCAPE && _previousKeyStateEscape == WM_KEYDOWN && msg.message == WM_KEYUP)
		return -1;
	// Keep track of Escape state
	if (msg.wParam = VK_ESCAPE)
		_previousKeyStateEscape = msg.message;

	return 0;
}


void GraphicsApp::Update()
{
	
}


void GraphicsApp::Draw()
{
	float clearColour [] = { 0.5f, 0.5f, 0.5f, 1.0f };
	_deviceContext->ClearRenderTargetView(_renderTargetView, clearColour);

	_swapChain->Present(0, 0);
}


HRESULT GraphicsApp::Initialise(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height)
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels [] = { D3D_FEATURE_LEVEL_11_0 };
	
	DXGI_SWAP_CHAIN_DESC swapDesc;
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapDesc, &_swapChain, &_device, nullptr, &_deviceContext);
	if (FAILED(hr))
	{
		Error(hWnd, hr);
		return hr;
	}
	
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_backBuffer));
	if (FAILED(hr))
	{
		Error(hWnd, hr);
		return hr;
	}

	hr = _device->CreateRenderTargetView(_backBuffer, NULL, &_renderTargetView);
	if (FAILED(hr))
	{
		Error(hWnd, hr);
		return hr;
	}
	
	return hr;
}


void GraphicsApp::Release()
{
	SAFE_RELEASE(_renderTargetView);
	//SAFE_RELEASE(_backBuffer); // Already released by the render target view?
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_swapChain);
	SAFE_RELEASE(_device);
}
