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
