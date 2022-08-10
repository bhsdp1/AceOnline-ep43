// GameDataLast.cpp: implementation of the CGameDataLast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDataLast.h"

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <stdio.h>
#include "sha256.h"		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 

//#include "DXUtil.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CGameData::CGameData()
{
	//	FLOG( "CGameData()" );
	pTotal_header = NULL;
	memset(m_EncodeStrFilePath,0x00,sizeof(m_EncodeStrFilePath));
	memset(m_ZipFilePath,0x00,sizeof(m_ZipFilePath));
    memset(m_EncodeString,0x00,sizeof(m_EncodeString));
	m_it = m_mapDataHeader.begin();
	m_bEncode = FALSE;
}

CGameData::~CGameData()
{
	//	FLOG( "~CGameData()" );
	map<string,DataHeader*>::iterator it = m_mapDataHeader.begin();

	while(it != m_mapDataHeader.end())
	{
		DataHeader* pHeader = it->second;
		SAFE_DELETE (pHeader);
		it++;
	}		
	SAFE_DELETE( pTotal_header );
}

int CGameData::GetTotalNumber()
{
	//	FLOG( "CGameData::GetTotalNumber()" );
	return pTotal_header->m_DataNumber;
}

DataHeader * CGameData::GetStartPosition()
{
	//	FLOG( "CGameData::GetStartPosition()" );
	m_it = m_mapDataHeader.begin();

	return m_it->second;
}

DataHeader* CGameData::GetNext()
{
	m_it++;
	if(m_it == m_mapDataHeader.end() )
		return NULL;
	return m_it->second;

}

DataHeader* CGameData::Find(char* strName)
{
	//	FLOG( "CGameData::Find(char* strName)" );
	map<string,DataHeader*>::iterator it = m_mapDataHeader.find(strName);

	if(it != m_mapDataHeader.end())
	{
		return it->second;
	}	
	return NULL;
}

BOOL CGameData::SetFile(char *filename,  BOOL encode, char* encodeString, int encodeSize, BOOL bAllLoading)
{
	//	FLOG( "CGameData::SetFile(char *filename,  BOOL encode, char* encodeString, int encodeSize)" );
	strcpy(m_ZipFilePath, filename);
	if(encode)
	{
		SetEncodeString(encodeString, encodeSize);
	}
	m_bEncode = encode;
	if(bAllLoading)
	{
		return make_parse_file_ext();
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		체크섬 처리
/// \author		// 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
///				// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 함수 수정
/// \date		2007-04-06 ~ 2007-04-06
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameData::GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath)
{
	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	//*o_puiCheckSum	= 0;			// 2007-05-28 by cmkwon
	memset(o_byObjCheckSum, 0x00, 32);	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	*o_pnFileSize	= 0;			// 2007-05-28 by cmkwon
	
	UINT uiCheckSum = 0;
	if(strlen(pFilePath) <=0)
	{
		return FALSE;
	}
	FILE *fp;
    fp=fopen(pFilePath, "rb");		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - binary로 읽도록 수정
	if(NULL == fp)
	{
		return FALSE;
	}
	fseek( fp, 0L, SEEK_END );
	long lFileSize = ftell( fp );
	*o_pnFileSize = lFileSize;			// 2007-05-28 by cmkwon
	fseek( fp, 0L, SEEK_SET );	

	BYTE *pFileData = new BYTE [lFileSize];
	memset(pFileData, 0x00, lFileSize);
	fread(pFileData, lFileSize, 1, fp);	
// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
//	for(int i=0; i < lFileSize/sizeof(UINT); i++)
//	{
//		uiCheckSum ^= ((UINT*)pFileData)[i];
//	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	sha256_encode(pFileData, lFileSize, o_byObjCheckSum);
	
	fclose(fp);
	delete [] pFileData;

	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 기존 소스
	//*o_puiCheckSum	= uiCheckSum;			// 2007-05-28 by cmkwon
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DataHeader* CGameData::FindFromFile(char* strName)
/// \brief		파일에서 원하는 데이타를 찾아서 로딩한다.
/// \author		dhkwon
/// \date		2004-06-03 ~ 2004-06-03
/// \warning	m_bEncode가 TRUE이면 실패한다.
///				리턴된 pDataHeader는 외부에서 지워줘야 한다.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DataHeader* CGameData::FindFromFile(char* strName)
{
	if(m_bEncode)
	{
		return NULL;
	}
	TotalHeader totalHeader;
	memset( &totalHeader, 0x00, sizeof(totalHeader));
	int ReadFile = open(m_ZipFilePath, O_RDONLY | O_BINARY);

	long allLength = _lseek(ReadFile, 0, SEEK_END);
	int readPointer=0;
	_lseek(ReadFile, 0, SEEK_SET);
	read( ReadFile, (char*)&totalHeader,sizeof(totalHeader));
	readPointer += sizeof(TotalHeader);
	DataHeader* pDataHeader = new DataHeader;
	for( int i=0 ; i < (totalHeader.m_DataNumber) ; i++)
	{
		memset(pDataHeader, 0x00, sizeof(DataHeader) );
		_lseek( ReadFile, readPointer, SEEK_SET );
		read( ReadFile, (char*)pDataHeader,sizeof(DataHeader)-4);// 4: data pointer
		if(strcmp(pDataHeader->m_FileName, strName ) == 0)
		{
			pDataHeader->m_pData = new char[pDataHeader->m_DataSize+1];
			memset(pDataHeader->m_pData, 0x00, pDataHeader->m_DataSize+1);	// 2006-04-03 by ispark
			read( ReadFile, pDataHeader->m_pData,pDataHeader->m_DataSize);
			close( ReadFile );
			return pDataHeader;
		}
		readPointer += sizeof(DataHeader)-4 + pDataHeader->m_DataSize;// 4: data pointer
	}
	close( ReadFile );
//	delete pDataHeader;
	SAFE_DELETE(pDataHeader);
	DBGOUT("====> Not Data File (%s)\n", strName);
	return NULL;
}

BOOL CGameData::make_parse_file_ext()
{
	//	FLOG( "CGameData::make_parse_file_ext()" );
	int EncodeFile,ReadFile;
    char Data[300],temp[300], DataBuff[MAXBUFF],Encode[1024];
	int CurrentReadPoint=0, maxsize;
	// read encode string
	if( m_bEncode && strlen(m_EncodeStrFilePath) > 0 )
	{
		memset(Encode,0x00,sizeof(Encode));
		EncodeFile = open(m_EncodeStrFilePath, O_RDONLY );

		read( EncodeFile, m_EncodeString, sizeof(m_EncodeString) );
		close(EncodeFile);
	}
	int readsize=0;
	pTotal_header = new TotalHeader;
	memset(temp,0x00,sizeof(temp));
	memset(Data,0x00,sizeof(Data));
	ReadFile = open(m_ZipFilePath, O_RDONLY | O_BINARY);
	if(ReadFile==-1)
	{
		char buf[512];
		wsprintf(buf, "ERROR CGameData:: resource read(%s)",m_ZipFilePath);
//		FLOG( buf );
//		DBGOUT("ERROR CGameData:: resource read(%s)\n",m_ZipFilePath);
		SAFE_DELETE(pTotal_header);
		return FALSE;
	}
	maxsize = strlen(m_EncodeString);
	long length = _lseek(ReadFile, 0, SEEK_END);
	char* pTemp = new char[length];
	int readPointer=0;
	_lseek(ReadFile, 0, SEEK_SET);
	int n;
	while(n=read( ReadFile, DataBuff,sizeof(DataBuff)))
	{
		if(n == -1)
		{
			DBGOUT("%s ERROR ReadFile(%s - %d)\n", m_ZipFilePath, strerror( errno ), errno);
			SAFE_DELETE_ARRAY(pTemp)
			SAFE_DELETE(pTotal_header);
			close(ReadFile);
			return FALSE;
		}
		if(m_bEncode)
		{
			for(int j=0; j<n;j++)
			{
					DataBuff[j] ^= m_EncodeString[j%maxsize]; 
			}
		}
		memcpy(&pTemp[readPointer],DataBuff,n);
		readPointer += n;
		memset(DataBuff,0x00,sizeof(DataBuff));
	}
	close(ReadFile);

	// read total header
	readPointer = 0;
	memcpy( pTotal_header, &pTemp[readPointer], sizeof(TotalHeader) );
	readPointer += sizeof(TotalHeader);
	// read data header
	for( int i=0 ; i < (pTotal_header->m_DataNumber) ; i++)
	{
		DataHeader* pHeader = NULL;
		pHeader = new DataHeader;
		memcpy((void*) pHeader, &pTemp[readPointer], sizeof(DataHeader)-4);
		readPointer += sizeof(DataHeader)-4;
		pHeader->m_pData = new char[pHeader->m_DataSize+1];
		memset(pHeader->m_pData, 0x00, pHeader->m_DataSize+1);			// 2006-04-03 by ispark
		memcpy((void*) pHeader->m_pData, &pTemp[readPointer], pHeader->m_DataSize );
		readPointer += pHeader->m_DataSize;
		m_mapDataHeader[pHeader->m_FileName] = pHeader;
	}	
//	delete pTemp;
	SAFE_DELETE_ARRAY(pTemp);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// TotalHeader Class
//////////////////////////////////////////////////////////////////////

TotalHeader::TotalHeader()
{
	//	FLOG( "TotalHeader()" );
	m_EncodeNum = 1000;
	m_Identification = 1000;
	m_FileSize = 0;
	m_DataNumber = 0;
	m_Parity = 0;
}

TotalHeader::~TotalHeader()
{
	//	FLOG( "~TotalHeader()" );

}

//////////////////////////////////////////////////////////////////////
// DataHeader Class
//////////////////////////////////////////////////////////////////////

DataHeader::DataHeader()
{
	//	FLOG( "DataHeader()" );
	m_EncodeNum = 0;
	m_DataSize = 0;
	m_Parity = 0;
	m_pData = NULL;
	memset(m_FileName,0x00,sizeof(m_FileName));
}

DataHeader::~DataHeader()
{
	//	FLOG( "~DataHeader()" );
//	delete [] m_pData;
	SAFE_DELETE_ARRAY(m_pData);
}


