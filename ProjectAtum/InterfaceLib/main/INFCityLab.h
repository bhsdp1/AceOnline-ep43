// INFCityStore.h: interface for the CINFCityLab class.
// 2004.07.15 ydKim
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYLAB_H__AD058082_959A_4B02_BEDD_AAE54DE7B3E0__INCLUDED_)
#define AFX_INFCITYLAB_H__AD058082_959A_4B02_BEDD_AAE54DE7B3E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 2008-03-14 by bhsohn 조합식 개선안
#include "INFNumEditBox.h"

#define LAB_BUTTON_NUMBER			3

class CINFImage;
class CD3DHanFont;
class CINFImageEx;
class CINFGroupImage;
class CINFItemMixFactoryWnd;		// 2013-03-18 by bhsohn 팩토리 조합 추가

class CINFCityLab : public CINFBase  
{
public:
	CINFCityLab(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityLab();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);	

	// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
//	void InitData();	// m_vecsource를 초기화 m_vectarget 초기화 	
	void InitData(BOOL bItemsResult=FALSE);	// m_vecsource를 초기화 m_vectarget 초기화 	
	// END 2013-04-08 by bhsohn 인게임 조합창 추가 구현
	void InvenToSourceItem(CItemInfo* pItemInfo, int nCount);
	void SetPrice(int nPrice);
	void DeleteTargetItem(int nItemUniqueNumber);
	BOOL PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg);
	BOOL PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg);
	// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
//	void PutTargetItem(ITEM_GENERAL* pItem);
	void PutTargetItem(ITEM_GENERAL* pItem, BOOL bUpdateItemCount=FALSE);
	// END 2013-04-08 by bhsohn 인게임 조합창 추가 구현
	CItemInfo*	GetTargetItemInfo();
	CItemInfo*	FindItemFromSource(UID64_t UniqueNumber);
	CItemInfo*  FindItemFromTarget(UID64_t UniqueNumber);
	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	CItemInfo*	GetSTargetItemInfo();
	CItemInfo*	FindItemLabFactoryFromSource(int nFindItemType);
	CItemInfo*	FindItemLabFactoryFromTarget(int nFindItemType);
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경

	void ReSetTargetItemNum(UID64_t UniqueNumber, int nItemNum);

	void SetSelectItem(CItemInfo* pItemInfo);

	// 2008-03-14 by bhsohn 조합식 개선안	
	void				OnOpenInfWnd();		// 처음 상점 오픈
	void				OnCloseInfWnd();	// 상점 클로우즈

	// 인챈트 실패 경고 해야 하는지 체크.
	bool IsWarning_EnchantFail ( void );
	bool IsWarning_EngineEnchantFail ( void );			// 2013-09-26 by ssjung 엔진 인첸트시 경고 메시지 출력

	void UpLoadItem(CItemInfo* i_pItem); // 2012-02-03 by jskim 팩토리 더블클릭 구현 

	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	void FieldSocketItemUseEnchantOk(MSG_FC_ITEM_USE_ENCHANT_OK* pMsg);
	BOOL IsRegisterEnchantFromSourceByKind(BYTE kind);
	BOOL ISStateNormal();
	void ChangeAniState(int nAniState);
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경

#ifdef C_WARNING_APPEARANCE_CHANGE
	bool IsWarning_AppearanceChange ( void ); // 2012-10-30 by mspark, 외형 변경 경고 메시지 추가
#endif

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	void ShowItemMixWnd(BOOL bShow, POINT* pBkPos=NULL);

	vector<CItemInfo*>  *GetvecSource() {return &m_vecSource;}
	vector<CItemInfo*>  *GetvecTarget()	{return &m_vecTarget;}

	char* GetMixPrice() {return m_szPrice;}		// 조합 가격 결정		
	//  다음 아이템 조합 요청
	void ItemMixItemsResult(MSG_FC_ITEM_MIX_ITEMS_RESULT* pMsg);	
	void ErrorMixItem(Err_t ErrorCode);
	// 목적 아이템을 가져온다.	
	
	void UpdateMixPrice();
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	void FieldSocketItemChangedShapeItemNum( MSG_FC_ITEM_CHANGED_SHAPEITEMNUM* pMsg );
	BOOL IsIsAppearanceChangeCheck() {return m_bIsAppearanceChangeCheck;}
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

protected :

	// 유효하지 않은 조합 관련 에러 메시지 출력.
	void ErrorMsg_InvalidEnchantList ( char *pszMessage , const bool a_bInitData = true );
	
private:
	// 2013-03-18 by bhsohn 팩토리 조합 추가
	// 2007-12-17 by bhsohn 조합 가격 표시
//	void UpdateMixPrice();

public:
	CItemInfo	*		m_pSelectItem;
	BOOL				m_bIsEnchantCheck;

	// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가 
	BOOL				m_bIsAppearanceChangeCheck;
	// end 2012-10-30 by mspark, 외형 변경 경고 메시지 추가

protected:
	BOOL				m_bRestored;

	CINFImageEx	*		m_pImgButton[LAB_BUTTON_NUMBER][4];	// 0 : 제조(SEND), 1 : cancel, 2 : ok
	int					m_nButtonState[2];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	CINFGroupImage	*	m_pImgBack;
#else	   
	CINFImageEx	*		m_pImgBack;
#endif
	CINFImageEx	*		m_pImgBackFactory;	// 연구소배경 // 2008-03-14 by bhsohn 조합식 개선안	   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*		m_pImgBackLab;
#else
    CINFImageEx	*		m_pImgPrice;
#endif
	
	CINFImageEx	*		m_pImgTitle;
	
	// 2008-03-14 by bhsohn 조합식 개선안
#ifdef C_INGAME_MIX_ITEM
#else
	CINFNumEditBox*		m_pNumEditBox; // 2013-03-18 by bhsohn 팩토리 조합 추가
#endif

	CD3DHanFont *		m_pFontPrice;
	char				m_szPrice[64];
	vector<CItemInfo*>  m_vecSource;
	vector<CItemInfo*>  m_vecTarget;
	BOOL				m_bShowTarget;
	BUILDINGNPC*		m_pBuildingInfo;

	BOOL				m_bSelectDown;
	
	// 2013-03-18 by bhsohn 팩토리 조합 추가
	CINFItemMixFactoryWnd*		m_pINFItemMixFactoryWnd;
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	enum LABORATORY_GROUP
	{
		LABORATORY_GROUP_STARGET,
		LABORATORY_GROUP_ENCHANT,
		LABORATORY_GROUP_ETC,
		LABORATORY_GROUP_TTARGET,
		LABORATORY_GROUP_END
	};
	enum LABORATORY_ITEM
	{
		LABORATORY_ITEM_STARGET,
		LABORATORY_ITEM_ENCHANT,
		LABORATORY_ITEM_ETC1,
		LABORATORY_ITEM_ETC2,
		LABORATORY_ITEM_TTARGET,
		LABORATORY_ITEM_END
	};
	enum LABORATORY_ANIMATION_STATE
	{
		LABORATORY_ANIMATION_STATE_NORMAL,
		LABORATORY_ANIMATION_STATE_ICON_START,
		LABORATORY_ANIMATION_STATE_ICON_ING,
		LABORATORY_ANIMATION_STATE_LINE_START,
		LABORATORY_ANIMATION_STATE_LINE_ING,
		LABORATORY_ANIMATION_STATE_RESULT_START,
		LABORATORY_ANIMATION_STATE_RESULT_ING,
		LABORATORY_ANIMATION_STATE_RESULT_RECIVE,
		LABORATORY_ANIMATION_STATE_RESULT_SUCCESS,
		LABORATORY_ANIMATION_STATE_RESULT_SUCCESS_END,
		LABORATORY_ANIMATION_STATE_END
	};
	int			m_nAniState;

	int			m_nSucAniLoopCnt;

	CINFImageEx*	m_pImgLabIcon[LABORATORY_GROUP_END];
	CINFImageEx*	m_pImgLabLine[LABORATORY_GROUP_END];
	BOOL			m_bAniView[LABORATORY_GROUP_END];

	MSG_FC_ITEM_USE_ENCHANT m_UseEchantMsg;
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
};

#endif // !defined(AFX_INFCITYSTORE_H__AD058082_959A_4B02_BEDD_AAE54DE7B3E0__INCLUDED_)
