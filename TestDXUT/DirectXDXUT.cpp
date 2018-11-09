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
#include "DirectXDXUT.h"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "comctl32.lib")

CDirectXDXUT::CDirectXDXUT()
{
}

CDirectXDXUT::~CDirectXDXUT()
{
}

/**
 * @ingroup TestTriangle
 * @brief �ﰢ���� �׸��� ���� �غ� �۾��� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CDirectXDXUT::CreateChild()
{
	DXUTInit( true, true );
	DXUTSetWindow( m_hWnd, m_hWnd, m_hWnd );
	DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 700, 700 );

	DXUTSetMediaSearchPath( _T("..\\LIbDXUT11\\Media\\") );

	CHECK_FAILED( m_clsDRM.OnD3D11CreateDevice( DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext() ) );

	m_clsDlg.Init( &m_clsDRM );
	m_clsDlg.SetLocation( 0, 0 );
	m_clsDlg.SetSize( 700, 700 );

	m_clsDlg.AddButton( IDC_BTN_1, _T("Button1"), 20, 30, 100, 20 );

	return true;
}

/**
 * @ingroup TestTriangle
 * @brief �ﰢ���� ȭ�鿡 �׷��ش�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CDirectXDXUT::DrawChild()
{
	DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"" );
	m_clsDlg.OnRender( 1.0 );
	DXUT_EndPerfEvent();

	return true;
}
