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

#include "TestXNAMath.h"

/** Matrix ���� �Լ� �׽�Ʈ  */
void TestMatrixFunction()
{
	// Identity matrix �� �����Ѵ�.
	//+----------------------------------------+
	//| 1.000000, 0.000000, 0.000000, 0.000000 |
	//| 0.000000, 1.000000, 0.000000, 0.000000 |
	//| 0.000000, 0.000000, 1.000000, 0.000000 |
	//| 0.000000, 0.000000, 0.000000, 1.000000 |
	//+----------------------------------------+
	PrintMatrix( XMMatrixIdentity() );

	// Ȯ��/��� matrix �� �����Ѵ�.
	//+----------------------------------------+
	//| 3.000000, 0.000000, 0.000000, 0.000000 |
	//| 0.000000, 4.000000, 0.000000, 0.000000 |
	//| 0.000000, 0.000000, 5.000000, 0.000000 |
	//| 0.000000, 0.000000, 0.000000, 1.000000 |
	//+----------------------------------------+
	PrintMatrix( XMMatrixScaling( 3.0, 4.0, 5.0 ) );

	// �̵� matrix �� �����Ѵ�.
	//+----------------------------------------+
	//| 1.000000, 0.000000, 0.000000, 0.000000 |
	//| 0.000000, 1.000000, 0.000000, 0.000000 |
	//| 0.000000, 0.000000, 1.000000, 0.000000 |
	//| 3.000000, 4.000000, 5.000000, 1.000000 |
	//+----------------------------------------+
	PrintMatrix( XMMatrixTranslation( 3.0, 4.0, 5.0 ) );

	// X �� �������� 45�� ȸ���ϴ� matrix �� �����Ѵ�.
	//+----------------------------------------+
	//| 1.000000, 0.000000, 0.000000, 0.000000 |
	//| 0.000000, 0.707107, 0.707107, 0.000000 |
	//| 0.000000, -0.707107, 0.707107, 0.000000 |
	//| 0.000000, 0.000000, 0.000000, 1.000000 |
	//+----------------------------------------+
	PrintMatrix( XMMatrixRotationX( XM_PI / 4 ) );
}
