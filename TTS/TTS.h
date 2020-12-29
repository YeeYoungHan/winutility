#pragma once

#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable:4995)

#include <stdio.h>
#include <windows.h>
#include <atlbase.h>
#include <sapi.h>
#include <sphelper.h>
#include <string>

// Voice.cpp
void PrintVoiceList( );
bool VoiceSetup( const char * pszVoice, ISpVoice * pVoice );

// StringUtility.cpp
bool WcharToChar( const WCHAR * pszWchar, std::string & strOutput );
bool CharToWchar( const char * pszChar, WCHAR * pszOutput, int iOutputSize );
