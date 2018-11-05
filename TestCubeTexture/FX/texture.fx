struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	Material gMaterial;
};

Texture2D gShaderResVar;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCCORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCCORD;
};

// mat     : ������ Ambient, Diffuse, Specular Light
// L       : ���౤�� Ambient, Diffuse, Specular Light
// normal  : ���� ����
// toEye   : �������� �þƷ� ���ϴ� ����
// ambient : [out] Ambient Light ���
// diffuse : [out] Diffuse Light ���
// spec    : [out] Specular Light ���
void ComputeDirectionalLight( Material mat, DirectionalLight L, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec )
{
	// ����� �ʱ�ȭ�Ѵ�.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �� ���ʹ� �Ի籤�� �ݴ� �����̴�. ��, �繰�� ǥ�鿡�� �������� ���� �����̴�.
	float3 lightVec = -L.Direction;

	// Ambient Light (�ֺ���,ȯ�汤) �� ���Ѵ�.
	ambient = mat.Ambient * L.Ambient;	

	// Diffuse Light (�л걤,���ݻ籤) �� ����� �� ���Ϳ� ǥ�� ���� ������ �������� ����Ѵ�.
	// ������ cosine �̹Ƿ� �� ���Ϳ� ǥ�� ���� ���Ͱ� 0 ���̸� �ִ밪�̰� 90 ���̸� ���� ����� 0 �� �ȴ�.
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if( diffuseFactor > 0.0f )
	{
		// �л걤 ���Ⱑ �����ϸ� �л걤�� ����Ѵ�.
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;

		// Specular Light (�ݿ���,���ݻ籤)�� ����Ѵ�.
		// r = �ݻ纤��
		// toEye = �þƺ��� : �������� �þƷ� ���ϴ� ����
		// �ݻ纤�Ϳ� �þƺ����� �������� �ݻ纤�Ϳ� �þƺ����� cosine �� ���� ��, mat.Specular.w ���� �ŵ������Ѵ�.
		float3 r         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(r, toEye), 0.0f), mat.Specular.w);

		spec    = specFactor * mat.Specular * L.Specular;
	}
}

VertexOut VS( VertexIn vin )
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	vout.Tex = vin.Tex;
	
	return vout;
}
  
float4 PS( VertexOut pin ) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW); 

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 texColor = gShaderResVar.Sample( samAnisotropic, pin.Tex );

	// Sum the light contribution from each light source.
	float4 A, D, S;

	ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
	ambient += A;  
	diffuse += D;
	spec    += S;
	   
	float4 litColor = texColor * ( ambient + diffuse ) + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a;

	return litColor;
}

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS() ) );
	}
}
