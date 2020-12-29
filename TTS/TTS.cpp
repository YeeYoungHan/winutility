#include "TTS.h"

bool TTS( const char * pszVoice, const char * pszText, const char * pszWaveFile )
{
	HRESULT				hr = S_OK;
	CComPtr <ISpVoice>		cpVoice;
	CComPtr <ISpStream>		cpStream;
	CSpStreamFormat			cAudioFmt;

	CoInitialize(NULL);

	hr = cpVoice.CoCreateInstance( CLSID_SpVoice );

  if( SUCCEEDED(hr) )
	{
		hr = cAudioFmt.AssignFormat(SPSF_22kHz16BitMono);
	}
	
	if( SUCCEEDED(hr) )
	{
		hr = SPBindToFile( "c:\\temp\\ttstemp.wav",  SPFM_CREATE_ALWAYS, &cpStream, &cAudioFmt.FormatId(),cAudioFmt.WaveFormatExPtr() );
	}
	
  if( SUCCEEDED(hr) )
	{
		hr = cpVoice->SetOutput( cpStream, TRUE );
	}

  if( SUCCEEDED(hr) )
	{
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
	}
	
	if( SUCCEEDED(hr) )
	{
		hr = cpStream->Close();
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
