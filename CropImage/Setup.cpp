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

#include "StdAfx.h"
#include "Setup.h"

CSetup gclsSetup;

CSetup::CSetup() : m_iCropWidth(847), m_iCropHeight(551), m_iCropFileStartNo(1), m_bUseThumbnail(false), m_iThumbnailWidth(102), m_iThumbnailHeight(66)
{
	m_strFilePath = GetProgramDirectory();
	m_strFilePath.append( "\\CropImage.ini" );
}

CSetup::~CSetup()
{
}

bool CSetup::Get()
{
	char szTemp[1024];

	m_iCropWidth = ::GetPrivateProfileInt( SETUP_CROP_IMAGE, SETUP_WIDTH, 847, m_strFilePath.c_str() );
	m_iCropHeight = ::GetPrivateProfileInt( SETUP_CROP_IMAGE, SETUP_HEIGHT, 551, m_strFilePath.c_str() );
	m_iCropFileStartNo = ::GetPrivateProfileInt( SETUP_CROP_IMAGE, SETUP_START_NO, 1, m_strFilePath.c_str() );
	
	if( ::GetPrivateProfileString( SETUP_CROP_IMAGE, SETUP_OUTPUT_FOLDER, "", szTemp, sizeof(szTemp), m_strFilePath.c_str() ) > 0 )
	{
		m_strOutputFolderPath = szTemp;
	}

	int iTemp = ::GetPrivateProfileInt( SETUP_THUMBNAIL_IMAGE, SETUP_USE_THUMBNAIL, 0, m_strFilePath.c_str() );
	if( iTemp == 1 )
	{
		m_bUseThumbnail = true;
	}
	else
	{
		m_bUseThumbnail = false;
	}

	m_iThumbnailWidth = ::GetPrivateProfileInt( SETUP_THUMBNAIL_IMAGE, SETUP_WIDTH, 102, m_strFilePath.c_str() );
	m_iThumbnailHeight = ::GetPrivateProfileInt( SETUP_THUMBNAIL_IMAGE, SETUP_HEIGHT, 66, m_strFilePath.c_str() );

	if( ::GetPrivateProfileString( SETUP_THUMBNAIL_IMAGE, SETUP_THUMBNAIL_PREFIX, "", szTemp, sizeof(szTemp), m_strFilePath.c_str() ) > 0 )
	{
		m_strThumbnailPrefix = szTemp;
	}

	return true;
}

bool CSetup::Put()
{
	SaveInt( SETUP_CROP_IMAGE, SETUP_WIDTH, m_iCropWidth );
	SaveInt( SETUP_CROP_IMAGE, SETUP_HEIGHT, m_iCropHeight );
	SaveInt( SETUP_CROP_IMAGE, SETUP_START_NO, m_iCropFileStartNo );

	::WritePrivateProfileString( SETUP_CROP_IMAGE, SETUP_OUTPUT_FOLDER, m_strOutputFolderPath.c_str(), m_strFilePath.c_str() );

	SaveInt( SETUP_THUMBNAIL_IMAGE, SETUP_USE_THUMBNAIL, m_bUseThumbnail ? 1 : 0 );
	SaveInt( SETUP_THUMBNAIL_IMAGE, SETUP_WIDTH, m_iThumbnailWidth );
	SaveInt( SETUP_THUMBNAIL_IMAGE, SETUP_HEIGHT, m_iThumbnailHeight );

	::WritePrivateProfileString( SETUP_THUMBNAIL_IMAGE, SETUP_THUMBNAIL_PREFIX, m_strThumbnailPrefix.c_str(), m_strFilePath.c_str() );

	return true;
}

void CSetup::SaveInt( const char * pszAppName, const char * pszKeyName, int iValue )
{
	char szValue[11];

	_snprintf( szValue, sizeof(szValue), "%d", iValue );

	::WritePrivateProfileString( pszAppName, pszKeyName, szValue, m_strFilePath.c_str() );
}
