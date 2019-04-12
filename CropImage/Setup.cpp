#include "StdAfx.h"
#include "Setup.h"

CSetup::CSetup() : m_iCropWidth(847), m_iCropHeight(551)
{
}

CSetup::~CSetup()
{
}

bool CSetup::Get()
{
	std::string strPath = GetProgramDirectory();
	strPath.append( "\\CropImage.ini" );

	m_iCropWidth = ::GetPrivateProfileInt( "CropImage", "Width", 847, strPath.c_str() );
	m_iCropHeight = ::GetPrivateProfileInt( "CropImage", "Height", 551, strPath.c_str() );

	return true;
}

bool CSetup::Put()
{


	return true;
}
