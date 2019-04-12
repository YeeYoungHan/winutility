#pragma once

#define SETUP_CROP_IMAGE			"CropImage"
#define SETUP_THUMBNAIL_IMAGE	"ThumbnailImage"

#define SETUP_WIDTH						"Width"
#define SETUP_HEIGHT					"Height"
#define SETUP_OUTPUT_FOLDER		"Folder"
#define SETUP_USE_THUMBNAIL		"UseThumbnail"
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
