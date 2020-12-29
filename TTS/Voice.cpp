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

#include "TTS.h"

void PrintVoiceList( )
{
	HRESULT hr;

	ISpObjectTokenCategory * pCategory = NULL;
	IEnumSpObjectTokens * pEnum = NULL;
	ULONG	iCount;

  CoInitialize(NULL);

	hr = CoCreateInstance( CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void **)&pCategory );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pCategory->SetId( SPCAT_VOICES, TRUE );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = pCategory->EnumTokens( NULL, NULL, &pEnum );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pEnum->GetCount( &iCount );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	while( SUCCEEDED( hr ) && iCount-- )
	{
		CComPtr<ISpObjectToken> pToken;
	
		hr = pEnum->Next( 1, &pToken, NULL );
		if( SUCCEEDED( hr ) )
		{
			WCHAR	* pszId;

			hr = pToken->GetId( &pszId );
			if( SUCCEEDED( hr ) )
			{
				int	i, iLen = (int)wcslen(pszId);

				for( i = iLen - 1; i >= 0; --i )
				{
					if( pszId[i] == _T('\\') )
					{
						std::string strId;

						WcharToChar( pszId + i + 1, strId );

						printf( "%s\n", strId.c_str() );
						break;
					}
				}
			}
		}
	}

FUNC_END:
	if( pEnum ) pEnum->Release();
	if( pCategory ) pCategory->Release();

	CoUninitialize();
}

bool VoiceSetup( const char * pszVoice, ISpVoice * pVoice )
{
	if( pszVoice == NULL || strlen(pszVoice) == 0 ) return false;
	if( pVoice == NULL ) return false;

	HRESULT hr;

	ISpObjectTokenCategory * pCategory = NULL;
	IEnumSpObjectTokens * pEnum = NULL;
	ULONG	iCount;
	bool bRes = false;

	hr = CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void **)&pCategory);
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pCategory->SetId( SPCAT_VOICES, TRUE );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = pCategory->EnumTokens( NULL, NULL, &pEnum );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pEnum->GetCount( &iCount );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	while( SUCCEEDED( hr ) && iCount-- )
	{
		CComPtr<ISpObjectToken> pToken;
	
		hr = pEnum->Next( 1, &pToken, NULL );
		if( SUCCEEDED( hr ) )
		{
			WCHAR	* pszId;

			hr = pToken->GetId( &pszId );
			if( SUCCEEDED( hr ) )
			{
				int	i, iLen = (int)wcslen(pszId);
				std::string strId;

				for( i = iLen - 1; i >= 0; --i )
				{
					if( pszId[i] == _T('\\') )
					{
						WcharToChar( pszId + i + 1, strId );
						break;
					}
				}

				if( !strcmp( strId.c_str(), pszVoice ) )
				{
					pVoice->SetVoice( pToken );
					bRes = true;
					break;
				}
			}
		}
	}

FUNC_END:
	if( pEnum ) pEnum->Release();
	if( pCategory ) pCategory->Release();

	return bRes;
}
