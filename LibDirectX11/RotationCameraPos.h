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

#ifndef _ROTATION_CAMERA_POS_H_
#define _ROTATION_CAMERA_POS_H_

#include <d3dx11.h>
#include <xnamath.h>

#define MAX_XY_ANGLE (XM_PI / 2.0f - 0.01f)

/**
 * @ingroup LibDirectX11
 * @brief ȸ�� ī�޶� ��ġ�� �����ϴ� Ŭ����
 */
class CRotationCameraPos
{
public:
	CRotationCameraPos();
	~CRotationCameraPos();

	void Move( int iX, int iY );
	XMVECTOR GetVector();
	XMFLOAT3 GetEyePos();

private:
	float m_fDistance;	// �߾ӿ��� ī�޶������ �Ÿ�
	float m_fXZAngle;		// ī�޶� �߾��� �ٷκ��� ��ġ���� X��� Z�� ������ ����
	float m_fXYAngle;		// ī�޶� �߾��� �ٷκ��� ��ġ���� X��� Y�� ������ ����
};

#endif
