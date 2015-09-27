#include "GraphicsApp.h"

//SAFE_RELEASE moved to GameObject.h

using namespace DirectX;

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
	if (msg.wParam == VK_ESCAPE && msg.message == WM_KEYUP)
	{
		return -1;
	}

	return 0;
}

DirectX::XMVECTOR eye;
DirectX::XMMATRIX world;
void GraphicsApp::Update()
{
	static DWORD timeStart = timeGetTime();
	DWORD time = timeGetTime();
	//std::cout << (time - timeStart) << std::endl;
	_elapsedTime = (float)time - (float)timeStart;

	// Update view matrix
	eye = DirectX::XMVectorSet(-3.0f, 2.0f, -3.0f, 1.0f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&_viewMatrix, DirectX::XMMatrixLookAtLH(eye, at, up));

	//world = DirectX::XMMatrixTranslation(-1.0f, 0.0f, -1.0f) * DirectX::XMMatrixRotationY(sinf(_elapsedTime / 1000.0f) - cosf(_elapsedTime / 1000.0f));
	_object.Update(_elapsedTime);
}


void GraphicsApp::Draw()
{
	if (false)
	{ // Grey
		float clearColour[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		_deviceContext->ClearRenderTargetView(_renderTargetView, clearColour);
	}
	else
	{ // CornflowerBlue
		_deviceContext->ClearRenderTargetView(_renderTargetView, DirectX::Colors::CornflowerBlue);
	}
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	CBFrame cbFrame;
	DirectX::XMStoreFloat3(&cbFrame.eyePosW, eye);
	cbFrame.light.ambientLight = DirectX::XMFLOAT4(0.5, 0.5, 0.5f, 1.0f);
	cbFrame.light.diffuseLight = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cbFrame.light.specularLight = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cbFrame.light.lightVecW = DirectX::XMFLOAT3(0.0f, 1.0f, -1.0f);
	cbFrame.light.specularPower = 20.0f;
	cbFrame.pad = 0.0f;
	DirectX::XMStoreFloat4x4(&cbFrame.view, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&_viewMatrix)));
	_deviceContext->UpdateSubresource(_cbFrame, 0, nullptr, &cbFrame, 0, 0);

	//CBObject cbObject;
	//cbObject.ambientMtrl = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//cbObject.diffuseMtrl = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//cbObject.specularMtrl = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	////DirectX::XMStoreFloat4x4(&cbObject.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	//DirectX::XMStoreFloat4x4(&cbObject.world, DirectX::XMMatrixTranspose(world));
	//_deviceContext->UpdateSubresource(_cbObject, 0, nullptr, &cbObject, 0, 0);

	_deviceContext->VSSetShader(_vertexShader, nullptr, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, &_cbGlobal);
	_deviceContext->VSSetConstantBuffers(1, 1, &_cbFrame);
	//_deviceContext->VSSetConstantBuffers(2, 1, &_cbObject);

	_deviceContext->GSSetShader(_geometryShader, nullptr, 0);

	_deviceContext->PSSetShader(_pixelShader, nullptr, 0);
	_deviceContext->PSSetConstantBuffers(0, 1, &_cbGlobal);
	_deviceContext->PSSetConstantBuffers(1, 1, &_cbFrame);
	//_deviceContext->PSSetConstantBuffers(2, 1, &_cbObject);
	//_deviceContext->PSSetShaderResources(0, 1, &_texture);
	_deviceContext->PSSetSamplers(0, 1, &_samplerState);

	_deviceContext->IASetInputLayout(_vertexLayout);
	/*UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);*/

	_object.Draw(_deviceContext, _cbObject);

	_deviceContext->DrawIndexed(36, 0, 0);
	
	_swapChain->Present(0, 0);
}


HRESULT GraphicsApp::Initialise(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height)
{
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels [] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));
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
		Error(hr);
		return hr;
	}
	
	ID3D11Texture2D * backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	hr = _device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
	backBuffer->Release();
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	hr = _device->CreateTexture2D(&depthDesc, nullptr, &_depthStencilBuffer);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	/*D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));
	depthViewDesc.Format = depthDesc.Format;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;*/
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	_device->CreateDepthStencilState(&dssDesc, &_depthStencilState);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	_deviceContext->RSSetViewports(1, &viewport);

	
	hr = InitShaders();

	hr = InitBuffers();


	hr = _object.Initialise(_device);


	// Load the Texture
	/*hr = DirectX::CreateDDSTextureFromFile(_device, L"Assets\\Crate_COLOR.dds", nullptr, &_texture);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}*/

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _device->CreateSamplerState(&sampDesc, &_samplerState);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	// Initialize the world matrices
	//DirectX::XMStoreFloat4x4(&_worldMatrix, DirectX::XMMatrixIdentity());

	// Initialize the view matrix
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&_viewMatrix, DirectX::XMMatrixLookAtLH(eye, at, up));

	CBGlobal cbGlobal;
	DirectX::XMStoreFloat4x4(&_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width / (FLOAT)height, 0.1f, 100.0f));
	DirectX::XMStoreFloat4x4(&cbGlobal.projection, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&_projectionMatrix)));
	_deviceContext->UpdateSubresource(_cbGlobal, 0, nullptr, &cbGlobal, 0, 0);



	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	hr = _device->CreateRasterizerState(&cmdesc, &_rasterizerState);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}
	_deviceContext->RSSetState(_rasterizerState);

	return hr;
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		Error(hr);
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT GraphicsApp::InitShaders()
{
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"Shaders.fx", "VS", "vs_4_0", &pVSBlob);
	/*if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}*/
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	hr = _device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_vertexShader);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	hr = _device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_vertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	_deviceContext->IASetInputLayout(_vertexLayout);


	ID3DBlob * pGSBlob = nullptr;
	hr = CompileShaderFromFile(L"Shaders.fx", "GS", "gs_4_0", &pGSBlob);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	hr = _device->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), nullptr, &_geometryShader);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}


	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"Shaders.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	hr = _device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	return hr;
}

HRESULT GraphicsApp::InitBuffers()
{
	//// Create vertex buffer
	//Vertex vertices[] =
	//{
	//	{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	//	{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },
	//	{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
	//									 
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	//	{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },
	//									 
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	//	{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	//	{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) },
	//									 
	//	{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	//	{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) },
	//									 
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	//	{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	//	{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//									 
	//	{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
	//	{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },
	//	{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) },
	//};

	//D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	//ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = &vertices[0];
	//HRESULT hr = _device->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	//if (FAILED(hr))
	//{
	//	Error(hr);
	//	return hr;
	//}


	//// Set vertex buffer
	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	//// Create index buffer
	//WORD indices[] =
	//{
	//	3, 1, 0,
	//	2, 1, 3,

	//	6, 4, 5,
	//	7, 4, 6,

	//	11, 9, 8,
	//	10, 9, 11,

	//	14, 12, 13,
	//	15, 12, 14,

	//	19, 17, 16,
	//	18, 17, 19,

	//	22, 20, 21,
	//	23, 20, 22
	//};

	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	//ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = &indices[0];
	//hr = _device->CreateBuffer(&bd, &InitData, &_indexBuffer);
	//if (FAILED(hr))
	//{
	//	Error(hr);
	//	return hr;
	//}

	//// Set index buffer
	//_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Create the constant buffers
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBGlobal);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HRESULT hr = _device->CreateBuffer(&bd, nullptr, &_cbGlobal);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	bd.ByteWidth = sizeof(CBFrame);
	hr = _device->CreateBuffer(&bd, nullptr, &_cbFrame);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	bd.ByteWidth = sizeof(CBObject);
	hr = _device->CreateBuffer(&bd, nullptr, &_cbObject);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	return hr;
}


void GraphicsApp::Release()
{
	if (_deviceContext) _deviceContext->ClearState();

	/*SAFE_RELEASE(_texture);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_vertexBuffer);*/

	SAFE_RELEASE(_samplerState);
	SAFE_RELEASE(_cbObject);
	SAFE_RELEASE(_cbFrame);
	SAFE_RELEASE(_cbGlobal);
	SAFE_RELEASE(_vertexLayout);
	SAFE_RELEASE(_pixelShader);
	SAFE_RELEASE(_geometryShader);
	SAFE_RELEASE(_vertexShader);
	//SAFE_RELEASE(_depthStencilState); //!
	SAFE_RELEASE(_depthStencilView);
	SAFE_RELEASE(_depthStencilBuffer);
	SAFE_RELEASE(_renderTargetView);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_swapChain);
	SAFE_RELEASE(_device);
}
