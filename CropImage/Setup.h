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

#define SETUP_CROP_IMAGE				"CropImage"
#define SETUP_THUMBNAIL_IMAGE		"ThumbnailImage"

#define SETUP_WIDTH							"Width"
#define SETUP_HEIGHT						"Height"
#define SETUP_START_NO					"StartNo"
#define SETUP_OUTPUT_FOLDER			"Folder"
#define SETUP_USE_THUMBNAIL			"UseThumbnail"
#define SETUP_THUMBNAIL_PREFIX	"Prefix"

class CSetup
{
public:
	CSetup();
	~CSetup();

	bool Get();
	bool Put();

	int m_iCropWidth;
	int m_iCropHeight;
	int m_iCropFileStartNo;
	std::string m_strOutputFolderPath;

	bool m_bUseThumbnail;
	int m_iThumbnailWidth;
	int m_iThumbnailHeight;
	std::string m_strThumbnailPrefix;

private:
	void SaveInt( const char * pszAppName, const char * pszKeyName, int iValue );

	std::string m_strFilePath;
};

extern CSetup gclsSetup;
