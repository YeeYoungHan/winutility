#include "TTS.h"

bool WcharToChar( const WCHAR * pszWchar, std::string & strOutput )
{
	int iLength = WideCharToMultiByte( CP_ACP, 0, pszWchar, -1, NULL, 0, NULL, NULL );
	if( iLength == 0 )
	{
		return false;
	}

	char * pszAnsi = new char[iLength];
	if( pszAnsi == NULL )
	{
		return false;
	}

	WideCharToMultiByte( CP_ACP, 0, pszWchar, -1, pszAnsi, iLength, NULL, NULL );
	strOutput = pszAnsi;
	
	delete [] pszAnsi;

	return true;
}

bool CharToWchar( const char * pszChar, WCHAR * pszOutput, int iOutputSize )
{
	MultiByteToWideChar( CP_ACP, 0, pszChar, -1, pszOutput, iOutputSize );

	return true;
}
