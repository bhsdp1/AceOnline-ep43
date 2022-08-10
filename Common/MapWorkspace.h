// MapWorkspace.h: interface for the CMapWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPWORKSPACE_H__B3F69521_1FA0_4639_AB52_2DB5C7383DE5__INCLUDED_)
#define AFX_MAPWORKSPACE_H__B3F69521_1FA0_4639_AB52_2DB5C7383DE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketHeader.h"
#include "MapBlock.h"


///////////////////////////////////////////////////////////////////////////////
/// \class		CMapWorkspace
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-26 ~ 2004-03-26
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CMapProject;
class CIOCP;
struct PROJECTINFO;

class CMapWorkspace
{
	friend class CMapProject;

public:
	CMapWorkspace(CIOCP* pIOCP, int nSizeBlockX, int nSizeBlockZ);
	virtual ~CMapWorkspace();

	CMapProject* GetMapProjectByMapIndex(const MapIndex_t MapIndex);
	CMapProject* GetMapProjectByIndex(const int index);	// check: MapChannel 도입중, 20040322, kelovon
	int GetMapProjectCounts(void){	return m_vectorPtrMapProject.size();}

	BOOL LoadAllObjectInfofromDB(void);			// 모든 Object 정보를 DB에서 가져온다
	int ChangeMapInfluenceType2AllConflictArea(SHORT i_sMapInflTy, BOOL i_bAdd);	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트

	virtual CMapProject * CreateChildMapProject(PROJECTINFO * i_pProjectInfo);

	// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	//virtual	BOOL LoadAllProject(const char* pFileName);
	virtual	BOOL LoadAllProject(const char* pFileName, BOOL i_bIsArenaServer, vectMapIndexList *i_pvectArenaMapIndexList);

	// Map Block 관련
	static BLOCK_INDEX SelectBlockIndexWithPosition(float x, float z);
public:
	//
	CIOCP						*m_pIOCPServer;			// IOCP Server의 포인터
	MapIndex_t					m_DefaultMapIndex1;		// 2004-03-26 by cmkwon,

	// MapProject 관련
	vector<CMapProject*>		m_vectorPtrMapProject;	// 생성하는 MapProject 의 포인터
	ez_map<int, MAPOBJECTINFO>	m_mapMapObjectInfo;		// 모든 Object 정보 map
	vector<ObjectNum_t>			m_CinemaObjectNumList;	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 

	static int					ms_nSIZE_BLOCK_X;		// 맵의 BlockSize 로 FieldServer 와 NPCServer는 다른값을 사용할수 있다.
	static int					ms_nSIZE_BLOCK_Z;		// 맵의 BlockSize 로 FieldServer 와 NPCServer는 다른값을 사용할수 있다.
};

#endif // !defined(AFX_MAPWORKSPACE_H__B3F69521_1FA0_4639_AB52_2DB5C7383DE5__INCLUDED_)
