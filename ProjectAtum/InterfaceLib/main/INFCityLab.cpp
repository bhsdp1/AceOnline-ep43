// INFCityLab.cpp: implementation of the CINFCityLab class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityLab.h"

#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "INFCityBase.h"
#include "AtumDatabase.h"
#include "INFGameMain.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFWindow.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "StoreData.h"
//#include "INFScrollBar.h"

//#include "INFTrade.h"
#include "StoreData.h"
//#include "EnemyData.h"
#include "INFItemInfo.h"
#include "INFIcon.h"
//#include "INFImage.h"
//#include "GameDataLast.h"
#include "FieldWinSocket.h"
//#include "AtumDatabase.h"
//#include "SceneData.h"
#include "ItemInfo.h"
//#include "GameDataLast.h"
#include "Chat.h"
#include "Interface.h"
#include "dxutil.h"
// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
// end 2011. 10. 10 by jskim UI시스템 변경

// 2013-03-18 by bhsohn 팩토리 조합 추가
#include "INFItemMixFactoryWnd.h"
// END 2013-03-18 by bhsohn 팩토리 조합 추가

#define RENEW_SHOP_SIZE_WIDTH	230

// 2006-08-17 by dgwoo 아이콘의 수량을 찍을때 사용하는 DEFINE
//--------------------------------------------------------------------------//
#define LAB_ICON_X				25



//--------------------------------------------------------------------------//
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define BACK_START_X			(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 249)
#define BACK_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)
#define BACK_BK_IMAGE_POS_Y		(BACK_START_Y + 20)
#define SOURCE_SLOT_START_X		(BACK_START_X + 53)
#define SOURCE_SLOT_START_Y		(BACK_START_Y + 60)

// 2015-05-29 by jwlee 연구소 UI 변경
// #define LABFACTORY_SOURCE_SLOT_TARGET_START_X	(BACK_START_X + 34)
// #define LABFACTORY_SOURCE_SLOT_TARGET_START_Y	(BACK_BK_IMAGE_POS_Y + 38)
// #define LABFACTORY_SOURCE_SLOT_ENCHANT_START_X	(BACK_START_X + 34)
// #define LABFACTORY_SOURCE_SLOT_ENCHANT_START_Y	(BACK_BK_IMAGE_POS_Y + 104)
// #define LABFACTORY_SOURCE_SLOT_ETC_START_X		(BACK_START_X + 14)
// #define LABFACTORY_SOURCE_SLOT_ETC_START_Y		(BACK_BK_IMAGE_POS_Y + 170)
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
#define LABORATORY_SOURCE_SLOT_STARGET_START_X	(BACK_START_X + 34)
#define LABORATORY_SOURCE_SLOT_STARGET_START_Y	(BACK_BK_IMAGE_POS_Y + 38)
#define LABORATORY_SOURCE_SLOT_ENCHANT_START_X	(BACK_START_X + 34)
#define LABORATORY_SOURCE_SLOT_ENCHANT_START_Y	(BACK_BK_IMAGE_POS_Y + 104)
#define LABORATORY_SOURCE_SLOT_ETC_START_X		(BACK_START_X + 14)
#define LABORATORY_SOURCE_SLOT_ETC_START_Y		(BACK_BK_IMAGE_POS_Y + 170)
#define LABFACTORY_TARGET_SLOT_START_X	(BACK_START_X + 222)
#define LABFACTORY_TARGET_SLOT_START_Y	(BACK_BK_IMAGE_POS_Y + 104)

#define LAB_MIXING_ICON0_FRAME_MAX_X	9
#define LAB_MIXING_ICON0_FRAME_MAX_Y	1
#define LAB_MIXING_ICON0_FRAME_TIME		(0.8f / (LAB_MIXING_ICON0_FRAME_MAX_X * LAB_MIXING_ICON0_FRAME_MAX_Y))

#define LAB_MIXING_ICON1_FRAME_MAX_X	18
#define LAB_MIXING_ICON1_FRAME_MAX_Y	1
#define LAB_MIXING_ICON1_FRAME_TIME		(0.8f / (LAB_MIXING_ICON1_FRAME_MAX_X * LAB_MIXING_ICON1_FRAME_MAX_Y))

#define LAB_MIXING_ICONSUC_FRAME_MAX_X	13
#define LAB_MIXING_ICONSUC_FRAME_MAX_Y	1
#define LAB_MIXING_ICONSUC_FRAME_TIME	(1.0f / (LAB_MIXING_ICONSUC_FRAME_MAX_X * LAB_MIXING_ICONSUC_FRAME_MAX_Y))
#define LAB_MIXING_ICONSUC_FRAME_COUNT	2

#define LAB_MIXING_LINE0_FRAME_MAX_X	9
#define LAB_MIXING_LINE0_FRAME_MAX_Y	1
#define LAB_MIXING_LINE0_FRAME_TIME		(0.6f / (LAB_MIXING_LINE0_FRAME_MAX_X * LAB_MIXING_LINE0_FRAME_MAX_Y))

#define LAB_MIXING_LINE1_FRAME_MAX_X	9
#define LAB_MIXING_LINE1_FRAME_MAX_Y	1
#define LAB_MIXING_LINE1_FRAME_TIME		(0.6f / (LAB_MIXING_LINE1_FRAME_MAX_X * LAB_MIXING_LINE1_FRAME_MAX_Y))

#define LAB_MIXING_LINE2_FRAME_MAX_X	9
#define LAB_MIXING_LINE2_FRAME_MAX_Y	1
#define LAB_MIXING_LINE2_FRAME_TIME		(0.6f / (LAB_MIXING_LINE2_FRAME_MAX_X * LAB_MIXING_LINE2_FRAME_MAX_Y))

#define LAB_MIXING_LINE3_FRAME_MAX_X	9
#define LAB_MIXING_LINE3_FRAME_MAX_Y	1
#define LAB_MIXING_LINE3_FRAME_TIME		(0.5f / (LAB_MIXING_LINE3_FRAME_MAX_X * LAB_MIXING_LINE3_FRAME_MAX_Y))

#define IS_ENCHANT_SOURCE_ETC(_IT_)	(ITEMKIND_GAMBLE == (_IT_) || ITEMKIND_PREVENTION_DELETE_ITEM == (_IT_))
#else
#define LABFACTORY_SOURCE_SLOT_TARGET_START_X	(BACK_START_X + 34)
#define LABFACTORY_SOURCE_SLOT_TARGET_START_Y	(BACK_BK_IMAGE_POS_Y + 38)
#define LABFACTORY_SOURCE_SLOT_ENCHANT_START_X	(BACK_START_X + 34)
#define LABFACTORY_SOURCE_SLOT_ENCHANT_START_Y	(BACK_BK_IMAGE_POS_Y + 104)
#define LABFACTORY_SOURCE_SLOT_ETC_START_X		(BACK_START_X + 14)
#define LABFACTORY_SOURCE_SLOT_ETC_START_Y		(BACK_BK_IMAGE_POS_Y + 170)
#endif
// end 2015-05-29 by jwlee 연구소 UI 변경

#define SLOT_INTERVAL_X			42
#define SLOT_INTERVAL_Y			37

// 2008-03-14 by bhsohn 조합식 개선안
#define TARGET_SLOT_START_X		(BACK_START_X + 53)
#define TARGET_SLOT_START_Y		(BACK_START_Y + 184)

#define TARGET_FACTORY_SLOT_START_X		(BACK_START_X + 53)
 
#define CASH_START_X			(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 251)
#define OK_BUTTON_START_X		(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 430)
#define OK_BUTTON_START_Y		(BACK_START_Y + 230)
#define CANCEL_BUTTON_START_X	(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 462)
#define CANCEL_BUTTON_START_Y	(BACK_START_Y + 230)
#define BUTTON_SIZE_X			30
#define BUTTON_SIZE_Y			30

#define SOURCE_NUMBER_X			4
#define SOURCE_NUMBER_Y			2

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define CASH_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y + 239)//241)
#define SPI_START_X				(CASH_START_X + 134)
#define SPI_START_Y				(CASH_START_Y - 5)
#else
#define CASH_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y +239)//241)
#define SPI_START_X				(CASH_START_X + 134)
#define SPI_START_Y				(CASH_START_Y - 5)
#endif

// 2008-03-14 by bhsohn 조합식 개선안
// 숫자 입력 컨트롤 (확인)
#define		MIX_NUM_EDIT_X		(BACK_START_X+134)
#define		MIX_NUM_EDIT_Y		(BACK_START_Y+197)
#define		MIX_NUM_EDIT_W		(70)
#define		MIX_NUM_EDIT_H		(20)
// 최대 글씨 수 
//#define		MIX_MAX_STRING_LEN					4

// 2010. 04. 21 by jskim 신규 럭키 머신 구현
#define		RARE_FIX_PREFIX							1
#define		RARE_FIX_SUFFIX							2

#define		RARE_FIX_ITEM							1
#define		ITIALIZE_ITEM							2
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
#else			 
#define BACK_START_X			(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 249)
#define BACK_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y + 33)
#define SOURCE_SLOT_START_X		(BACK_START_X + 30)
#define SOURCE_SLOT_START_Y		(BACK_START_Y + 34)

#define SLOT_INTERVAL_X			31
#define SLOT_INTERVAL_Y			32

// 2008-03-14 by bhsohn 조합식 개선안
#define TARGET_SLOT_START_X		(BACK_START_X + 30)
#define TARGET_SLOT_START_Y		(BACK_START_Y + 150)

#define TARGET_FACTORY_SLOT_START_X		(BACK_START_X + 123)
 
#define CASH_START_X			(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 251)
#define OK_BUTTON_START_X		(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 361)
#define OK_BUTTON_START_Y		(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y + 240)
#define CANCEL_BUTTON_START_X	(CITY_BASE_NPC_BOX_START_X+RENEW_SHOP_SIZE_WIDTH + 398)
#define CANCEL_BUTTON_START_Y	(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y + 240)
#define BUTTON_SIZE_X			35
#define BUTTON_SIZE_Y			16

#define SOURCE_NUMBER_X			4
#define SOURCE_NUMBER_Y			2

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define CASH_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y + 239)//241)
#define SPI_START_X				(CASH_START_X + 5)
#define SPI_START_Y				(CASH_START_Y + 1)
#else
#define CASH_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y +239)//241)
#define SPI_START_X				(CASH_START_X + 4)
#define SPI_START_Y				(CASH_START_Y + 1)
#endif

// 2008-03-14 by bhsohn 조합식 개선안
// 숫자 입력 컨트롤 (확인)
#define		MIX_NUM_EDIT_X		(BACK_START_X+31)
#define		MIX_NUM_EDIT_Y		(BACK_START_Y+162)
#define		MIX_NUM_EDIT_W		(70)
#define		MIX_NUM_EDIT_H		(20)
// 최대 글씨 수 
//#define		MIX_MAX_STRING_LEN					4

// 2010. 04. 21 by jskim 신규 럭키 머신 구현
#define		RARE_FIX_PREFIX							1
#define		RARE_FIX_SUFFIX							2

#define		RARE_FIX_ITEM							1
#define		ITIALIZE_ITEM							2
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINFCityLab::CINFCityLab(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent						= pParent;
	m_pBuildingInfo					= pBuilding;
	m_bRestored						= FALSE;
	m_pImgBack						= NULL;
	// 2008-03-14 by bhsohn 조합식 개선안
	m_pImgBackFactory				= NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBackLab					= NULL;
#endif
	m_pImgTitle						= NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgPrice						= NULL;
#endif
	m_pFontPrice					= NULL;

	m_nButtonState[0]				= BUTTON_STATE_NORMAL;
	m_nButtonState[1]				= BUTTON_STATE_NORMAL;
	m_pSelectItem					= NULL;
	m_bShowTarget					= FALSE;

	memset( m_pImgButton, 0x00, sizeof(DWORD)*LAB_BUTTON_NUMBER*4);
	memset( m_szPrice, 0x00, 64);

	m_bSelectDown = FALSE;

	// 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_INGAME_MIX_ITEM
	m_pNumEditBox= NULL; // 2013-03-18 by bhsohn 팩토리 조합 추가
#endif

	// 2010. 06. 18 by jskim 인챈트 경고 메시지 추가
	m_bIsEnchantCheck = FALSE;
	//end 2010. 06. 18 by jskim 인챈트 경고 메시지 추가

	// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가
	m_bIsAppearanceChangeCheck = FALSE;
	// end 2012-10-30 by mspark, 외형 변경 경고 메시지 추가

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	m_pINFItemMixFactoryWnd = NULL;
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	m_nAniState = LABORATORY_ANIMATION_STATE_NORMAL;
	m_nSucAniLoopCnt = 0;
	for (int i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		m_pImgLabIcon[i] = NULL;
		m_pImgLabLine[i] = NULL;
		m_bAniView[i] = FALSE;
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
}

CINFCityLab::~CINFCityLab()
{
	SAFE_DELETE(m_pImgBack);
	// 2008-03-14 by bhsohn 조합식 개선안
	SAFE_DELETE(m_pImgBackFactory);	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	SAFE_DELETE(m_pImgBackLab);	
#endif
	SAFE_DELETE(m_pImgTitle);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	SAFE_DELETE(m_pImgPrice);
#endif
	SAFE_DELETE(m_pFontPrice);

	// 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_INGAME_MIX_ITEM
	SAFE_DELETE(m_pNumEditBox);	// 2013-03-18 by bhsohn 팩토리 조합 추가
#endif
	
	int i; for(i=0;i<4;i++)
	{
		SAFE_DELETE(m_pImgButton[0][i]);
		SAFE_DELETE(m_pImgButton[1][i]);		
		SAFE_DELETE(m_pImgButton[2][i]);		
	}
	// 2013-03-18 by bhsohn 팩토리 조합 추가
	SAFE_DELETE(m_pINFItemMixFactoryWnd);
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
	InitData();
}

HRESULT CINFCityLab::InitDeviceObjects()
{
	FLOG( "CINFCityLab::InitDeviceObjects()" );
	DataHeader	* pDataHeader;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "lab_sh");
	m_pImgBack = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pImgBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pImgBack->RestoreDeviceObjects();
#else 
	m_pImgBack = new CINFImageEx;
	pDataHeader = FindResource("shlabbk");
	m_pImgBack->InitDeviceObjects(pDataHeader) ;
#endif
	// 2008-03-14 by bhsohn 조합식 개선안
	m_pImgBackFactory = new CINFImageEx;
	pDataHeader = FindResource("shlabbk1");
	m_pImgBackFactory->InitDeviceObjects(pDataHeader ) ;
	  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBackLab = new CINFImageEx;
	pDataHeader = FindResource("shlabbk");
	m_pImgBackLab->InitDeviceObjects(pDataHeader ) ;
#endif																								  

	m_pImgTitle = new CINFImageEx;
	pDataHeader = FindResource("lab_ti");
	m_pImgTitle->InitDeviceObjects(pDataHeader ) ;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
 	m_pImgPrice = new CINFImageEx;
	pDataHeader = FindResource("shlacost");
 	m_pImgPrice->InitDeviceObjects(pDataHeader ) ;
#endif

	int i; for(i=0;i<4;i++)
	{
		char buf[16];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgButton[0][i] = new CINFImageEx;
		wsprintf(buf, "oks0%d",i);
		pDataHeader = FindResource(buf);
		m_pImgButton[0][i]->InitDeviceObjects(pDataHeader ) ;

		m_pImgButton[1][i] = new CINFImageEx;		
		wsprintf(buf, "cans0%d",i);
		pDataHeader = FindResource(buf);
		m_pImgButton[1][i]->InitDeviceObjects(pDataHeader ) ;

		m_pImgButton[2][i] = new CINFImageEx;		
		wsprintf(buf, "oks0%d",i);
		pDataHeader = FindResource(buf);
		m_pImgButton[2][i]->InitDeviceObjects(pDataHeader ) ;	
#else 
	  	m_pImgButton[0][i] = new CINFImageEx;
		wsprintf(buf, "shlama0%d",i);
		pDataHeader = FindResource(buf);
		m_pImgButton[0][i]->InitDeviceObjects(pDataHeader ) ;

		m_pImgButton[1][i] = new CINFImageEx;		
		wsprintf(buf, "shmcan0%d",i);
		pDataHeader = FindResource(buf);
		m_pImgButton[1][i]->InitDeviceObjects(pDataHeader) ;

		m_pImgButton[2][i] = new CINFImageEx;		
		wsprintf(buf, "shlaok0%d",i);
		pDataHeader = FindResource(buf);
		m_pImgButton[2][i]->InitDeviceObjects(pDataHeader) ;
#endif
	}

	m_pFontPrice = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontPrice->InitDeviceObjects(g_pD3dDev);

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	// 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_INGAME_MIX_ITEM
	{
		if(NULL == m_pNumEditBox)
		{
			m_pNumEditBox = new CINFNumEditBox;
		}
		char chBuff[32];
		char chMaxMixCnt[64];

		wsprintf(chBuff, "1");
		wsprintf(chMaxMixCnt, "%d", COUNT_MAX_MIXING_COUNT);

		POINT ptPos = {MIX_NUM_EDIT_X, MIX_NUM_EDIT_Y};
		m_pNumEditBox->InitDeviceObjects(9, ptPos, MIX_NUM_EDIT_W, TRUE, MIX_NUM_EDIT_H);				
		m_pNumEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pNumEditBox->SetString(chBuff, 32);
	}	
#endif
	// end 2008-03-14 by bhsohn 조합식 개선안

	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM
	if(NULL == m_pINFItemMixFactoryWnd)
	{		
		m_pINFItemMixFactoryWnd = new CINFItemMixFactoryWnd(this);
		m_pINFItemMixFactoryWnd->SetGameData(m_pGameData);		
		m_pINFItemMixFactoryWnd->InitDeviceObjects();
	}
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	ChangeAniState(LABORATORY_ANIMATION_STATE_NORMAL);
	for (i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabIcon[i] == NULL)
		{
			char buf[16];
			switch (i)
			{
			case LABORATORY_GROUP_STARGET:
			case LABORATORY_GROUP_ENCHANT:
				wsprintf(buf,"lab_icon%d",0);
				break;
			case LABORATORY_GROUP_ETC:
				wsprintf(buf,"lab_icon%d",1);
				break;
			case LABORATORY_GROUP_TTARGET:
				wsprintf(buf,"lab_icon%d",2);
				break;
			}
			pDataHeader = FindResource(buf);
			if(pDataHeader)
			{
				m_pImgLabIcon[i] = new CINFImageEx;
				m_pImgLabIcon[i]->InitDeviceObjects(pDataHeader);
			}
		}
	}
	for (i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabLine[i] == NULL)
		{
			char buf[16];
			wsprintf(buf,"lab_line%d",i);
			pDataHeader = FindResource(buf);
			if(pDataHeader)
			{
				m_pImgLabLine[i] = new CINFImageEx;
				m_pImgLabLine[i]->InitDeviceObjects(pDataHeader);
			}
		}
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경

	return S_OK;
}


HRESULT CINFCityLab::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		int i; for(i=0;i<4;i++)
		{
			m_pImgButton[0][i]->RestoreDeviceObjects();
			m_pImgButton[0][i]->Move(OK_BUTTON_START_X, OK_BUTTON_START_Y);
			m_pImgButton[1][i]->RestoreDeviceObjects();
			m_pImgButton[1][i]->Move(CANCEL_BUTTON_START_X, CANCEL_BUTTON_START_Y);
			m_pImgButton[2][i]->RestoreDeviceObjects();
			m_pImgButton[2][i]->Move(OK_BUTTON_START_X, OK_BUTTON_START_Y);
		}

		m_pImgBack->RestoreDeviceObjects();
		m_pImgBack->Move(BACK_START_X, BACK_START_Y);
		// 2008-03-14 by bhsohn 조합식 개선안

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgBackFactory->RestoreDeviceObjects();
		m_pImgBackFactory->Move(BACK_START_X, BACK_BK_IMAGE_POS_Y);
		// end 2008-03-14 by bhsohn 조합식 개선안
		m_pImgBackLab->RestoreDeviceObjects();
		m_pImgBackLab->Move(BACK_START_X,BACK_BK_IMAGE_POS_Y);																								  
#else				
		m_pImgBackFactory->RestoreDeviceObjects();
		m_pImgBackFactory->Move(BACK_START_X, BACK_START_Y);		
		// end 2008-03-14 by bhsohn 조합식 개선안
#endif
		m_pImgTitle->RestoreDeviceObjects();
		m_pImgTitle->Move(CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgPrice->RestoreDeviceObjects();
		m_pImgPrice->Move(CASH_START_X, CASH_START_Y);
#endif
		m_pFontPrice->RestoreDeviceObjects();

		// 2008-03-14 by bhsohn 조합식 개선안		
#ifndef C_INGAME_MIX_ITEM
		m_pNumEditBox->RestoreDeviceObjects();		// 2013-03-18 by bhsohn 팩토리 조합 추가
#endif
		// 2008-03-14 by bhsohn 조합식 개선안

		// 2013-03-18 by bhsohn 팩토리 조합 추가
		if(m_pINFItemMixFactoryWnd)
		{		
			m_pINFItemMixFactoryWnd->RestoreDeviceObjects();
		}
		// END 2013-03-18 by bhsohn 팩토리 조합 추가
		
		m_bRestored = TRUE;
	}
	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	int i;
	for ( i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabIcon[i])
		{
			m_pImgLabIcon[i]->RestoreDeviceObjects();
		}
	}

	for ( i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabLine[i])
		{
			m_pImgLabLine[i]->RestoreDeviceObjects();
			switch (i)
			{
			case LABORATORY_GROUP_STARGET:
				m_pImgLabLine[i]->Move(LABORATORY_SOURCE_SLOT_STARGET_START_X + 35,LABORATORY_SOURCE_SLOT_STARGET_START_Y - 2);
				break;
			case LABORATORY_GROUP_ENCHANT:
				m_pImgLabLine[i]->Move(LABORATORY_SOURCE_SLOT_ENCHANT_START_X + 35,LABORATORY_SOURCE_SLOT_ENCHANT_START_Y - 1);
				break;
			case LABORATORY_GROUP_ETC:
				m_pImgLabLine[i]->Move(LABORATORY_SOURCE_SLOT_ETC_START_X + 73,LABORATORY_SOURCE_SLOT_ETC_START_Y + 7);
				break;
			}
		}
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
	return S_OK;
}

HRESULT CINFCityLab::DeleteDeviceObjects()
{
	int i; for(i=0;i<4;i++)
	{
		m_pImgButton[0][i]->DeleteDeviceObjects();
		m_pImgButton[1][i]->DeleteDeviceObjects();
		m_pImgButton[2][i]->DeleteDeviceObjects();
		SAFE_DELETE(m_pImgButton[0][i]);
		SAFE_DELETE(m_pImgButton[1][i]);
		SAFE_DELETE(m_pImgButton[2][i]);
	}
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
	m_pImgBack->DeleteDeviceObjects();
	m_pImgBackFactory->DeleteDeviceObjects();
	m_pImgBackLab->DeleteDeviceObjects();
	m_pImgTitle->DeleteDeviceObjects();
	m_pFontPrice->DeleteDeviceObjects();
	SAFE_DELETE(m_pImgBack);
	SAFE_DELETE(m_pImgBackFactory);
	SAFE_DELETE(m_pImgBackLab);
	SAFE_DELETE(m_pImgTitle);
	SAFE_DELETE(m_pFontPrice);
#else
	m_pImgBack->DeleteDeviceObjects();
	// 2008-03-14 by bhsohn 조합식 개선안
	m_pImgBackFactory->DeleteDeviceObjects();

	m_pImgTitle->DeleteDeviceObjects();
	m_pImgPrice->DeleteDeviceObjects();
	m_pFontPrice->DeleteDeviceObjects();
	SAFE_DELETE(m_pImgBack);
	// 2008-03-14 by bhsohn 조합식 개선안
	SAFE_DELETE(m_pImgBackFactory);
	
	SAFE_DELETE(m_pImgTitle);
	SAFE_DELETE(m_pImgPrice);
	SAFE_DELETE(m_pFontPrice);
#endif


	// 2013-03-18 by bhsohn 팩토리 조합 추가
	// 2008-03-14 by bhsohn 조합식 개선안	
#ifndef C_INGAME_MIX_ITEM
	if(m_pNumEditBox)
	{
		m_pNumEditBox->DeleteDeviceObjects();	
		SAFE_DELETE(m_pNumEditBox);
	}		
#endif
	// end 2008-03-14 by bhsohn 조합식 개선안

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	if(m_pINFItemMixFactoryWnd)
	{		
		m_pINFItemMixFactoryWnd->DeleteDeviceObjects();	
		SAFE_DELETE(m_pINFItemMixFactoryWnd);
	}
		// END 2013-03-18 by bhsohn 팩토리 조합 추가

	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	for (i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabIcon[i])
		{
			m_pImgLabIcon[i]->DeleteDeviceObjects();
			SAFE_DELETE(m_pImgLabIcon[i]);
		}
	}

	for (i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabLine[i])
		{
			m_pImgLabLine[i]->DeleteDeviceObjects();
			SAFE_DELETE(m_pImgLabLine[i]);
		}
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경

	return S_OK;
}

HRESULT CINFCityLab::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		int i; for(i=0;i<4;i++)
		{
			m_pImgButton[0][i]->InvalidateDeviceObjects();
			m_pImgButton[1][i]->InvalidateDeviceObjects();
			m_pImgButton[2][i]->InvalidateDeviceObjects();
		}

		m_pImgBack->InvalidateDeviceObjects();
		// 2008-03-14 by bhsohn 조합식 개선안
		m_pImgBackFactory->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgBackLab->InvalidateDeviceObjects();
		m_pImgTitle->InvalidateDeviceObjects();
#else					 
		m_pImgTitle->InvalidateDeviceObjects();
		m_pImgPrice->InvalidateDeviceObjects();
#endif
		m_pFontPrice->InvalidateDeviceObjects();
		// 2008-03-14 by bhsohn 조합식 개선안	
#ifndef C_INGAME_MIX_ITEM
		m_pNumEditBox->InvalidateDeviceObjects(); // 2013-03-18 by bhsohn 팩토리 조합 추가
#endif

		// 2013-03-18 by bhsohn 팩토리 조합 추가
		if(m_pINFItemMixFactoryWnd)
		{
			m_pINFItemMixFactoryWnd->InvalidateDeviceObjects();
		}
		// END 2013-03-18 by bhsohn 팩토리 조합 추가

		m_bRestored = FALSE;
	}
	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	int i;
	for ( i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabIcon[i])
		{
			m_pImgLabIcon[i]->InvalidateDeviceObjects();
		}
	}

	for (i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_pImgLabLine[i])
		{
			m_pImgLabLine[i]->InvalidateDeviceObjects();
		}
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
	return S_OK;
}

void CINFCityLab::Render()
{
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->Render();
		return;
	}
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

//	m_pImgTitle->Render();
	// 2008-03-14 by bhsohn 조합식 개선안
	//m_pImgBack->Render();	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBack->Render();
#endif
	if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
	{
		m_pImgBackFactory->Render();				
	}
	else
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgBackLab->Render();   
#else
		m_pImgBack->Render();				
#endif
	}	
	// end 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgPrice->Render();
#endif

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	// 2008-03-14 by bhsohn 조합식 개선안	
#ifndef C_INGAME_MIX_ITEM
	if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
	{
		m_pNumEditBox->Render();
	}
#endif
	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	int i;
	for (i = 0 ; i < LABORATORY_GROUP_END ; i++)
	{
		if (m_bAniView[i] && m_pImgLabLine[i] &&
			(m_nAniState == LABORATORY_ANIMATION_STATE_LINE_ING ||
			m_nAniState == LABORATORY_ANIMATION_STATE_RESULT_ING))
		{
			m_pImgLabLine[i]->Render();
			// 1~2프레임 전의 이미지를 투명값을 주어 출력하므로써 잔상효과처럼 보이도록 함
			DWORD dwNowLineFrame = m_pImgLabLine[i]->GetFrame();
			if(dwNowLineFrame >= 1)
			{
				if (dwNowLineFrame >= 2)
				{
					m_pImgLabLine[i]->SetUVCurrentRect(dwNowLineFrame - 2);
					m_pImgLabLine[i]->SetColor(0x10ffffff); // alpha = 6%
					m_pImgLabLine[i]->Render();
				}
				m_pImgLabLine[i]->SetUVCurrentRect(dwNowLineFrame - 1);
				m_pImgLabLine[i]->SetColor(0x50ffffff); // alpha = 32%
				m_pImgLabLine[i]->Render();
				m_pImgLabLine[i]->SetUVCurrentRect(dwNowLineFrame);
				m_pImgLabLine[i]->SetColor(0xffffffff); // alpha = 100%
			}
		}
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경

	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	vector<CItemInfo*>::iterator it = m_vecSource.begin();
	// 2015-05-29 by jwlee 연구소 UI 변경
//	int i = 0;
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	i = 0;
	int nRenderCountLabFacetoryETC = 0;
#else
	int i = 0;
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
	while(it != m_vecSource.end())
	{
		CItemInfo* pItem = *it;
		char buf[20];
//		wsprintf(buf, "%08d",pItem->ItemNum);					// 2005-08-23 by ispark


		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		//wsprintf(buf, "%08d",pItem->ItemInfo->SourceIndex);
		if( !pItem->ShapeItemNum )
			wsprintf(buf, "%08d", pItem->ItemInfo->SourceIndex);
		else
		{
			ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItem->ShapeItemNum );
			if( pShapeItem )
				wsprintf(buf, "%08d", pShapeItem->SourceIndex );
			else
				wsprintf(buf, "%08d", pItem->ItemInfo->SourceIndex);
		}

		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현




		// 2015-05-29 by jwlee 연구소 UI 변경
// 		pIcon->SetIcon( buf, SOURCE_SLOT_START_X+i%SOURCE_NUMBER_X*SLOT_INTERVAL_X+1, 
// 			SOURCE_SLOT_START_Y+i/SOURCE_NUMBER_X*SLOT_INTERVAL_Y+1, 1.0f );
// 		pIcon->Render();
// 		// 2006-08-16 by dgwoo 팩토리의 아이템 갯수 출력.
// 		if(IS_COUNTABLE_ITEM(pItem->Kind))
// 		{
// 			wsprintf( buf, "%d", pItem->CurrentCount);
// 			SIZE size = m_pFontPrice->GetStringSize(buf);
// 			m_pFontPrice->DrawText(SOURCE_SLOT_START_X+i%SOURCE_NUMBER_X*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 
// 				SOURCE_SLOT_START_Y+i/SOURCE_NUMBER_X*SLOT_INTERVAL_Y+1,GUI_FONT_COLOR_W,buf,0L);
// 		}
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
		if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
		{
		
			if (IS_ENCHANT_TARGET_ITEMKIND(pItem->Kind))
			{
				pIcon->SetIcon( buf, LABORATORY_SOURCE_SLOT_STARGET_START_X,
				 				LABORATORY_SOURCE_SLOT_STARGET_START_Y , 1.0f );
				pIcon->Render();
				if(m_nAniState == LABORATORY_ANIMATION_STATE_ICON_ING) m_pImgLabIcon[LABORATORY_GROUP_STARGET]->Render();
			}
			else if (ITEMKIND_ENCHANT == pItem->Kind)
			{
				pIcon->SetIcon( buf, LABORATORY_SOURCE_SLOT_ENCHANT_START_X, 
								LABORATORY_SOURCE_SLOT_ENCHANT_START_Y, 1.0f );
				pIcon->Render();
				if(m_nAniState == LABORATORY_ANIMATION_STATE_ICON_ING) m_pImgLabIcon[LABORATORY_GROUP_ENCHANT]->Render();
			}
			else if (IS_ENCHANT_SOURCE_ETC(pItem->Kind))
			{
				pIcon->SetIcon( buf, LABORATORY_SOURCE_SLOT_ETC_START_X + nRenderCountLabFacetoryETC * SLOT_INTERVAL_X, 
								LABORATORY_SOURCE_SLOT_ETC_START_Y, 1.0f );
				pIcon->Render();
				if(m_nAniState == LABORATORY_ANIMATION_STATE_ICON_ING) m_pImgLabIcon[LABORATORY_GROUP_ETC]->Render();
				nRenderCountLabFacetoryETC++;
			}
		}
		else
		{
		pIcon->SetIcon( buf, SOURCE_SLOT_START_X+i%SOURCE_NUMBER_X*SLOT_INTERVAL_X+1, 
			SOURCE_SLOT_START_Y+i/SOURCE_NUMBER_X*SLOT_INTERVAL_Y+1, 1.0f );
		pIcon->Render();

		if(IS_COUNTABLE_ITEM(pItem->Kind))
		{
			wsprintf( buf, "%d", pItem->CurrentCount);
			SIZE size = m_pFontPrice->GetStringSize(buf);
			m_pFontPrice->DrawText(SOURCE_SLOT_START_X+i%SOURCE_NUMBER_X*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 
				SOURCE_SLOT_START_Y+i/SOURCE_NUMBER_X*SLOT_INTERVAL_Y+1,GUI_FONT_COLOR_W,buf,0L);
		}
		}
#else
		pIcon->SetIcon( buf, SOURCE_SLOT_START_X+i%SOURCE_NUMBER_X*SLOT_INTERVAL_X+1, 
			SOURCE_SLOT_START_Y+i/SOURCE_NUMBER_X*SLOT_INTERVAL_Y+1, 1.0f );
		pIcon->Render();
		// 2006-08-16 by dgwoo 팩토리의 아이템 갯수 출력.
		if(IS_COUNTABLE_ITEM(pItem->Kind))
		{
			wsprintf( buf, "%d", pItem->CurrentCount);
			SIZE size = m_pFontPrice->GetStringSize(buf);
			m_pFontPrice->DrawText(SOURCE_SLOT_START_X+i%SOURCE_NUMBER_X*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 
				SOURCE_SLOT_START_Y+i/SOURCE_NUMBER_X*SLOT_INTERVAL_Y+1,GUI_FONT_COLOR_W,buf,0L);
 		}
#endif
		// end 2015-05-29 by jwlee 연구소 UI 변경
		
		it++;
		i++;
	}
	if( m_bShowTarget )
	{
		it = m_vecTarget.begin();		
		i = 0;
		
		// 2008-03-14 by bhsohn 조합식 개선안
		int nSlotStartX = TARGET_SLOT_START_X;
		if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
		{
			nSlotStartX = TARGET_FACTORY_SLOT_START_X;
		}
		
		// 2015-05-29 by jwlee 연구소 UI 변경
		// if(it != m_vecTarget.end())
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
		while(it != m_vecTarget.end())
#else
		if(it != m_vecTarget.end())
#endif
		// end 2015-05-29 by jwlee 연구소 UI 변경
		{
			CItemInfo* pItem = *it;
			char buf[20];
//			wsprintf(buf, "%08d",pItem->ItemNum);				// 2005-08-23 by ispark


			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

			//wsprintf(buf, "%08d",pItem->ItemInfo->SourceIndex);
			if( !pItem->ShapeItemNum )
				wsprintf( buf, "%08d",pItem->ItemInfo->SourceIndex );
			else
			{
				ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItem->ShapeItemNum );
				if( pShapeItem )
					wsprintf( buf, "%08d", pShapeItem->SourceIndex );
				else
					wsprintf( buf, "%08d",pItem->ItemInfo->SourceIndex );
			}

			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


			// 2015-05-29 by jwlee 연구소 UI 변경
// 			// 2008-03-14 by bhsohn 조합식 개선안
// 			//pIcon->SetIcon( buf, TARGET_SLOT_START_X+i*SLOT_INTERVAL_X+1, TARGET_SLOT_START_Y+1, 1.0f );
// 			pIcon->SetIcon( buf, nSlotStartX+i*SLOT_INTERVAL_X+1, TARGET_SLOT_START_Y+1, 1.0f );
// 			
// 			pIcon->Render();
// 
// 			// 2008-03-14 by bhsohn 조합식 개선안
// 			if(IS_COUNTABLE_ITEM(pItem->Kind))
// 			{
// 				// 결과에 대한 카운터블 아이템 수량 표시 
// 				wsprintf( buf, "%d", pItem->CurrentCount);
// 				SIZE size = m_pFontPrice->GetStringSize(buf);
// 				
// 				// 2008-03-14 by bhsohn 조합식 개선안
// 				//m_pFontPrice->DrawText(TARGET_SLOT_START_X+i*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 
// 				m_pFontPrice->DrawText(nSlotStartX+i*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 				
// 					TARGET_SLOT_START_Y+1,GUI_FONT_COLOR_W,buf,0L);
// 			}
// 			// end 2008-03-14 by bhsohn 조합식 개선안
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
			if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
			{
				if(IS_ENCHANT_TARGET_ITEMKIND(pItem->Kind))
			{
				pIcon->SetIcon( buf, LABFACTORY_TARGET_SLOT_START_X, LABFACTORY_TARGET_SLOT_START_Y, 1.0f );
				pIcon->Render();
					// 2015-07-28 by jwlee 연구소 UI 변경 2차
					if(m_nAniState == LABORATORY_ANIMATION_STATE_RESULT_SUCCESS) 
					// end 2015-07-28 by jwlee 연구소 UI 변경 2차
					{
						m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->Render();
					}
				}
			}
			else
			{
			pIcon->SetIcon( buf, nSlotStartX+i*SLOT_INTERVAL_X+1, TARGET_SLOT_START_Y+1, 1.0f );
			pIcon->Render();
				if(IS_COUNTABLE_ITEM(pItem->Kind))
				{
					// 결과에 대한 카운터블 아이템 수량 표시 
					wsprintf( buf, "%d", pItem->CurrentCount);
					SIZE size = m_pFontPrice->GetStringSize(buf);

					//m_pFontPrice->DrawText(TARGET_SLOT_START_X+i*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 
					m_pFontPrice->DrawText(nSlotStartX+i*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 				
						TARGET_SLOT_START_Y+1,GUI_FONT_COLOR_W,buf,0L);
				}
				break;
			}
#else
			// 2008-03-14 by bhsohn 조합식 개선안
			//pIcon->SetIcon( buf, TARGET_SLOT_START_X+i*SLOT_INTERVAL_X+1, TARGET_SLOT_START_Y+1, 1.0f );
			pIcon->SetIcon( buf, nSlotStartX+i*SLOT_INTERVAL_X+1, TARGET_SLOT_START_Y+1, 1.0f );
			
			pIcon->Render();

			// 2008-03-14 by bhsohn 조합식 개선안
			if(IS_COUNTABLE_ITEM(pItem->Kind))
			{
				// 결과에 대한 카운터블 아이템 수량 표시 
				wsprintf( buf, "%d", pItem->CurrentCount);
				SIZE size = m_pFontPrice->GetStringSize(buf);
				
				// 2008-03-14 by bhsohn 조합식 개선안
				//m_pFontPrice->DrawText(TARGET_SLOT_START_X+i*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 
				m_pFontPrice->DrawText(nSlotStartX+i*SLOT_INTERVAL_X+1+LAB_ICON_X-size.cx, 				
					TARGET_SLOT_START_Y+1,GUI_FONT_COLOR_W,buf,0L);
			}
 			// end 2008-03-14 by bhsohn 조합식 개선안
#endif
			// end 2015-05-29 by jwlee 연구소 UI 변경

			// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
			it++;
#endif
			// end 2015-05-29 by jwlee 연구소 UI 변경
			i++;

		}
		// OK
		m_pImgButton[2][m_nButtonState[0]]->Render();
	}
	else
	{
		// SEND
		m_pImgButton[0][m_nButtonState[0]]->Render();
	}
	// CANCEL
	m_pImgButton[1][m_nButtonState[1]]->Render();
	if(m_szPrice[0] != NULL)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		SIZE sSize = m_pFontPrice->GetStringSize(m_szPrice);
		m_pFontPrice->DrawText(SPI_START_X - sSize.cx, SPI_START_Y, GUI_FONT_COLOR, m_szPrice, 0L );
#else				 
		m_pFontPrice->DrawText(SPI_START_X, SPI_START_Y, GUI_FONT_COLOR, m_szPrice, 0L );
#endif
	}
}

void CINFCityLab::Tick()
{
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->Tick();
		return;
	}
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	int i = 0;
	int nAniCount = 0;
	int nComAniCount = 0;
	switch (m_nAniState)
	{
	case LABORATORY_ANIMATION_STATE_ICON_ING:
		nComAniCount = 0;
		nAniCount = 0;
		for (i = 0 ; i < LABORATORY_GROUP_END - 1 ; i++)
		{
			if(m_bAniView[i])
			{
				nAniCount++;
				
				m_pImgLabIcon[i]->Tick();
				if (m_pImgLabIcon[i]->GetFrame() >= m_pImgLabIcon[i]->GetFrameTotal()) // 2015-06-15 by jwlee GetFrameTotal() 함수 추가
				{
					nComAniCount++;
				}
			}
		}
		if (nAniCount == nComAniCount)
		{
			ChangeAniState(LABORATORY_ANIMATION_STATE_LINE_START);
		}
		break;
	case LABORATORY_ANIMATION_STATE_LINE_ING:
		nComAniCount = 0;
		nAniCount = 0;
		for (i = 0 ; i < LABORATORY_GROUP_END - 1 ; i++)
		{
			if(m_bAniView[i])
			{
				nAniCount++;
				if (m_pImgLabLine[i]->GetFrame() < m_pImgLabLine[i]->GetFrameTotal()) // 2015-06-15 by jwlee GetFrameTotal() 함수 추가
				{
					m_pImgLabLine[i]->Tick();
				}
				else
				{
					nComAniCount++;
				}
			}
		}
		if (nAniCount == nComAniCount)
		{
			ChangeAniState(LABORATORY_ANIMATION_STATE_RESULT_START);
		}
		break;
	case LABORATORY_ANIMATION_STATE_RESULT_ING:
		if (m_bAniView[LABORATORY_GROUP_TTARGET])
		{
			if(m_pImgLabLine[LABORATORY_GROUP_TTARGET]->GetFrame() < m_pImgLabLine[LABORATORY_GROUP_TTARGET]->GetFrameTotal()) // 2015-06-15 by jwlee GetFrameTotal() 함수 추가
			{
				m_pImgLabLine[LABORATORY_GROUP_TTARGET]->Tick();
			}
			else
			{
				m_vecTarget = m_vecSource;
				// 2015-07-28 by jwlee 연구소 UI 변경 2차
				m_vecSource.clear();

				g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENCHANT, (char*)&m_UseEchantMsg, sizeof(m_UseEchantMsg));
				g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark
				memset(&m_UseEchantMsg,0x00,sizeof(m_UseEchantMsg));


				ChangeAniState(LABORATORY_ANIMATION_STATE_RESULT_RECIVE);	
				m_nSucAniLoopCnt = 0;
				// end 2015-07-28 by jwlee 연구소 UI 변경 2차
			}
		}
		break;
	case LABORATORY_ANIMATION_STATE_RESULT_SUCCESS:
		if(m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->GetFrame() >= m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->GetFrameTotal()) // 2015-06-15 by jwlee GetFrameTotal() 함수 추가
		{
			m_nSucAniLoopCnt++;
			m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->SetFrame(0);
			m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->SetUVCurrentRect(0);
		}
		if (m_nSucAniLoopCnt >= LAB_MIXING_ICONSUC_FRAME_COUNT)
		{
			ChangeAniState(LABORATORY_ANIMATION_STATE_RESULT_SUCCESS_END);
		}
		else m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->Tick();
// 		if(m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->GetFrame() < m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->GetFrameTotal()) // 2015-06-15 by jwlee GetFrameTotal() 함수 추가
// 		{
// 			m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->Tick();
// 		}
// 		else ChangeAniState(LABORATORY_ANIMATION_STATE_RESULT_SUCCESS_END);
		break;
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
}

int CINFCityLab::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM	
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		return m_pINFItemMixFactoryWnd->WndProc(uMsg, wParam, lParam);
	}					
#endif	
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{		
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
			
			// 2015-05-29 by jwlee 연구소 UI 변경
// 			if( pt.x > SOURCE_SLOT_START_X &&
// 				pt.x < SOURCE_SLOT_START_X + SLOT_INTERVAL_X*4)
// 			{
// 				if(	pt.y > SOURCE_SLOT_START_Y && 
// 					pt.y < SOURCE_SLOT_START_Y+SLOT_INTERVAL_Y*2)
// 				{
// 					int x = (pt.x - SOURCE_SLOT_START_X) / SLOT_INTERVAL_X;
// 					int y = (pt.y - SOURCE_SLOT_START_Y) / SLOT_INTERVAL_Y;
// 					int index = x + y * SOURCE_NUMBER_X;
// 					if( index >= 0 && m_vecSource.size() > index )
// 					{
// 						CItemInfo* pItemInfo = m_vecSource[index];
// 						if(pItemInfo)
// 						{
// 							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
// 							//g_pGameMain->m_pItemInfo->SetItemInfoUser( pItemInfo, pt.x, pt.y );
// 							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
// 							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
// 						}
// 					}
// 				}
// 				else if( m_bShowTarget &&
// 					pt.y > TARGET_SLOT_START_Y && 
// 					pt.y < TARGET_SLOT_START_Y+SLOT_INTERVAL_Y)
// 				{
// 					// 2008-03-14 by bhsohn 조합식 개선안
// 					//int index = (pt.x - TARGET_SLOT_START_X) / SLOT_INTERVAL_X;
// 					int nSlotStartX = TARGET_SLOT_START_X;
// 					if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
// 					{
// 						nSlotStartX = TARGET_FACTORY_SLOT_START_X;
// 					}				
// 					int index = (pt.x - nSlotStartX) / SLOT_INTERVAL_X;
// 					// end 2008-03-14 by bhsohn 조합식 개선안
// 					
// 					if( index >= 0 && m_vecTarget.size() > index )
// 					{
// 						CItemInfo* pItemInfo = m_vecTarget[index];
// 						if(pItemInfo)
// 						{
// 							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
// 							//g_pGameMain->m_pItemInfo->SetItemInfoUser( pItemInfo, pt.x, pt.y );
// 							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
// 							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
// 						}
// 					}
// 				}
// 				else
// 				{
// 					g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
// 				}
// 			}
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
			if (m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
			{
				if (ISStateNormal())
				{
					if (pt.x >= LABORATORY_SOURCE_SLOT_STARGET_START_X &&
						pt.x <= LABORATORY_SOURCE_SLOT_STARGET_START_X + SLOT_INTERVAL_X &&
						pt.y >= LABORATORY_SOURCE_SLOT_STARGET_START_Y &&
						pt.y <= LABORATORY_SOURCE_SLOT_STARGET_START_Y + SLOT_INTERVAL_Y)
					{
						CItemInfo* pItemInfo = FindItemLabFactoryFromSource(LABORATORY_ITEM_STARGET);
					if(pItemInfo)
					{
						g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
					}
				}
					else if (pt.x >= LABORATORY_SOURCE_SLOT_ENCHANT_START_X &&
						pt.x <= LABORATORY_SOURCE_SLOT_ENCHANT_START_X + SLOT_INTERVAL_X &&
						pt.y >= LABORATORY_SOURCE_SLOT_ENCHANT_START_Y &&
						pt.y <= LABORATORY_SOURCE_SLOT_ENCHANT_START_Y + SLOT_INTERVAL_Y)
					{
						CItemInfo* pItemInfo = FindItemLabFactoryFromSource(LABORATORY_ITEM_ENCHANT);
					if(pItemInfo)
					{
						g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
					}
				}
					else if (pt.x >= LABORATORY_SOURCE_SLOT_ETC_START_X &&
						pt.x <= LABORATORY_SOURCE_SLOT_ETC_START_X + SLOT_INTERVAL_X &&
						pt.y >= LABORATORY_SOURCE_SLOT_ETC_START_Y &&
						pt.y <= LABORATORY_SOURCE_SLOT_ETC_START_Y + SLOT_INTERVAL_Y)
					{
						CItemInfo* pItemInfo = FindItemLabFactoryFromSource(LABORATORY_ITEM_ETC1);
					if(pItemInfo)
					{
						g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
					}
				}
					else if (pt.x >= LABORATORY_SOURCE_SLOT_ETC_START_X + SLOT_INTERVAL_X&&
						pt.x <= LABORATORY_SOURCE_SLOT_ETC_START_X + SLOT_INTERVAL_X * 2&&
						pt.y >= LABORATORY_SOURCE_SLOT_ETC_START_Y &&
						pt.y <= LABORATORY_SOURCE_SLOT_ETC_START_Y + SLOT_INTERVAL_Y)
					{
						CItemInfo* pItemInfo = FindItemLabFactoryFromSource(LABORATORY_ITEM_ETC2);
					if(pItemInfo)
					{
						g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
					}
				}
					else
					{
						g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
					}
				}
				else if(m_nAniState == LABORATORY_ANIMATION_STATE_RESULT_SUCCESS_END)
				{
					if( m_bShowTarget &&
					pt.x >= LABFACTORY_TARGET_SLOT_START_X && 
					pt.x <= LABFACTORY_TARGET_SLOT_START_X+SLOT_INTERVAL_X &&
					pt.y >= LABFACTORY_TARGET_SLOT_START_Y && 
					pt.y <= LABFACTORY_TARGET_SLOT_START_Y+SLOT_INTERVAL_Y)
				{
						CItemInfo* pItemInfo = FindItemLabFactoryFromTarget(LABORATORY_ITEM_TTARGET);
					if(pItemInfo)
					{
						g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
					}
				}
				else
				{
					g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
				}
			}
			}
			else
			{
			if( pt.x > SOURCE_SLOT_START_X &&
				pt.x < SOURCE_SLOT_START_X + SLOT_INTERVAL_X*4)
			{
				if(	pt.y > SOURCE_SLOT_START_Y && 
					pt.y < SOURCE_SLOT_START_Y+SLOT_INTERVAL_Y*2)
				{
					int x = (pt.x - SOURCE_SLOT_START_X) / SLOT_INTERVAL_X;
					int y = (pt.y - SOURCE_SLOT_START_Y) / SLOT_INTERVAL_Y;
					int index = x + y * SOURCE_NUMBER_X;
					if( index >= 0 && m_vecSource.size() > index )
					{
						CItemInfo* pItemInfo = m_vecSource[index];
						if(pItemInfo)
						{
							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							//g_pGameMain->m_pItemInfo->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
						}
					}
				}
				else if( m_bShowTarget &&
					pt.y > TARGET_SLOT_START_Y && 
					pt.y < TARGET_SLOT_START_Y+SLOT_INTERVAL_Y)
				{
					// 2008-03-14 by bhsohn 조합식 개선안
					//int index = (pt.x - TARGET_SLOT_START_X) / SLOT_INTERVAL_X;
					int nSlotStartX = TARGET_SLOT_START_X;
					if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
					{
						nSlotStartX = TARGET_FACTORY_SLOT_START_X;
					}				
					int index = (pt.x - nSlotStartX) / SLOT_INTERVAL_X;
					// end 2008-03-14 by bhsohn 조합식 개선안

					if( index >= 0 && m_vecTarget.size() > index )
					{
						CItemInfo* pItemInfo = m_vecTarget[index];
						if(pItemInfo)
						{
							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							//g_pGameMain->m_pItemInfo->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
						}
					}
				}
				else
				{
					g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
				}
			}
			}
#else
			if( pt.x > SOURCE_SLOT_START_X &&
				pt.x < SOURCE_SLOT_START_X + SLOT_INTERVAL_X*4)
			{
				if(	pt.y > SOURCE_SLOT_START_Y && 
					pt.y < SOURCE_SLOT_START_Y+SLOT_INTERVAL_Y*2)
				{
					int x = (pt.x - SOURCE_SLOT_START_X) / SLOT_INTERVAL_X;
					int y = (pt.y - SOURCE_SLOT_START_Y) / SLOT_INTERVAL_Y;
					int index = x + y * SOURCE_NUMBER_X;
					if( index >= 0 && m_vecSource.size() > index )
					{
						CItemInfo* pItemInfo = m_vecSource[index];
						if(pItemInfo)
						{
							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							//g_pGameMain->m_pItemInfo->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
						}
					}
				}
				else if( m_bShowTarget &&
					pt.y > TARGET_SLOT_START_Y && 
					pt.y < TARGET_SLOT_START_Y+SLOT_INTERVAL_Y)
				{
					// 2008-03-14 by bhsohn 조합식 개선안
					//int index = (pt.x - TARGET_SLOT_START_X) / SLOT_INTERVAL_X;
					int nSlotStartX = TARGET_SLOT_START_X;
					if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
					{
						nSlotStartX = TARGET_FACTORY_SLOT_START_X;
					}				
					int index = (pt.x - nSlotStartX) / SLOT_INTERVAL_X;
					// end 2008-03-14 by bhsohn 조합식 개선안
					
					if( index >= 0 && m_vecTarget.size() > index )
					{
						CItemInfo* pItemInfo = m_vecTarget[index];
						if(pItemInfo)
						{
							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							//g_pGameMain->m_pItemInfo->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y );
							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
						}
					}
				}
				else
				{
					g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
				}
			}
#endif
			// end 2015-05-29 by jwlee 연구소 UI 변경
			if(pt.y > OK_BUTTON_START_Y && 
				pt.y < OK_BUTTON_START_Y+BUTTON_SIZE_Y)
			{
				if( pt.x > OK_BUTTON_START_X && 
					pt.x < OK_BUTTON_START_X+BUTTON_SIZE_X)
				{
					if(m_nButtonState[0] != BUTTON_STATE_DOWN)
						m_nButtonState[0] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[0] = BUTTON_STATE_NORMAL;
				}
				/////////////////////////////////////////////////////////////////////////////
				
				if( pt.x > CANCEL_BUTTON_START_X && 
					pt.x < CANCEL_BUTTON_START_X+BUTTON_SIZE_X)
				{
					if(m_nButtonState[1] != BUTTON_STATE_DOWN)
						m_nButtonState[1] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}
			else
			{
				m_nButtonState[0] = BUTTON_STATE_NORMAL;
				m_nButtonState[1] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

//			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
//				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_LAB_POS)
//			{
//				m_bSelectDown = TRUE;
//			}

			// 2013-03-18 by bhsohn 팩토리 조합 추가
			// 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_INGAME_MIX_ITEM
			BOOL bOldEditMode = m_pNumEditBox->IsEditMode();
			if(TRUE == m_pNumEditBox->OnLButtonDown(pt) && (m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
			{			
				if(!bOldEditMode)
				{
					UpdateMixPrice();// 조합가격 표시
				}
				m_pNumEditBox->EnableEdit(TRUE);
				// 버튼위에 마우스가 있다.
				return  INF_MSGPROC_BREAK;				
			}
			m_pNumEditBox->EnableEdit(FALSE);
			if(bOldEditMode)
			{				
				UpdateMixPrice();// 조합가격 표시
			}
#endif
			// end 2008-03-14 by bhsohn 조합식 개선안

			if(pt.y > OK_BUTTON_START_Y && 
				pt.y < OK_BUTTON_START_Y + BUTTON_SIZE_Y)
			{
				if( pt.x > OK_BUTTON_START_X && 
					pt.x < OK_BUTTON_START_X+BUTTON_SIZE_X)
				{
					m_nButtonState[0] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if( pt.x > CANCEL_BUTTON_START_X && 
					pt.x < CANCEL_BUTTON_START_X+BUTTON_SIZE_X)
				{
					m_nButtonState[1] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nButtonState[1] = BUTTON_STATE_NORMAL;
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
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
						
			if(pt.y > OK_BUTTON_START_Y && 
				pt.y < OK_BUTTON_START_Y + BUTTON_SIZE_Y)
			{
				if( pt.x > OK_BUTTON_START_X && 
					pt.x < OK_BUTTON_START_X+BUTTON_SIZE_X)
				{
					// 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_INGAME_MIX_ITEM
					m_pNumEditBox->EnableEdit(FALSE);// 2013-03-18 by bhsohn 팩토리 조합 추가
#endif
					
					if(m_bShowTarget)
					{
						OnButtonClicked(2);// OK
					}
					else
					{
						// 2015-05-29 by jwlee 연구소 UI 변경
						// OnButtonClicked(0); // SEND
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
						if (m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
						{
							if (ISStateNormal())
							{
						OnButtonClicked(0); // SEND
							}
						}
						else OnButtonClicked(0); // SEND
#else
						OnButtonClicked(0); // SEND
#endif
						// end 2015-05-29 by jwlee 연구소 UI 변경
					}
					m_nButtonState[0] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if( pt.x > CANCEL_BUTTON_START_X && 
					pt.x < CANCEL_BUTTON_START_X+BUTTON_SIZE_X)
				{
					// 2015-05-29 by jwlee 연구소 UI 변경
// 					OnButtonClicked(1);
// 					m_nButtonState[1] = BUTTON_STATE_UP;
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
					if (m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
					{
						if (ISStateNormal())
						{
					OnButtonClicked(1);
					m_nButtonState[1] = BUTTON_STATE_UP;
						}
					}
					else
					{
						OnButtonClicked(1);
						m_nButtonState[1] = BUTTON_STATE_UP;
					}
#else
					OnButtonClicked(1);
 					m_nButtonState[1] = BUTTON_STATE_UP;
#endif
					// end 2015-05-29 by jwlee 연구소 UI 변경
				}
				else 
				{
					m_nButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}	
			int nSourceMaxCount = 0;
			if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
			{
				// 2009. 01. 21 by ckPark 인챈트 확률 증가 카드
				//nSourceMaxCount = 2;
				// 확률 증가 카드 때문에 최대 4개로 늘린다
				// 2011-11-04 by hsson 조합 시 재료 아이템이 최대 4개인것을 5개까지 늘림
				//nSourceMaxCount = 3;
				nSourceMaxCount = 4;
				// end 2011-11-04 by hsson 조합 시 재료 아이템이 최대 4개인것을 5개까지 늘림
				// end 2009. 01. 21 by ckPark 인챈트 확률 증가 카드
				// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
				if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
					((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
					m_vecSource.size() <= nSourceMaxCount)
				{
					CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;
					if(pItemInfo->Wear != WEAR_NOT_ATTACHED)
					{
						((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
							STRMSG_C_CITYLAP_0002, _MESSAGE );//"착용된 아이템은 올릴 수 없습니다."
					}
					else
					{
						// 2007-12-13 by dgwoo 시간제 아이템이면서 사용한 아이템이라면 팩토리에 올릴수 없다.
						if((pItemInfo->ItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT 
							|| COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
							|| COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)) // 2008-11-26 by bhsohn 절대시간 제한 아이템 구현
							&& pItemInfo->GetItemPassTime() != 0)
						{
							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_071212_0100,COLOR_ERROR);//"해당 아이템은 조합 할 수 있는 상태가 아닙니다."
						}
						else
						{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
							// 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)
							if(pItemInfo->FixedTermShape.nStatShapeItemNum && pItemInfo->FixedTermShape.nStatLevel)
							{
								g_pStoreData->RqCollectionShapeChange(pItemInfo->UniqueNumber,0);
							}
#endif
							if (IS_ENCHANT_TARGET_ITEMKIND(pItemInfo->Kind) &&
								pt.x >= LABORATORY_SOURCE_SLOT_STARGET_START_X &&
								pt.x <= LABORATORY_SOURCE_SLOT_STARGET_START_X + SLOT_INTERVAL_X &&
								pt.y >= LABORATORY_SOURCE_SLOT_STARGET_START_Y &&
								pt.y <= LABORATORY_SOURCE_SLOT_STARGET_START_Y + SLOT_INTERVAL_Y)
							{
								if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
									((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
									m_vecSource.size() <= nSourceMaxCount)
								{
									if(FindItemFromSource(pItemInfo->UniqueNumber) == NULL)
									{
										if (IsRegisterEnchantFromSourceByKind(pItemInfo->Kind))
										{
											InvenToSourceItem(pItemInfo, 1);
										}
										else
										{
											// 더이상 등록 할 수 없습니다.
										}
									}
								}
							}
							else if (ITEMKIND_ENCHANT == pItemInfo->Kind &&
								pt.x >= LABORATORY_SOURCE_SLOT_ENCHANT_START_X &&
								pt.x <= LABORATORY_SOURCE_SLOT_ENCHANT_START_X + SLOT_INTERVAL_X &&
								pt.y >= LABORATORY_SOURCE_SLOT_ENCHANT_START_Y &&
								pt.y <= LABORATORY_SOURCE_SLOT_ENCHANT_START_Y + SLOT_INTERVAL_Y)
							{
								
								if(FindItemFromSource(pItemInfo->UniqueNumber) == NULL)
								{
									if (IsRegisterEnchantFromSourceByKind(pItemInfo->Kind))
									{
										InvenToSourceItem(pItemInfo, 1);
									}
									else
									{
										// 더이상 등록 할 수 없습니다.
									}
								}
							}
							else if (IS_ENCHANT_SOURCE_ETC(pItemInfo->Kind) &&
								pt.x >= LABORATORY_SOURCE_SLOT_ETC_START_X &&
								pt.x <= LABORATORY_SOURCE_SLOT_ETC_START_X + SLOT_INTERVAL_X * 2 &&
								pt.y >= LABORATORY_SOURCE_SLOT_ETC_START_Y &&
								pt.y <= LABORATORY_SOURCE_SLOT_ETC_START_Y + SLOT_INTERVAL_Y)
							{
								if(FindItemFromSource(pItemInfo->UniqueNumber) == NULL)
								{
									if (IsRegisterEnchantFromSourceByKind(pItemInfo->Kind))
									{
										InvenToSourceItem(pItemInfo, 1);
									}
									else
									{
										// 더이상 등록 할 수 없습니다.
									}
								}
							}
						}
					}
				}
#endif
				// end 2015-05-29 by jwlee 연구소 UI 변경
			}
			else if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
			{
				nSourceMaxCount = (SOURCE_NUMBER_X*SOURCE_NUMBER_Y)-1;
			}
			// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
			else if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
			{
#endif
				// end 2015-05-29 by jwlee 연구소 UI 변경
				
			if( pt.x > SOURCE_SLOT_START_X &&
				pt.x < SOURCE_SLOT_START_X + SLOT_INTERVAL_X*4 &&
				pt.y > SOURCE_SLOT_START_Y &&
				pt.y < SOURCE_SLOT_START_Y + SLOT_INTERVAL_Y*2 &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
				m_vecSource.size() <= nSourceMaxCount )//(SOURCE_NUMBER_X*SOURCE_NUMBER_Y)-1)
			{
				CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;

				if(IS_COUNTABLE_ITEM(pItemInfo->Kind))
				{					
					if(m_pSelectItem == NULL)
					{
						m_pSelectItem = pItemInfo;

						// 빌딩종류가 팩토리가 아니거나, 아이템이 인첸트,갬블이 아니면 메시지 박스를 띄운다.
						if(	BUILDINGKIND_LABORATORY == m_pBuildingInfo->BuildingKind && 
							IS_SPECIAL_COUNTABLE_ITEM(pItemInfo->Kind) )
						{
							if(FindItemFromSource(pItemInfo->UniqueNumber) == NULL)
							{
									//InvenToSourceItem(((CINFGameMain*)m_pParent)->m_pInven->m_pSelectItem, 1);
									InvenToSourceItem(pItemInfo, 1);
							}
							m_pSelectItem = NULL;
						}
						else
						{
							((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
								STRMSG_C_CITYLAP_0001, _Q_LAB_ITEM_NUMBER, (DWORD)this, pItemInfo->CurrentCount);//"몇개를 올리시겠습니까?"
						}
					}					
				}
				else
				{
					if(pItemInfo->Wear != WEAR_NOT_ATTACHED)
					{
						((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
							STRMSG_C_CITYLAP_0002, _MESSAGE );//"착용된 아이템은 올릴 수 없습니다."
					}
					else
					{
						// 2007-12-13 by dgwoo 시간제 아이템이면서 사용한 아이템이라면 팩토리에 올릴수 없다.
						if((pItemInfo->ItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT 
							|| COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
							|| COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)) // 2008-11-26 by bhsohn 절대시간 제한 아이템 구현
							&& pItemInfo->GetItemPassTime() != 0)
						{
							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_071212_0100,COLOR_ERROR);//"해당 아이템은 조합 할 수 있는 상태가 아닙니다."
						}
						else
						{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
							// 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)
							if(pItemInfo->FixedTermShape.nStatShapeItemNum && pItemInfo->FixedTermShape.nStatLevel)
							{
								g_pStoreData->RqCollectionShapeChange(pItemInfo->UniqueNumber,0);
							}
#endif
							// end 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)
							InvenToSourceItem(pItemInfo, 1);
						}
						
					}
					//m_vecSource.push_back(((CINFGameMain*)m_pParent)->m_pInven->m_pSelectItem);
				}
				// 2008-08-22 by bhsohn EP3 인벤토리 처리
				if(g_pGameMain && g_pGameMain->m_pInven)
				{
					g_pGameMain->SetToolTip(NULL, 0, 0);
					g_pGameMain->m_pInven->SetItemInfo(NULL, 0, 0);
					g_pGameMain->m_pInven->SetMultiSelectItem(NULL);	// 다중 처리 제거
					g_pGameMain->m_pInven->SetSelectItem(NULL);
				}
					// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
				}
#endif
				// end 2015-05-29 by jwlee 연구소 UI 변경
			}
		}
		break;
	case WM_KEYDOWN:
		{
			// 2013-03-18 by bhsohn 팩토리 조합 추가
			// 2008-03-14 by bhsohn 조합식 개선안
#ifndef C_INGAME_MIX_ITEM
			if(m_pNumEditBox->WndProc(uMsg, wParam, lParam))
			{
				UpdateMixPrice();// 조합가격 표시
				return INF_MSGPROC_BREAK;
			}
#endif
			// end 2008-03-14 by bhsohn 조합식 개선안
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityLab::OnButtonClicked(int nButton)
{
	switch(nButton)
	{
	case 0:	//	SEND
		{
			if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_LABORATORY)
			{
				if ( g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_ENCHANT_PREVENTION ) )
				{
					return;
				}

				// 2013-09-26 by ssjung 엔진 인첸트시 경고 메시지 출력
#ifdef C_ENGINE_ENCHANT_WARNING
				if( FALSE == m_bIsEnchantCheck && IsWarning_EngineEnchantFail())
				{
					g_pGameMain->m_pInfWindow->AddMsgBox (STRMSG_C_130926_0001, _Q_ENCHANT_PREVENTION );
					return;
				}
#endif
				// end 2013-09-26 by ssjung 엔진 인첸트시 경고 메시지 출력

				// 2010. 06. 18 by jskim 인챈트 경고 메시지 추가
				if ( FALSE == m_bIsEnchantCheck && IsWarning_EnchantFail() )
				{
					g_pGameMain->m_pInfWindow->AddMsgBox ( STRMSG_C_100618_0407, _Q_ENCHANT_PREVENTION );	//"인첸트 파괴방지 카드가 없어 인첸트 아이템이 파괴 될수 있습니다."	
					return;
				}

				m_bIsEnchantCheck = FALSE;
				//end 2010. 06. 18 by jskim 인챈트 경고 메시지 추가
				BOOL bGamble = FALSE;

				MSG_FC_ITEM_USE_ENCHANT sMsg;
					memset((char*)&sMsg, 0x00, sizeof(sMsg));

				// 2010. 04. 21 by jskim 신규 럭키 머신 구현
				int nIsPrefix = NULL;
				int nOverlapItem = NULL;
				//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
				
				vector<CItemInfo*>::iterator it = m_vecSource.begin();

				CItemInfo *pcItemInfo = NULL;

				while(it != m_vecSource.end())
				{
					pcItemInfo = (CItemInfo *)*it;

					if( pcItemInfo->Kind == ITEMKIND_ENCHANT || pcItemInfo->Kind == ITEMKIND_GAMBLE)
					{
						// 2010. 04. 21 by jskim 신규 럭키 머신 구현
						//if(sMsg.EnchantItemUniqueNumber != 0)
//						
// 						{
// 							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITYLAP_0003,COLOR_ERROR);//"잘못된 아이템 구성입니다."
// 							InitData();
// 							return;
// 						}
						if(nOverlapItem == NULL)
						{
							nOverlapItem = pcItemInfo->Kind;
						}
						else
						{
							if(nOverlapItem != pcItemInfo->Kind)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
						}
					
						//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
						if(pcItemInfo->Kind == ITEMKIND_GAMBLE)
						{
							bGamble = TRUE;
						}
						// 2010. 04. 21 by jskim 신규 럭키 머신 구현
						//sMsg.EnchantItemUniqueNumber = (*it)->UniqueNumber;
						if(pcItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX))
						{
							if(sMsg.EnchantItemUniqueNumber != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							sMsg.EnchantItemUniqueNumber = pcItemInfo->UniqueNumber;
							nIsPrefix = RARE_FIX_PREFIX;							
						}
						else if(pcItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX))
						{
							if(sMsg.EnchantItemUniqueNumber2 != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							sMsg.EnchantItemUniqueNumber2 = pcItemInfo->UniqueNumber;
							nIsPrefix = RARE_FIX_SUFFIX;
						}

						else if(pcItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE))
						{
							if(sMsg.EnchantItemUniqueNumber != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							sMsg.EnchantItemUniqueNumber = pcItemInfo->UniqueNumber;
							nIsPrefix = RARE_FIX_PREFIX;
						}
						else if(pcItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
						{
							if(sMsg.EnchantItemUniqueNumber2 != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							sMsg.EnchantItemUniqueNumber2 = pcItemInfo->UniqueNumber;
							nIsPrefix = RARE_FIX_SUFFIX;
						}
						else
						{
							if(sMsg.EnchantItemUniqueNumber != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							sMsg.EnchantItemUniqueNumber = pcItemInfo->UniqueNumber;
						}
						//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
					}
					else if(pcItemInfo->Kind == ITEMKIND_PREVENTION_DELETE_ITEM)
					{
// 						// 2008-05-29 by bhsohn 인첸트 관련 버그수정
// 						if(sMsg.AttachItemUniqueNumber != 0)
// 						{
// 							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITYLAP_0003,COLOR_ERROR);//"잘못된 아이템 구성입니다."
// 							InitData();
// 							return;
// 						}
//						sMsg.AttachItemUniqueNumber = pcItemInfo->UniqueNumber;
// 						// end 2008-05-29 by bhsohn 인첸트 관련 버그수정

						
						// 2009. 01. 21 by ckPark 인챈트 확률 증가 카드
						// 파괴방지카드나 확률증가카드 둘다 아이템카인드가 같으므로
						// DES_ENCHANT_INCREASE_PROBABILITY 이 존재하면 확률 증가 카드로 인식
						if( pcItemInfo->ItemInfo->GetParameterValue(DES_ENCHANT_INCREASE_PROBABILITY) != 0.0f )
						{
							if(sMsg.IncreaseProbabilityItemUID != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							
							sMsg.IncreaseProbabilityItemUID = pcItemInfo->UniqueNumber;
						}
						// DES_ENCHANT_INCREASE_PROBABILITY 이 존재하지 않으면 파괴방지 카드로 인식
						else
						{
							if(sMsg.AttachItemUniqueNumber != 0)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							sMsg.AttachItemUniqueNumber = pcItemInfo->UniqueNumber;
						}
						// end 2009. 01. 21 by ckPark 인챈트 확률 증가 카드

					}
					// 2010. 02. 11 by ckPark 발동류 장착아이템

// 					else if(IS_WEAPON((*it)->Kind)
// 						|| ((*it)->Kind == ITEMKIND_DEFENSE)
// 						// 2008-09-26 by bhsohn 신규 인첸트 처리
// 						|| ((*it)->Kind == ITEMKIND_SUPPORT)
// 						|| ((*it)->Kind == ITEMKIND_RADAR))	
// 						// end 2008-09-26 by bhsohn 신규 인첸트 처리
					else if( IS_ENCHANT_TARGET_ITEMKIND( pcItemInfo->Kind ) )

					// end 2010. 02. 11 by ckPark 발동류 장착아이템
					{
						// 2008-05-29 by bhsohn 인첸트 관련 버그수정
						if(sMsg.TargetItemUniqueNumber != 0)
						{
							ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
							return;
						}											
						// end 2008-05-29 by bhsohn 인첸트 관련 버그수정
						sMsg.TargetItemUniqueNumber = pcItemInfo->UniqueNumber;
					}
					else
					{
						// 2008-05-29 by bhsohn 인첸트 관련 버그수정
						ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
						return;
						// end 2008-05-29 by bhsohn 인첸트 관련 버그수정
					}

					it++;
				}

				// 2010. 04. 21 by jskim 신규 럭키 머신 구현
				if(sMsg.EnchantItemUniqueNumber == 0 && sMsg.EnchantItemUniqueNumber2 != 0)
				{	
					sMsg.EnchantItemUniqueNumber = sMsg.EnchantItemUniqueNumber2;
					sMsg.EnchantItemUniqueNumber2 = NULL;
				}

				if(sMsg.EnchantItemUniqueNumber != 0 && sMsg.EnchantItemUniqueNumber2 != 0)
				{
					int IS_FIX_INFO = NULL;
					vector<CItemInfo*>::iterator it = m_vecSource.begin();
					while(it != m_vecSource.end())
					{
						if(IS_FIX_INFO == 0)
						{
							if((*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE) ||
								(*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
							{
								IS_FIX_INFO = ITIALIZE_ITEM;
							}
							else if((*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX) ||
									(*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX))
							{
								IS_FIX_INFO = RARE_FIX_ITEM;
							}
						}
						else
						{
							if(((*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE) ||
								(*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE)) && 
								IS_FIX_INFO == RARE_FIX_ITEM)
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
							else if(((*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX) ||
									(*it)->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX)) && 
									IS_FIX_INFO == ITIALIZE_ITEM)	
							{
								ErrorMsg_InvalidEnchantList ( STRMSG_C_CITYLAP_0003 );
								return;
							}
						}
						it++;
					}
				}
				//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

				if(bGamble == TRUE && sMsg.AttachItemUniqueNumber != 0)
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITYLAP_0003,COLOR_ERROR);
					InitData();
				}
				else if(sMsg.TargetItemUniqueNumber != 0 && sMsg.EnchantItemUniqueNumber != 0)
				{
					// 2015-05-29 by jwlee 연구소 UI 변경
					//m_vecTarget = m_vecSource;
					//m_vecSource.clear();
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
#else
					m_vecTarget = m_vecSource;
					m_vecSource.clear();
#endif
					// end 2015-05-29 by jwlee 연구소 UI 변경
					m_bShowTarget = FALSE;
					// 2015-07-28 by jwlee 연구소 UI 변경 2차
// 					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENCHANT, (char*)&sMsg, sizeof(sMsg));
// 
// 					g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
					ChangeAniState(LABORATORY_ANIMATION_STATE_ICON_START);
					m_UseEchantMsg = sMsg;
#else
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENCHANT, (char*)&sMsg, sizeof(sMsg));
					 
					g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark
#endif
					// end 2015-07-28 by jwlee 연구소 UI 변경 2차

					//InitData();
				}
				else
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITYLAP_0003,COLOR_ERROR);
					InitData();
				}
			}
			else if(m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY)
			{
// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가
#ifdef C_WARNING_APPEARANCE_CHANGE
				if ( g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_APPEARANCE_CHANGE_PREVENTION ) )
				{
					return;
				}

				// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
#ifndef C_INGAME_MIX_ITEM				
 				if ( FALSE == m_bIsAppearanceChangeCheck && IsWarning_AppearanceChange())
 				{
 					g_pGameMain->m_pInfWindow->AddMsgBox ( STRMSG_C_121030_0001, _Q_APPEARANCE_CHANGE_PREVENTION );	
 					return;
 				}
#endif
				// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
				
				m_bIsAppearanceChangeCheck = FALSE;
#endif
// end  2012-10-30 by mspark, 외형 변경 경고 메시지 추가
				if(m_vecSource.size() == 0)
				{
					break;
				}
				char pPacket[SIZE_MAX_PACKET];
				MSG_FC_ITEM_MIX_ITEMS sMsg;
				memset(&sMsg, 0x00, sizeof(MSG_FC_ITEM_MIX_ITEMS));
				sMsg.NumOfItems = m_vecSource.size();
				
				// 2008-03-14 by bhsohn 조합식 개선안
				// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifndef C_INGAME_MIX_ITEM
 				char chBuff[256];
 				m_pNumEditBox->GetString(chBuff, 256);				
 				sMsg.nMixCounts = atoi(chBuff);
#else
				sMsg.nMixCounts = 1;
				if(m_pINFItemMixFactoryWnd)
				{
					sMsg.nMixCounts = m_pINFItemMixFactoryWnd->GetSendMakeNumCnt();	// 아이템 갯수를 얻어오자
				}
#endif
				// END 2013-03-18 by bhsohn 팩토리 조합 추가
				if(sMsg.nMixCounts > COUNT_MAX_MIXING_COUNT)
				{
					sMsg.nMixCounts = COUNT_MAX_MIXING_COUNT;
				}
				// end 2008-03-14 by bhsohn 조합식 개선안

				memcpy( pPacket, (char*)&sMsg, sizeof(sMsg) );

				int i = 0;

				vector<CItemInfo*>::iterator it = m_vecSource.begin();

				while ( it != m_vecSource.end() )
				{
					ITEM_UNIQUE_NUMBER_W_COUNT itemCount;
					itemCount.ItemUniqueNumber = (*it)->UniqueNumber;
					itemCount.Count = (*it)->CurrentCount;
					memcpy( pPacket + sizeof(sMsg) + i*sizeof(ITEM_UNIQUE_NUMBER_W_COUNT),
						(char*)&itemCount, 
						sizeof(ITEM_UNIQUE_NUMBER_W_COUNT));
					it++;
					i++;
				}				

				g_pFieldWinSocket->SendMsg( T_FC_ITEM_MIX_ITEMS, 
											pPacket, 
											sizeof(sMsg)+sizeof(ITEM_UNIQUE_NUMBER_W_COUNT)*m_vecSource.size() );

				g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark
				// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
#ifndef C_INGAME_MIX_ITEM				
				m_vecTarget = m_vecSource;
#else
				{
					vector<CItemInfo*>::iterator itSource = m_vecSource.begin();
					while(itSource != m_vecSource.end())
					{
						CItemInfo* pSourceItem = *itSource;
						m_vecTarget.push_back(pSourceItem);
						itSource++;
					}
				}
#endif
				// END 2013-04-08 by bhsohn 인게임 조합창 추가 구현
				m_vecSource.clear();
				m_bShowTarget = FALSE;

			}
		}
		break;

	case 1:	//	CANCEL
		{
			InitData();
		}
		break;

	case 2: // OK
		{
			InitData();
		}
		break;
	}
}

void CINFCityLab::InvenToSourceItem(CItemInfo* pItemInfo, int nCount)
{
	if(ITEMKIND_ENCHANT == pItemInfo->Kind || ITEMKIND_GAMBLE == pItemInfo->Kind)
	{
		MSG_FC_INFO_GET_ENCHANT_COST sMsg;
		sMsg.EnchantItemNum = pItemInfo->ItemNum;
		g_pFieldWinSocket->SendMsg( T_FC_INFO_GET_ENCHANT_COST, (char*)&sMsg, sizeof(sMsg));
	}
	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
	if (IS_ENCHANT_TARGET_ITEMKIND(pItemInfo->Kind))
	{
		m_bAniView[LABORATORY_GROUP_STARGET] = TRUE;
	}
	else if (ITEMKIND_ENCHANT == pItemInfo->Kind)
	{
		m_bAniView[LABORATORY_GROUP_ENCHANT] = TRUE;
	}
	else if (IS_ENCHANT_SOURCE_ETC(pItemInfo->Kind))
	{
		m_bAniView[LABORATORY_GROUP_ETC] = TRUE;
	}
#endif
	// end 2015-05-29 by jwlee 연구소 UI 변경
	// Inven에서 지운다.
	if(IS_COUNTABLE_ITEM(pItemInfo->Kind))
	{
		ASSERT_ASSERT(pItemInfo->CurrentCount >= nCount);
		
		vector<CItemInfo*>::iterator it = m_vecSource.begin();
		while( it != m_vecSource.end() )
		{
			if((*it)->UniqueNumber == pItemInfo->UniqueNumber )
			{
				(*it)->CurrentCount += nCount;
				break;
			}
			it++;
		}
		if( it == m_vecSource.end() )
		{
			CItemInfo* pNewItem = new CItemInfo((ITEM_GENERAL*)pItemInfo);

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			//pNewItem->SetEnchantParam( pItemInfo->GetEnchantParamFactor(), pItemInfo->GetEnchantNumber() );
			// 발동류 인챈트 및 쿨타임 추가 복사
			pNewItem->CopyItemInfo( pItemInfo );
			// end 2010. 02. 11 by ckPark 발동류 장착아이템

			pNewItem->CurrentCount = nCount;
			m_vecSource.push_back(pNewItem);
			// 2007-12-17 by bhsohn 조합 가격 표시
			// 조합가격 표시
			UpdateMixPrice();
		}
		g_pStoreData->UpdateItemCount( pItemInfo->UniqueNumber, pItemInfo->CurrentCount - nCount);
	}
	else 
	{
		CItemInfo* pNewItem = new CItemInfo((ITEM_GENERAL*)pItemInfo);

		// 2010. 02. 11 by ckPark 발동류 장착아이템
		//pNewItem->SetEnchantParam( pItemInfo->GetEnchantParamFactor(), pItemInfo->GetEnchantNumber() );
		// 발동류 인챈트 및 쿨타임 추가 복사
		pNewItem->CopyItemInfo( pItemInfo );
		// end 2010. 02. 11 by ckPark 발동류 장착아이템

//      인챈트시 총알수가 1이 대는 현상
//		pNewItem->CurrentCount = nCount;
		m_vecSource.push_back(pNewItem);
		g_pStoreData->DeleteItem( pItemInfo->UniqueNumber );
		// 2007-12-17 by bhsohn 조합 가격 표시
		// 조합가격 표시
		UpdateMixPrice();
	}
	
	// 2006-06-21 by ispark, 삭제
//	if(	(g_pStoreData->m_mapItemUniqueNumber.size()/6)-2 < g_pGameMain->m_pInven->m_nInvenCurrentScroll)
//	{
//		g_pGameMain->m_pInven->m_nInvenCurrentScroll = (g_pStoreData->m_mapItemUniqueNumber.size()/6)-2;
//	}
}

BOOL CINFCityLab::PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg)
{
	vector<CItemInfo*>::iterator it = m_vecTarget.begin();
	while(it != m_vecTarget.end())
	{
		if((*it)->UniqueNumber == pMsg->ItemUID )
		{
			(*it)->ChangeRareInfo(pMsg->PrefixCodeNum, pMsg->SuffixCodeNum);
			return TRUE;
		}
		it++;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFCityLab::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)
/// \brief		인챈트 시도 결과 - 성공시 서버에서 넘겨준다.
/// \author		dhkwon
/// \date		2004-07-18 ~ 2004-07-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//BOOL CINFCityLab::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)
//{
//	FLOG( "CStoreData::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)" );
//	vector<CItemInfo*>::iterator it = m_vecTarget.begin();
//	while(it != m_vecTarget.end())
//	{
//		if((*it)->UniqueNumber == pMsg->Enchant.TargetItemUniqueNumber )
//		{
//			(*it)->PutEnchant( pMsg->Enchant.DestParameter, pMsg->Enchant.ParameterValue );
//			return TRUE;
//		}
//		it ++;
//	}
//	return FALSE;
//}
BOOL CINFCityLab::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)
{
	FLOG( "CStoreData::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)" );
	vector<CItemInfo*>::iterator it = m_vecTarget.begin();
	while(it != m_vecTarget.end())
	{
		if((*it)->UniqueNumber == pMsg->Enchant.TargetItemUniqueNumber )
		{
			(*it)->AddEnchantItem( pMsg->Enchant.EnchantItemNum );
//			ITEM* pEnchantITEM = g_pDatabase->GetServerItemInfo( pMsg->Enchant.EnchantItemNum );
//			if( pEnchantITEM )
//			{
//				if( pEnchantITEM->DestParameter1 != DES_NULL )
//				{
//					(*it)->PutEnchant(pEnchantITEM->DestParameter1, pEnchantITEM->ParameterValue1);
//				}
//				if( pEnchantITEM->DestParameter2 != DES_NULL )
//				{
//					(*it)->PutEnchant(pEnchantITEM->DestParameter2, pEnchantITEM->ParameterValue2);
//				}
//				if( pEnchantITEM->DestParameter3 != DES_NULL )
//				{
//					(*it)->PutEnchant(pEnchantITEM->DestParameter3, pEnchantITEM->ParameterValue3);
//				}
//				if( pEnchantITEM->DestParameter4 != DES_NULL )
//				{
//					(*it)->PutEnchant(pEnchantITEM->DestParameter4, pEnchantITEM->ParameterValue4);
//				}
//			}
			return TRUE;
		}
		it ++;
	}
	return FALSE;
}

// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
//void CINFCityLab::InitData()
void CINFCityLab::InitData(BOOL bItemsResult/*=FALSE*/)
{
	vector<CItemInfo*>::iterator it = m_vecSource.begin();
	while(it != m_vecSource.end())
	{
		(*it)->ItemWindowIndex = POS_INVALID_POSITION;
		if(g_pStoreData)
		{
			g_pStoreData->PutItem((char*)((ITEM_GENERAL*)(*it)), TRUE);
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( (*it)->UniqueNumber );
			ASSERT_ASSERT( pItemInfo );

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			//pItemInfo->SetEnchantParam( (*it)->GetEnchantParamFactor(), (*it)->GetEnchantNumber() );
			// 발동류 인챈트 및 쿨타임 추가 복사
			pItemInfo->CopyItemInfo( (*it) );
			// end 2010. 02. 11 by ckPark 발동류 장착아이템

		}
		SAFE_DELETE(*it);
		it++;
	}
	m_vecSource.clear();
	it = m_vecTarget.begin();
	while(it != m_vecTarget.end())
	{
		(*it)->ItemWindowIndex = POS_INVALID_POSITION;
		if(g_pStoreData)
		{
			g_pStoreData->PutItem((char*)((ITEM_GENERAL*)(*it)), TRUE);
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( (*it)->UniqueNumber );
			ASSERT_ASSERT( pItemInfo );

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			//pItemInfo->SetEnchantParam( (*it)->GetEnchantParamFactor(), (*it)->GetEnchantNumber() );
			// 발동류 인챈트 및 쿨타임 추가 복사
			pItemInfo->CopyItemInfo( (*it) );
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		SAFE_DELETE(*it);
		it++;
	}
	m_vecTarget.clear();
	
	g_pShuttleChild->ResortingItem();// 여기
	if(g_pGameMain && g_pGameMain->m_pInven)
	{
		g_pGameMain->m_pInven->SetScrollEndLine();				// 2006-06-21 by ispark
		g_pGameMain->m_pInven->SetAllIconInfo();
	}
	memset( m_szPrice, 0x00, 64);
	m_bShowTarget = FALSE;
	// 2010. 06. 18 by jskim 인챈트 경고 메시지 추가
	m_bIsEnchantCheck = FALSE;
	if(g_pGameMain)
	{
		g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_ENCHANT_PREVENTION);
	}
	//end 2010. 06. 18 by jskim 인챈트 경고 메시지 추가		
	
// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가
#ifdef C_WARNING_APPEARANCE_CHANGE
	m_bIsAppearanceChangeCheck = FALSE;
	if(g_pGameMain)
	{
		g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_APPEARANCE_CHANGE_PREVENTION);
	}
#endif
// end 2012-10-30 by mspark, 외형 변경 경고 메시지 추가	

	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM				
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->InitData(bItemsResult);
	}
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

	// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
 	ChangeAniState(LABORATORY_ANIMATION_STATE_NORMAL);
#endif
 	// end 2015-05-29 by jwlee 연구소 UI 변경
}

void CINFCityLab::SetPrice(int nPrice)
{
	wsprintf( m_szPrice, "%d", nPrice);
	if( g_pGameMain->m_pInven->GetItemSpi() < nPrice )
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITYLAP_0004,COLOR_ERROR);//"수수료가 부족합니다."
	}
}

void CINFCityLab::DeleteTargetItem(int nItemUniqueNumber)
{
	vector<CItemInfo*>::iterator it =  m_vecTarget.begin();
	while( it != m_vecTarget.end() )
	{
		if( (*it)->UniqueNumber == nItemUniqueNumber )
		{
			SAFE_DELETE( *it );
			m_vecTarget.erase( it );
			break;
		}
		it++;
	}
	if(m_vecTarget.size() > 0)
	{
		DBGOUT("Enchant/Mix Result(View)\n");
		m_bShowTarget = TRUE;
		g_pD3dApp->m_bRequestEnable = TRUE;			// 2006-06-19 by ispark
		g_pD3dApp->m_fRequestEnableTime = REQUEST_ENABLE_INIT_TIME;
	}
	else
	{
		m_bShowTarget = FALSE;
	}
}

void CINFCityLab::PutTargetItem(ITEM_GENERAL* pItem, BOOL bUpdateItemCount/*=FALSE*/)
{
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM					
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->SetFinalResultTargetUniqueNum(pItem->UniqueNumber);
	}									
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가

	vector<CItemInfo*>::iterator it =  m_vecTarget.begin();
	while( it != m_vecTarget.end() )
	{
		if( (*it)->UniqueNumber == pItem->UniqueNumber )
		{
			DBGOUT("Mix System Result - Already Being Item.");
			// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
#ifdef C_INGAME_MIX_ITEM								
			if(bUpdateItemCount)
			{
				(*it)->CurrentCount = pItem->CurrentCount;
			}								
#endif
			// END 2013-04-08 by bhsohn 인게임 조합창 추가 구현
			return;
		}
		it++;
	}
	CItemInfo * pNewItem = new CItemInfo(pItem);
	m_vecTarget.push_back(pNewItem);
	m_bShowTarget = TRUE;
	g_pD3dApp->m_bRequestEnable = TRUE;			// 2006-06-19 by ispark
	g_pD3dApp->m_fRequestEnableTime = REQUEST_ENABLE_INIT_TIME;
	
}

CItemInfo* CINFCityLab::GetTargetItemInfo()
{
	vector<CItemInfo*>::iterator it =  m_vecTarget.begin();
	while( it != m_vecTarget.end() )
	{
		if( (*it)->Kind != ITEMKIND_ENCHANT 
			&& (*it)->Kind != ITEMKIND_GAMBLE 
			&& (*it)->Kind != ITEMKIND_PREVENTION_DELETE_ITEM)
		{
			return *it;
		}
		it++;
	}

	return NULL;
}

CItemInfo* CINFCityLab::FindItemFromSource(UID64_t UniqueNumber)
{
	vector<CItemInfo*>::iterator it = m_vecSource.begin();
	while(it != m_vecSource.end())
	{
		if(UniqueNumber == (*it)->UniqueNumber)
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}

CItemInfo* CINFCityLab::FindItemFromTarget(UID64_t UniqueNumber)
{
	vector<CItemInfo*>::iterator it = m_vecTarget.begin();
	while(it != m_vecTarget.end())
	{
		if(UniqueNumber == (*it)->UniqueNumber)
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}
// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
CItemInfo* CINFCityLab::GetSTargetItemInfo()
{
	vector<CItemInfo*>::iterator it = m_vecSource.begin();
	while( it != m_vecSource.end() )
	{
		if( IS_ENCHANT_TARGET_ITEMKIND((*it)->Kind))
		{
			return *it;
		}
		it++;
	}
	
	return NULL;
}
CItemInfo* CINFCityLab::FindItemLabFactoryFromSource(int nFindItemType)
{
	int nCount = 0;
	for(int i = 0 ; i < m_vecSource.size() ; i++)
	{
		switch (nFindItemType)
		{
		case LABORATORY_ITEM_STARGET:
			if(IS_ENCHANT_TARGET_ITEMKIND(m_vecSource[i]->Kind))
			{
				return m_vecSource[i];
			}
			break;
		case LABORATORY_ITEM_ENCHANT:
			if(ITEMKIND_ENCHANT == m_vecSource[i]->Kind)
			{
				return m_vecSource[i];
			}
			break;
		case LABORATORY_ITEM_ETC1:
			if(IS_ENCHANT_SOURCE_ETC(m_vecSource[i]->Kind))
			{
				return m_vecSource[i];
			}
		case LABORATORY_ITEM_ETC2:
			if(IS_ENCHANT_SOURCE_ETC(m_vecSource[i]->Kind))
			{
				nCount++;
				if (nCount >= 2)
				{
					return m_vecSource[i];
				}
				
			}
			break;
		}
	}
	return NULL;
}
CItemInfo* CINFCityLab::FindItemLabFactoryFromTarget(int nFindItemType)
{
	vector<CItemInfo*>::iterator it = m_vecTarget.begin();
	while(it != m_vecSource.end())
	{
		switch (nFindItemType)
		{
		case LABORATORY_ITEM_TTARGET:
			if(IS_ENCHANT_TARGET_ITEMKIND((*it)->Kind))
			{
				return (*it);
			}
			break;
		}
		it++;
	}
	return NULL;
}
#endif
// end 2015-05-29 by jwlee 연구소 UI 변경
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityLab::ReSetTargetItemNum(UID64_t UniqueNumber, int nItemNum)
/// \brief		조합시 아이템 넘버만 바뀐다.
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityLab::ReSetTargetItemNum(UID64_t UniqueNumber, int nItemNum)
{
	CItemInfo* pItemInfo = FindItemFromTarget(UniqueNumber);
	ITEM* pItem = g_pDatabase->GetServerItemInfo(nItemNum);

	if(pItem == NULL || pItemInfo == NULL)
	{
		return;
	}
	
	pItemInfo->ItemInfo = pItem;
	pItemInfo->ItemNum = nItemNum;

	// 2009-04-07 by bhsohn 아이템 넘버 버그 수정
	pItemInfo->ResetRealItemInfo();
	// end 2009-04-07 by bhsohn 아이템 넘버 버그 수정
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityLab::SetSelectItem(CItemInfo* pItemInfo)
/// \brief		
/// \author		ispark
/// \date		2006-07-27 ~ 2006-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityLab::SetSelectItem(CItemInfo* pItemInfo)
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityLab::UpdateMixPrice()
/// \brief		
/// \author		// 2007-12-17 by bhsohn 조합 가격 표시
/// \date		2007-12-17 ~ 2007-12-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityLab::UpdateMixPrice()
{
	if(NULL == m_pBuildingInfo 
		|| m_pBuildingInfo->BuildingKind != BUILDINGKIND_FACTORY)
	{
		return;
	}
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifndef C_INGAME_MIX_ITEM	
	char chBuff[256];
	m_pNumEditBox->GetString(chBuff, 256);
	INT nMixCounts = atoi(chBuff);		

	if(nMixCounts <= 0)
	{
		return;
	}
	else if(nMixCounts > COUNT_MAX_MIXING_COUNT)
	{
		nMixCounts = COUNT_MAX_MIXING_COUNT;
	}
	// end 2008-03-14 by bhsohn 조합식 개선안

	int nCnt =0;
	ITEM_MIXING_INFO	stMixInfo;
	memset(&stMixInfo, 0x00, sizeof(ITEM_MIXING_INFO));
	vector<CItemInfo*>::iterator it = m_vecSource.begin();
	while(it != m_vecSource.end())
	{
		if(nCnt >= COUNT_ITEM_MIXING_SOURCE)
		{
			break;
		}
		CItemInfo* pItem = (*it);
		// 아이템 번호
		stMixInfo.SourceItem[nCnt].ItemNum = pItem->ItemNum;
		// 갯수
		if(IS_COUNTABLE_ITEM(pItem->Kind))
		{
			// 2008-03-14 by bhsohn 조합식 개선안
			//stMixInfo.SourceItem[nCnt].Count = pItem->CurrentCount;			
			stMixInfo.SourceItem[nCnt].Count = pItem->CurrentCount/nMixCounts;			
		}
		else
		{
			stMixInfo.SourceItem[nCnt].Count = 1;			
		}
		nCnt++;
		it++;
	}
	stMixInfo.NumOfSourceItems = nCnt;

	ITEM_MIXING_INFO* pMixInfo = g_pDatabase->GetMixerPrice(&stMixInfo);
	if(pMixInfo)
	{		
		int nAllPrice = pMixInfo->MixingCost*nMixCounts;
		if(nAllPrice >= 0)
		{
			SetPrice(nAllPrice);
		}
	}
#else	
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->UpdateMixPrice();		
	}
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void				OnOpenInfWnd();
/// \brief		// 처음 상점 오픈
/// \author		// 2008-03-14 by bhsohn 조합식 개선안
/// \date		2008-03-14 ~ 2008-03-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityLab::OnOpenInfWnd()
{
	char chBuff[32];
	wsprintf(chBuff, "1");
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifndef C_INGAME_MIX_ITEM
	m_pNumEditBox->SetString(chBuff, 32);
	m_pNumEditBox->EnableEdit(FALSE);
#else
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->SetMakeNumCnt(1);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void				OnOpenInfWnd();
/// \brief		// 처음 상점 오픈
/// \author		// 2008-03-14 by bhsohn 조합식 개선안
/// \date		2008-03-14 ~ 2008-03-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityLab::OnCloseInfWnd()
{
#ifdef C_INGAME_MIX_ITEM
	// 2013-03-18 by bhsohn 팩토리 조합 추가
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		ShowItemMixWnd(FALSE);
	}	
#else

	m_pNumEditBox->EnableEdit(FALSE);	
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
}


/************************************************************************************************************************************************************
**
**	인챈트 실패 경고 해야 하는지 체크.
**
**	Create Info :	2010. 09. 27. by hsLee.
**
**	Base Info : 'ITEMKIND_PREVENTION_DELETE_ITEM' - 인챈트 파괴 방지 카드와 인챈트 확률 증가 카드는 Kind가 같기 때문에 
**				'DES_ENCHANT_INCREASE_PROBABILITY' DesParam값의 유무로 구별. 
**
**	Update Info : '인챈트 파괴 방지 카드'와 '인챈트 확률 증가 카드'의 Kind값이 같아서 경고 메시지 없이 넘어가는 문제 수정.	2010. 09. 27. by hsLee.
** 
*************************************************************************************************************************************************************/
bool CINFCityLab :: IsWarning_EnchantFail ( void )
{

	bool bHavePreventionDeleteItem = FALSE;
	bool bWaringEnchantLevel = FALSE;
	bool bEnableEnchant = FALSE;
	bool bEnchantOnceApply = FALSE;		// 2013-08-28 by ssjung 10인첸트 카드 일 때 인첸트 확률이 100%이므로 2차 경고창을 띄우지 않도록 처리 
	
	vector<CItemInfo*>::iterator itr_SourceItem = m_vecSource.begin();
	
	CItemInfo *pcItemInfo = NULL;
	
	while( itr_SourceItem != m_vecSource.end() )
	{
		pcItemInfo = (CItemInfo *)*itr_SourceItem;
		
		if ( pcItemInfo->Kind == ITEMKIND_PREVENTION_DELETE_ITEM && pcItemInfo->ItemInfo->GetParameterValue(DES_ENCHANT_INCREASE_PROBABILITY) == 0.0f )
		{
			bHavePreventionDeleteItem = TRUE;
		}
		
		if ( pcItemInfo->GetEnchantNumber() >= 5 )
		{
			bWaringEnchantLevel = TRUE;
		}
		
		if ( pcItemInfo->Kind == ITEMKIND_ENCHANT && !pcItemInfo->ItemInfo->IsExistDesParam ( DES_ENCHANT_INITIALIZE ) )
		{
			bEnableEnchant = TRUE;
		}

// 2013-08-28 by ssjung 10인첸트 카드 일 때 인첸트 확률이 100%이므로 2차 경고창을 띄우지 않도록 처리 
		if(!bEnchantOnceApply)	// ssjung_111
		{
			if ( pcItemInfo->Kind == ITEMKIND_ENCHANT && !pcItemInfo->ItemInfo->IsExistDesParam ( DES_ENCHANT_ONCE_APPLY ) )
			{
				bEnchantOnceApply = TRUE;
			}
		}
// end 2013-08-28 by ssjung 10인첸트 카드 일 때 인첸트 확률이 100%이므로 2차 경고창을 띄우지 않도록 처리
		
		itr_SourceItem++;
	}

//	if ( bEnableEnchant && bWaringEnchantLevel && !bHavePreventionDeleteItem)
	if ( bEnableEnchant && bWaringEnchantLevel && !bHavePreventionDeleteItem && bEnchantOnceApply)  
		// 2013-08-28 by ssjung 10인첸트 카드 일 때 인첸트 확률이 100%이므로 2차 경고창을 띄우지 않도록 처리 
		return true;

	return false;

}

// 2013-09-26 by ssjung 엔진 인첸트시 경고 메시지 출력
bool CINFCityLab :: IsWarning_EngineEnchantFail ( void )
{
	vector<CItemInfo*>::iterator itr_SourceItem = m_vecSource.begin();
	
	CItemInfo *pcItemInfo = NULL;
	
	while( itr_SourceItem != m_vecSource.end() )
	{
		pcItemInfo = (CItemInfo *)*itr_SourceItem;
		if(ITEMKIND_SUPPORT == pcItemInfo->Kind)
		{
			return true;
		}

		itr_SourceItem++;
	}
	return false;
}
// end 2013-09-26 by ssjung 엔진 인첸트시 경고 메시지 출력

/**************************************************************************************
**
**	유효하지 않은 조합 관련 에러 메시지 출력.
**
**	Create Info :	2010. 09. 27. by hsLee.
**
***************************************************************************************/
void CINFCityLab :: ErrorMsg_InvalidEnchantList ( char *pszMessage , const bool a_bInitData /*= true*/ )
{

	if ( pszMessage && pszMessage[0] != 0 )
		g_pD3dApp->m_pChat->CreateChatChild ( pszMessage , COLOR_ERROR ); //"잘못된 아이템 구성입니다."

	if ( a_bInitData )
		InitData();
}
// 2012-02-03 by jskim 팩토리 더블클릭 구현 
void CINFCityLab::UpLoadItem(CItemInfo* i_pItem)
{
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM	
	if(m_vecSource.size() >= COUNT_ITEM_MIXING_SOURCE)
	{
		return;
	}	
#else
	if(m_vecSource.size() <= (SOURCE_NUMBER_X*SOURCE_NUMBER_Y)-1)
#endif
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
	{
		if(IS_COUNTABLE_ITEM(i_pItem->Kind))
		{					
			if(m_pSelectItem == NULL)
			{
				m_pSelectItem = i_pItem;
				
			// 빌딩종류가 팩토리가 아니거나, 아이템이 인첸트,갬블이 아니면 메시지 박스를 띄운다.
			if(	BUILDINGKIND_LABORATORY == m_pBuildingInfo->BuildingKind && 
				IS_SPECIAL_COUNTABLE_ITEM(i_pItem->Kind) )
			{
				if(FindItemFromSource(i_pItem->UniqueNumber) == NULL)
				{
					//								InvenToSourceItem(((CINFGameMain*)m_pParent)->m_pInven->m_pSelectItem, 1);
						// 2015-05-29 by jwlee 연구소 UI 변경
						//InvenToSourceItem(i_pItem, 1);
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
						if (IsRegisterEnchantFromSourceByKind(i_pItem->Kind))
						{
					InvenToSourceItem(i_pItem, 1);
				}
						else
						{
							// 더이상 등록 할 수 없습니다.
						}
#else
						InvenToSourceItem(i_pItem, 1);
#endif
						// end 2015-05-29 by jwlee 연구소 UI 변경
					}
					m_pSelectItem = NULL;
				}
				else
				{
					// 2015-05-29 by jwlee 연구소 UI 변경
// 					((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
// 							STRMSG_C_CITYLAP_0001, _Q_LAB_ITEM_NUMBER, (DWORD)this, i_pItem->CurrentCount);//"몇개를 올리시겠습니까?"
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
					if(BUILDINGKIND_LABORATORY == m_pBuildingInfo->BuildingKind)
					{
						m_pSelectItem = NULL;
						// 등록할 수 없는 아이템입니다.
			}
			else
			{
				((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
					STRMSG_C_CITYLAP_0001, _Q_LAB_ITEM_NUMBER, (DWORD)this, i_pItem->CurrentCount);//"몇개를 올리시겠습니까?"
				}
#else
					((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
 							STRMSG_C_CITYLAP_0001, _Q_LAB_ITEM_NUMBER, (DWORD)this, i_pItem->CurrentCount);//"몇개를 올리시겠습니까?"
#endif
					// end 2015-05-29 by jwlee 연구소 UI 변경
				}
			}
		}
		else
		{
			if(i_pItem->Wear != WEAR_NOT_ATTACHED)
			{
				((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
					STRMSG_C_CITYLAP_0002, _MESSAGE );//"착용된 아이템은 올릴 수 없습니다."
			}
			else
			{
				// 2007-12-13 by dgwoo 시간제 아이템이면서 사용한 아이템이라면 팩토리에 올릴수 없다.
				if((i_pItem->ItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT 
					|| COMPARE_BIT_FLAG(i_pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
					|| COMPARE_BIT_FLAG(i_pItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)) // 2008-11-26 by bhsohn 절대시간 제한 아이템 구현
					&& i_pItem->GetItemPassTime() != 0)
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_071212_0100,COLOR_ERROR);//"해당 아이템은 조합 할 수 있는 상태가 아닙니다."
				}
				else
				{
					// 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
					if(i_pItem->FixedTermShape.nStatShapeItemNum && i_pItem->FixedTermShape.nStatLevel)
					{
						g_pStoreData->RqCollectionShapeChange(i_pItem->UniqueNumber,0);
					}
#endif
					//end 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)
					
					// 2015-05-29 by jwlee 연구소 UI 변경
					// InvenToSourceItem(i_pItem, 1);
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
					if(BUILDINGKIND_LABORATORY == m_pBuildingInfo->BuildingKind)
					{
						if(IsRegisterEnchantFromSourceByKind(i_pItem->Kind))
						{
					InvenToSourceItem(i_pItem, 1);
						}
						else
						{
							// 더이상 등록 할 수 없습니다.
						}
					}
					else
					{
						InvenToSourceItem(i_pItem, 1);
					}
					m_pSelectItem = NULL;
#else
					InvenToSourceItem(i_pItem, 1);
#endif
					// end 2015-05-29 by jwlee 연구소 UI 변경
				}
				
			}
			//m_vecSource.push_back(((CINFGameMain*)m_pParent)->m_pInven->m_pSelectItem);
		}
		// 2008-08-22 by bhsohn EP3 인벤토리 처리
		if(g_pGameMain && g_pGameMain->m_pInven)
		{
			g_pGameMain->SetToolTip(NULL, 0, 0);
			g_pGameMain->m_pInven->SetItemInfo(NULL, 0, 0);
			g_pGameMain->m_pInven->SetMultiSelectItem(NULL);	// 다중 처리 제거
			g_pGameMain->m_pInven->SetSelectItem(NULL);
		}
				// end 2008-08-22 by bhsohn EP3 인벤토리 처리
	}	
	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM		
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->InitSearchItemSourceItem();
	}
#endif
}// end 2012-02-03 by jskim 팩토리 더블클릭 구현 

// 2015-05-29 by jwlee 연구소 UI 변경
#if defined(C_LABORATORY_UI_CHANGE_JWLEE)
void CINFCityLab::FieldSocketItemUseEnchantOk(MSG_FC_ITEM_USE_ENCHANT_OK* pMsg)
{
	// 2015-07-28 by jwlee 연구소 UI 변경 2차
	if(pMsg->bSuccessFlag)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0417, COLOR_SYSTEM);
		ChangeAniState(LABORATORY_ANIMATION_STATE_RESULT_SUCCESS);
		m_bShowTarget = TRUE;
	}
	else
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0418, COLOR_SYSTEM);
		ChangeAniState(LABORATORY_ANIMATION_STATE_NORMAL);
		m_bShowTarget = FALSE;
	}
	m_nSucAniLoopCnt = 0;
	g_pD3dApp->m_bRequestEnable = TRUE;
	// end 2015-07-28 by jwlee 연구소 UI 변경 2차
}
BOOL CINFCityLab::IsRegisterEnchantFromSourceByKind(BYTE Kind)
{
	BOOL bResult = FALSE;
	int nCount = 0;

	vector<CItemInfo*>::iterator it = m_vecSource.begin();
	while(it != m_vecSource.end())
	{
		if(IS_ENCHANT_TARGET_ITEMKIND(Kind))
		{
			if(IS_ENCHANT_TARGET_ITEMKIND((*it)->Kind))
			{
				nCount++;
			}
		}
		else if (ITEMKIND_ENCHANT == Kind)
		{
			if(ITEMKIND_ENCHANT == (*it)->Kind)
			{
				nCount++;
			}
		}
		else if (IS_ENCHANT_SOURCE_ETC(Kind))
		{
			if(IS_ENCHANT_SOURCE_ETC((*it)->Kind))
			{
				nCount++;
			}
		}
		it++;
	}

	if(IS_ENCHANT_TARGET_ITEMKIND(Kind))
	{
		if(nCount == 0) bResult = TRUE;
	}
	else if (ITEMKIND_ENCHANT == Kind)
	{
		if(nCount == 0) bResult = TRUE;
	}
	else if (IS_ENCHANT_SOURCE_ETC(Kind))
	{
		if (nCount <= 1) bResult = TRUE;
	}
	return bResult;
}
BOOL CINFCityLab::ISStateNormal()
{
	if (m_nAniState == LABORATORY_ANIMATION_STATE_NORMAL)
	{
		return TRUE;
	}
	return FALSE;
}
void CINFCityLab::ChangeAniState(int nAniState)
{
	int i = 0;
	switch (nAniState)
	{
	case LABORATORY_ANIMATION_STATE_NORMAL:
		m_nAniState = LABORATORY_ANIMATION_STATE_NORMAL;
		for (i = 0 ; i < LABORATORY_GROUP_END; i++)
		{
			m_bAniView[i] = FALSE;
		}
		break;
	case LABORATORY_ANIMATION_STATE_ICON_START:

		if(m_pImgLabIcon[LABORATORY_GROUP_STARGET])
		{
			m_pImgLabIcon[LABORATORY_GROUP_STARGET]->SetUVAnimation(LAB_MIXING_ICON0_FRAME_MAX_X,LAB_MIXING_ICON0_FRAME_MAX_Y,LAB_MIXING_ICON0_FRAME_TIME);
			m_pImgLabIcon[LABORATORY_GROUP_STARGET]->Move(LABORATORY_SOURCE_SLOT_STARGET_START_X - 6,LABORATORY_SOURCE_SLOT_STARGET_START_Y - 6);
			m_pImgLabIcon[LABORATORY_GROUP_STARGET]->SetFrame(0);
		}
		if(m_pImgLabIcon[LABORATORY_GROUP_ENCHANT])
		{
			m_pImgLabIcon[LABORATORY_GROUP_ENCHANT]->SetUVAnimation(LAB_MIXING_ICON0_FRAME_MAX_X,LAB_MIXING_ICON0_FRAME_MAX_Y,LAB_MIXING_ICON0_FRAME_TIME);
			m_pImgLabIcon[LABORATORY_GROUP_ENCHANT]->Move(LABORATORY_SOURCE_SLOT_ENCHANT_START_X - 6,LABORATORY_SOURCE_SLOT_ENCHANT_START_Y - 6);
			m_pImgLabIcon[LABORATORY_GROUP_ENCHANT]->SetFrame(0);
		}
		if(m_pImgLabIcon[LABORATORY_GROUP_ETC])
		{
			m_pImgLabIcon[LABORATORY_GROUP_ETC]->SetUVAnimation(LAB_MIXING_ICON1_FRAME_MAX_X,LAB_MIXING_ICON1_FRAME_MAX_Y,LAB_MIXING_ICON1_FRAME_TIME);
			m_pImgLabIcon[LABORATORY_GROUP_ETC]->Move(LABORATORY_SOURCE_SLOT_ETC_START_X - 6,LABORATORY_SOURCE_SLOT_ETC_START_Y - 7);
			m_pImgLabIcon[LABORATORY_GROUP_ETC]->SetFrame(0);
		}
		
		
		m_nAniState = LABORATORY_ANIMATION_STATE_ICON_ING;
		break;
	case LABORATORY_ANIMATION_STATE_LINE_START:

		if(m_pImgLabLine[LABORATORY_GROUP_STARGET])
		{
			m_pImgLabLine[LABORATORY_GROUP_STARGET]->SetUVAnimation(LAB_MIXING_LINE0_FRAME_MAX_X,LAB_MIXING_LINE0_FRAME_MAX_Y,LAB_MIXING_LINE0_FRAME_TIME);
			m_pImgLabLine[LABORATORY_GROUP_STARGET]->Move(LABORATORY_SOURCE_SLOT_STARGET_START_X + 33,LABORATORY_SOURCE_SLOT_STARGET_START_Y + 10);
			m_pImgLabLine[LABORATORY_GROUP_STARGET]->SetFrame(1);
		}
		if(m_pImgLabLine[LABORATORY_GROUP_ENCHANT])
		{
			m_pImgLabLine[LABORATORY_GROUP_ENCHANT]->SetUVAnimation(LAB_MIXING_LINE1_FRAME_MAX_X,LAB_MIXING_LINE1_FRAME_MAX_Y,LAB_MIXING_LINE1_FRAME_TIME);
			m_pImgLabLine[LABORATORY_GROUP_ENCHANT]->Move(LABORATORY_SOURCE_SLOT_ENCHANT_START_X + 33,LABORATORY_SOURCE_SLOT_ENCHANT_START_Y + 10);
			m_pImgLabLine[LABORATORY_GROUP_ENCHANT]->SetFrame(1);
		}
		if(m_pImgLabLine[LABORATORY_GROUP_ETC])
		{
			m_pImgLabLine[LABORATORY_GROUP_ETC]->SetUVAnimation(LAB_MIXING_LINE2_FRAME_MAX_X,LAB_MIXING_LINE2_FRAME_MAX_Y,LAB_MIXING_LINE2_FRAME_TIME);
			m_pImgLabLine[LABORATORY_GROUP_ETC]->Move(LABORATORY_SOURCE_SLOT_ETC_START_X + 75,LABORATORY_SOURCE_SLOT_ETC_START_Y - 47);
			m_pImgLabLine[LABORATORY_GROUP_ETC]->SetFrame(1);
		}

		m_nAniState = LABORATORY_ANIMATION_STATE_LINE_ING;
		break;
	case LABORATORY_ANIMATION_STATE_RESULT_START:
		for (i = 0 ; i < LABORATORY_GROUP_END; i++)
		{
			m_bAniView[i] = FALSE;
		}
		if(m_pImgLabLine[LABORATORY_GROUP_TTARGET])
		{
			m_pImgLabLine[LABORATORY_GROUP_TTARGET]->SetUVAnimation(LAB_MIXING_LINE3_FRAME_MAX_X,LAB_MIXING_LINE3_FRAME_MAX_Y,LAB_MIXING_LINE3_FRAME_TIME);
			m_pImgLabLine[LABORATORY_GROUP_TTARGET]->Move(LABFACTORY_TARGET_SLOT_START_X - 80,LABFACTORY_TARGET_SLOT_START_Y + 10);
			m_pImgLabLine[LABORATORY_GROUP_TTARGET]->SetFrame(1);
			m_bAniView[LABORATORY_GROUP_TTARGET] = TRUE;
		}
		m_nAniState = LABORATORY_ANIMATION_STATE_RESULT_ING;
		break;
	case LABORATORY_ANIMATION_STATE_RESULT_RECIVE:
		m_nAniState = LABORATORY_ANIMATION_STATE_RESULT_RECIVE;
		break;
	case LABORATORY_ANIMATION_STATE_RESULT_SUCCESS:
		m_nAniState = LABORATORY_ANIMATION_STATE_RESULT_SUCCESS;
		if(m_pImgLabIcon[LABORATORY_GROUP_TTARGET])
		{
			m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->SetUVAnimation(LAB_MIXING_ICONSUC_FRAME_MAX_X,LAB_MIXING_ICONSUC_FRAME_MAX_Y,LAB_MIXING_ICONSUC_FRAME_TIME);
			m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->Move(LABFACTORY_TARGET_SLOT_START_X - 6,LABFACTORY_TARGET_SLOT_START_Y - 6);
			m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->SetFrame(1);
			//m_pImgLabIcon[LABORATORY_GROUP_TTARGET]->SetLoop(TRUE);
		}
		break;
	case LABORATORY_ANIMATION_STATE_RESULT_SUCCESS_END:
		m_nAniState = LABORATORY_ANIMATION_STATE_RESULT_SUCCESS_END;
		break;
	}
}
#endif
// end 2015-05-29 by jwlee 연구소 UI 변경

// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가
#ifdef C_WARNING_APPEARANCE_CHANGE
bool CINFCityLab :: IsWarning_AppearanceChange ( void )
{	
	bool bHaveAppearanceItem = FALSE;
	bool bHaveGear = FALSE;

	vector<CItemInfo*>::iterator itr_SourceItem = m_vecSource.begin();
	
	CItemInfo *pcItemInfo = NULL;
	
	while( itr_SourceItem != m_vecSource.end() )
	{
		pcItemInfo = (CItemInfo *)*itr_SourceItem;
		
		if ( pcItemInfo->GetRealItemInfo()->ArrDestParameter[1] == DES_FIXED_TERM_SHAPE_TIME )
		{
			bHaveAppearanceItem = TRUE;
		}

		if ( pcItemInfo->Kind == ITEMKIND_DEFENSE )
		{
			bHaveGear = TRUE;
		}
		
		itr_SourceItem++;		
	}

	if( bHaveAppearanceItem && bHaveGear )
	{
		return true;
	}
	else
	{
		return false;
	}	
}
#endif
// end 2012-10-30 by mspark, 외형 변경 경고 메시지 추가

///////////////////////////////////////////////////////////////////////////////
/// \fn			void				OnOpenInfWnd();
/// \brief		
/// \author		// 2013-03-18 by bhsohn 팩토리 조합 추가
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityLab::ShowItemMixWnd(BOOL bShow, POINT* pBkPos/*=NULL*/)
{
	if(!m_pINFItemMixFactoryWnd)
	{
		return;
	}
	m_pINFItemMixFactoryWnd->ShowWnd(bShow, 0, 0, pBkPos);
}

void CINFCityLab::ItemMixItemsResult(MSG_FC_ITEM_MIX_ITEMS_RESULT* pMsg)
{
	if(m_pINFItemMixFactoryWnd)
	{
		m_pINFItemMixFactoryWnd->ItemMixItemsResult(pMsg);
	}	
}

void CINFCityLab::ErrorMixItem(Err_t ErrorCode)
{
	if(m_pINFItemMixFactoryWnd)
	{
		m_pINFItemMixFactoryWnd->ErrorMixItem(ErrorCode);
	}	
}

// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
void CINFCityLab::FieldSocketItemChangedShapeItemNum( MSG_FC_ITEM_CHANGED_SHAPEITEMNUM* pMsg )
{
	if(m_pINFItemMixFactoryWnd && (m_pBuildingInfo && m_pBuildingInfo->BuildingKind == BUILDINGKIND_FACTORY))
	{
		m_pINFItemMixFactoryWnd->SetFinalResultTargetUniqueNum(pMsg->nItemUID);
	}	

}
	