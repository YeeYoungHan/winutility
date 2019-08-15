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
#include <sys/stat.h>

/**
 * @ingroup PcapViewer
 * @brief ���ڰ� ����� ���ڿ��� HEX �� ����� ���ڿ��� ��ȯ�Ѵ�.
 * @param pszInput	���ڰ� ����� ���ڿ�
 * @param iInputLen	pszInput ������ ����
 * @param strOutput [out] HEX �� ����� ���ڿ�
 */
void StringToHex( const char * pszInput, int iInputLen, std::string & strOutput )
{
	char szHex[3];

	strOutput.clear();

	for( int i = 0; i < iInputLen; ++i )
	{
		snprintf( szHex, sizeof(szHex), "%02x", (unsigned char)pszInput[i] );
		strOutput.append( szHex );
	}
}

/**
 * @ingroup PcapViewer
 * @brief ���ڿ��� ��� �������� �˻��Ѵ�.
 * @param pszText		���ڿ�
 * @param iTextLen	���ڿ� ����
 * @returns ���ڿ��� ��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsPrintString( const char * pszText, int iTextLen )
{
	for( int i = 0; i < iTextLen; ++i )
	{
		if( isprint( (unsigned char)pszText[i] ) == 0 ) 
		{
			if( pszText[i] == 0x0d || pszText[i] == 0x0a ) continue;

			return false;
		}
	}

	return true;
}

/**
 * @ingroup PcapViewer
 * @brief ������ �����ϴ��� �˻��Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsExistFile( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	return true;
}
