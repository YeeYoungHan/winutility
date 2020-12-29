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
