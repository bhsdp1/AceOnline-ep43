// INFCityShop.cpp: implementation of the CINFCityShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityShop.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "FieldWinSocket.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "D3DHanFont.h"
#include "INFIcon.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFCityBase.h"
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Chat.h"
#include "StoreData.h"
#include "dxutil.h"
#include "INFImageEx.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define CITY_SHOP_START_X			(CITY_BASE_NPC_BOX_START_X+427)
#define CITY_SHOP_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)

#define SHOP_TAB_START_X			(150)//(221+28)
#define SHOP_TAB_START_Y			35
#define SHOP_TAB_NUMBER				4
#define SHOP_ITEM_SLOT_START_X		(2)//(227+28)
#define SHOP_ITEM_SLOT_START_Y		30
#define SHOP_ITEM_SLOT_SIZE_X		322//157
#define SHOP_ITEM_SLOT_SIZE_Y		38
#define SHOP_ICON_START_X			29//(230+28)
#define SHOP_ICON_START_Y			36
#define SHOP_ICON_SIZE				30
#define SHOP_ITEM_SLOT_INTERVAL		38
#define SHOP_ITEM_NUMBER_X			314
#define SHOP_NUMBER_BUTTON_X		(SHOP_TAB_START_X + 170)
#define SHOP_NUMBER_UP_BUTTON_Y		8
#define SHOP_NUMBER_BUTTON_SIZE_X	8
#define SHOP_NUMBER_BUTTON_SIZE_Y	7
#define SHOP_NUMBER_DOWN_BUTTON_Y	22
#define SHOP_BUY_BUTTON_START_X		(344+28)
#define SHOP_BUY_BUTTON_START_Y		70
#define SHOP_BUY_BUTTON_SIZE_X		39
#define SHOP_BUY_BUTTON_SIZE_Y		14
#define SHOP_CASH_START_X			260
#define SHOP_OK_BUTTON_START_X		(SHOP_TAB_START_X + 122)
#define SHOP_OK_BUTTON_START_Y		368
#define SHOP_OK_BUTTON_SIZE_X		35
#define SHOP_OK_BUTTON_SIZE_Y		16
#define SHOP_CANCEL_BUTTON_START_X	(SHOP_TAB_START_X + 152)
#define SHOP_CANCEL_BUTTON_START_Y	368

#define SHOP_SCROLL_START_X			SHOP_TAB_START_X + 188
#define SHOP_SCROLL_START_Y			34
#define SHOP_SCROLL_UP_START_Y		51
#define SHOP_SCROLL_SIZE_X			30
#define SHOP_SCROLL_SIZE_Y			300
#define SHOP_SCROLL_WEHELL_SIZE_X	270
#define SHOP_SCROLL_WEHELL_SIZE_Y	300
#define SHOP_SCROLL_BAR_SIZE_Y		25//30
#define SHOP_SCROLL_DOWN_START_Y	217

#define SHOP_SCROLL_BAR_START_Y		67
#define SHOP_SCROLL_BAR_END_Y		215

#define SHOP_TAB_BUTTON_SIZE_X		BOX_SIZE_X / 4
#define SHOP_TAB_BUTTON_SIZE_Y		24

#define SHOP_SCROLL_LENGTH			301	// 150 : scroll bar line length
#define SHOP_SCROLL_MOVE_LENGTH		(301-SHOP_SCROLL_BAR_SIZE_Y)
//#define SHOP_SCROLL_NUMBER(i)		(m_mapItemInfo[i].size()<=0 ? 1:m_mapItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_NUMBER(i)		(m_vecItemInfo[i].size()<=0 ? 0:m_vecItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_INTERVAL(i)		(SHOP_SCROLL_LENGTH / SHOP_SCROLL_NUMBER(i))

#define SHOP_ITEM_NAME_START_X		73

#define SHOP_ITEM_PRICE_START_X		272

#define SHOP_SELL_ITEM_START_X		25
#define SHOP_SELL_ITEM_START_Y		SHOP_TAB_START_Y
#define SHOP_SELL_ITEM_END_X		SHOP_SELL_ITEM_START_X + 345
#define SHOP_SELL_ITEM_END_Y		SHOP_SELL_ITEM_START_Y + 350

#define SHOP_TAB_B_GEAR				0
#define SHOP_TAB_I_GEAR				1
#define SHOP_TAB_M_GEAR				2
#define SHOP_TAB_A_GEAR				3

#define BOX_SIZE_X					368
#define BOX_SIZE_Y					428
#define END_OF_INVEN_X				200

#define SHOP_TEXT_SHOW				2
#define SHOP_TEXT_SHOW_SIZE_X		140
#define SHOP_TEXT_SHOW_SIZE_TRI_X	100

#define SOURCEINDEXTOITEMNUM(i,j)	(((i/100)*100)+j)

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define SHOP_ITEM_NUMBER_Y			68//70
#define SHOP_CASH_START_Y			341//241
#define SHOP_ITEM_NAME_START_Y		42//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#define FONT_TOTAL_PRICE_X			2
#define FONT_TOTAL_PRICE_Y			1
#define SHOP_TEX_X					CITY_SHOP_START_X + 282
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 405
#else
#define SHOP_ITEM_NUMBER_Y			68//70//54
#define SHOP_CASH_START_Y			341//241
#define SHOP_ITEM_NAME_START_Y		42//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#define FONT_TOTAL_PRICE_X			1
#define FONT_TOTAL_PRICE_Y			1//3
#define SHOP_TEX_X					CITY_SHOP_START_X + 282
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 405
#endif				   
#else	  
#define CITY_SHOP_START_X			(CITY_BASE_NPC_BOX_START_X+230)
#define CITY_SHOP_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y-9)

#define SHOP_TAB_START_X			(196+9)//(221+28)
#define SHOP_TAB_START_Y			33
#define SHOP_TAB_NUMBER				4
#define SHOP_ITEM_SLOT_START_X		(SHOP_TAB_START_X + 2)//(227+28)
#define SHOP_ITEM_SLOT_START_Y		51
#define SHOP_ITEM_SLOT_SIZE_X		253//157
#define SHOP_ITEM_SLOT_SIZE_Y		36
#define SHOP_ICON_START_X			(SHOP_ITEM_SLOT_START_X + 3)//(230+28)
#define SHOP_ICON_START_Y			53
#define SHOP_ICON_SIZE				30
#define SHOP_ITEM_SLOT_INTERVAL		36
#define SHOP_ITEM_NUMBER_X			(SHOP_TAB_START_X+210)
#define SHOP_NUMBER_BUTTON_X		(SHOP_TAB_START_X + 242)
#define SHOP_NUMBER_UP_BUTTON_Y		58
#define SHOP_NUMBER_BUTTON_SIZE_X	8
#define SHOP_NUMBER_BUTTON_SIZE_Y	7
#define SHOP_NUMBER_DOWN_BUTTON_Y	71
#define SHOP_BUY_BUTTON_START_X		(344+28)
#define SHOP_BUY_BUTTON_START_Y		70
#define SHOP_BUY_BUTTON_SIZE_X		39
#define SHOP_BUY_BUTTON_SIZE_Y		14
#define SHOP_CASH_START_X			(SHOP_TAB_START_X + 95)
#define SHOP_OK_BUTTON_START_X		(SHOP_TAB_START_X + 205)
#define SHOP_OK_BUTTON_START_Y		240
#define SHOP_OK_BUTTON_SIZE_X		35
#define SHOP_OK_BUTTON_SIZE_Y		16
#define SHOP_CANCEL_BUTTON_START_X	(SHOP_TAB_START_X + 242)
#define SHOP_CANCEL_BUTTON_START_Y	240

#define SHOP_SCROLL_START_X			(SHOP_TAB_START_X + 260)
#define SHOP_SCROLL_UP_START_Y		51
#define SHOP_SCROLL_SIZE_X			11
#define SHOP_SCROLL_SIZE_Y			12
#define SHOP_SCROLL_BAR_SIZE_Y		38//30
#define SHOP_SCROLL_DOWN_START_Y	217

#define SHOP_SCROLL_BAR_START_Y		65
#define SHOP_SCROLL_BAR_END_Y		215

#define SHOP_TAB_BUTTON_SIZE_X		60
#define SHOP_TAB_BUTTON_SIZE_Y		14

#define SHOP_SCROLL_LENGTH			150	// 150 : scroll bar line length
#define SHOP_SCROLL_MOVE_LENGTH		(150-SHOP_SCROLL_BAR_SIZE_Y)
//#define SHOP_SCROLL_NUMBER(i)		(m_mapItemInfo[i].size()<=0 ? 1:m_mapItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_NUMBER(i)		(m_vecItemInfo[i].size()<=0 ? 0:m_vecItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_INTERVAL(i)		(SHOP_SCROLL_LENGTH / SHOP_SCROLL_NUMBER(i))

#define SHOP_ITEM_NAME_START_X		(SHOP_TAB_START_X + 46)

#define SHOP_SELL_ITEM_START_X		(227+28)
#define SHOP_SELL_ITEM_START_Y		51
#define SHOP_SELL_ITEM_END_X		(383+28)
#define SHOP_SELL_ITEM_END_Y		228

#define SHOP_TAB_B_GEAR				0
#define SHOP_TAB_I_GEAR				1
#define SHOP_TAB_M_GEAR				2
#define SHOP_TAB_A_GEAR				3

#define BOX_SIZE_X					417
#define BOX_SIZE_Y					275
#define END_OF_INVEN_X				200

#define SOURCEINDEXTOITEMNUM(i,j)	(((i/100)*100)+j)

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define SHOP_ITEM_NUMBER_Y			68//70
#define SHOP_CASH_START_Y			239//241
#define SHOP_ITEM_NAME_START_Y		51//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#define FONT_TOTAL_PRICE_X			2
#define FONT_TOTAL_PRICE_Y			1
#define SHOP_TEX_X					CITY_SHOP_START_X + 420
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 12
#else
#define SHOP_ITEM_NUMBER_Y			68//70//54
#define SHOP_CASH_START_Y			239//241
#define SHOP_ITEM_NAME_START_Y		51//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#define FONT_TOTAL_PRICE_X			1
#define FONT_TOTAL_PRICE_Y			1//3
#define SHOP_TEX_X					CITY_SHOP_START_X + 420
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 12
#endif
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CINFCityShop::CINFCityShop(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	FLOG( "CINFCityShop(CAtumNode* pParent)" );
	m_pParent = pParent;	// CGameMain*
	m_pGameData = NULL;

	int i;
	for(i=0;i<4;i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOkButton[i] = NULL;
		m_pCancelButton[i] = NULL;
		m_pBuyButton[i] = NULL;// [3]은 NULL
#endif
		m_pItemTab[i] = NULL;
		m_pnSelectItemNumber[i] = NULL;
		m_nCurrentScrollNumber[i] = 0;
		m_nCurrentSelectSlot[i] = -1;
		m_nMouseScrollPosition[i] = 0;
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pCash = NULL;
	m_pScrollUpDown = NULL;
#endif
	m_pSlot = NULL;
	
	m_pScrollBar = NULL;
	m_pSelectSlot = NULL;
	m_pFontTotalPrice = NULL;

	m_nCurrentTab = SHOP_TAB_B_GEAR;
	m_nOkButtonState = BUTTON_STATE_NORMAL;
	m_nCancelButtonState = BUTTON_STATE_NORMAL;

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_nBuyButtonState[i] = BUTTON_STATE_NORMAL;
		m_pFontItemName[i] = NULL;
		m_pFontItemPrice[i] = NULL;
		m_pFontItemNumber[i] = NULL;
	}
	m_bScrollLock = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_bInputItemDone = FALSE;
	m_pBuildingInfo = pBuilding;

	m_pInfluenceTex = NULL;
}

CINFCityShop::~CINFCityShop()
{
	FLOG( "~CINFCityShop()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	SAFE_DELETE(m_pOkButton);
	SAFE_DELETE(m_pCancelButton);
#endif
	int i;
	for(i=0;i<4;i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		SAFE_DELETE(m_pOkButton[i]);
		SAFE_DELETE(m_pCancelButton[i]);
		SAFE_DELETE(m_pBuyButton[i]);
#endif
		SAFE_DELETE(m_pItemTab[i]);
		SAFE_DELETE_ARRAY(m_pnSelectItemNumber[i]);

//		map<int, SHOP_ITEM*>::iterator it = m_mapItemInfo[i].begin();
//		while(it != m_mapItemInfo[i].end())
//		{
//			SHOP_ITEM* pItem = it->second;
//			delete pItem;
//			pItem = NULL;
//			it++;
//		}
//		m_mapItemInfo[i].clear();
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while(it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();

	}
	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		SAFE_DELETE(m_pFontItemName[i]);
		SAFE_DELETE(m_pFontItemPrice[i]);
		SAFE_DELETE(m_pFontItemNumber[i]);
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	SAFE_DELETE(m_pCash);
	SAFE_DELETE(m_pScrollUpDown);
#endif
    SAFE_DELETE(m_pSlot);
	SAFE_DELETE(m_pScrollBar);
	SAFE_DELETE(m_pSelectSlot);
	SAFE_DELETE(m_pFontTotalPrice);

	SAFE_DELETE(m_pInfluenceTex);
}


void CINFCityShop::InitData(BOOL bColorShop)//UINT nShopUniqueNumber, UINT nNPCTalkIndex)
{
	if(IS_BT(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		m_nCurrentTab = SHOP_TAB_B_GEAR;
	}
	else if(IS_OT(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		m_nCurrentTab = SHOP_TAB_M_GEAR;
	}
	else if(IS_ST(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		m_nCurrentTab = SHOP_TAB_I_GEAR;
	}
	else if(IS_DT(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		m_nCurrentTab = SHOP_TAB_A_GEAR;
	}
	for(int i=0;i<4;i++)
	{
		SAFE_DELETE_ARRAY(m_pnSelectItemNumber[i]);
		m_nCurrentScrollNumber[i] = 0;
		m_nCurrentSelectSlot[i] = -1;
		m_nMouseScrollPosition[i] = 0;					// 2006-04-05 by ispark
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while(it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();
	}	
}

HRESULT CINFCityShop::InitDeviceObjects()
{
	FLOG( "CINFCityShop::InitDeviceObjects()" );
	DataHeader	* pDataHeader;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pOkButton = new CINFImageBtn;
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	//m_pOkButton->InitDeviceObjects("sauok") ;
	m_pOkButton->InitDeviceObjects("sauok","STRTOOLTIP71");
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	
	m_pCancelButton = new CINFImageBtn;
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	//m_pCancelButton->InitDeviceObjects("shmcan0") ;	
	m_pCancelButton->InitDeviceObjects("shmcan0","STRTOOLTIP41");
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	
	m_pTriSlot = new CINFImageEx;
	pDataHeader = FindResource("tslot");
	m_pTriSlot->InitDeviceObjects(pDataHeader) ;
	
	m_pTriSelectSlot = new CINFImageEx;
	pDataHeader = FindResource("trislot");
	m_pTriSelectSlot->InitDeviceObjects(pDataHeader) ;
#endif
	int i;
	for(i=0;i<4;i++)
	{
		char buf[16];

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOkButton[i] = new CINFImageEx;
		wsprintf(buf, "sauok%d",i);
		pDataHeader = FindResource(buf);
		m_pOkButton[i]->InitDeviceObjects(pDataHeader) ;

		m_pCancelButton[i] = new CINFImageEx;
		wsprintf(buf, "shmcan0%d",i);
		pDataHeader = FindResource(buf);
		m_pCancelButton[i]->InitDeviceObjects(pDataHeader) ;

		wsprintf(buf, "sbuy%d",i);//sbuy3은 없음.
		pDataHeader = FindResource(buf);
		if(pDataHeader)
		{
			m_pBuyButton[i] = new CINFImageEx;
			m_pBuyButton[i]->InitDeviceObjects(pDataHeader) ;
		}
		
#endif
		m_pItemTab[i] = new CINFImageEx;
		wsprintf(buf, "stab%d",i);
		pDataHeader = FindResource(buf);
		m_pItemTab[i]->InitDeviceObjects(pDataHeader) ;

	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
 	m_pCash = new CINFImageEx;
	pDataHeader = FindResource("scash");
 	m_pCash->InitDeviceObjects(pDataHeader) ;


#endif

	m_pSlot = new CINFImageEx;
	pDataHeader = FindResource("auslot");
	m_pSlot->InitDeviceObjects(pDataHeader) ;

	

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
 	m_pScrollUpDown = new CINFImageEx;
	pDataHeader = FindResource("c_scrlbt");
 	m_pScrollUpDown->InitDeviceObjects(pDataHeader) ;
#endif

	m_pScrollBar = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pScrollBar->InitDeviceObjects(pDataHeader);

	m_pSelectSlot = new CINFImageEx;
	pDataHeader = FindResource("slosel");
	m_pSelectSlot->InitDeviceObjects(pDataHeader) ;

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
		m_pFontItemName[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontItemPrice[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
		m_pFontItemPrice[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontItemNumber[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
		m_pFontItemNumber[i]->InitDeviceObjects(g_pD3dDev);
	}
	m_pFontTotalPrice = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontTotalPrice->InitDeviceObjects(g_pD3dDev);

	m_pInfluenceTex = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pInfluenceTex->InitDeviceObjects(g_pD3dDev);

	return S_OK ;
}

HRESULT CINFCityShop::RestoreDeviceObjects()
{
	FLOG( "CINFCityShop::RestoreDeviceObjects()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pOkButton->RestoreDeviceObjects();
	m_pOkButton->SetBtnPosition(CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y);
	
	m_pCancelButton->RestoreDeviceObjects();
	m_pCancelButton->SetBtnPosition(CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y);

	m_pTriSelectSlot->RestoreDeviceObjects();
	m_pTriSlot->RestoreDeviceObjects();
#endif
	int i;
	for(i=0;i<4;i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOkButton[i]->RestoreDeviceObjects();
		m_pCancelButton[i]->RestoreDeviceObjects();
		if(m_pBuyButton[i])
			m_pBuyButton[i]->RestoreDeviceObjects();
#endif
		m_pItemTab[i]->RestoreDeviceObjects();
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pCash->RestoreDeviceObjects();
	m_pScrollUpDown->RestoreDeviceObjects();
	
#endif

	m_pSlot->RestoreDeviceObjects();
	m_pSelectSlot->RestoreDeviceObjects();
	m_pScrollBar->RestoreDeviceObjects();

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i]->RestoreDeviceObjects();
		m_pFontItemPrice[i]->RestoreDeviceObjects();
		m_pFontItemNumber[i]->RestoreDeviceObjects();
	}
	m_pFontTotalPrice->RestoreDeviceObjects();

	m_bRestored = TRUE;

	m_pInfluenceTex->RestoreDeviceObjects();

	return S_OK ;
}

HRESULT CINFCityShop::DeleteDeviceObjects()
{
	FLOG( "CINFCityShop::DeleteDeviceObjects()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pOkButton->DeleteDeviceObjects();
	m_pCancelButton->DeleteDeviceObjects();
	SAFE_DELETE(m_pOkButton);
	SAFE_DELETE(m_pCancelButton);
	m_pTriSelectSlot->DeleteDeviceObjects();
    SAFE_DELETE(m_pTriSelectSlot);
	m_pTriSlot->DeleteDeviceObjects();
    SAFE_DELETE(m_pTriSlot);

#endif
	
	int i;
	for(i=0;i<4;i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOkButton[i]->DeleteDeviceObjects();
		m_pCancelButton[i]->DeleteDeviceObjects();
		if(m_pBuyButton[i])
			m_pBuyButton[i]->DeleteDeviceObjects();
 		SAFE_DELETE(m_pOkButton[i]);
 		SAFE_DELETE(m_pCancelButton[i]);
 		SAFE_DELETE(m_pBuyButton[i]);
#endif
		m_pItemTab[i]->DeleteDeviceObjects();
		SAFE_DELETE(m_pItemTab[i]);
//		map<int, SHOP_ITEM*>::iterator it = m_mapItemInfo[i].begin();
//		while(it != m_mapItemInfo[i].end())
//		{
//			SHOP_ITEM* pItem = it->second;
//			delete pItem;
//			pItem = NULL;
//			it++;
//		}
//		m_mapItemInfo[i].clear();
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while(it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pCash->DeleteDeviceObjects();
    SAFE_DELETE(m_pCash);
	m_pScrollUpDown->DeleteDeviceObjects();
	SAFE_DELETE(m_pScrollUpDown);

	
#endif
	m_pSlot->DeleteDeviceObjects();

	SAFE_DELETE(m_pItemTab[3]);
	SAFE_DELETE(m_pSlot);


	m_pScrollBar->DeleteDeviceObjects();
	SAFE_DELETE(m_pScrollBar);
	m_pSelectSlot->DeleteDeviceObjects();
	SAFE_DELETE(m_pSelectSlot);
	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i]->DeleteDeviceObjects();
		SAFE_DELETE(m_pFontItemName[i]);
		m_pFontItemPrice[i]->DeleteDeviceObjects();
		SAFE_DELETE(m_pFontItemPrice[i]);
		m_pFontItemNumber[i]->DeleteDeviceObjects();
		SAFE_DELETE(m_pFontItemNumber[i]);
	}

	m_pFontTotalPrice->DeleteDeviceObjects();
	SAFE_DELETE(m_pFontTotalPrice);

//	memset(&m_buildingInfo, 0x00, sizeof(BUILDINGNPC));
	m_bInvalidated = FALSE;

	m_pInfluenceTex->DeleteDeviceObjects();
	SAFE_DELETE(m_pInfluenceTex);
	return S_OK ;
}


HRESULT CINFCityShop::InvalidateDeviceObjects()
{
	FLOG( "CINFCityShop::InvalidateDeviceObjects()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pOkButton->InvalidateDeviceObjects();
	m_pCancelButton->InvalidateDeviceObjects();
	m_pTriSelectSlot->InvalidateDeviceObjects();
	m_pTriSlot->InvalidateDeviceObjects();

#endif
	int i;
	for(i=0;i<4;i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOkButton[i]->InvalidateDeviceObjects();
		m_pCancelButton[i]->InvalidateDeviceObjects();
		if(m_pBuyButton[i])
			m_pBuyButton[i]->InvalidateDeviceObjects();
#endif
		m_pItemTab[i]->InvalidateDeviceObjects();
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pCash->InvalidateDeviceObjects();
	m_pScrollUpDown->InvalidateDeviceObjects();
	

#endif
	m_pSlot->InvalidateDeviceObjects();
	m_pScrollBar->InvalidateDeviceObjects();
	m_pSelectSlot->InvalidateDeviceObjects();

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i]->InvalidateDeviceObjects();
		m_pFontItemPrice[i]->InvalidateDeviceObjects();
		m_pFontItemNumber[i]->InvalidateDeviceObjects();
	}
	m_pFontTotalPrice->InvalidateDeviceObjects();
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	m_pInfluenceTex->InvalidateDeviceObjects();
	return S_OK ;
}


void CINFCityShop::Tick()
{
	FLOG( "CINFCityShop::Tick()" );


}

void CINFCityShop::Render()
{
	FLOG( "CINFCityShop::Render()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = CITY_SHOP_START_X/*+SHOP_TAB_START_X*/;
	int nPosY = CITY_SHOP_START_Y+SHOP_TAB_START_Y;
	//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(m_pBuildingInfo->BuildingIndex);
	if (pInfo->BuildingKind != BUILDINGKIND_TRIGGER_CRYSTAL)
	{
	m_pItemTab[m_nCurrentTab]->Move(nPosX, nPosY);
	m_pItemTab[m_nCurrentTab]->Render();
	}
	//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경

	int nSelectNumber = 0;

	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
	int i;
	for(i=0;i<m_nCurrentScrollNumber[m_nCurrentTab];i++)
	{
		if(it != m_vecItemInfo[m_nCurrentTab].end())
		{
			it++;
			nSelectNumber++;
		}
		else
		{
			break;
		}
	}
	if(m_pnSelectItemNumber[m_nCurrentTab])
	{
//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
		int PlusNumver = 0;
		if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
			PlusNumver =1;
	

		for(i=0;i<(SHOP_ITEM_SLOT_NUMBER + PlusNumver);i++)
		{

			if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
			{
				if(m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && m_nCurrentSelectSlot[m_nCurrentTab] == nSelectNumber + i)
				{
					m_pTriSelectSlot->Move( nPosX, nPosY + SHOP_ITEM_SLOT_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL * i);
					m_pTriSelectSlot->Render();
				}
				else
				{
					m_pTriSlot->Move( nPosX, nPosY + SHOP_ITEM_SLOT_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL * i);
					m_pTriSlot->Render();
				}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			}
			else
			{
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			if(m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && m_nCurrentSelectSlot[m_nCurrentTab] == nSelectNumber + i)
			{
				m_pSelectSlot->Move( nPosX, nPosY + SHOP_ITEM_SLOT_START_Y + SHOP_ITEM_SLOT_INTERVAL * i);
				m_pSelectSlot->Render();
			}
			else
			{
				m_pSlot->Move( nPosX, nPosY + SHOP_ITEM_SLOT_START_Y + SHOP_ITEM_SLOT_INTERVAL * i);
				m_pSlot->Render();
			}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			}
			
			if(it != m_vecItemInfo[m_nCurrentTab].end())
			{
				SHOP_ITEM* pItem = *it;
				char buf[64];
				// 2005-08-23 by ispark
				ITEM* pOriItem = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
				wsprintf( buf, "%08d", pOriItem->SourceIndex);
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경			
				if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
				{
				
					#ifdef C_EPSODE4_UI_CHANGE_JSKIM
					{
						

						char RemainCountForLimitedEditionBuf[64];					
						wsprintf(RemainCountForLimitedEditionBuf, "%03d", pItem->RemainCountForLimitedEdition);
						SIZE sPrice = m_pFontItemPrice[i]->GetStringSize( RemainCountForLimitedEditionBuf ); 
						m_pFontItemPrice[i]->DrawText( nPosX + 280 - sPrice.cx ,nPosY + SHOP_ITEM_NAME_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, RemainCountForLimitedEditionBuf, 0L);
							
						
					}
					#endif
				
					((CINFGameMain*)m_pParent)->m_pIcon->SetIcon( buf, nPosX + SHOP_ICON_START_X, nPosY + SHOP_ICON_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL*i, 1.0f );
					((CINFGameMain*)m_pParent)->m_pIcon->Render();

					
				}
				else
				{
				((CINFGameMain*)m_pParent)->m_pIcon->SetIcon( buf, nPosX + SHOP_ICON_START_X, nPosY + SHOP_ICON_START_Y + SHOP_ITEM_SLOT_INTERVAL*i, 1.0f );
				((CINFGameMain*)m_pParent)->m_pIcon->Render();

				}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경				
				char temp1[64];
				char temp2[64];
				wsprintf( temp1, "%d", (int)(CAtumSJ::GetCityWarTex(pItem->Price, m_fTexRate) + pItem->Price));
				MakeCurrencySeparator( temp2, temp1, 3, ',' );
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
				vector<string> vecMessage;
				if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
				{
					if( m_pFontItemName[i]->GetStringSize( pItem->ItemName ).cx <= SHOP_TEXT_SHOW_SIZE_TRI_X )
				{
					m_pFontItemName[i]->DrawText( nPosX + SHOP_ITEM_NAME_START_X, nPosY + SHOP_ITEM_NAME_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, pItem->ItemName, 0L);															
				}
				else
				{				
						StringCullingUserDataEx( pItem->ItemName, SHOP_TEXT_SHOW_SIZE_TRI_X, &vecMessage, m_pFontItemName[i] );// 2011. 10. 10 by jskim UI시스템 변경
						for(int j=0; j < SHOP_TEXT_SHOW; j++)
						{
							m_pFontItemName[i]->DrawText( nPosX + SHOP_ITEM_NAME_START_X, nPosY + SHOP_ITEM_NAME_START_Y - SHOP_ITEM_SLOT_INTERVAL -  8 + SHOP_ITEM_SLOT_INTERVAL*i + ( j * m_pFontItemName[i]->GetStringSize( pItem->ItemName ).cy), GUI_FONT_COLOR, (char*)vecMessage[j].c_str(), 0L);															
						}
					}
				}
				else
				{
					if( m_pFontItemName[i]->GetStringSize( pItem->ItemName ).cx <= SHOP_TEXT_SHOW_SIZE_X )
					{				
				m_pFontItemName[i]->DrawText( nPosX + SHOP_ITEM_NAME_START_X, nPosY + SHOP_ITEM_NAME_START_Y + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, pItem->ItemName, 0L);
				}
					else
					{	
//						StringCullingUserDataEx( pItem->ItemName, SHOP_TEXT_SHOW_SIZE_X, &vecMessage, m_pFontItemName[i] );// 2011. 10. 10 by jskim UI시스템 변경
						StringCullingUserDataEx( pItem->ItemName, SHOP_TEXT_SHOW_SIZE_X, &vecMessage, m_pFontItemName[i],FALSE,TRUE );// 2013-04-05 by ssjung 샾 일때 아이템 이름 글자 수 넘어 갈때 처리 보정

						for(int j=0; j < SHOP_TEXT_SHOW; j++)
						{
							m_pFontItemName[i]->DrawText( nPosX + SHOP_ITEM_NAME_START_X, nPosY + SHOP_ITEM_NAME_START_Y - 8 + SHOP_ITEM_SLOT_INTERVAL*i + ( j * m_pFontItemName[i]->GetStringSize( pItem->ItemName ).cy), GUI_FONT_COLOR, (char*)vecMessage[j].c_str(), 0L);
						}						
					}
				}				
				
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
				wsprintf(buf, "%s", temp2);
#else
				wsprintf(buf, "%7s", temp2);
#endif			
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
				if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
				{
					SIZE sPrice = m_pFontItemPrice[i]->GetStringSize( buf );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
                    m_pFontItemPrice[i]->DrawText( nPosX + 213 - sPrice.cx ,nPosY + SHOP_ITEM_NAME_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
#else

					m_pFontItemPrice[i]->DrawText( nPosX + SHOP_ITEM_PRICE_START_X - sPrice.cx ,nPosY + SHOP_ITEM_NAME_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);

#endif
					
					it++;
					
					wsprintf(buf, "%3d", m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]);
					SIZE sNum = m_pFontItemPrice[i]->GetStringSize( buf );
					m_pFontItemNumber[i]->DrawText( nPosX + SHOP_ITEM_NUMBER_X - sNum.cx, nPosY + SHOP_ITEM_NAME_START_Y - SHOP_ITEM_SLOT_INTERVAL + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
				}
				else
				{
				SIZE sPrice = m_pFontItemPrice[i]->GetStringSize( buf );
				m_pFontItemPrice[i]->DrawText( nPosX + SHOP_ITEM_PRICE_START_X - sPrice.cx ,nPosY + SHOP_ITEM_NAME_START_Y + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
				it++;
				
				wsprintf(buf, "%3d", m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]);
				SIZE sNum = m_pFontItemPrice[i]->GetStringSize( buf );
				m_pFontItemNumber[i]->DrawText( nPosX + SHOP_ITEM_NUMBER_X - sNum.cx, nPosY + SHOP_ITEM_NAME_START_Y + SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);

				}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			}
		}		
		//m_pCash->Move( nPosX + SHOP_CASH_START_X, nPosY + SHOP_CASH_START_Y );
		//m_pCash->Render();
		char temp1[64];
		char temp2[64];
		wsprintf( temp1, "%d", GetTotalPrice() );
		MakeCurrencySeparator( temp2, temp1, 3, ',' );
		SIZE sTotal = m_pFontItemPrice[i]->GetStringSize( temp2 );

	
		m_pFontTotalPrice->DrawText( nPosX + SHOP_CASH_START_X - sTotal.cx, nPosY + SHOP_CASH_START_Y, GUI_FONT_COLOR, temp2, 0L);



	}
	
	m_pOkButton->Render();
	m_pCancelButton->Render();


	//////////////////

	// scroll rendering
// 	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y);
// 	m_pScrollUpDown->Render();
// 	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y);
// 	m_pScrollUpDown->Render();

//	m_pScrollBar->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X,
//		CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab));
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
	{
		m_pScrollBar->Move( CITY_SHOP_START_X + SHOP_SCROLL_START_X,CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y - SHOP_ITEM_SLOT_INTERVAL+m_nMouseScrollPosition[m_nCurrentTab]);
		
	}
	else
	{
		m_pScrollBar->Move( CITY_SHOP_START_X + SHOP_SCROLL_START_X,CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nMouseScrollPosition[m_nCurrentTab]);
					
	}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경	
	m_pScrollBar->Render();

	if (pInfo->BuildingKind != BUILDINGKIND_TRIGGER_CRYSTAL)
	{
		//////////////////
		// 2006-02-08 by ispark, 세력 세금
		char chTexbuf[30] = {0,};
		sprintf(chTexbuf, STRMSG_C_060208_0000, m_fTexRate);
		m_pInfluenceTex->DrawText(SHOP_TEX_X, SHOP_TEX_Y, GUI_FONT_COLOR, chTexbuf, 0L);
	}
#else 
	int nPosX = CITY_SHOP_START_X+SHOP_TAB_START_X;
	int nPosY = CITY_SHOP_START_Y+SHOP_TAB_START_Y;
	
	m_pItemTab[m_nCurrentTab]->Move(nPosX, nPosY);
	m_pItemTab[m_nCurrentTab]->Render();

	int nSelectNumber = 0;

	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
	for(int i=0;i<m_nCurrentScrollNumber[m_nCurrentTab];i++)
	{
		if(it != m_vecItemInfo[m_nCurrentTab].end())
		{
			it++;
			nSelectNumber++;
		}
		else
		{
			break;
		}
	}
	if(m_pnSelectItemNumber[m_nCurrentTab])
	{
		for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
		{
			if(m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && m_nCurrentSelectSlot[m_nCurrentTab] == nSelectNumber+i)
			{
				
				m_pSelectSlot->Move(CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X, CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_INTERVAL*i);
				m_pSelectSlot->Render();
			}
			else
			{
				m_pSlot->Move(CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X, CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_INTERVAL*i);
				m_pSlot->Render();
			}
			if(it != m_vecItemInfo[m_nCurrentTab].end())
			{
				SHOP_ITEM* pItem = *it;
				char buf[64];
				// 2005-08-23 by ispark
				ITEM* pOriItem = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
				wsprintf( buf, "%08d", pOriItem->SourceIndex);
				((CINFGameMain*)m_pParent)->m_pIcon->SetIcon( buf,CITY_SHOP_START_X+SHOP_ICON_START_X,CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+1, 1.0f );
				((CINFGameMain*)m_pParent)->m_pIcon->Render();

				char temp1[64];
				char temp2[64];
				wsprintf( temp1, "%d", (int)(CAtumSJ::GetCityWarTex(pItem->Price, m_fTexRate) + pItem->Price));
				MakeCurrencySeparator( temp2, temp1, 3, ',' );
				m_pFontItemName[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X, CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, pItem->ItemName, 0L);
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
				wsprintf(buf, "%s", temp2);
#else
				wsprintf(buf, "%7s", temp2);
#endif
				m_pFontItemPrice[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X,CITY_SHOP_START_Y+SHOP_ITEM_PRICE_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
				it++;

				wsprintf(buf, "%3d", m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]);
				m_pFontItemNumber[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NUMBER_X, (CITY_SHOP_START_Y+SHOP_ITEM_NUMBER_Y+SHOP_ITEM_SLOT_INTERVAL*i)-1, GUI_FONT_COLOR, buf, 0L);
			}
		}
		m_pCash->Move(CITY_SHOP_START_X+SHOP_CASH_START_X, CITY_SHOP_START_Y+SHOP_CASH_START_Y);
		m_pCash->Render();
		char temp1[64];
		char temp2[64];
		wsprintf( temp1, "%d", GetTotalPrice() );
		MakeCurrencySeparator( temp2, temp1, 3, ',' );
		m_pFontTotalPrice->DrawText( CITY_SHOP_START_X+SHOP_CASH_START_X + FONT_TOTAL_PRICE_X, CITY_SHOP_START_Y+SHOP_CASH_START_Y + FONT_TOTAL_PRICE_Y, GUI_FONT_COLOR, temp2, 0L);
	}

	m_pOkButton[m_nOkButtonState]->Move(CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y);
	m_pOkButton[m_nOkButtonState]->Render();
	m_pCancelButton[m_nCancelButtonState]->Move(CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y);
	m_pCancelButton[m_nCancelButtonState]->Render();

	//////////////////

	// scroll rendering
	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y);
	m_pScrollUpDown->Render();
	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y);
	m_pScrollUpDown->Render();

//	m_pScrollBar->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X,
//		CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab));
	m_pScrollBar->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X,
		CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nMouseScrollPosition[m_nCurrentTab]);
	m_pScrollBar->Render();

	//////////////////
	// 2006-02-08 by ispark, 세력 세금
	char chTexbuf[30] = {0,};
	sprintf(chTexbuf, STRMSG_C_060208_0000, m_fTexRate);
		m_pInfluenceTex->DrawText(SHOP_TEX_X, SHOP_TEX_Y, GUI_FONT_COLOR, chTexbuf, 0L);

#endif


	
}


int CINFCityShop::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCityShop::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	if( g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_SHOP_SELL_ITEM ) ||
		g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_SHOP_SELL_ENERGY ) ||
		g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_SHOP_MULTI_SELL_ITEM ))
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_RBUTTONDOWN:
		{
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
			{
				CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;

				if(pItemInfo->Wear == WEAR_NOT_ATTACHED )
				{
					if( IS_COUNTABLE_ITEM(pItemInfo->Kind))
					{
						char buf[128];
						ITEM *pITEM = pItemInfo->GetItemInfo();
						if(pITEM)
						{
							char temp1[64];
							char temp2[64];
							wsprintf( temp1, "%d", CAtumSJ::GetItemSellingPriceAtShop(pITEM) );
							MakeCurrencySeparator( temp2, temp1, 3, ',' );
							wsprintf( buf, STRMSG_C_SHOP_0007, pITEM->ItemName, pITEM->MinTradeQuantity, temp2 );//"%s 몇개 파시겠습니까?[가격:%d개당 %s(스피)]"
							g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ENERGY,
								(DWORD)pItemInfo, 
								pItemInfo->CurrentCount);
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0008, COLOR_SYSTEM );//"서버로부터 정보 받는 중... 다시 시도 하세요."
						}
					}
					else
					{
						char buf[128];
						ITEM *pITEM = pItemInfo->GetItemInfo();
						if(pITEM)
						{
							char temp1[64];
							char temp2[64];
							wsprintf( temp1, "%d", CAtumSJ::GetItemSellingPriceAtShop(pITEM) );
							MakeCurrencySeparator( temp2, temp1, 3, ',' );
							wsprintf( buf, STRMSG_C_SHOP_0009, pITEM->ItemName, temp2);//"%s 를 파시겠습니까?[가격:%s(스피)]"
							g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ITEM,
								(DWORD)pItemInfo);
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0008, COLOR_SYSTEM );//"서버로부터 정보 받는중...다시 시도하세요."
						}
					}
				}
				else
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_SHOP_0010, _MESSAGE );//"장착된 아이템은 팔 수 없습니다."
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.x > CITY_SHOP_START_X &&
				pt.x < CITY_SHOP_START_X+BOX_SIZE_X &&
				pt.y > CITY_SHOP_START_Y &&
				pt.y < CITY_SHOP_START_Y+BOX_SIZE_Y )
			{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if( pt.x> CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X && 
					pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
					pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
				{
					if(m_nOkButtonState != BUTTON_STATE_DOWN)
					{
						m_nOkButtonState = BUTTON_STATE_UP;
					}
				}
				else
				{
					m_nOkButtonState = BUTTON_STATE_NORMAL;
				}
				if( pt.x> CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
					pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
					pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y &&
					pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
				{
					if(m_nCancelButtonState != BUTTON_STATE_DOWN)
					{
						m_nCancelButtonState = BUTTON_STATE_UP;
					}
				}
				else
				{
					m_nCancelButtonState = BUTTON_STATE_NORMAL;
				}
#else
				m_pOkButton->OnMouseMove(pt);
				m_pCancelButton->OnMouseMove(pt);
#endif

				//2012-04-22 by jhahn EP4 트리거 상점 버그 수정
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(m_pBuildingInfo->BuildingIndex);
				int TriggerInterval = 0;
				int nPlusNumber = 0 ;
				if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
				{
					TriggerInterval = 40;

					// 2013-02-13 by bhsohn Sound 애쉬레인 상점에서 9번째 슬롯 안골라지는 버그 수정
					nPlusNumber = 1;
					// END 2013-02-13 by bhsohn Sound 애쉬레인 상점에서 9번째 슬롯 안골라지는 버그 수정
				}
					//end 2012-04-22 by jhahn EP4 트리거 상점 버그 수정

				// 2013-02-13 by bhsohn Sound 애쉬레인 상점에서 9번째 슬롯 안골라지는 버그 수정				
//				for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
				for(int i=0;i<SHOP_ITEM_SLOT_NUMBER+nPlusNumber;i++)
				{
					if( pt.x > CITY_SHOP_START_X+SHOP_ICON_START_X && 
						pt.x < CITY_SHOP_START_X+SHOP_ICON_START_X+SHOP_ICON_SIZE &&
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
						pt.y > CITY_SHOP_START_Y+SHOP_TAB_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i - TriggerInterval &&
						pt.y < CITY_SHOP_START_Y+SHOP_TAB_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_ICON_SIZE - TriggerInterval)
#else
						pt.y > CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y < CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_ICON_SIZE)
#endif
					{
						vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
						for( int j=0;j<i+m_nCurrentScrollNumber[m_nCurrentTab];j++)
						{
							if(it != m_vecItemInfo[m_nCurrentTab].end())
							{
								it++;
							}
							else
							{
								break;
							}
						}
						if(it != m_vecItemInfo[m_nCurrentTab].end())
						{
							((CINFGameMain*)m_pParent)->SetItemInfo( 0, (*it)->ItemNum, pt.x, pt.y -(i * TriggerInterval) );
						}
						break;
					}
					else if( pt.x > CITY_SHOP_START_X+SHOP_ICON_START_X+SHOP_ICON_SIZE ||
						(pt.x > CITY_SHOP_START_X+END_OF_INVEN_X && pt.x < CITY_SHOP_START_X+SHOP_ICON_START_X) ||
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
						pt.y < CITY_SHOP_START_Y+SHOP_ICON_START_Y ||							  
#else 
						pt.y < CITY_SHOP_START_Y+SHOP_ICON_START_Y-5 ||
#endif
						pt.y > CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*SHOP_ITEM_SLOT_NUMBER)
					{
						((CINFGameMain*)m_pParent)->SetItemInfo( 0, NULL, 0, 0);
					}

				}
			}
			else
			{
				((CINFGameMain*)m_pParent)->SetItemInfo( 0, NULL, 0, 0);
			}
			if(m_bScrollLock)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if(pt.x > CITY_SHOP_START_X+SHOP_SCROLL_START_X - 4 && 
					pt.x < CITY_SHOP_START_X+SHOP_SCROLL_START_X+SHOP_SCROLL_SIZE_X + 5)
#else
				if(pt.x > CITY_SHOP_START_X+SHOP_SCROLL_START_X - 4 && 
					pt.x < CITY_SHOP_START_X+SHOP_SCROLL_START_X+SHOP_SCROLL_SIZE_X + 5)
#endif
				{
					if(pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y &&
						pt.y < CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+SHOP_SCROLL_LENGTH)
					{
	//					int interval = SHOP_SCROLL_INTERVAL(m_nCurrentTab);
						if(GetScrollLine() > 0)
						{
							int nMove = pt.y - m_ptMouse.y;
						if(nMove != 0)
						{
								m_nMouseScrollPosition[m_nCurrentTab] += nMove; 
								m_nCurrentScrollNumber[m_nCurrentTab] = GetScrollLineNumber(m_nMouseScrollPosition[m_nCurrentTab]);
								if(m_nMouseScrollPosition[m_nCurrentTab] <= 0)
								{
								m_nCurrentScrollNumber[m_nCurrentTab] = 0;
									m_nMouseScrollPosition[m_nCurrentTab] = 0;
								}
								else if(m_nMouseScrollPosition[m_nCurrentTab] >= SHOP_SCROLL_MOVE_LENGTH)
								{
									m_nCurrentScrollNumber[m_nCurrentTab] = GetScrollLine();
									m_nMouseScrollPosition[m_nCurrentTab] = SHOP_SCROLL_MOVE_LENGTH;
								}

							m_ptMouse = pt;
						}
						}
					}
					else if(pt.y <= CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y)
					{
						m_nCurrentScrollNumber[m_nCurrentTab] = 0;	
						m_nMouseScrollPosition[m_nCurrentTab] = 0;
					}
					else if(pt.y >= CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+SHOP_SCROLL_LENGTH)
					{
						m_nCurrentScrollNumber[m_nCurrentTab] = GetScrollLine();
						m_nMouseScrollPosition[m_nCurrentTab] = SHOP_SCROLL_MOVE_LENGTH;
					}
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_bScrollLock = FALSE;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			if( pt.x>CITY_SHOP_START_X+ SHOP_OK_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				m_nOkButtonState = BUTTON_STATE_DOWN;
			}
			if( pt.x>CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				m_nCancelButtonState = BUTTON_STATE_DOWN;
			}
#else
			if(m_pOkButton->OnLButtonDown(pt))
			{

			}
			if(m_pCancelButton->OnLButtonDown(pt))
			{

			}
#endif


//			for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
//			{
//				if( pt.x>CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X && 
//					pt.x<CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X+SHOP_BUY_BUTTON_SIZE_X &&
//					pt.y>CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
//					pt.y<CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_BUY_BUTTON_SIZE_Y+SHOP_ITEM_SLOT_INTERVAL*i)
//				{
//					m_nBuyButtonState[i] = BUTTON_STATE_DOWN;
//				}
//			}
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(m_pBuildingInfo->BuildingIndex);
		
			int i;
			if (pInfo->BuildingKind != BUILDINGKIND_TRIGGER_CRYSTAL)
			{
				for(i=0;i<SHOP_TAB_NUMBER;i++)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if( pt.x>CITY_SHOP_START_X+SHOP_TAB_BUTTON_SIZE_X*i && 
					pt.x<CITY_SHOP_START_X+SHOP_TAB_BUTTON_SIZE_X*(i+1) &&						  
#else						 
				if( pt.x>CITY_SHOP_START_X+SHOP_TAB_START_X+SHOP_TAB_BUTTON_SIZE_X*i && 
					pt.x<CITY_SHOP_START_X+SHOP_TAB_START_X+SHOP_TAB_BUTTON_SIZE_X*(i+1) &&
#endif
					pt.y>CITY_SHOP_START_Y+SHOP_TAB_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_TAB_START_Y+SHOP_TAB_BUTTON_SIZE_Y)
				{
					OnCancelButtonClicked();
					m_nCurrentTab = i;
					break;
				}
			}

			}


// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			if( pt.x > CITY_SHOP_START_X+SHOP_SCROLL_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_SCROLL_START_X + SHOP_SCROLL_SIZE_X &&
#else
			if( pt.x > CITY_SHOP_START_X+SHOP_SCROLL_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_SCROLL_START_X+SHOP_SCROLL_SIZE_X &&
#endif
				GetScrollLine() > 0)
			{
				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y+SHOP_SCROLL_SIZE_Y)
				{
					m_nCurrentScrollNumber[m_nCurrentTab]--;
					if(m_nCurrentScrollNumber[m_nCurrentTab]<0)
					{
						m_nCurrentScrollNumber[m_nCurrentTab] = 0;
						m_nMouseScrollPosition[m_nCurrentTab] = 0;
					}
					else
					{
						m_nMouseScrollPosition[m_nCurrentTab] = GetScrollLineInterval(m_nCurrentScrollNumber[m_nCurrentTab]);
					}
				}
				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y+SHOP_SCROLL_SIZE_Y)
				{
					m_nCurrentScrollNumber[m_nCurrentTab]++;
					if(m_nCurrentScrollNumber[m_nCurrentTab] > GetScrollLine())
					{
						m_nCurrentScrollNumber[m_nCurrentTab] = GetScrollLine();
						m_nMouseScrollPosition[m_nCurrentTab] = SHOP_SCROLL_MOVE_LENGTH;
					}
					else
					{
						m_nMouseScrollPosition[m_nCurrentTab] = GetScrollLineInterval(m_nCurrentScrollNumber[m_nCurrentTab]);
					}
				}
				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nMouseScrollPosition[m_nCurrentTab] &&
					pt.y < CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nMouseScrollPosition[m_nCurrentTab]+SHOP_SCROLL_BAR_SIZE_Y)
				{
					m_bScrollLock = TRUE;
					m_ptMouse = pt;
				}
			}
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			int PlusNumber = 0 , PlusInterVal = 0;
			if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
			{				
				PlusNumber = 1;
				PlusInterVal = SHOP_ITEM_SLOT_INTERVAL;				
			}

			if( pt.x > CITY_SHOP_START_X+SHOP_NUMBER_BUTTON_X && 
				pt.x < CITY_SHOP_START_X+SHOP_NUMBER_BUTTON_X+SHOP_NUMBER_BUTTON_SIZE_X)
			{
				for(i=0;i<SHOP_ITEM_SLOT_NUMBER+PlusNumber;i++)		   // 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
				{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
					if( pt.y>CITY_SHOP_START_Y + SHOP_TAB_START_Y + SHOP_ITEM_SLOT_START_Y + SHOP_NUMBER_UP_BUTTON_Y - PlusInterVal + SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y<CITY_SHOP_START_Y + SHOP_TAB_START_Y + SHOP_ITEM_SLOT_START_Y + SHOP_NUMBER_UP_BUTTON_Y - PlusInterVal + SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
#else				
					if( pt.y>CITY_SHOP_START_Y+SHOP_NUMBER_UP_BUTTON_Y - PlusInterVal +SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y<CITY_SHOP_START_Y+SHOP_NUMBER_UP_BUTTON_Y - PlusInterVal +SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)			  
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
#endif
					{
						if(m_vecItemInfo[m_nCurrentTab].size() > i+m_nCurrentScrollNumber[m_nCurrentTab] )
						{
							int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]];
							if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_TUNING_COLOR)
							{
								if(m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] >= 1)
									m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 1;
								else
									m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] += 
									m_vecItemInfo[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
							}
							else
							{
								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] += 
									m_vecItemInfo[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
							}

							if( GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
							{//더한 후 price가 원래돈보다 많으면 취소해야 한다.
								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = temp;
							}
						}
//						int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]++;
//						if( GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//						{//더한 후 price가 원래돈보다 많으면 취소해야 한다.
//							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = temp;
//						}
					}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
					if( pt.y>CITY_SHOP_START_Y+SHOP_TAB_START_Y + SHOP_ITEM_SLOT_START_Y + SHOP_NUMBER_DOWN_BUTTON_Y - PlusInterVal + SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y<CITY_SHOP_START_Y+SHOP_TAB_START_Y + SHOP_ITEM_SLOT_START_Y + SHOP_NUMBER_DOWN_BUTTON_Y - PlusInterVal + SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
#else				
					if( pt.y>CITY_SHOP_START_Y+SHOP_NUMBER_DOWN_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y<CITY_SHOP_START_Y+SHOP_NUMBER_DOWN_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
#endif
					{
						if(m_vecItemInfo[m_nCurrentTab].size() > i+m_nCurrentScrollNumber[m_nCurrentTab] )
						{
							int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]];
							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] -= 
								m_vecItemInfo[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
							if(m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]<0)
								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
						}
//						m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] --;
//						if(m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]<0)
//							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
					}
				}
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			PlusNumber = 0 , PlusInterVal = 0;
			if (pInfo->BuildingKind == BUILDINGKIND_TRIGGER_CRYSTAL)
			{				
				PlusNumber = 1;
				PlusInterVal = SHOP_ITEM_SLOT_INTERVAL;				
			}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
			if( pt.x > CITY_SHOP_START_X && 
				pt.x < CITY_SHOP_START_X + SHOP_ITEM_SLOT_SIZE_X)								  
#else		  
			if( pt.x > CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X + SHOP_ITEM_SLOT_SIZE_X)
#endif
			{
		//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
				for(i=0;i<(SHOP_ITEM_SLOT_NUMBER + PlusNumber);i++)
		//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
				{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
					if( pt.y > CITY_SHOP_START_Y + SHOP_TAB_START_Y + SHOP_ITEM_SLOT_START_Y - PlusInterVal + SHOP_ITEM_SLOT_SIZE_Y * i &&
						pt.y < CITY_SHOP_START_Y + SHOP_TAB_START_Y + SHOP_ITEM_SLOT_START_Y  -PlusInterVal + SHOP_ITEM_SLOT_SIZE_Y*(i+1))
#else
					if( pt.y > CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y - PlusInterVal + SHOP_ITEM_SLOT_SIZE_Y*i &&
						pt.y < CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y - PlusInterVal + SHOP_ITEM_SLOT_SIZE_Y*(i+1))
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
#endif
					{
//						OnCancelButtonClicked();
						if(m_nCurrentSelectSlot[m_nCurrentTab] != m_nCurrentScrollNumber[m_nCurrentTab]+i)						
						{
							OnCancelButtonClicked();
						}

						m_nCurrentSelectSlot[m_nCurrentTab] = m_nCurrentScrollNumber[m_nCurrentTab]+i;

			
					
					//2012-04-22 by jhahn EP4 트리거 상점 버그 수정
						BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(m_pBuildingInfo->BuildingIndex);
					
						if (pInfo->BuildingKind != BUILDINGKIND_TRIGGER_CRYSTAL)
						{
							OnChangeWearItem();
						}
					//end 2012-04-22 by jhahn EP4 트리거 상점 버그 수정
						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if( pt.x > CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X &&
				pt.x < CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X+SHOP_ITEM_SLOT_SIZE_X &&
				pt.y > CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y &&
				pt.y < CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_SIZE_Y*SHOP_ITEM_SLOT_NUMBER &&
				GetScrollLine() > 0)
			{
				if((int)wParam<0)
				{
					m_nCurrentScrollNumber[m_nCurrentTab]++;
					if(m_nCurrentScrollNumber[m_nCurrentTab] > GetScrollLine())
					{
						m_nCurrentScrollNumber[m_nCurrentTab] = GetScrollLine();
						m_nMouseScrollPosition[m_nCurrentTab] = SHOP_SCROLL_MOVE_LENGTH;
					}
					else
					{
						m_nMouseScrollPosition[m_nCurrentTab] = GetScrollLineInterval(m_nCurrentScrollNumber[m_nCurrentTab]);
					}
//					if(m_nCurrentScrollNumber[m_nCurrentTab]>SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab))
//						m_nCurrentScrollNumber[m_nCurrentTab] = SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab);
					return INF_MSGPROC_BREAK;
				}
				else
				{
					m_nCurrentScrollNumber[m_nCurrentTab]--;
					if(m_nCurrentScrollNumber[m_nCurrentTab]<0)
					{
						m_nCurrentScrollNumber[m_nCurrentTab] = 0;
						m_nMouseScrollPosition[m_nCurrentTab] = 0;
					}
					else
					{
						m_nMouseScrollPosition[m_nCurrentTab] = GetScrollLineInterval(m_nCurrentScrollNumber[m_nCurrentTab]);
					}

					return INF_MSGPROC_BREAK;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_bScrollLock = FALSE;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
			if( pt.x>CITY_SHOP_START_X+ SHOP_OK_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y &&
				pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				int nBuy = 0;
				if(m_nOkButtonState == BUTTON_STATE_DOWN)
				{
					for(int i=0; i<SHOP_ITEM_TAB_NUMBER; i++)
					{
						for(int j=0; j<m_vecItemInfo[i].size(); j++)
						{
							if(m_pnSelectItemNumber[i][j])
							{
								nBuy = 1;
							}
						}
					}
					if(nBuy)
					{
						OnOkButtonClicked();
					
					}
					else
					{
						OnBuyButtonClicked(m_nCurrentSelectSlot[m_nCurrentTab]);
					}
				}
				m_nOkButtonState = BUTTON_STATE_UP;
			if( pt.x>CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				if(m_nCancelButtonState == BUTTON_STATE_DOWN)
				{
					OnCancelButtonClicked();
				}
				m_nCancelButtonState = BUTTON_STATE_UP;
			}
			}
#else
			if(m_pOkButton->OnLButtonUp(pt))
			{
				int nBuy = 0;
				for(int i=0; i<SHOP_ITEM_TAB_NUMBER; i++)
				{
					for(int j=0; j<m_vecItemInfo[i].size(); j++)
					{
						if(m_pnSelectItemNumber[i][j])
						{
							nBuy = 1;
						}
					}
				}
				if(nBuy)
				{
					OnOkButtonClicked();					
				
				}
				else
				{
					OnBuyButtonClicked(m_nCurrentSelectSlot[m_nCurrentTab]);
				
				}
			}
			if(m_pCancelButton->OnLButtonUp(pt))
			{
				OnCancelButtonClicked();
			}
#endif
//			for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
//			{
//				if( pt.x>CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X && 
//					pt.x<CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X+SHOP_BUY_BUTTON_SIZE_X &&
//					pt.y>CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i && 
//					pt.y<CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_BUY_BUTTON_SIZE_Y+SHOP_ITEM_SLOT_INTERVAL*i)
//				{
//					if(m_nBuyButtonState[i] == BUTTON_STATE_DOWN)
//					{
//						OnBuyButtonClicked(i);
//					}
//					m_nBuyButtonState[i] = BUTTON_STATE_UP;
//				}
//			}

			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
				pt.x > CITY_SHOP_START_X+SHOP_SELL_ITEM_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_SELL_ITEM_END_X &&
				pt.y > CITY_SHOP_START_Y+SHOP_SELL_ITEM_START_Y &&
				pt.y<CITY_SHOP_START_Y+SHOP_SELL_ITEM_END_Y)
			{
				CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;

				if(pItemInfo->Wear == WEAR_NOT_ATTACHED )
				{
					// 2007-03-02 by bhsohn 다중 선택 추가 보안					
					int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
					if(nSelItemCnt <= 0)
					{						
					if( IS_COUNTABLE_ITEM(pItemInfo->Kind))
					{
						char buf[128];
						ITEM *pITEM = pItemInfo->GetItemInfo();
						if(pITEM)
						{
							char temp1[64];
							char temp2[64];
							wsprintf( temp1, "%d", CAtumSJ::GetItemSellingPriceAtShop(pITEM) );
							MakeCurrencySeparator( temp2, temp1, 3, ',' );
							wsprintf( buf, STRMSG_C_SHOP_0007, pITEM->ItemName, pITEM->MinTradeQuantity, temp2 );//"%s 몇개 파시겠습니까?[가격:%d개당 %s(스피)]"
							g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ENERGY,
								(DWORD)pItemInfo, 
								pItemInfo->CurrentCount);
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0008, COLOR_SYSTEM );
						}
					}
					else
					{
						char buf[128];
						ITEM *pITEM = pItemInfo->GetItemInfo();						
						if(pITEM)
						{
							char temp1[64];
							char temp2[64];
							wsprintf( temp1, "%d", CAtumSJ::GetItemSellingPriceAtShop(pITEM) );
							MakeCurrencySeparator( temp2, temp1, 3, ',' );
							wsprintf( buf, STRMSG_C_SHOP_0009, pITEM->ItemName, temp2);
							g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ITEM,
								(DWORD)pItemInfo);
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0008, COLOR_SYSTEM );
						}
					}
					}
					else
					{
						// 다중 아이템 선택 창 팝업 
						((CINFGameMain*)m_pParent)->PopupMultiItemSelect();
						// 2008-11-13 by bhsohn EP3-2 에서 다중 선택 판매 안되는 버그 수정
						g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
						g_pGameMain->ClearSetIconInfo();
						return INF_MSGPROC_BREAK;
						// end 2008-11-13 by bhsohn EP3-2 에서 다중 선택 판매 안되는 버그 수정
					}
					// end 2007-03-02 by bhsohn 다중 선택 추가 보안
				}
				else
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_SHOP_0010, _MESSAGE );
				}
			}

			((CINFGameMain*)m_pParent)->SetItemInfo( 0, 0, 0, 0 );
		}
		break;
		// 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
	case WM_CHAR:
 		{
 			char code = wParam;
// 			if(m_pnSelectItemNumber[m_nCurrentTab] && g_pD3dApp->m_bChatMode == FALSE && m_pBuildingInfo->BuildingKind != BUILDINGKIND_TUNING_COLOR)
// 			{
// 				if(code >= '0' && code <= '9' && m_nCurrentSelectSlot[m_nCurrentTab]>=0)
// 				{
// 					int temp = m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]];
// 					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] *= 10;
// 					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] += code - '0';
// 					if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
// 						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = temp;
// 				}
// 				else if( code == VK_BACK )
// 				{
// 					if( m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && 
// 						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] > 0 )
// 					{
// 						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] /= 10;
// 					}
// 				}
// 			}
 			if( code == VK_ESCAPE )
 			{
 				OnCancelButtonClicked();
 			}
 		}
		break;
	case WM_KEYDOWN:
		{
			DWORD dwCode = lParam;
			if(m_pnSelectItemNumber[m_nCurrentTab] && g_pD3dApp->m_bChatMode == FALSE && m_pBuildingInfo->BuildingKind != BUILDINGKIND_TUNING_COLOR)
			{
				int nNumber = g_pGameMain->GetKeyDownToNumber(dwCode);				
				if((nNumber != -1 ) && m_nCurrentSelectSlot[m_nCurrentTab]>=0)
				{
					int temp = m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
					if(temp >= 9999 )
					{
						temp = 9999;
					}
					else
					{
					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] *= 10;
					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] += nNumber;
					}
#else
					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] *= 10;
					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] += nNumber;					
#endif
					if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
					{
						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = temp;
					}
				}
				else if(0x000e0001 == dwCode)
				{
					if( m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && 
						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] > 0 )
					{
						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] /= 10;
					}
				}
			}			
		}
		break;
		// end 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityShop::OnOkButtonClicked()
{
	FLOG( "CINFCityShop::OnOkButtonClicked()" );
	for(int tab=0;tab<4;tab++)
	{
//		map<int,SHOP_ITEM*>::iterator it = m_mapItemInfo[tab].begin();
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[tab].begin();
		int i=0;
//		while(it != m_mapItemInfo[tab].end())
		while(it != m_vecItemInfo[tab].end())
		{
			if(m_pnSelectItemNumber[tab][i]>0)
			{
				// 2005-12-09 by ispark, 갯수가 정해질 때
				if(m_pBuildingInfo->BuildingKind != BUILDINGKIND_TUNING_COLOR)
				{
					// 2008-07-16 by bhsohn 논 카운터블 아이템 10개 이상 구매불가
					//BuyItem((*it)->ItemNum, m_pnSelectItemNumber[tab][i]);
					BuyItem((*it)->ItemNum, m_pnSelectItemNumber[tab][i], (*it)->ItemKind);
				}
				else
					BuyItemColorShop((*it)->ItemNum);

				// 2005-03-07 by jschoi
//				g_pD3dApp->m_bRequestEnable = FALSE;
//
////				SHOP_ITEM* pItem = it->second;
//				SHOP_ITEM* pItem = *it;
//				MSG_FC_SHOP_BUY_ITEM sMsg;
//				memset(&sMsg,0x00,sizeof(sMsg));
//				sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
//				sMsg.ItemNum = pItem->ItemNum;
//				sMsg.Amount = m_pnSelectItemNumber[tab][i];
//				g_pFieldWinSocket->SendMsg( T_FC_SHOP_BUY_ITEM, (char*)&sMsg, sizeof(sMsg) );
//				m_pnSelectItemNumber[tab][i] = 0;
			}
			it++;
			i++;
		}
	}
}

void CINFCityShop::OnCancelButtonClicked()
{
	for(int tab=0;tab<4;tab++)
	{
//		map<int,SHOP_ITEM*>::iterator it = m_mapItemInfo[tab].begin();
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[tab].begin();
		int i=0;
//		while(it != m_mapItemInfo[tab].end())
		while(it != m_vecItemInfo[tab].end())
		{
			m_pnSelectItemNumber[tab][i] = 0;
			it++;
			i++;
		}
	}
//	MSG_FC_EVENT_LEAVE_BUILDING sMsg;
//	sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
//	g_pFieldWinSocket->SendMsg( T_FC_EVENT_LEAVE_BUILDING, (char*)&sMsg, sizeof(sMsg) );
	
}

void CINFCityShop::OnBuyButtonClicked(int nSlotNumber)
{
	FLOG( "CINFCityShop::OnBuyButtonClicked(int nSlotNumber)" );
	if( nSlotNumber < 0 )
	{
		return;
	}
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
	for(int i=0;i<nSlotNumber;i++)
	{
		if(it != m_vecItemInfo[m_nCurrentTab].end())
		{
			it++;
		}
		else
		{
			break;
		}
	}
	if(it != m_vecItemInfo[m_nCurrentTab].end())
	{
		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber] == 0)
		{
			m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber] = (*it)->MinTradeQuantity;
			if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
			{
				m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
					g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0011,COLOR_ERROR );//"아이템을 구입하기 위한 스피가 부족합니다"
			}
		}
		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber]>0)
		{
			if(m_pBuildingInfo->BuildingKind != BUILDINGKIND_TUNING_COLOR)
			{
				// 2008-07-16 by bhsohn 논 카운터블 아이템 10개 이상 구매불가
				//BuyItem((*it)->ItemNum, m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber]);
				BuyItem((*it)->ItemNum, m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber], (*it)->ItemKind);				
			}
			else
				BuyItemColorShop((*it)->ItemNum);

		}
		m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber] = 0;
	}

}

UINT CINFCityShop::GetTotalPrice()
{
	FLOG( "CINFCityShop::GetTotalPrice()" );
	int total = 0;
	for(int tab=0;tab<4;tab++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[tab].begin();
		int i=0;
		while(it != m_vecItemInfo[tab].end())
		{
			if(m_pnSelectItemNumber[tab][i]>0)
			{
				SHOP_ITEM* pItem = *it;
				int price = pItem->Price * m_pnSelectItemNumber[tab][i] / (pItem->MinTradeQuantity == 0 ? 1.0f : pItem->MinTradeQuantity);
				int tex = CAtumSJ::GetCityWarTex(price, m_fTexRate); 
				total += price + tex;
			}
			it++;
			i++;
		}
	}
	
	return total;
}

void CINFCityShop::BuyItem(int nItemNum, int nAmount, int ItemKind)
{
	FLOG( "CINFCityShop::BuyItem(int nItemNum, int nAmount)" );

	// 2008-07-16 by bhsohn 논 카운터블 아이템 10개 이상 구매불가	
	if( !IS_COUNTABLE_ITEM(ItemKind) && IS_GENERAL_ITEM(ItemKind))
	{
		if(nAmount > MAX_ITEM_COUNTS_FOR_BUY)
		{
			char errStr[256];
			wsprintf(errStr,STRMSG_C_080716_0204);	// "해당아이템은 한번 구매시 최대 개수는 10개입니다."	
			g_pD3dApp->m_pChat->CreateChatChild(errStr,COLOR_ERROR);
			return;
		}
	}	
	// end 2008-07-16 by bhsohn 논 카운터블 아이템 10개 이상 구매불가

	// 2005-03-07 by jschoi
	g_pD3dApp->m_bRequestEnable = FALSE;

	MSG_FC_SHOP_BUY_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
	sMsg.ItemNum = nItemNum;
	sMsg.Amount = nAmount;
	g_pFieldWinSocket->SendMsg( T_FC_SHOP_BUY_ITEM, (char*)&sMsg, sizeof(sMsg) );
//	char buffer[SIZE_MAX_PACKET];
//	int nType = T_FC_SHOP_BUY_ITEM;
//	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

	// 2013-06-20 by bhsohn 상점에서 아이템 구매후 숫자 초기화 코드 추가
#ifdef	C_SHOP_ITEMCNT_INIT_BHSOHN
	InitSelectItemNum(); // 갯수 초기화
#endif
	// END 2013-06-20 by bhsohn 상점에서 아이템 구매후 숫자 초기화 코드 추가

	
			
}

void CINFCityShop::InputColorShopItem()
{
	// 칼라상점 선택시 클라이언트 내에서 검색후 아이템 설정
	for(int i=0;i<4;i++)
	{
		// 2006-04-05 by ispark
		SAFE_DELETE_ARRAY(m_pnSelectItemNumber[i]);
		m_nCurrentScrollNumber[i] = 0;
		m_nCurrentSelectSlot[i] = -1;
		m_nMouseScrollPosition[i] = 0;
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while(it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();
	}
#ifdef _REWORKED_COLORSHOP
	vector<ITEM*> vectColorList = g_pDatabase->GetColourItemsByReqUnitKind(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind);
	vector<ITEM*>::iterator it = vectColorList.begin();
	while(it != vectColorList.end())
	{
		ITEM *item = *it;
		if(item &&
			// 2007-08-10 by dgwoo 캐쉬아이템은 컬러상점에서 판매하지 않는다.
			!COMPARE_BIT_FLAG(item->ItemAttribute, ITEM_ATTR_CASH_ITEM))		
		{
			SHOP_ITEM* pItemShop = new SHOP_ITEM;
			pItemShop->ItemNum							= item->ItemNum;
			strcpy(pItemShop->ItemName, item->ItemName);
			pItemShop->MinTradeQuantity					= item->MinTradeQuantity;
			// 2015-05-28 Future, temporary fix (not sure if this does the work)
#ifndef SHOP_PRICES_PER_BUILDING_NPC
			pItemShop->Price							= item->Price;
#else
			pItemShop->Price = item->ItemNum == COLOR_RESET_ITEMNUM ? 10000 : DEFAULT_COLOR_PRICE;
#endif // !SHOP_PRICES_PER_BUILDING_NPC
			pItemShop->ItemKind							= item->Kind;

			// 2009-01-28 by bhsohn 캐쉬샵 처리
			pItemShop->CashShopIndex	= 0;
			// end 2009-01-28 by bhsohn 캐쉬샵 처리
#ifdef _REWORKED_COLORSHOP
			if (item->ItemNum == COLOR_RESET_ITEMNUM)
			{
				if (IS_BGEAR(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
				{
					m_vecItemInfo[SHOP_TAB_B_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_B_GEAR;
				}
				if (IS_IGEAR(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
				{
					m_vecItemInfo[SHOP_TAB_I_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_I_GEAR;
				}
				if (IS_MGEAR(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
				{
					m_vecItemInfo[SHOP_TAB_M_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_M_GEAR;
				}
				if (IS_AGEAR(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
				{
					m_vecItemInfo[SHOP_TAB_A_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_A_GEAR;
				}
			}
			else
			{
#endif
				if(IS_BGEAR(item->ReqUnitKind))
				{					
					m_vecItemInfo[SHOP_TAB_B_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_B_GEAR;
				}
				if(IS_IGEAR(item->ReqUnitKind))
				{								
					m_vecItemInfo[SHOP_TAB_I_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_I_GEAR;
				}
				if(IS_MGEAR(item->ReqUnitKind))
				{					
					m_vecItemInfo[SHOP_TAB_M_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_M_GEAR;
				}
				if(IS_AGEAR(item->ReqUnitKind))
				{					
					m_vecItemInfo[SHOP_TAB_A_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_A_GEAR;
				}
#ifdef _REWORKED_COLORSHOP
			}
#endif
		}
		it++;
	}
#else
	CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
	int i;
	if(pItemInfo)
	{
		for(i=1; i <= g_pD3dApp->GetUnitArmorColorMax();i++)
		{			
			ITEM *item = g_pDatabase->GetServerItemInfo(SOURCEINDEXTOITEMNUM(pItemInfo->ItemInfo->SourceIndex, i));
			if(item &&
				// 2007-08-10 by dgwoo 캐쉬아이템은 컬러상점에서 판매하지 않는다.
				!COMPARE_BIT_FLAG(item->ItemAttribute, ITEM_ATTR_CASH_ITEM))		
			{
				SHOP_ITEM* pItemShop = new SHOP_ITEM;
				pItemShop->ItemNum							= item->ItemNum;
				strcpy(pItemShop->ItemName, item->ItemName);
				pItemShop->MinTradeQuantity					= item->MinTradeQuantity;
				// 2015-05-28 Future, temporary fix (not sure if this does the work)
#ifndef SHOP_PRICES_PER_BUILDING_NPC
				pItemShop->Price							= item->Price;
#endif // !SHOP_PRICES_PER_BUILDING_NPC
				pItemShop->ItemKind							= item->Kind;
				
				// 2009-01-28 by bhsohn 캐쉬샵 처리
				pItemShop->CashShopIndex	= 0;
				// end 2009-01-28 by bhsohn 캐쉬샵 처리

				if(IS_BGEAR(item->ReqUnitKind))
				{					
					m_vecItemInfo[SHOP_TAB_B_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_B_GEAR;
				}
				if(IS_IGEAR(item->ReqUnitKind))
				{								
					m_vecItemInfo[SHOP_TAB_I_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_I_GEAR;
				}
				if(IS_MGEAR(item->ReqUnitKind))
				{					
					m_vecItemInfo[SHOP_TAB_M_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_M_GEAR;
				}
				if(IS_AGEAR(item->ReqUnitKind))
				{					
					m_vecItemInfo[SHOP_TAB_A_GEAR].push_back(pItemShop);
					m_nCurrentTab = SHOP_TAB_A_GEAR;
				}
			}
		}
	}
#endif
	
#ifdef _REWORKED_COLORSHOP
	int i;
#endif
	for(i=0;i<4;i++)
	{
		SAFE_DELETE_ARRAY(m_pnSelectItemNumber[i]);
		if(m_vecItemInfo[i].size() > 0)
		{
			int num = m_vecItemInfo[i].size();
			m_pnSelectItemNumber[i] = new int[num];
			memset(m_pnSelectItemNumber[i], 0x00, num*sizeof(UINT));
		}
	}
}

int CINFCityShop::InputItem(char* pMsg)
{
	FLOG( "CINFCityShop::InputItem(char* pMsg)" );
	if(pMsg)
	{
		ITEM *item = g_pDatabase->GetServerItemInfo(((SHOP_ITEM*)pMsg)->ItemNum);
		if(item)
		{
			// 2006-01-11 by ispark, 같은 정보가 있는지 확인
			// 렉에 의해서 중복 입력이 되는 현상이 있다. 그래서 같은 정보가 있으면 넘어감
		
			if(IS_BGEAR(item->ReqUnitKind))
			{
				SHOP_ITEM* pItem = new SHOP_ITEM;
				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
//				m_mapItemInfo[SHOP_TAB_B_GEAR][pItem->ItemNum] = pItem;
				if(FindCityShopData(SHOP_TAB_B_GEAR, pItem) == NULL)
					m_vecItemInfo[SHOP_TAB_B_GEAR].push_back(pItem);
			}
			if(IS_IGEAR(item->ReqUnitKind))
			{
				SHOP_ITEM* pItem = new SHOP_ITEM;
				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
//				m_mapItemInfo[SHOP_TAB_I_GEAR][pItem->ItemNum] = pItem;
				if(FindCityShopData(SHOP_TAB_I_GEAR, pItem) == NULL)
					m_vecItemInfo[SHOP_TAB_I_GEAR].push_back(pItem);
			}
			if(IS_MGEAR(item->ReqUnitKind))
			{
				SHOP_ITEM* pItem = new SHOP_ITEM;
				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
//				m_mapItemInfo[SHOP_TAB_M_GEAR][pItem->ItemNum] = pItem;
				if(FindCityShopData(SHOP_TAB_M_GEAR, pItem) == NULL)
					m_vecItemInfo[SHOP_TAB_M_GEAR].push_back(pItem);
			}
			if(IS_AGEAR(item->ReqUnitKind))
			{
				SHOP_ITEM* pItem = new SHOP_ITEM;
				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
//				m_mapItemInfo[SHOP_TAB_A_GEAR][pItem->ItemNum] = pItem;
				if(FindCityShopData(SHOP_TAB_A_GEAR, pItem) == NULL)
					m_vecItemInfo[SHOP_TAB_A_GEAR].push_back(pItem);
			}
		}
		else
		{
			SHOP_ITEM* pItem = new SHOP_ITEM;
			memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
//			m_mapItemInfo[SHOP_TAB_B_GEAR][pItem->ItemNum] = pItem;
			if(FindCityShopData(SHOP_TAB_B_GEAR, pItem) == NULL)
			m_vecItemInfo[SHOP_TAB_B_GEAR].push_back(pItem);
		}
	}
	return sizeof(SHOP_ITEM);
}

void CINFCityShop::UpdateFindShopItem(int nTab, ITEM* pITEM)
{
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[nTab].begin();
	while(it != m_vecItemInfo[nTab].end())
	{
		if( (*it)->ItemNum == pITEM->ItemNum )
		{
			(*it)->ItemKind = pITEM->Kind;
			strcpy( (*it)->ItemName, pITEM->ItemName );
			(*it)->ItemNum = pITEM->ItemNum;
			(*it)->MinTradeQuantity = pITEM->MinTradeQuantity;
#ifndef SHOP_PRICES_PER_BUILDING_NPC
			(*it)->Price = pITEM->Price;
#endif // !SHOP_PRICES_PER_BUILDING_NPC
			return;
		}
		it++;
	}
	SHOP_ITEM* pNew = new SHOP_ITEM;
	pNew->ItemKind = pITEM->Kind;
	strcpy( pNew->ItemName, pITEM->ItemName );
	pNew->ItemNum = pITEM->ItemNum;
	pNew->MinTradeQuantity = pITEM->MinTradeQuantity;
#ifndef SHOP_PRICES_PER_BUILDING_NPC
	pNew->Price = pITEM->Price;
#endif // !SHOP_PRICES_PER_BUILDING_NPC

	
	// 2009-01-28 by bhsohn 캐쉬샵 처리	
	pNew->CashShopIndex= 0;	
	// end 2009-01-28 by bhsohn 캐쉬샵 처리
	m_vecItemInfo[nTab].push_back( pNew );
}

void CINFCityShop::UpdateItem(int nItemNum)
{
	FLOG( "CINFCityShop::UpdateItem(int nItemNum)" );
	ITEM *pITEM = g_pDatabase->GetServerItemInfo(nItemNum);
	if(pITEM)
	{
		if(IS_BGEAR(pITEM->ReqUnitKind))
		{
			UpdateFindShopItem( SHOP_TAB_B_GEAR, pITEM);
		}
		if(IS_IGEAR(pITEM->ReqUnitKind))
		{
			UpdateFindShopItem( SHOP_TAB_B_GEAR, pITEM);
		}
		if(IS_MGEAR(pITEM->ReqUnitKind))
		{
			UpdateFindShopItem( SHOP_TAB_B_GEAR, pITEM);
		}
		if(IS_AGEAR(pITEM->ReqUnitKind))
		{
			UpdateFindShopItem( SHOP_TAB_B_GEAR, pITEM);
		}
		for(int i=0;i<4;i++)
		{
			SAFE_DELETE_ARRAY(m_pnSelectItemNumber[i]);
			if(m_vecItemInfo[i].size() > 0)
			{
				int num = m_vecItemInfo[i].size();
				m_pnSelectItemNumber[i] = new int[num];
				memset(m_pnSelectItemNumber[i], 0x00, num*sizeof(UINT));
			}
		}
	}
}


struct sort_EnergyItemByItemName
{
	bool operator()(SHOP_ITEM *p1, SHOP_ITEM *p2)
	{
		return strcmp(p1->ItemName, p2->ItemName) < 0;
	}
};


///////////////////////////////////////////////////////////////////////////////
/// \fn			void sortItemKindEnergyByItemName(vector<SHOP_ITEM*> *i_pVectorShopItemPtr)
/// \brief		
/// \author		cmkwon
/// \date		2004-12-03 ~ 2004-12-03
/// \warning	인자의 벡터는 정렬되어 있어야 한다
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void sortItemKindEnergyByItemName(vector<SHOP_ITEM*> *i_pVectorShopItemPtr)
{
	vector<SHOP_ITEM*>::iterator itStart = i_pVectorShopItemPtr->end(), itEnd = i_pVectorShopItemPtr->end();
	vector<SHOP_ITEM*>::iterator itr(i_pVectorShopItemPtr->begin());
	for(; itr != i_pVectorShopItemPtr->end(); itr++)
	{
		if((*itr)->ItemKind == ITEMKIND_ENERGY)
		{
			if (i_pVectorShopItemPtr->end() == itStart)
			{
				itStart = itr;
			}
			else
			{
				itEnd = itr;
			}
		}
	}

	if (i_pVectorShopItemPtr->end() == itStart
		|| i_pVectorShopItemPtr->end() == itEnd)
	{
		return;
	}
	itEnd++;

	sort(itStart, itEnd, sort_EnergyItemByItemName());
}


int orderByKindRefineryItem(ITEM* i_pItem)
{
	switch(i_pItem->Kind)
	{
	case ITEMKIND_BULLET:
		{
		}
		return 100;
	case ITEMKIND_ENERGY:
		{
			// 2009-04-21 by bhsohn 아이템 DesParam추가
			//switch(i_pItem->DestParameter1)
			switch(i_pItem->ArrDestParameter[0])
			{
			case DES_GRADUAL_HP_UP:		return 101;
			case DES_GRADUAL_SP_UP:		return 102;
			case DES_GRADUAL_DP_UP:		return 103;
			case DES_GRADUAL_EP_UP:		return 104;
			}			
		}
		return 200;
	case ITEMKIND_SUPPORT:
		{
		}
		return 300;
	case ITEMKIND_TANK:
		{
		}
		return 400;
	case ITEMKIND_RADAR:
		{
			
		}
		return 500;
	}

	return 1000;
}


struct CompareRefineryItemByKind
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		ITEM* pITEM1 = g_pDatabase->GetServerItemInfo(pPtr1->ItemNum);
		ITEM* pITEM2 = g_pDatabase->GetServerItemInfo(pPtr2->ItemNum);
		if( pITEM1 == NULL || pITEM2 == NULL )
		{
			return false;
		}

		int order1 = orderByKindRefineryItem(pITEM1);		
		int order2 = orderByKindRefineryItem(pITEM2);
		if(order1 < order2)
		{
			return true;
		}
		return false;
	}
};

void CINFCityShop::InputItemDone()
{
	FLOG( "CINFCityShop::InputItemDone()" );
	for(int i=0;i<4;i++)
	{
		SAFE_DELETE_ARRAY(m_pnSelectItemNumber[i]);
		if(m_vecItemInfo[i].size() > 0)
		{
			int num = m_vecItemInfo[i].size();
			m_pnSelectItemNumber[i] = new int[num];
			memset(m_pnSelectItemNumber[i], 0x00, num*sizeof(UINT));
			if(m_pBuildingInfo)
			{
				switch(m_pBuildingInfo->BuildingKind)
				{
				case BUILDINGKIND_REFINERY:
					{
						sort(m_vecItemInfo[i].begin(), m_vecItemInfo[i].end(), CompareRefineryItemByKind());

						sortItemKindEnergyByItemName(&m_vecItemInfo[i]);
					}
					break;
				default:
					{
						sort(m_vecItemInfo[i].begin(), m_vecItemInfo[i].end(), CompareLevel());
					}
				}
			}			
		}

	}
	//	m_bInputItemDone = TRUE;
	///////////////////////////////
	// UNLOCK BUILDING BUTTON
	///////////////////////////////
}


void CINFCityShop::SellItem( LONGLONG nUniqueNumber, int nAmount/*개수**/, BYTE itemKind )
{
	FLOG( "CINFCityShop::SellItem( LONGLONG nUniqueNumber, int nAmount/*개수*/, BYTE itemKind )" );
	
	// 2005-03-07 by jschoi
	g_pD3dApp->m_bRequestEnable = FALSE;

	MSG_FC_SHOP_SELL_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	sMsg.Amount = nAmount;
	sMsg.ItemKind = itemKind;
	sMsg.ItemUniqueNumber = nUniqueNumber;
	sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
	int nType = T_FC_SHOP_SELL_ITEM;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CINFCityShop::OnChangeWearItem()
{
	int nSlotNumber = m_nCurrentSelectSlot[m_nCurrentTab];
	ITEM *item = NULL;
	if( nSlotNumber < 0 )
		return;

	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
	for(int i=0;i<nSlotNumber;i++){
		if(it != m_vecItemInfo[m_nCurrentTab].end()){
			it++;
		}
		else{
			break;
		}
	}
	if(it != m_vecItemInfo[m_nCurrentTab].end()){
		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber] == 0){
			item = g_pDatabase->GetServerItemInfo((*it)->ItemNum);
		}
	}

	if(item)
	{
		if(m_pBuildingInfo->BuildingKind != BUILDINGKIND_TUNING_COLOR)
			g_pGameMain->ChangeItemByShop(item->ItemNum);
		else
		{
			g_pGameMain->ChangeItemByShop(item->ItemNum);
			/*
			// g_pGameMain->ChangeItemByShop(item->ItemNum, nUnitColor);
			CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
			if(pItemInfo)
			{
				int nArmorColor = 0;
				if(pItemInfo->ItemInfo->SourceIndex == 0)
					nArmorColor = pItemInfo->ItemNum;				
				else
					nArmorColor = pItemInfo->ItemInfo->SourceIndex;
				int nColorNum = item->ItemNum - nArmorColor;
				g_pGameMain->ChangeItemByShop(item->ItemNum, nColorNum);
			}*/
		}
	}
}


void CINFCityShop::BuyItemColorShop(int nItemNum)
{
	FLOG( "CINFCityShop::BuyItemColorShop(int nItemNum)" );
	// 2006-05-20 by ispark, 이미 하고 있는 색깔이라면
	CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
	if(pItemInfo)
	{
		int nArmorColor = 0;
		if(pItemInfo->ItemInfo->SourceIndex == 0)
			nArmorColor = pItemInfo->ItemNum;				
		else
			nArmorColor = pItemInfo->ItemInfo->SourceIndex;
		int nColorNum = nItemNum - nArmorColor;

		if(nColorNum == g_pShuttleChild->GetAmorColor())
		{
			char errStr[64];
			wsprintf(errStr,STRMSG_C_060519_0003);	// "현재 쓰고 있는 칼라입니다."
			g_pD3dApp->m_pChat->CreateChatChild(errStr,COLOR_ERROR);
			return;
		}
	}

	MSG_FC_SHOP_BUY_COLOR_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
	sMsg.ItemNum = nItemNum;
	g_pFieldWinSocket->SendMsg( T_FC_SHOP_BUY_COLOR_ITEM, (char*)&sMsg, sizeof(sMsg) );
}

int CINFCityShop::GetScrollLine()
{
	int nInvenTotalNum = SHOP_SCROLL_NUMBER(m_nCurrentTab);
	int ntemp = nInvenTotalNum - SHOP_ITEM_SLOT_NUMBER;
	if(ntemp < 0)
	{
		ntemp = 0;
	}
	return ntemp;
}

float CINFCityShop::GetScrollLineInterval(int nScrollLineNum)
{
	int ntemp = GetScrollLine();
	float fLineNum = 0.0f;
	if(ntemp)
		fLineNum = (SHOP_SCROLL_MOVE_LENGTH * nScrollLineNum) / ntemp;

	return fLineNum;
}

int CINFCityShop::GetScrollLineNumber(float nLength)
{
	int ntemp = GetScrollLine();
	float fLineNum = (nLength * ntemp) / SHOP_SCROLL_MOVE_LENGTH;

	return (int)fLineNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityShop::FindCityShopData(SHOP_ITEM *pShopItem)
/// \brief		샵 아이템 검색
/// \author		ispark
/// \date		2006-01-11 ~ 2006-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHOP_ITEM* CINFCityShop::FindCityShopData(int nTabIndex, SHOP_ITEM *pShopItem)
{
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[nTabIndex].begin();
	while(it != m_vecItemInfo[nTabIndex].end())
	{
		SHOP_ITEM* pitShopSkill = *it;
		if(pShopItem->ItemNum == pitShopSkill->ItemNum)
		{
			return pShopItem;
		}

		it++;
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityShop::FindCityShopDataByColor(int nIndex)
/// \brief		현재탭의 색깔 데이타를 뽑느다.
/// \author		ispark
/// \date		2006-05-19 ~ 2006-05-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHOP_ITEM* CINFCityShop::FindCityShopDataByColor(int nColor)
{
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
	while(it != m_vecItemInfo[m_nCurrentTab].end())
	{
		SHOP_ITEM* pitShopSkill = *it;
		ITEM* item = g_pDatabase->GetServerItemInfo(pitShopSkill->ItemNum);

		if(item->ItemNum == nColor)
		{
			return pitShopSkill;
		}
		it++;
	}
			
	return NULL;
}
void	CINFCityShop::UpdateItemRemainCount( ItemNum_t nItemNum, INT nRemainCount )
{

	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[1].begin();
	while(it != m_vecItemInfo[1].end())
	{
		SHOP_ITEM* pitShopSkill = *it;
		if(nItemNum == pitShopSkill->ItemNum)
		{
			pitShopSkill->RemainCountForLimitedEdition	= nRemainCount;
		}
		it++;
	}

	


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-06-20 by bhsohn 상점에서 아이템 구매후 숫자 초기화 코드 추가
/// \date		2013-06-20 ~ 2013-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityShop::InitSelectItemNum()
{
#ifdef	C_SHOP_ITEMCNT_INIT_BHSOHN
#else
	return;
#endif
	if((m_nCurrentTab <0) || (m_nCurrentTab >= SHOP_ITEM_TAB_NUMBER))
	{
		return;
	}
	int nMaxSlot = m_vecItemInfo[m_nCurrentTab].size();
	if((m_nCurrentSelectSlot[m_nCurrentTab] <0) || (m_nCurrentSelectSlot[m_nCurrentTab] >= nMaxSlot  ))
	{
		return;
	}
	
	OnCancelButtonClicked();

}