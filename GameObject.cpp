#include "GameObject.h"


GameObject::GameObject()
{

}


GameObject::~GameObject()
{
	SAFE_RELEASE(_texture);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_vertexBuffer);
}


HRESULT GameObject::Initialise(ID3D11Device * device)
{
	// Initialise world matrix
	_worldMatrix = DirectX::XMMatrixIdentity();

	// Load texture
	DirectX::CreateDDSTextureFromFile(device, L"Assets\\Crate_COLOR.dds", nullptr, &_texture);

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
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_vertexBuffer);
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
	float timef = time / 1000.0f;
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
	context->PSSetShaderResources(0, 1, &_texture);
	context->VSSetConstantBuffers(2, 1, &constantBuffer);
	context->PSSetConstantBuffers(2, 1, &constantBuffer);

	context->DrawIndexed(36, 0, 0);
}
