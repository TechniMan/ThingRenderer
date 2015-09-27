Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);

SamplerState samLinear : register(s0);

struct Light {
	float4 AmbientLight;
	float4 DiffuseLight;
	float4 SpecularLight;
	float3 LightVecW;
	float SpecularPower;
};

cbuffer cbGlobal : register(b0)
{
	matrix Projection;
}

cbuffer cbFrame : register(b1)
{
	matrix View;
	Light light;
	float3 EyePosW;
	float pad;
}

cbuffer cbObject : register(b2)
{
	matrix World;
	//matrix BoneTransforms[96];
	float4 AmbientMtrl;
	float4 DiffuseMtrl;
	float4 SpecularMtrl;
}


struct VS_IN
{
	float3 PosL : POSITION;
	float3 NormL : NORMAL;
	float2 Tex : TEXCOORD;
	//float4 Tan : TANGENT;
	//float3 Weights : WEIGHTS;
	//uint4  BoneIdx : BONEINDICES;
};

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormW : NORMAL;
	float2 Tex : TEXCOORD;
};



VS_OUT VS(VS_IN vertex)
{
	VS_OUT v;
	v.PosW = mul(float4(vertex.PosL, 1.0f), World).xyz;
	v.PosH = mul(float4(v.PosW, 1.0f), View);
	v.PosH = mul(v.PosH, Projection);
	v.NormW = mul(float4(normalize(vertex.NormL), 0.0f), World).xyz;
	v.NormW = normalize(v.NormW);
	v.Tex = vertex.Tex;
	return v;
}


[MaxVertexCount(3)]
void GS(triangle VS_OUT inVert[3], inout TriangleStream<VS_OUT> output)
{
	VS_OUT outVert = (VS_OUT)0;

	for (int i = 0; i < 3; ++i)
	{
		outVert.PosH  = inVert[i].PosH;
		outVert.PosW  = inVert[i].PosW;
		outVert.NormW = inVert[i].NormW;
		outVert.Tex   = inVert[i].Tex;

		output.Append(outVert);
	}
}


float4 PS(VS_OUT pixel) : SV_Target
{
	// Normalise vectors
	float3 lightDirection = normalize(light.LightVecW);
	float3 normal = normalize(pixel.NormW);
	
	// Get texture data
	float4 texDiffuse = txDiffuse.Sample(samLinear, pixel.Tex);
	//float4 texNormal = txNormal.Sample(samLinear, pixel.Tex);
	float4 texSpecular = txSpecular.Sample(samLinear, pixel.Tex);

	// Ambient
	float3 ambient = (light.AmbientLight.rgb * AmbientMtrl.rgb);

	// Diffuse
	float  diffuseAmount = max(dot(lightDirection, normal), 0.0f);
	float3 diffuse = (light.DiffuseLight.rgb * DiffuseMtrl.rgb) * diffuseAmount;

	// Specular
	float3 r = reflect(-lightDirection, normal);
	float3 toEye = normalize(EyePosW - pixel.PosW);
	float  specularAmount = pow(max(dot(r, toEye), 0.0f), light.SpecularPower);
	if (diffuseAmount <= 0.0f) specularAmount = 0.0f;
	float3 specular = (light.SpecularLight.rgb * SpecularMtrl.rgb) * specularAmount;

	// Final colour
	float3 finalColour = (texDiffuse.rgb * (diffuse + ambient)) + (specular * texSpecular.rgb);

	// Fog
	// fade finalColour towards dark grey depending on distance from eye

	return float4(finalColour.r, finalColour.g, finalColour.b, texDiffuse.a);
}