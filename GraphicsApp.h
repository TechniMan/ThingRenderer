#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <iostream>

#include "DDSTextureLoader.h"
#include "Error.h"
#include "GameObject.h"



// Vertex moved to GameObject.h

struct Light {
	DirectX::XMFLOAT4 ambientLight;
	DirectX::XMFLOAT4 diffuseLight;
	DirectX::XMFLOAT4 specularLight;
	DirectX::XMFLOAT3 lightVecW;
	float specularPower;
};

struct CBGlobal
{
	DirectX::XMFLOAT4X4 projection;
};

struct CBFrame
{
	DirectX::XMFLOAT4X4 view;
	Light light;
	DirectX::XMFLOAT3 eyePosW;
	float pad;
};

// CBObject moved to GameObject.h



class GraphicsApp
{
private:
	// App
	ID3D11Device * _device = nullptr;
	ID3D11DeviceContext * _deviceContext = nullptr;
	IDXGISwapChain * _swapChain = nullptr;
	ID3D11RenderTargetView * _renderTargetView = nullptr;
	ID3D11Texture2D * _depthStencilBuffer = nullptr;
	ID3D11DepthStencilState * _depthStencilState = nullptr;
	ID3D11DepthStencilView * _depthStencilView = nullptr;
	ID3D11VertexShader * _vertexShader = nullptr;
	ID3D11GeometryShader * _geometryShader = nullptr;
	ID3D11PixelShader * _pixelShader = nullptr;
	ID3D11InputLayout * _vertexLayout = nullptr;
	ID3D11Buffer * _cbGlobal = nullptr;
	ID3D11Buffer * _cbFrame = nullptr;
	ID3D11Buffer * _cbObject = nullptr;
	ID3D11SamplerState * _samplerState = nullptr;
	ID3D11RasterizerState * _rasterizerState = nullptr;
	DirectX::XMFLOAT4X4 _viewMatrix;
	DirectX::XMFLOAT4X4 _projectionMatrix;

	// Object
	/*ID3D11Buffer * _vertexBuffer = nullptr;
	ID3D11Buffer * _indexBuffer = nullptr;
	ID3D11ShaderResourceView * _texture = nullptr;
	DirectX::XMFLOAT4X4 _worldMatrix;*/
	GameObject _object;

	float _elapsedTime = 0.0f;

public:
	GraphicsApp();
	~GraphicsApp();

	HRESULT Initialise(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height);
	HRESULT InitShaders();
	HRESULT InitBuffers();
	void Release();

	int HandleInput(MSG msg);
	void Update();
	void Draw();
};

