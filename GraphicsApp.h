#pragma once

#include <d3d11.h>

#include "Error.h"

class GraphicsApp
{
private:
	ID3D11Device * _device = nullptr;
	ID3D11DeviceContext * _deviceContext = nullptr;
	IDXGISwapChain * _swapChain = nullptr;
	ID3D11Texture2D * _backBuffer = nullptr;
	ID3D11RenderTargetView * _renderTargetView = nullptr;

	UINT _previousKeyStateEscape;

public:
	GraphicsApp();
	~GraphicsApp();

	HRESULT Initialise(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height);
	void Release();
	int HandleInput(MSG msg);
	void Update();
	void Draw();
};

