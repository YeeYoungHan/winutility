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

#include "AccessLog.h"

CAccessLog::CAccessLog()
{
	Clear();
}

bool CAccessLog::Parse( const char * pszLog )
{
	bool bDateStart = false;
	bool bStringStart = false;
	int iPos = 0, iType = 0;

	Clear();

	for( int i = 0; pszLog[i]; ++i )
	{
		if( pszLog[i] == '[' )
		{
			bDateStart = true;
		}
		else if( pszLog[i] == ']' )
		{
			bDateStart = false;
		}
		else if( pszLog[i] == '"' )
		{
			if( bStringStart == false )
			{
				bStringStart = true;
			}
			else
			{
				bStringStart = false;
			}
		}

		if( bDateStart == false && bStringStart == false && pszLog[i] == ' ' )
		{
			switch( iType )
			{
			case 0:
				m_strClientIp.append( pszLog + iPos, i - iPos );
				break;
			case 3:
				{
					std::string strDate;

					strDate.append( pszLog + iPos, i - iPos );
					ParseDate( strDate.c_str() );
				}
				break;
			case 4:
				{
					std::string strRequest;

					strRequest.append( pszLog + iPos, i - iPos );
					ParseRequest( strRequest.c_str() );
				}
				break;
			case 5:
				{
					std::string strStatus;

					strStatus.append( pszLog + iPos, i - iPos );
					m_iStatus = atoi( strStatus.c_str() );
				}
				break;
			case 6:
				{
					std::string strSize;

					strSize.append( pszLog + iPos, i - iPos );
					m_iSize = atoi( strSize.c_str() );
				}
				break;
			}

			++iType;
			iPos = i + 1;

			if( iType > 6 ) break;
		}
	}

	return true;
}

void CAccessLog::Clear()
{
	m_strClientIp.clear();
	m_strDate.clear();
	m_strTime.clear();
	m_strPath.clear();
	m_iStatus = 0;
	m_iSize = 0;
}

bool CAccessLog::ParseDate( const char * pszDate )
{
	int iPos = 0;

	for( int i = 0; pszDate[i]; ++i )
	{
		if( pszDate[i] == '[' )
		{
			iPos = i + 1;
		}
		else if( pszDate[i] == ':' )
		{
			m_strDate.append( pszDate + iPos, i - iPos );
			m_strTime.append( pszDate + i + 1, 8 );
			return true;
		}
	}

	return false;
}

bool CAccessLog::ParseRequest( const char * pszRequest )
{
	int iPos = 0, iType = 0;

	for( int i = 0; pszRequest[i]; ++i )
	{
		if( pszRequest[i] == '"' )
		{
			iPos = i + 1;
		}
		else if( pszRequest[i] == ' ' )
		{
			if( iType == 1 )
			{
				m_strPath.append( pszRequest + iPos, i - iPos );
				return true;
			}
			
			++iType;
			iPos = i + 1;
		}
	}

	return false;
}
