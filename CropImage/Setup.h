#pragma once

class CSetup
{
public:
	CSetup();
	~CSetup();

	bool Get();
	bool Put();

	int m_iCropWidth;
	int m_iCropHeight;
};
