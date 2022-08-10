// GameDataLast.h: interface for the CGameDataLast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEDATALAST1_H__A520AAFF_7E8F_4A82_813D_B008287952A3__INCLUDED_)
#define AFX_GAMEDATALAST1_H__A520AAFF_7E8F_4A82_813D_B008287952A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  MAXBUFF  65536


class TotalHeader  
{
public:
	int m_EncodeNum;
	int m_Identification;
	int m_FileSize;
	int m_DataNumber;
	int m_Parity;

	TotalHeader();
	~TotalHeader();
};


class DataHeader  
{
public:
	int m_EncodeNum;
	int m_DataSize;
	int m_Parity;
	char m_FileName[10];
	char *m_pData;

	DataHeader();
	~DataHeader();
};

class CGameData  
{
public:
	CGameData();
	virtual ~CGameData();
	BOOL SetFile(char *filename, BOOL encode, char* encodeString, int encodeSize, BOOL bAllLoading = TRUE);
	int GetTotalNumber();
	DataHeader * GetStartPosition();
	DataHeader*  GetNext();
	void SetEncodeString(char* szEncode, int size) { memcpy( m_EncodeString, szEncode, size); }
	DataHeader* Find(char* strName);
	void SetEncode(BOOL encode) { m_bEncode = encode; }
	DataHeader* FindFromFile(char* strName);

	// 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 아해와 같이 수정됨
	//static BOOL GetCheckSum(UINT *o_puiCheckSum, int *o_pnFileSize, char* pFilePath);
	static BOOL GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath);

private:
	TotalHeader* pTotal_header;
	char m_ZipFilePath[256];
	char m_EncodeStrFilePath[256];
	char m_EncodeString[256];
	BOOL m_bEncode;

	map<string,DataHeader*> m_mapDataHeader;
	map<string,DataHeader*>::iterator m_it;
	
protected:
	BOOL make_parse_file_ext();
};


#endif // !defined(AFX_GAMEDATALAST1_H__A520AAFF_7E8F_4A82_813D_B008287952A3__INCLUDED_)
