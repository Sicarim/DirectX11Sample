/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "stdafx.h"
#include "DirectXCubeLighting.h"
#include "trace.h"

CDirectXCubeLighting::CDirectXCubeLighting() : m_bMouseDown(false), m_iIndexCount(0)
{
	memset( &m_sttMousePos, 0, sizeof(m_sttMousePos) );
}

CDirectXCubeLighting::~CDirectXCubeLighting()
{
}

/**
 * @ingroup TestCubeLighting
 * @brief �ﰢ���� �׸��� ���� �غ� �۾��� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CDirectXCubeLighting::CreateChild()
{
	XMFLOAT3 f3Init = XMFLOAT3( 0.0,  0.0,  0.0 );

	// ť�� ���� ����
	Vertex arrCube[] =
	{
		{ XMFLOAT3(  0.5,  0.5,  0.5 ), f3Init },	// index 0
		{ XMFLOAT3(  0.5, -0.5,  0.5 ), f3Init },	// index 1
		{ XMFLOAT3( -0.5, -0.5,  0.5 ), f3Init },	// index 2
		{ XMFLOAT3( -0.5,  0.5,  0.5 ), f3Init },	// index 3
		{ XMFLOAT3(  0.5,  0.5, -0.5 ), f3Init },	// index 4
		{ XMFLOAT3(  0.5, -0.5, -0.5 ), f3Init },	// index 5
		{ XMFLOAT3( -0.5, -0.5, -0.5 ), f3Init },	// index 6
		{ XMFLOAT3( -0.5,  0.5, -0.5 ), f3Init },	// index 7
	};

	UINT arrIndex[] =
	{
		// �ո�
		0, 2, 1,
		0, 3, 2,

		// �޸�
		4, 6, 7,
		4, 5, 6,

		// ���ʸ�
		3, 6, 2,
		3, 7, 6,

		// �����ʸ�
		0, 1, 5,
		0, 5, 4,

		// ����
		0, 4, 7,
		0, 7, 3,

		// �Ʒ���
		1, 6, 5,
		1, 2, 6
	};

	// ���� ������ ����Ѵ�.
	int iVertexCount = _countof(arrCube);
	m_iIndexCount = _countof(arrIndex);
	UINT i0, i1, i2;
	XMVECTOR v0, v1, v2, vU, vV, vN;

	for( int i = 0; i < m_iIndexCount; i += 3 )
	{
		i0 = arrIndex[i];
		i1 = arrIndex[i+1];
		i2 = arrIndex[i+2];

		v0 = XMLoadFloat3( &arrCube[i0].Pos );
		v1 = XMLoadFloat3( &arrCube[i1].Pos );
		v2 = XMLoadFloat3( &arrCube[i2].Pos );

		vU = v1 - v0;
		vV = v2 - v0;

		vN = XMVector3Cross( vU, vV );
		vN = XMVector3Normalize( vN );

		v0 = XMLoadFloat3( &arrCube[i0].Normal );
		v1 = XMLoadFloat3( &arrCube[i1].Normal );
		v2 = XMLoadFloat3( &arrCube[i2].Normal );

		v0 += vN;
		v1 += vN;
		v2 += vN;

		XMStoreFloat3( &arrCube[i0].Normal, v0 );
		XMStoreFloat3( &arrCube[i1].Normal, v1 );
		XMStoreFloat3( &arrCube[i2].Normal, v2 );
	}

	for( int i = 0; i < iVertexCount; ++i )
	{
		vN = XMLoadFloat3( &arrCube[i].Normal );
		vN = XMVector3Normalize( vN );
		XMStoreFloat3( &arrCube[i].Normal, vN );
	}

	// ���� ���۸� �����Ѵ�.
	D3D11_BUFFER_DESC sttBD;
  
	sttBD.Usage = D3D11_USAGE_IMMUTABLE;
	sttBD.ByteWidth = sizeof(arrCube);
	sttBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	sttBD.CPUAccessFlags = 0;
	sttBD.MiscFlags = 0;
	sttBD.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sttSRD;
	sttSRD.pSysMem = arrCube;
  
	CHECK_FAILED( m_pclsDevice->CreateBuffer( &sttBD, &sttSRD, &m_pclsVB ) );

	// �ε��� ���۸� �����Ѵ�.
	sttBD.ByteWidth = sizeof(arrIndex);
	sttBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

	sttSRD.pSysMem = arrIndex;

	CHECK_FAILED( m_pclsDevice->CreateBuffer( &sttBD, &sttSRD, &m_pclsIB ) );

	// 
	if( CreateEffect( "FX/lighting.fxo", &m_pclsEffect ) == false ) return false;

	m_pclsEffectTech = m_pclsEffect->GetTechniqueByName("ColorTech");
	m_pclsWorldViewProj = m_pclsEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pclsWorld = m_pclsEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pclsWorldInvTranspose = m_pclsEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_pclsEyePosW = m_pclsEffect->GetVariableByName("gEyePosW")->AsVector();
	m_pclsDirectionalLight = m_pclsEffect->GetVariableByName("gDirLight");
	m_pclsMaterial = m_pclsEffect->GetVariableByName("gMaterial");

	D3D11_INPUT_ELEMENT_DESC arrVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3DX11_PASS_DESC sttPassDesc;

	m_pclsEffectTech->GetPassByIndex(0)->GetDesc( &sttPassDesc );
	CHECK_FAILED( m_pclsDevice->CreateInputLayout( arrVertexDesc, 2, sttPassDesc.pIAInputSignature, sttPassDesc.IAInputSignatureSize, &m_pclsInputLayout ) );

	XMMATRIX sttI = XMMatrixIdentity();
	XMStoreFloat4x4( &m_sttView, sttI );

	// 5.6.3 ī�޶� ���̴� ������ ����ϱ� ���ؼ� ���� ���� ��ȯ�� �ʿ��ϴ�.
	// ���� ���� ��ȯ�� ���ؼ� ���� �þ߰��� 45���̰� ��Ⱦ�� 1�̸� ����� ����� z=1 �̰� �� ����� z=1000 �� �������� ����� �����Ѵ�.
	XMMATRIX sttP = XMMatrixPerspectiveFovLH( 0.25f * XM_PI, 1.0f, 1.0f, 1000.0f );
	XMStoreFloat4x4( &m_sttProj, sttP );

	// ť�� ��ġ ����
	XMStoreFloat4x4( &m_arrCubeWorld[0], XMMatrixTranslation( 0.0f, 0.0f, 0.0f ) );

	// ���౤ ����
	m_clsDirectionalLight.m_f4Ambient = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
	m_clsDirectionalLight.m_f4Diffuse = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
	m_clsDirectionalLight.m_f4Specular = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
	m_clsDirectionalLight.m_f3Direction = XMFLOAT3( -0.5f, -0.5f, 0.5f );

	// ���� ���� (������)
	m_clsMaterial.m_f4Ambient = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	m_clsMaterial.m_f4Diffuse = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	m_clsMaterial.m_f4Specular = XMFLOAT4( 1.0f, 0.0f, 0.0f, 100.0f );

	return true;
}

/**
 * @ingroup TestCubeLighting
 * @brief �ﰢ���� ȭ�鿡 �׷��ش�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CDirectXCubeLighting::DrawChild()
{
	m_pclsContext->IASetInputLayout( m_pclsInputLayout );

	UINT iStride = sizeof(Vertex);
	UINT iOffset = 0;

	m_pclsContext->IASetVertexBuffers( 0, 1, &(m_pclsVB.p), &iStride, &iOffset );
	m_pclsContext->IASetIndexBuffer( m_pclsIB.p, DXGI_FORMAT_R32_UINT, 0 );

	XMMATRIX view  = XMLoadFloat4x4( &m_sttView );
	XMMATRIX proj  = XMLoadFloat4x4( &m_sttProj );
	XMMATRIX world, worldViewProj;

	m_pclsDirectionalLight->SetRawValue( &m_clsDirectionalLight, 0, sizeof(m_clsDirectionalLight) );
	m_pclsEyePosW->SetRawValue( &m_f3EyePos, 0, sizeof(m_f3EyePos) );
	
	D3DX11_TECHNIQUE_DESC sttTechDesc;
	m_pclsEffectTech->GetDesc( &sttTechDesc );

	for( UINT p = 0; p < sttTechDesc.Passes; ++p )
	{
		XMMATRIX world = XMLoadFloat4x4( &m_arrCubeWorld[0] ); 
		XMMATRIX worldViewProj = world * view * proj;

		XMMATRIX A = world;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		XMMATRIX worldInvTranspose = XMMatrixTranspose( XMMatrixInverse(&det, A) );

		m_pclsWorld->SetMatrix( (float*)&m_arrCubeWorld[0] );
		m_pclsWorldViewProj->SetMatrix( (float*)&worldViewProj );
		m_pclsWorldInvTranspose->SetMatrix( (float*)&worldInvTranspose );

		m_pclsMaterial->SetRawValue( &m_clsMaterial, 0, sizeof(m_clsMaterial) );

		m_pclsEffectTech->GetPassByIndex(p)->Apply( 0, m_pclsContext );
		m_pclsContext->DrawIndexed( m_iIndexCount, 0, 0 );
	}

	return true;
}

/**
 * @ingroup TestCubeLighting
 * @brief ī�޶� ��ġ�� ī�޶� ť�긦 �ٶ󺸴� ������ �̿��Ͽ��� View ����� �����Ѵ�.
 * @returns true �� �����Ѵ�.
 */
bool CDirectXCubeLighting::Update()
{
	// 5.6.2 ���� �������� �þ� �������� ��ǥ ��ȯ ������ ���� �þ� ����� �����Ѵ�.
	XMVECTOR pos = m_clsCamPos.GetVector();
	TRACE( "x(%.2f) y(%.2f) z(%.2f)\n", pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2] );

	// ť�� ��ġ
	XMVECTOR target = XMVectorZero();

	// ī�޶� Ÿ���� �ٶ� ���� ���� ����.
	XMVECTOR up = XMVectorSet( 0.0, 1.0, 0.0, 0.0 );

	// ī�޶� ��ġ�� ī�޶� ť�긦 �ٶ󺸴� ������ �̿��Ͽ��� View ����� �����Ѵ�.
	XMMATRIX view = XMMatrixLookAtLH( pos, target, up );

	XMStoreFloat4x4( &m_sttView, view );

	//
	m_f3EyePos = m_clsCamPos.GetEyePos();
	
	return true;
}

/**
 * @ingroup TestCubeLighting
 * @brief ���콺 ��ư down �̺�Ʈ �ڵ鷯
 * @param hWnd	������ �ڵ�
 * @param x			���콺 X ��ġ
 * @param y			���콺 Y ��ġ
 */
void CDirectXCubeLighting::OnMouseDown( HWND hWnd, int x, int y )
{
	m_sttMousePos.x = x;
	m_sttMousePos.y = y;
	m_bMouseDown = true;

	SetCapture( hWnd );
}

/**
 * @ingroup TestCubeLighting
 * @brief ���콺 ��ư up �̺�Ʈ �ڵ鷯
 * @param x			���콺 X ��ġ
 * @param y			���콺 Y ��ġ
 */
void CDirectXCubeLighting::OnMouseUp( int x, int y )
{
	ReleaseCapture();
	m_bMouseDown = false;
}

/**
 * @ingroup TestCubeLighting
 * @brief ���콺 �̵� �̺�Ʈ �ڵ鷯
 * @param hWnd	������ �ڵ�
 * @param x			���콺 X ��ġ
 * @param y			���콺 Y ��ġ
 */
void CDirectXCubeLighting::OnMouseMove( HWND hWnd, int x, int y )
{
	if( m_bMouseDown )
	{
		m_clsCamPos.Move( x - m_sttMousePos.x, y - m_sttMousePos.y );

		m_sttMousePos.x = x;
		m_sttMousePos.y = y;

		Update();
		Draw();
	}
}
