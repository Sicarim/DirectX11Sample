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

void PrintVector( XMVECTOR & sttVec )
{
	printf( "[ %f, %f, %f, %f ]\n", sttVec.m128_f32[0], sttVec.m128_f32[1], sttVec.m128_f32[2], sttVec.m128_f32[3] );
}

void PrintFloat4( XMFLOAT4 & sttFloat4 )
{
	printf( "[ %f, %f, %f, %f ]\n", sttFloat4.x, sttFloat4.y, sttFloat4.z, sttFloat4.w );
}

/** XMVECTOR �Է�/��� �׽�Ʈ */
void TestVectorSetGet()
{
	XMVECTOR sttVec;

	// XMFLOAT2 ������ ����� ������ XMVECTOR �� �����Ѵ�.
	XMFLOAT2 sttFloat2( 1.0, 2.0 );

	sttVec = XMLoadFloat2( &sttFloat2 );
	PrintVector( sttVec );
	// [ 1.000000, 2.000000, 0.000000, 0.000000 ]

	// XMVECTOR �� z, w �� ���� �����Ѵ�.
	sttVec = XMVectorSetZ( sttVec, 3.0 );
	sttVec = XMVectorSetW( sttVec, 4.0 );

	// XMVECTOR �� ����� ������ XMFLOAT4 �� �����Ѵ�.
	XMFLOAT4 sttFloat4;

	XMStoreFloat4( &sttFloat4, sttVec );
	PrintFloat4( sttFloat4 );
	// [ 1.000000, 2.000000, 3.000000, 4.000000 ]

	// XMVECTOR �� x, y, z, w �� ����Ѵ�.
	printf( "[ %f, %f, %f, %f ]\n", XMVectorGetX( sttVec ), XMVectorGetY( sttVec ), XMVectorGetZ( sttVec ), XMVectorGetW( sttVec ) );
	// [ 1.000000, 2.000000, 3.000000, 4.000000 ]

	FLOAT fX;

	XMVectorGetXPtr( &fX, sttVec );
	printf( "x = %f\n", fX );
	// x = 1.000000
}

/** XMVECTOR ��� ���� �׽�Ʈ */
void TestVectorArithmetic()
{
	XMVECTOR sttVec;

	XMFLOAT4 sttFloat4( 1.0, 2.0, 3.0, 4.0 );
	sttVec = XMLoadFloat4( &sttFloat4 );

	// ���ϱ�
	// [ 2.000000, 4.000000, 6.000000, 8.000000 ] ��µȴ�.
	PrintVector( XMVectorAdd( sttVec, sttVec ) );

	// ����
	// [ 0.000000, 0.000000, 0.000000, 0.000000 ] ��µȴ�.
	PrintVector( XMVectorSubtract( sttVec, sttVec ) );

	// ������
	// [ 1.000000, 1.000000, 1.000000, 1.000000 ] ��µȴ�.
	PrintVector( XMVectorDivide( sttVec, sttVec ) );

	// ���ϱ�
	// [ 1.000000, 4.000000, 9.000000, 16.000000 ]
	PrintVector( XMVectorMultiply( sttVec, sttVec ) );
}

/** XMVECTOR length �׽�Ʈ */
void TestVectorLength()
{
	XMVECTOR sttVec;

	XMFLOAT4 sttFloat4( 1.0, 2.0, 3.0, 4.0 );
	sttVec = XMLoadFloat4( &sttFloat4 );

	// vector length = sqrt( x*x + y*y + z*z + w*w ) = sqrt( 1 + 4 + 9 + 16 ) = 5.4772255..
	// [ 5.477226, 5.477226, 5.477226, 5.477226 ] ��µȴ�.
	PrintVector( XMVector4Length( sttVec ) );

	// [ 5.477226, 5.477226, 5.477226, 5.477226 ] ��µȴ�.
	PrintVector( XMVector4LengthEst( sttVec ) );

	// XMVector4LengthSq = length * length = x*x + y*y + z*z + w*w
	// [ 30.000000, 30.000000, 30.000000, 30.000000 ]
	PrintVector( XMVector4LengthSq( sttVec ) );
}

/** XMVECTOR �� �������ͷ� ��ȯ�ϴ� �׽�Ʈ */
void TestVectorNormalize()
{
	XMVECTOR sttVec;

	XMFLOAT4 sttFloat4( 1.0, 2.0, 3.0, 4.0 );
	sttVec = XMLoadFloat4( &sttFloat4 );

	// unit vector = ( x / length, y / length, z / length, w / length ) = ( 0.18257, 0.36515, 0.54772, 0.73030 )
	// [ 0.182574, 0.365148, 0.547723, 0.730297 ]  ��µȴ�.
	PrintVector( XMVector4Normalize( sttVec ) );

	// [ 0.182556, 0.365112, 0.547668, 0.730225 ]  ��µȴ�.
	PrintVector( XMVector4NormalizeEst( sttVec ) );
}

/** XMVECTOR cross ���� �׽�Ʈ */
void TestVectorCross()
{
	XMFLOAT4 sttFloat1( 1.0, 2.0, 3.0, 0.0 );
	XMVECTOR sttVec1 = XMLoadFloat4( &sttFloat1 );

	XMFLOAT4 sttFloat2( -1.0, -2.0, 3.0, 0.0 );
	XMVECTOR sttVec2 = XMLoadFloat4( &sttFloat2 );

	// vector cross = ( vec1Y * vec2Z - vec1Z * vec2Y, vec1Z * vec2X - vec1X * vec2Z, vec1X * vec2Y - vec1Y * vec2X )
	//              = ( 2 * 3 - 3 * (-2), 3 * (-1) - 1 * 3, 1 * (-2) - 2 * (-1) )
	//              = ( 12, -6, 0 )
	// [ 12.000000, -6.000000, 0.000000, 0.000000 ] ��µȴ�.
	PrintVector( XMVector3Cross( sttVec1, sttVec2 ) );
}

/** XMVECTOR dot ���� �׽�Ʈ */
void TestVectorDot()
{
	XMFLOAT4 sttFloat1( 1.0, 2.0, 3.0, 0.0 );
	XMVECTOR sttVec1 = XMLoadFloat4( &sttFloat1 );

	XMFLOAT4 sttFloat2( -1.0, 2.0, 3.0, 0.0 );
	XMVECTOR sttVec2 = XMLoadFloat4( &sttFloat2 );

	// vector dot = vec1X * vec2X + vec1Y * vec2Y + vec1Z * vec2Z
	//            = 1 * (-1) + 2 * 2 + 3 * 3 = -1 + 4 + 9 = 12
	// [ 12.000000, 12.000000, 12.000000, 12.000000 ] ��µȴ�.
	PrintVector( XMVector3Dot( sttVec1, sttVec2 ) );
}
