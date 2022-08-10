// INFWorldMap.h: interface for the CINFWorldMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWORLDMAP_H__674C7CC4_E8A2_4E8C_8BB0_50FFF3F0496A__INCLUDED_)
#define AFX_INFWORLDMAP_H__674C7CC4_E8A2_4E8C_8BB0_50FFF3F0496A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2014-06-17 월드맵 추가작업 (주석처리)
//#include "INFBase.h"
//#include "AtumParam.h"
// END 2014-06-17 월드맵 추가작업 (주석처리)

#define WORLD_MAP_RAW_NUM	4
#define WORLD_MAP_COL_NUM	2

struct WorldMap_MapInfo
{
	POINT ptMapPos;
	CINFImageEx* pToolTipImg;
	MAP_INFO* mapInfo;
	DWORD dwCityWarp;
};

class CINFWorldMap : public CINFBase
{
public:
	CINFWorldMap();
	virtual ~CINFWorldMap();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWnd(BOOL bShow);
	
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int OnRButtonDown(WPARAM wParam, LPARAM lParam);
	int OnRButtonUp(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnMouseWheel(WPARAM wParam, LPARAM lParam);

	BOOL IsShowWorldMap() {return m_bShowWnd;}

	int GetHeight();
	int GetWidth();
	void ChangeWorldMapScale(float oldScale, float newScale);
	void CheckWorldMapPosition();
	void RenderWorldMap();
	void RenderLineHighlight();
	void RenderImage(CINFImageEx* image, int x, int y, int nWidth, int nHeight, float fScale);
	void RenderToolTip();
	void CheckToolTipPosition();
	void LoadMapInfo();
	void InitMapInfo(MapIndex_t mapIdx, LONG x, LONG y, DWORD cityWarp = 0x0);

	DataHeader* FindWorldMapImgResource(char* szRcName);
//-----------------------------------------------------------------------------------------------------------

private:
	/// 월드맵 인터페이스 ///
	BOOL							m_bShowWnd;					// 현재 창의 보여지는지 체크
	CINFImageEx*					m_pBackImg;					// 배경 이미지 

	/// 마우스 이벤트 ///
	BOOL							m_bIsMouseDown;				// 마우스 좌버튼이 눌린 상태 : 이미지 드래그
	BOOL							m_bIsMouseDownAndMoved;		// 마우스 클릭 이벤트 검사용
	POINT							m_ptClickedPoint;			// 마우스가 눌렸던 위치 : 이미지 드래그 기준좌표
	POINT							m_ptWorldMapPosOld;			// 마우스가 눌렸을 때 월드맵의 시작 위치

	/// 월드맵 이미지 ///
	CINFImageEx*					m_pWorldMapInfoImg;			// 월드맵 정보(좌측) 이미지
	POINT							m_ptWorldMapPos;			// 월드맵의 시작 위치
	float							m_fWorldMapScale;			// 월드맵의 크기 비율
	CINFImageEx*					m_arrWorldMapImg[WORLD_MAP_RAW_NUM][WORLD_MAP_COL_NUM];	
																// 월드맵 분할 이미지

	/// 월드맵 효과들 ///
	CINFImageEx*					m_pCurrentMapImg;			// 현재 맵 이미지
	int								m_nCurrentMapTwinkleStep;	// 현재 맵을 반짝이게 만들어 주세요.
	CINFImageEx*					m_pStrategyPointMapImg;		// 거점전 맵 이미지
	CINFImageEx*					m_pOutPostMapImg;			// 전진기지전 맵 이미지
	CINFImageEx*					m_pMSWarMapImg;				// 모선전 맵 이미지
	int								m_nMapTwinkleStep;			// 세력전도 반짝이게 만들어 주세요.
	CINFImageEx*					m_pCannotMoveMapImg;		// 이동 불가 맵 이미지
	
	/// 맵 툴팁 ///
	vector<WorldMap_MapInfo>		m_vecMapInfo;				// 각 맵들의 정보
	CGameData*						m_pMapImgData;				// 월드맵 툴팁 이미지 리소스 데이터
	WorldMap_MapInfo*				m_pToolTipShowMap;			// 표시될 툴팁 : 없으면 NULL
	CINFGroupImage*					m_pToolTipBackImg;			// 툴팁 배경
	CD3DHanFont*					m_pToolTipMapNameFont;		// 툴팁 맵이름
	CINFImageEx*					m_pToolTipBaseEnImg;		// 툴팁 거점전 표시
	CINFImageEx*					m_pToolTipBaseWarImg;		// 툴팁 거점전 표시
	CINFImageEx*					m_pToolTipOutPostEnImg;		// 툴팁 전진기지전 표시
	CINFImageEx*					m_pToolTipOutPostWarImg;	// 툴팁 전진기지전 표시
	CINFImageEx*					m_pToolTipMSWarEnImg;		// 툴팁 모선전 표시
	CINFImageEx*					m_pToolTipMSWarWarImg;		// 툴팁 모선전 표시
	CINFImageEx*					m_pToolTipWarpEnImg;		// 툴팁 워프 지역 표시
	CINFImageEx*					m_pToolTipANIImg;			// 툴팁 세력 ANI 표시
	CINFImageEx*					m_pToolTipBCUImg;			// 툴팁 세력 BCU 표시
	POINT							m_ptToolTipPos;				// 툴팁 위치

	/// 세력 길 하이라이트 ///
	CINFImageEx*					m_pLineHighlightANIImg;		// 알링턴 지역 길 하이라이트
	CINFImageEx*					m_pLineHighlightBCUImg;		// 바이제니유 지역 길 하이라이트
	CINFImageEx*					m_pLineHighlightNeutralImg;	// 중립 지역 길 하이라이트

	/// 드래그, 확대축소 ///
	POINT							m_ptWorldMapReturnPos;		// 돌아올 위치
	float							m_fWorldMapTargetScale;		// 확대축소

};

#endif // !defined(AFX_INFWORLDMAP_H__674C7CC4_E8A2_4E8C_8BB0_50FFF3F0496A__INCLUDED_)
