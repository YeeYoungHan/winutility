#include "StdAfx.h"
#include "Setup.h"

CSetup gclsSetup;

CSetup::CSetup() : m_iCropWidth(847), m_iCropHeight(551)
{
	m_strFilePath = GetProgramDirectory();
	m_strFilePath.append( "\\CropImage.ini" );
}

CSetup::~CSetup()
{
}

bool CSetup::Get()
{
	m_iCropWidth = ::GetPrivateProfileInt( SETUP_CROP_IMAGE, SETUP_CROP_WIDTH, 847, m_strFilePath.c_str() );
	m_iCropHeight = ::GetPrivateProfileInt( SETUP_CROP_IMAGE, SETUP_CROP_HEIGHT, 551, m_strFilePath.c_str() );

	return true;
}

bool CSetup::Put()
{
	SaveInt( SETUP_CROP_IMAGE, SETUP_CROP_WIDTH, m_iCropWidth );
	SaveInt( SETUP_CROP_IMAGE, SETUP_CROP_HEIGHT, m_iCropHeight );

	return true;
}

void CSetup::SaveInt( const char * pszAppName, const char * pszKeyName, int iValue )
{
	char szValue[11];

	_snprintf( szValue, sizeof(szValue), "%d", iValue );

	::WritePrivateProfileString( pszAppName, pszKeyName, szValue, m_strFilePath.c_str() );
}
