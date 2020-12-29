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

bool TTS( const char * pszVoice, const char * pszText, const char * pszWaveFile )
{
	HRESULT				hr = S_OK;
	CComPtr <ISpVoice>		cpVoice;
	CComPtr <ISpStream>		cpStream;
	CSpStreamFormat			cAudioFmt;

	CoInitialize(NULL);

	hr = cpVoice.CoCreateInstance( CLSID_SpVoice );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = cAudioFmt.AssignFormat( SPSF_CCITT_uLaw_8kHzMono );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = SPBindToFile( pszWaveFile,  SPFM_CREATE_ALWAYS, &cpStream, &cAudioFmt.FormatId(),cAudioFmt.WaveFormatExPtr() );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = cpVoice->SetOutput( cpStream, TRUE );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	if( VoiceSetup( pszVoice, cpVoice ) == false )
	{
		printf( "voice(%s) is not found\n", pszVoice );
	}
	else
	{
		WCHAR szText[8192];

		CharToWchar( pszText, szText, sizeof(szText) );

		hr = cpVoice->Speak( szText, SPF_DEFAULT, NULL );
	}
	
FUNC_END:
	if( cpStream )
	{
		cpStream->Close();
	}

	cpStream.Release ();
	cpVoice.Release();

	CoUninitialize();

	return true;
}

int main( int argc, char * argv[] )
{
	if( argc == 2 )
	{
		if( !strcmp( argv[1], "-l" ) )
		{
			PrintVoiceList();
			return 0;
		}
	}

	if( argc != 4 )
	{
		printf( "%s {voice} {text} {wave file}\n", argv[0] );
		return 0;
	}

	TTS( argv[1], argv[2], argv[3] );

	return 0;
}
