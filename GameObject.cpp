#include "GameObject.h"


GameObject::GameObject()
{

}


GameObject::~GameObject()
{
	SAFE_RELEASE(_textureSpecular);
	SAFE_RELEASE(_textureNormal);
	SAFE_RELEASE(_textureDiffuse);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_vertexBuffer);
}


HRESULT GameObject::Initialise(ID3D11Device * device)
{
	// Initialise world matrix
	_worldMatrix = DirectX::XMMatrixIdentity();

	// Load textures
	/*ID3D11Texture2D * texture = nullptr;
	CD3D11_TEXTURE2D_DESC texDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, 2U, 2U, 1U, 1U);
	D3D11_SUBRESOURCE_DATA texData;
	ZeroMemory(&texData, sizeof(D3D11_SUBRESOURCE_DATA));
	unsigned char tex [] = { 255, 0, 0, 255,
					 	0, 255, 0, 255,
						 0, 0, 255, 255,
						 255, 255, 255, 255 };
	texData.pSysMem = &tex[0];
	texData.SysMemPitch = sizeof(unsigned char) * ARRAYSIZE(tex);
	HRESULT hr = device->CreateTexture2D(&texDesc, &texData, &texture);
	hr = device->CreateShaderResourceView(texture, NULL, &_textureDiffuse);*/
	HRESULT hr = DirectX::CreateDDSTextureFromFile(device, L"Assets\\Crate_COLOUR.dds", nullptr, &_textureDiffuse);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}
	hr = DirectX::CreateDDSTextureFromFile(device, L"Assets\\Crate_NORMAL.dds", nullptr, &_textureNormal);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}
	hr = DirectX::CreateDDSTextureFromFile(device, L"Assets\\Crate_SPECULAR.dds", nullptr, &_textureSpecular);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	// Create vertex buffer
	/*Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
	};*/
	Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f),  DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f),  DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f),  DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f),  DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f),  DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f),  DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f),  DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f),  DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f),  DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &vertices[0];
	hr = device->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	// Create index buffer
	UINT16 indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT16) * ARRAYSIZE(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &indices[0];
	hr = device->CreateBuffer(&bd, &InitData, &_indexBuffer);
	if (FAILED(hr))
	{
		Error(hr);
		return hr;
	}

	return hr;
}


void GameObject::Update(float time)
{
	// Update world matrix
	float timef = time / 2000.0f;
	_worldMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, timef, 0.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
				* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
}


void GameObject::Draw(ID3D11DeviceContext * context, ID3D11Buffer * constantBuffer)
{
	// Update constant buffer
	CBObject cbObject;
	cbObject.ambientMtrl = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	cbObject.diffuseMtrl = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cbObject.specularMtrl = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//DirectX::XMStoreFloat4x4(&cbObject.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	DirectX::XMStoreFloat4x4(&cbObject.world, DirectX::XMMatrixTranspose(_worldMatrix)); ///!
	context->UpdateSubresource(constantBuffer, 0, nullptr, &cbObject, 0, 0);

	// Set primitive topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set resources
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, offset);
	context->PSSetShaderResources(0, 1, &_textureDiffuse);
	context->PSSetShaderResources(1, 1, &_textureNormal);
	context->PSSetShaderResources(2, 1, &_textureSpecular);
	context->VSSetConstantBuffers(2, 1, &constantBuffer);
	context->PSSetConstantBuffers(2, 1, &constantBuffer);

	context->DrawIndexed(36, 0, 0);
}
