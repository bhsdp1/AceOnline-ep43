// WayPointManager.h: interface for the CWayPointManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAYPOINTMANAGER_H__C4B0A032_042A_4377_8EC6_ECA7C95E8FA7__INCLUDED_)
#define AFX_WAYPOINTMANAGER_H__C4B0A032_042A_4377_8EC6_ECA7C95E8FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
class CAtumDBHelper;
class CODBCStatement;

class CWayPointMove : public SWAY_POINT_MOVE
{
public:
	CWayPointMove();
	CWayPointMove(UINT dwIndex, BYTE byCAction);
	virtual ~CWayPointMove();

	void Clean();

	INT GetSize();
	UINT GetIndex()					{ return dwIndex; }

	BOOL Add(SWAY_POINT *pData);
	
	void Sort();

	SWAY_POINT *GetWayPoint(int iStep);
	INT GetCompletionAction();

private:
	vectorWayPoint m_vectWayPointList;
};

typedef vector<CWayPointMove> vectorWayPointPattern;

struct SSORT_WAY_POINT_PATTERN_BY_INDEX: binary_function<CWayPointMove, CWayPointMove, bool>
{
	bool operator()(CWayPointMove Param1, CWayPointMove Param2)
	{
        return Param1.GetIndex() < Param2.GetIndex();		// 오름 차순 정렬
    };
};

class CWayPointManager
{
	friend CAtumDBHelper;
public:
	CWayPointManager();
	virtual ~CWayPointManager();

	void Clean();
	BOOL Create();
	void Release();

	BOOL LoadDBWayPointData(CODBCStatement *i_odbcstmt);

public:
	BOOL AddPattern(CWayPointMove *pData);
	BOOL AddWayPoint(SWAY_POINT *pWayPoint);

	SWAY_POINT *GetWayPointInfo(int iIndex, int iStep);
	CWayPointMove *GetWayPointPattern(int iIndex);

	INT GetCompletionAction(int iIndex);
	INT GetTotalCountPattern();		// 총 패턴 수
	INT GetTotalCountData();		// 모든 패턴들의 총 데이터 수
	
	BOOL IsCollision(int iIndex, int iStep, D3DXVECTOR3 vecPosition);
	BOOL IsLastStep(int iIndex, int iStep);
	BOOL IsFirstStep(int iStep);

	void Sort(void);

private:
	vectorWayPointPattern m_vectWPPatternList;
};
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

#endif // !defined(AFX_WAYPOINTMANAGER_H__C4B0A032_042A_4377_8EC6_ECA7C95E8FA7__INCLUDED_)