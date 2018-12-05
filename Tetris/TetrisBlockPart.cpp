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
#include "TetrisBlock.h"
#include "trace.h"

CTetrisBlockPart::CTetrisBlockPart( float fLocalY, float fLocalZ, E_BOX_COLOR eColor ) : m_fLocalY(fLocalY), m_fLocalZ(fLocalZ), m_fWorldY(0.0f), m_fWorldZ(0.0f), m_eColor(eColor)
{
}

/**
 * @ingroup Tetris
 * @brief �Էµ� fY ��ŭ Y ������ �̵��ϰ� �Էµ� fZ ��ŭ Z ������ �̵��Ѵ�.
 * @param fY	Y ������ �̵��� ��
 * @param fZ	Z ������ �̵��� ��
 */
void CTetrisBlockPart::Move( float fY, float fZ )
{
	m_fWorldY += fY;
	m_fWorldZ += fZ;
}

/**
 * @ingroup Tetris
 * @brief ȸ���Ѵ�.
 */
void CTetrisBlockPart::Rotate()
{
	float fTemp = m_fLocalY;
	m_fLocalY = m_fLocalZ;
	m_fLocalZ = fTemp;

	m_fLocalZ *= -1;
}

/**
 * @ingroup Tetris
 * @brief ���� Y ���� �����Ѵ�.
 * @returns ���� Y ���� �����Ѵ�.
 */
float CTetrisBlockPart::GetY() const
{
	return m_fWorldY + m_fLocalY;
}

/**
 * @ingroup Tetris
 * @brief ���� Z ���� �����Ѵ�.
 * @returns ���� Z ���� �����Ѵ�.
 */
float CTetrisBlockPart::GetZ() const
{
	return m_fWorldZ + m_fLocalZ;
}

/**
 * @ingroup Tetris
 * @brief ������ Y ������ �˻��Ѵ�.
 * @param clsPart �˻� ��� ���簢�� ���� ���� ��ü
 * @returns ������ Y ���̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTetrisBlockPart::IsSameY( CTetrisBlockPart & clsPart )
{
	float fY = GetDistance( GetY(), clsPart.GetY() );

	if( fY <= BOX_WIDTH / 8 ) return true;

	return false;
}

/**
 * @ingroup Tetris
 * @brief �浹 ���θ� �˻��Ѵ�.
 * @param clsPart				�浹 �˻� ��� ���簢�� ���� ���� ��ü
 * @param bCheckBottom	�ϴ� �浹�� �˻��ϸ� true �� �Է��ϰ� �׷��� ������ false �� �Է��Ѵ�.
 * @returns �ϴܰ� �浹�Ǿ����� CT_BOTTOM �� �����ϰ� ��/��� �浹�Ǿ����� CT_LEFT_RIGHT �� �����Ѵ�. �浹���� �ʾ����� CT_NULL �� �����Ѵ�.
 */
E_COLLISION_TYPE CTetrisBlockPart::CheckCollision( CTetrisBlockPart & clsPart, bool bCheckBottom )
{
	float fMyY = GetY();
	float fMyZ = GetZ();
	float fOtherY = clsPart.GetY();
	float fOtherZ = clsPart.GetZ();
	float fY = GetDistance( fMyY, fOtherY );
	float fZ = GetDistance( fMyZ, fOtherZ );

	if( bCheckBottom )
	{
		if( fZ <= BOX_WIDTH / 8 )
		{
			if( fY <= BOX_WIDTH * 7/8 )
			{
				return CT_BOTTOM;
			}
		}
	}

	if( fY <= BOX_WIDTH / 8 )
	{
		if( fZ <= BOX_WIDTH * 7/8 ) 
		{
			return CT_LEFT_RIGHT;
		}
	}

	return CT_NULL;
}

/**
 * @ingroup Tetris
 * @brief ��Ʈ���� ����� ���簢�� ����Ʈ�� ������������ �����ϴ� �Լ�
 * @param clsFirst	ù��° ���簢�� ���� ���� ��ü
 * @param clsSecond �ι�° ���簢�� ���� ���� ��ü
 * @returns �ι�° ���簢�� ���� ���� ��ü�� ù��° ���簢�� ���� ���� ��ü���� �ڿ� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool TetrisBlockPartSort( const CTetrisBlockPart & clsFirst, const CTetrisBlockPart & clsSecond )
{
	float fFY = clsFirst.GetY();
	float fSY = clsSecond.GetY();
	float fY = GetDistance( fSY, fFY );

	if( fY <= BOX_WIDTH / 8 )
	{
		float fFZ = clsFirst.GetZ();
		float fSZ = clsSecond.GetZ();
		float fZ = GetDistance( fFZ, fSZ );

		if( fZ > BOX_WIDTH / 8 )
		{
			if( fSZ > fFZ ) return true;
		}
	}
	else
	{
		if( fSY > fFY ) return true;
	}

	return false;
}

/**
 * @ingroup Tetris
 * @brief �� ���� float �� �Ÿ� ��� �Լ�
 * @param f1 ù��° ��ġ
 * @param f2 �ι�° ��ġ
 * @returns �� ���� float �� �Ÿ��� �����Ѵ�.
 */
float GetDistance( float f1, float f2 )
{
	if( f1 >= f2 )
	{
		return f1 - f2;
	}

	return f2 - f1;
}
