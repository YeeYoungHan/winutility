#pragma once

#define SETUP_CROP_IMAGE	"CropImage"
#define SETUP_CROP_WIDTH	"Width"
#define SETUP_CROP_HEIGHT	"Height"

class CSetup
{
public:
	CSetup();
	~CSetup();

	bool Get();
	bool Put();

	int m_iCropWidth;
	int m_iCropHeight;

private:
	void SaveInt( const char * pszAppName, const char * pszKeyName, int iValue );

	std::string m_strFilePath;
};

extern CSetup gclsSetup;
