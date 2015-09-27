#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Error.h"
#include "DDSTextureLoader.h"

#define SAFE_RELEASE(p) if(p != nullptr) p->Release();

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texcoord;
};
struct CBObject
{
	DirectX::XMFLOAT4X4 world;
	//DirectX::XMFLOAT4X4 boneTransforms[96];
	DirectX::XMFLOAT4 ambientMtrl;
	DirectX::XMFLOAT4 diffuseMtrl;
	DirectX::XMFLOAT4 specularMtrl;
};

class GameObject
{
private:
	ID3D11Buffer * _vertexBuffer = nullptr;
	ID3D11Buffer * _indexBuffer = nullptr;
	ID3D11ShaderResourceView * _texture = nullptr;
	DirectX::XMMATRIX _worldMatrix;

public:
	GameObject();
	~GameObject();

	HRESULT Initialise(ID3D11Device * device);
	void Update(float time);
	void Draw(ID3D11DeviceContext * context, ID3D11Buffer * constantBuffer);
};

