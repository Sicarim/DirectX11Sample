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

#define BOX_WIDTH	0.15f

#include <list>

/**
 * @ingroup Tetris
 * @brief 정사각형 Texture
 */
enum E_BOX_COLOR
{
	BC_BLUE = 0,
	BC_GREEN,
	BC_ORANGE,
	BC_RED,
	BC_SKYBLUE,
	BC_VILOET,
	BC_YELLOW,
	BC_BLACK,
	BC_BLACK_TOP,
	BC_BLACK_BOTTOM
};

/**
 * @ingroup Tetris
 * @brief 충돌 타입
 */
enum E_COLLISION_TYPE
{
	CT_NULL,
	CT_LEFT_RIGHT,
	CT_BOTTOM
};

/**
 * @ingroup Tetris
 * @brief 테트리스 블록의 하나의 정사각형 정보를 저장하는 클래스
 */
class CTetrisBlockPart
{
public:
	CTetrisBlockPart( float fLocalY, float fLocalZ, E_BOX_COLOR eColor );

	void Move( float fY, float fZ );
	void Rotate();
	float GetY() const;
	float GetZ() const;
	bool IsSameY( CTetrisBlockPart & clsPart );
	E_COLLISION_TYPE CheckCollision( CTetrisBlockPart & clsPart, bool bCheckBottom );

	E_BOX_COLOR m_eColor;

private:
	float	m_fLocalY;
	float	m_fLocalZ;
	float m_fWorldY;
	float m_fWorldZ;
};

typedef std::list< CTetrisBlockPart > TETRIS_BLOCK_PART_LIST;

bool TetrisBlockPartSort( const CTetrisBlockPart & clsFirst, const CTetrisBlockPart & clsSecond );
float GetDistance( float f1, float f2 );
