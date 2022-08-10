#pragma once
#include "AtumThread.h"

class CScreenShotConverter :
	public CAtumThread
{
public:
	CScreenShotConverter(SCREENSHOT_TYPE targetType);
	virtual ~CScreenShotConverter();

public:
	virtual DWORD Run();
	void addFile(string fileName);

private:
	mt_vector<string> m_FilesToConvert;
	SCREENSHOT_TYPE m_TargetType;

// Helpers
private:
	void convertFile(const string& fileName);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};

