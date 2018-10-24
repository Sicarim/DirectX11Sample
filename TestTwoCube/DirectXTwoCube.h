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

#ifndef _DIRECT_X_TWO_CUBE_H_
#define _DIRECT_X_TWO_CUBE_H_

#include "DirectX11.h"
#include "trace.h"

#define MAX_XY_ANGLE (XM_PI / 2.0f - 0.01f)

/**
 * @ingroup TestTwoCube
 * @brief ���� ���� ����ü
 */
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

/**
 * @ingroup TestTwoCube
 * @brief ī�޶� ��ġ�� �����ϴ� Ŭ����
 */
class CCamPos
{
public:
	CCamPos() : m_fDistance(5.0), m_fXZAngle(0.0), m_fXYAngle(0.0)
	{}

	/**
	 * @ingroup TestTwoCube
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
	 * @ingroup TestTwoCube
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

	float m_fDistance;	// �߾ӿ��� ī�޶������ �Ÿ�
	float m_fXZAngle;		// ī�޶� �߾��� �ٷκ��� ��ġ���� X��� Z�� ������ ����
	float m_fXYAngle;		// ī�޶� �߾��� �ٷκ��� ��ġ���� X��� Y�� ������ ����
};

/**
 * @ingroup TestTwoCube
 * @brief 3D ť��
 */
class CDirectXTwoCube : public CDirectX11
{
public:
	CDirectXTwoCube();
	~CDirectXTwoCube();

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

	ID3DX11EffectTechnique * m_pclsEffectTech;
	ID3DX11EffectMatrixVariable * m_pclsWorldViewProj;

	CComPtr<ID3D11InputLayout> m_pclsInputLayout;

	XMFLOAT4X4 m_sttWorld;
	XMFLOAT4X4 m_sttView;
	XMFLOAT4X4 m_sttProj;

	XMFLOAT4X4 m_arrCubeWorld[2];

	CCamPos	m_clsCamPos;
	POINT		m_sttMousePos;
	bool		m_bMouseDown;

	int			m_iIndexCount;
};

#endif
