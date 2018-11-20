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

#pragma once

#include "DirectX11.h"
#include "trace.h"

#define MAX_XY_ANGLE (XM_PI / 2.0f - 0.01f)

/**
 * @ingroup TestCubeTexture
 * @brief ���� ���� ����ü
 */
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Texture;
};

/**
 * @ingroup TestCubeTexture
 * @brief ī�޶� ��ġ�� �����ϴ� Ŭ����
 */
class CCamPos
{
public:
	CCamPos() : m_fDistance(5.0), m_fXZAngle(0.0), m_fXYAngle(0.0)
	{}

	/**
	 * @ingroup TestCubeTexture
	 * @brief ī�޶� ��ġ�� �����Ѵ�.
	 * @param iX X �� �̵� �Ÿ�
	 * @param iY Y �� �̵� �Ÿ�
	 */
	void Move( int iX, int iY )
	{
		m_fXZAngle += iX * XM_PI / 400;
		m_fXYAngle += iY * XM_PI / 400;

		// 2PI �� �ʰ��ϸ� 2PI ���Ϸ� �����Ѵ�.
		if( m_fXZAngle > XM_2PI )
		{
			m_fXZAngle -= XM_2PI;
		}
		else if( m_fXZAngle < ((-1)*XM_2PI) )
		{
			m_fXZAngle += XM_2PI;
		}

		// 2/PI �� �ʰ��ϸ� ī�޶��� X ��ġ�� ���̳ʽ��� �Ǿ�� �ϹǷ� 2/PI �� �ʰ����� �� �Ѵ�.
		if( m_fXYAngle > MAX_XY_ANGLE )
		{
			m_fXYAngle = MAX_XY_ANGLE;
		}
		else if( m_fXYAngle < ((-1) * MAX_XY_ANGLE) )
		{
			m_fXYAngle = (-1) * MAX_XY_ANGLE;
		}

		TRACE( "XZ(%.2f) XY(%.2f)\n", m_fXZAngle, m_fXYAngle );
	}

	/**
	 * @ingroup TestCubeTexture
	 * @brief ī�޶� ��ġ�� ������ VECTOR �� �����Ѵ�.
	 * @returns ī�޶� ��ġ�� ������ VECTOR �� �����Ѵ�.
	 */
	XMVECTOR GetVector()
	{
		XMVECTOR clsVec;

		clsVec.m128_f32[0] = m_fDistance * cos(m_fXYAngle) * cos(m_fXZAngle);
		clsVec.m128_f32[1] = m_fDistance * sin(m_fXYAngle);
		clsVec.m128_f32[2] = m_fDistance * cos(m_fXYAngle) * sin(m_fXZAngle);
		clsVec.m128_f32[3] = 1.0;

		return clsVec;
	}

	XMFLOAT3 GetEyePos()
	{
		XMVECTOR clsVec = GetVector();
		XMFLOAT3 f3EyePos = XMFLOAT3( clsVec.m128_f32[0], clsVec.m128_f32[1], clsVec.m128_f32[2] );

		return f3EyePos;
	}

	float m_fDistance;	// �߾ӿ��� ī�޶������ �Ÿ�
	float m_fXZAngle;		// ī�޶� �߾��� �ٷκ��� ��ġ���� X��� Z�� ������ ����
	float m_fXYAngle;		// ī�޶� �߾��� �ٷκ��� ��ġ���� X��� Y�� ������ ����
};

class CDirectionalLight
{
public:
	CDirectionalLight()
	{
		memset( this, 0, sizeof(CDirectionalLight) );
	}

	XMFLOAT4 m_f4Ambient;
	XMFLOAT4 m_f4Diffuse;
	XMFLOAT4 m_f4Specular;
	XMFLOAT3 m_f3Direction;
	float		 m_fPad;
};

class CMaterial
{
public:
	CMaterial()
	{
		memset( this, 0, sizeof(CMaterial) );
	}

	XMFLOAT4 m_f4Ambient;
	XMFLOAT4 m_f4Diffuse;
	XMFLOAT4 m_f4Specular; // w = SpecPower
	XMFLOAT4 m_f4Reflect;
};

/**
 * @ingroup TestCubeTexture
 * @brief 3D ť��
 */
class CDirectXCubeTexture : public CDirectX11
{
public:
	CDirectXCubeTexture();
	~CDirectXCubeTexture();

	virtual bool CreateChild();
	virtual bool DrawChild();

	bool Update();

	void OnMouseDown( HWND hWnd, int x, int y );
	void OnMouseUp( int x, int y );
	void OnMouseMove( HWND hWnd, int x, int y );

protected:
	CComPtr<ID3D11Buffer> m_pclsVB;
	CComPtr<ID3D11Buffer> m_pclsIB;
	CComPtr<ID3DX11Effect> m_pclsEffect;
	CComPtr<ID3D11ShaderResourceView> m_pclsShaderResView;

	ID3DX11EffectTechnique * m_pclsEffectTech;
	ID3DX11EffectMatrixVariable * m_pclsWorldViewProj;
	ID3DX11EffectMatrixVariable * m_pclsWorld;
	ID3DX11EffectMatrixVariable * m_pclsWorldInvTranspose;
	ID3DX11EffectVectorVariable * m_pclsEyePosW;
	ID3DX11EffectVariable * m_pclsDirectionalLight;
	ID3DX11EffectVariable * m_pclsMaterial;
	ID3DX11EffectShaderResourceVariable * m_pclsShaderResVar;

	CComPtr<ID3D11InputLayout> m_pclsInputLayout;

	XMFLOAT4X4 m_arrCubeWorld[1];
	XMFLOAT4X4 m_sttView;
	XMFLOAT4X4 m_sttProj;

	XMFLOAT3	m_f3EyePos;

	CDirectionalLight	m_clsDirectionalLight;
	CMaterial m_clsMaterial;

	CCamPos	m_clsCamPos;
	POINT		m_sttMousePos;
	bool		m_bMouseDown;

	int			m_iIndexCount;
};

