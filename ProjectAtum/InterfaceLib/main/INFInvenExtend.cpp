// INFInvenExtend.cpp: implementation of the CINFInvenExtend class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RangeTime.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "INFCityBase.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "StoreData.h"
#include "INFTrade.h"
#include "Chat.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFWindow.h"
#include "ItemInfo.h"
#include "AtumSound.h"
#include "Skill.h"
#include "Interface.h"
// 2008-09-22 by bhsohn EP3 캐릭터 창
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "INFCommunity.h"
#include "INFGameMainQSlot.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "INFChangeCharactor.h"
// 2007-12-05 by bhsohn 포탈 사용시, 편대 비행 해제
#include "ClientParty.h"

// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
#include "INFLuckyMachine.h"
// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안
#include "INFItemMenuList.h"	// 2013-02-26 by bhsohn 인게임 조합 검색 처리

// 장착 
#include "INFInvenEquip.h"
#include "INFInvenItem.h"

#include "INFInvenExtend.h"

#include "PetManager.h"                      //2011-10-06 by jhahn 파트너 성장형 시스템

#include "INFSkill.h"						 //2011-10-06 by jhahn 파트너 성장형 시스템

#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFImageEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define INVEN_SCROLL_LINE_START_X		410
#define INVEN_SCROLL_LINE_START_Y		30
#define INVEN_SCROLL_BUTTON_START_X		406
#define INVEN_SCROLL_BUTTON_START_Y		30
#define INVEN_SCROLL_BUTTON_WIDTH		11
#define INVEN_SCROLL_BUTTON_HEIGHT		38

#define INVEN_SCROLL_LINE_LENGTH		190//(178/*187-INVEN_SCROLL_BUTTON_HEIGHT+8*/)// 15:오차 범위 조절값
#define INVEN_SCROLL_LINE_MOVE_LENGTH	(INVEN_SCROLL_LINE_LENGTH - INVEN_SCROLL_BUTTON_HEIGHT)
#define INVEN_SCROLL_NUMBER				((((float)(g_pStoreData->m_mapItemUniqueNumber.size()/ INVEN_X_NUMBER)-2 )<0)?0:(g_pStoreData->m_mapItemUniqueNumber.size() / INVEN_X_NUMBER-2))// min:0인경우 막아야 한다.
#define INVEN_SCROLL_INTERVAL			(INVEN_SCROLL_LINE_LENGTH / SetScrollLine())

#define INVEN_GARBAGE_START_X			271
#define INVEN_GARBAGE_START_Y			221
#define INVEN_GARBAGE_SIZE				24
#define INVEN_NOT_USE_SLOT_ICON			"07900270"

#define CITY_SHOP_INVEN_START_Y		(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)

#define ROTATION_BASE_GAB_W
#define ROTATION_BASE_GAB_H

#define INVEN_BACK_W			388
#define INVEN_BACK_H			237
#define INVEN_BACK_POS_X		19
#define INVEN_BACK_POS_Y		19

#define INVEN_FULL_BACK_W		426
#define INVEN_FULL_BACK_H		275


// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define SPI_START_X				70//68
#define SPI_START_Y				3//5
#else
#define SPI_START_X				68//130
#define SPI_START_Y				3//5
#endif


CINFInvenExtend::CINFInvenExtend(CAtumNode* pParent)
{
	FLOG( "CINFInvenExtend(CAtumNode* pParent)" );		
	m_pItemSl = NULL ;	
	
	m_pParent = pParent;

	int i;
	for(i=0;i<INVEN_NUMBER;i++)
	{
		m_pInvenDisplayInfo[i] = NULL;
	}

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수 변경.
	//for(i=0;i<WEAR_ITEM_NUMBER;i++)
	for(i=0;i<MAX_EQUIP_POS;i++)
	{
		m_pWearDisplayInfo[i] = NULL;
	}
//	m_nInvenCurrentScroll = 0;	
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;

	m_pSelectIcon = NULL;
	m_pSelectItem = NULL;
//	m_nSelectItemWindowPosition = -1;

	m_pFontSpi = NULL;
	m_pFontWeight[0] = NULL;
	m_pFontWeight[1] = NULL;
	m_pFontWeight[2] = NULL;

	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i] = NULL;
	}
	m_nItemSpi = 0;
	m_hyItemSpiUniqueNumber = 0;
	
	// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
	//m_pDeleteItem = NULL;
	InitDeleteItemInfo();
	

	m_bEnchantState = FALSE;// 인챈트 시나리오 : 1. m_pSelectItem, m_pSelectIcon, m_bEnchantState 세팅
							//					 2. MsgBox(_Q_USE_ENCHANT_ITEM_CARD) 생성 보여줌(cancel로 취소)
							//					 3. WM_LBUTTONDOWN메시지에서 선택된 아이템이 있으면 서버로 전송
							//					 4. 그렇지 않은 경우 m_pSelectItem, m_pSelectIcon, m_bEnchantState 초기화
	m_pEnchantItem = NULL;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_pCurrentItemInfo = NULL;
	m_nRenderMoveIconIntervalHeight = 0;
	m_nRenderMoveIconIntervalWidth  = 0;
	
	m_bSelectWearItem = FALSE;
	m_bMouseDownState = FALSE;
	
	
	//m_nInvenCurrentScrollPosition = 0;

	// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
	memset(&m_struDeleteItem, 0x00, sizeof(structDelItemInfo));

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	m_pINFInvenEquip = NULL;
	m_pINFInvenItem = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	m_vecWndOrder.clear();
	m_vecWndOrder.push_back(INVEN_ITEM_WND);
	m_vecWndOrder.push_back(INVEN_EQ_WND);	
	
	m_bShowEqWnd = TRUE;
	m_bShowEqShopWnd = TRUE;
	m_bItemSetPos = m_bEqSetPos = m_bEqShopSetPos= FALSE;
	m_pItemIvenPos.x = m_pItemIvenPos.y = m_pEqIvenNormalPos.x = m_pEqIvenNormalPos.y = 0;	
	m_pEqIvenShopPos.x= m_pEqIvenShopPos.y = 0;
#else
	m_bItemSetPos = FALSE;
	m_pItemIvenPos.x = m_pItemIvenPos.y = 0;	
#endif

	// 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
	m_bShopConcurrent = timeGetTime();
	// end 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정

	m_pItemMenuList = NULL;// 2013-02-26 by bhsohn 인게임 조합 검색 처리
}

CINFInvenExtend::~CINFInvenExtend()
{
	FLOG( "~CINFInvenExtend()" );
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	SAFE_DELETE(m_pINFInvenEquip);
	SAFE_DELETE(m_pINFInvenItem);
		
	SAFE_DELETE(m_pItemSl);	
	SAFE_DELETE(m_pFontSpi ); 
	SAFE_DELETE(m_pFontWeight[0] ); 
	SAFE_DELETE(m_pFontWeight[1] ); 
	SAFE_DELETE(m_pFontWeight[2] ); 
	
	int i;
	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		SAFE_DELETE(m_pFontItemNum[i]);
	}
	for(i=0;i<INVEN_NUMBER;i++)
	{
		SAFE_DELETE(m_pInvenDisplayInfo[i]);
	}

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수 변경.
	//for(i=0;i<WEAR_ITEM_NUMBER;i++)
	for(i=0;i<MAX_EQUIP_POS;i++)
	{
		SAFE_DELETE(m_pWearDisplayInfo[i]);
	}

	SAFE_DELETE(m_pItemMenuList);		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
}

HRESULT CINFInvenExtend::InitDeviceObjects()
{
	FLOG( "CINFInvenExtend::InitDeviceObjects()" );
	m_pFontSpi = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontSpi->InitDeviceObjects(g_pD3dDev);
	m_pFontWeight[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontWeight[0]->InitDeviceObjects(g_pD3dDev);
	m_pFontWeight[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontWeight[1]->InitDeviceObjects(g_pD3dDev);
	m_pFontWeight[2] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontWeight[2]->InitDeviceObjects(g_pD3dDev);

	for(int i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum[i]->InitDeviceObjects(g_pD3dDev);
	}
	
	DataHeader	* pDataHeader;
	
	m_pItemSl = new CINFImageEx;
	pDataHeader = FindResource("itemsl");
	m_pItemSl->InitDeviceObjects(pDataHeader);

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 인벤토리
	if(NULL == m_pINFInvenEquip)
	{
		m_pINFInvenEquip = new CINFInvenEquip(this);
		m_pINFInvenEquip->SetGameData(m_pGameData);
		m_pINFInvenEquip->InitDeviceObjects();
	}
	if(NULL == m_pINFInvenItem)
	{
		m_pINFInvenItem = new CINFInvenItem(this);
		m_pINFInvenItem->SetGameData(m_pGameData);
		m_pINFInvenItem->InitDeviceObjects();
	}	
	// end -08-22 by bhsohn EP3 인벤토리 처리	

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
#if defined(C_INGAME_MIX_ITEM)
	if(NULL == m_pItemMenuList)
	{
		m_pItemMenuList = new CINFItemMenuList;
		m_pItemMenuList->SetGameData( m_pGameData );
		m_pItemMenuList->InitDeviceObjects();
	}
#endif
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	return S_OK ;
}

HRESULT CINFInvenExtend::RestoreDeviceObjects()
{
	FLOG( "CINFInvenExtend::RestoreDeviceObjects()" );		
	m_pItemSl->RestoreDeviceObjects();	
	
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 인벤토리
	if(m_pINFInvenEquip)
	{
		m_pINFInvenEquip->RestoreDeviceObjects();
	}
	if(m_pINFInvenItem)
	{
		m_pINFInvenItem->RestoreDeviceObjects();
	}	
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리

	
	m_pFontSpi->RestoreDeviceObjects();
	m_pFontWeight[0]->RestoreDeviceObjects();
	m_pFontWeight[1]->RestoreDeviceObjects();
	m_pFontWeight[2]->RestoreDeviceObjects();

	for(int i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i]->RestoreDeviceObjects();
	}
	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	if(m_pItemMenuList)
	{
		m_pItemMenuList->RestoreDeviceObjects();
	}
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFInvenExtend::DeleteDeviceObjects()
{
	FLOG( "CINFInvenExtend::DeleteDeviceObjects()" );	
	
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 인벤토리
	if(m_pINFInvenEquip)
	{
		m_pINFInvenEquip->DeleteDeviceObjects();
		SAFE_DELETE(m_pINFInvenEquip);
	}
	if(m_pINFInvenItem)
	{
		m_pINFInvenItem->DeleteDeviceObjects();
		SAFE_DELETE(m_pINFInvenItem);
	}	
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	if(m_pItemMenuList)
	{
		m_pItemMenuList->DeleteDeviceObjects();
		SAFE_DELETE(m_pItemMenuList);
	}
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	m_pItemSl->DeleteDeviceObjects();
	SAFE_DELETE(m_pItemSl );
			
	m_pFontSpi->DeleteDeviceObjects();
	SAFE_DELETE(m_pFontSpi );
	m_pFontWeight[0]->DeleteDeviceObjects();
	SAFE_DELETE(m_pFontWeight[0] );
	m_pFontWeight[1]->DeleteDeviceObjects();
	SAFE_DELETE(m_pFontWeight[1] );
	m_pFontWeight[2]->DeleteDeviceObjects();
	SAFE_DELETE(m_pFontWeight[2] );

	for(int i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i]->DeleteDeviceObjects();
		SAFE_DELETE(m_pFontItemNum[i]);
	}
	m_bInvalidated = FALSE;	
	return S_OK ;
}


HRESULT CINFInvenExtend::InvalidateDeviceObjects()
{
	FLOG( "CINFInvenExtend::InvalidateDeviceObjects()" );		
	m_pItemSl->InvalidateDeviceObjects();
		
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 인벤토리
	if(m_pINFInvenEquip)
	{
		m_pINFInvenEquip->InvalidateDeviceObjects();		
	}
	if(m_pINFInvenItem)
	{
		m_pINFInvenItem->InvalidateDeviceObjects();		
	}	
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	if(m_pItemMenuList)
	{
		m_pItemMenuList->InvalidateDeviceObjects();
	}
		// END 2013-02-26 by bhsohn 인게임 조합 검색 처리
	
	m_pFontSpi->InvalidateDeviceObjects();
	m_pFontWeight[0]->InvalidateDeviceObjects();
	m_pFontWeight[1]->InvalidateDeviceObjects();
	m_pFontWeight[2]->InvalidateDeviceObjects();
	
	for(int i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i]->InvalidateDeviceObjects();
	}
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;
	return S_OK ;
}


void CINFInvenExtend::Tick()
{
	//m_fElapsedTime = g_pD3dApp->GetElapsedTime();
	FLOG( "CINFInvenExtend::Tick()" );

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nCnt = m_vecWndOrder.size()-1;	
	for(nCnt = m_vecWndOrder.size()-1;nCnt >= 0 ;nCnt--)
	{
		int nWndOrderTmp = m_vecWndOrder[nCnt];
		switch(nWndOrderTmp)
		{
		case INVEN_ITEM_WND:
			{
				if(m_pINFInvenItem && m_pINFInvenItem->IsShowWnd())
				{
					m_pINFInvenItem->Tick();
				}	
			}
			break;
		case INVEN_EQ_WND:
			{
				if(m_pINFInvenEquip && m_pINFInvenEquip->IsShowWnd())
				{
					m_pINFInvenEquip->Tick();
				}
			}
			break;
		}
	}	

	// end 2008-08-22 by bhsohn EP3 인벤토리 처리
#else
	if(m_pINFInvenItem && m_pINFInvenItem->IsShowWnd())
	{
		m_pINFInvenItem->Tick();
	    m_pINFInvenEquip->Tick();
	}	
#endif
	
}

void CINFInvenExtend::Render()
{
	FLOG( "CINFInvenExtend::Render()" );
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nCnt = m_vecWndOrder.size()-1;	
	for(nCnt = m_vecWndOrder.size()-1;nCnt >= 0 ;nCnt--)
	{
		int nWndOrderTmp = m_vecWndOrder[nCnt];
		switch(nWndOrderTmp)
		{
		case INVEN_ITEM_WND:
			{
				if(m_pINFInvenItem && m_pINFInvenItem->IsShowWnd())
				{
					m_pINFInvenItem->Render();
				}	
			}
			break;
		case INVEN_EQ_WND:
			{
				if(m_pINFInvenEquip && m_pINFInvenEquip->IsShowWnd())
				{
					m_pINFInvenEquip->Render();
				}
			}
			break;
		}
	}	
#else
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리
	if(m_pINFInvenItem && m_pINFInvenItem->IsShowWnd())
	{
		m_pINFInvenItem->Render();
		m_pINFInvenEquip->Render();
	}
#endif	
	// 2010. 05. 12 by jskim 기존 머신 축하 이펙트 보이는 부분을 인벤 쪽에서 럭키머신 쪽으로 변경
	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	//RenderLuckyMechine();	
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	//end 2010. 05. 12 by jskim 기존 머신 축하 이펙트 보이는 부분을 인벤 쪽에서 럭키머신 쪽으로 변경
}

void CINFInvenExtend::RenderSpi(int x, int y)
{
	char temp1[64];
	char temp2[64];
	wsprintf( temp1, "%d", m_nItemSpi );
	MakeCurrencySeparator( temp2, temp1, 3, ',' );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	SIZE sSize = m_pFontSpi->GetStringSize(temp2);
	m_pFontSpi->DrawText(x+SPI_START_X - sSize.cx, y + SPI_START_Y, GUI_FONT_COLOR,temp2, 0L);
#else		 
	m_pFontSpi->DrawText(x+SPI_START_X, y + SPI_START_Y, GUI_FONT_COLOR,temp2, 0L);
#endif
}

void CINFInvenExtend::RenderOnCityBase()
{
	//Render();
}
//{
//	FLOG( "CINFInvenExtend::RenderOnCityBase()" );
//	
//	g_pGameMain->m_pInfWindow->m_nLeftWindowInfo = 1;
//	g_pGameMain->m_nLeftWindowY = CITY_SHOP_INVEN_START_Y;
//	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//
//	//////////////////////////////////////////////////////////////////////////
//	// 윈도우 틀재작 - 수정 요망 -
//	int x = CITY_BASE_NPC_BOX_START_X;
//	int y = nWindowPosY;
//	int cx = 426;
//	int cy = 275;
//
//	g_pGameMain->m_pInfWindow->m_pBoxImage[0]->Move(x,y);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[0]->Render();
//	g_pGameMain->m_pInfWindow->m_pBoxImage[1]->Move(x+19, y);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[1]->SetScale(cx-38, 1);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[1]->Render();
//	g_pGameMain->m_pInfWindow->m_pBoxImage[2]->Move(x+cx-19,y);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[2]->Render();
//	
//	g_pGameMain->m_pInfWindow->m_pBoxImage[3]->Move(x, y+19);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[3]->SetScale(1, cy-38);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[3]->Render();
//	
//	g_pGameMain->m_pInfWindow->m_pBoxImage[5]->Move(x+cx-19, y+19);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[5]->SetScale(1, cy-38);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[5]->Render();
//	
//	g_pGameMain->m_pInfWindow->m_pBoxImage[6]->Move(x,y+cy-19);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[6]->Render();
//	g_pGameMain->m_pInfWindow->m_pBoxImage[7]->Move(x+19,y+cy-19);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[7]->SetScale(cx-38, 1);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[7]->Render();
//	g_pGameMain->m_pInfWindow->m_pBoxImage[8]->Move(x+cx-19,y+cy-19);
//	g_pGameMain->m_pInfWindow->m_pBoxImage[8]->Render();
//
//	g_pShuttleChild->SetRenderInven(TRUE);
//	//
//	//////////////////////////////////////////////////////////////////////////
//	// 2006-04-04 by ispark
////	RenderInvenBack(TRUE);
////	g_pShuttleChild->RenderMirror();
//
//	
//	// 로테이션 이미지
//	if(m_nButtonState == ROTATION_NONE || m_nRotationState == ROTATION_STATE_N)
//	{
//		m_pInvenRotationBase->Move(CITY_BASE_NPC_BOX_START_X+16, nWindowPosY + 207);
//		m_pInvenRotationBase->Render();
//	}
//	else
//	{
//		m_pInvenDirection[m_nButtonState][m_nRotationState]->Move(CITY_BASE_NPC_BOX_START_X+16, nWindowPosY + 207);
//		m_pInvenDirection[m_nButtonState][m_nRotationState]->Render();
//	}
//	
//	CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
//	for(int i=0;i<INVEN_Y_NUMBER;i++)
//	{
//		for(int j=0;j<INVEN_X_NUMBER;j++)
//		{
//			m_pItemSl->Move(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j, 
//				CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i);
//			m_pItemSl->Render();
//			if(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j])
//			{
//				// 2007-02-12 by bhsohn Item 다중 선택 처리
//				BOOL bMultiSel = FALSE;
//				if(g_pD3dApp->CheckMultItemSel(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->UniqueNumber))
//				{
//					// 다중 선택이 성공한 아이템
//					bMultiSel = TRUE;
//				}
//				// end 2007-02-12 by bhsohn Item 다중 선택 처리
//
//				// 2005-11-28 by ispark, SourceIndex로 변경
//				char buf[64];
//				wsprintf(buf, "%08d", m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->ItemInfo->SourceIndex);
//				pIconInfo->SetIcon(buf, 
//						CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 1,
//						CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 1, 1.0f);
//				pIconInfo->Render();
//				
//				if( IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->Kind) )
//				{
//					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( 
//						m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->UniqueNumber );
//					if( pItemInfo->CurrentCount > 1 )
//					{
//						// 갯수를 보여준다.
//						char buf[128];
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
//						wsprintf(buf, "%d",pItemInfo->CurrentCount);
//						int len = strlen(buf) - 1;			// 여기는 한개 이상 들어온다는 정의에 -1를 했다.
//						m_pFontItemNum[i]->DrawText(CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 21 - len*6,	// 여기서 6은 영문 숫자 텍스트 간격이다.
//							CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i - 1, 
//							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
//#else
//						wsprintf(buf, "%d",pItemInfo->CurrentCount);
//						int len = strlen(buf) - 1;			// 여기는 한개 이상 들어온다는 정의에 -1를 했다.
//						m_pFontItemNum[i]->DrawText(CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 21 - len*6,	// 여기서 6은 영문 숫자 텍스트 간격이다.
//							CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i - 1, 
//							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
////						wsprintf(buf, "%4.d",pItemInfo->CurrentCount);
////						m_pFontItemNum[i]->DrawText(CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 2,
////							CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 2, 
////							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
//#endif
//					}
//				}
//				// 2007-02-12 by bhsohn Item 다중 선택 처리				
//				if(bMultiSel)
//				{
//					float fXPos, fYPos;
//					fXPos = CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 1;
//					fYPos = CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 1;						
//					m_pMultiItemSelImage->Move(fXPos, fYPos);
//					m_pMultiItemSelImage->Render();
//					
//				}
//				// end 2007-02-12 by bhsohn Item 다중 선택 처리				
//			}
//		}
//	}
//
//	if( m_iWearPosition != POS_INVALID_POSITION )
//	{
//		if( m_tBlinkTime.IsUnderMiddle() )
//		{
//			RenderWearPosition( m_iWearPosition );
//		}
//	}
//
//	char temp1[64];
//	char temp2[64];
//	SIZE size;
//	// SPI
//	wsprintf( temp1, "%d", m_nItemSpi );
//	MakeCurrencySeparator( temp2, temp1, 3, ',' );
//	size = m_pFontSpi->GetStringSize(temp2);
//	m_pFontSpi->DrawText(INVEN_SPI_START_X+CITY_BASE_NPC_BOX_START_X-size.cx, nWindowPosY + INVEN_SPI_START_Y, GUI_FONT_COLOR_BM,temp2, 0L);
//
//	// War Point
//	wsprintf(temp1,"%d",g_pShuttleChild->m_myShuttleInfo.WarPoint);
//	MakeCurrencySeparator(temp2,temp1,3,',');
//	size = m_pFontSpi->GetStringSize(temp2);
//	m_pFontSpi->DrawText(INVEN_SPI_START_X+CITY_BASE_NPC_BOX_START_X-size.cx, nWindowPosY + INVEN_WARPOINT_Y, GUI_FONT_COLOR_BM,temp2, 0L);
//
//	char buff[64];
//	wsprintf(buff, "(%3d%%)", 
//		(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f ) );
//	m_pFontWeight[0]->DrawText(WEIGHT_START_X+CITY_BASE_NPC_BOX_START_X+10, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, STRMSG_C_INTERFACE_0026, 0 );//"적재량"
//	m_pFontWeight[1]->DrawText(WEIGHT_START_X+CITY_BASE_NPC_BOX_START_X+FONTWEITGHT_X1, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, buff, 0 );
//	
//	// 적재량
//	wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1);	// 2006-06-23 by ispark, -1은 스피 아이템을 제외하는 것이다.
//	m_pFontWeight[2]->DrawText(WEIGHT_START_X+CITY_BASE_NPC_BOX_START_X+FONTWEITGHT_X2, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, buff, 0 );
//}

void CINFInvenExtend::SetAllIconInfo()
{
	FLOG( "CINFInvenExtend::SetAllIconInfo()" );

	// 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정
	BYTE nSelectType = ITEM_NOT_POS;
	UID64_t nTempUniqueNum = 0;
	POINT ptPos;

	//아이콘 정보를 초기화 하기 전에 지금 선택하고 있는 아이템의 정보를 얻어와서 저장한다.
	if(g_pGameMain)
	{
		if(g_pGameMain->m_stSelectItem.pSelectItem && (ITEM_INVEN_POS == g_pGameMain->m_stSelectItem.bySelectType))
		{
			nSelectType = g_pGameMain->m_stSelectItem.bySelectType;
			ptPos = g_pGameMain->m_stSelectItem.ptIcon;
			
			nTempUniqueNum = g_pGameMain->m_stSelectItem.pSelectItem->pItem->UniqueNumber;
			g_pGameMain->SetSelectItem(NULL, ptPos, ITEM_NOT_POS);
		}
		else if(g_pGameMain->m_nMultiSelCnt != 0)
		{
			//멀티 셀렉트는 그냥 클리어
			g_pGameMain->ClearMultiSelectItem();
		}
	}
	// end 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정

	InitInvenIconInfo();
	InitWearIconInfo();
	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.begin();
	while(it != g_pStoreData->m_mapItemWindowPosition.end())
	{
		CItemInfo* pItemInfo = it->second;
//		int nMinIndex = m_nInvenCurrentScroll*INVEN_X_NUMBER;
//		int nMaxIndex = (m_nInvenCurrentScroll*INVEN_X_NUMBER)+(INVEN_X_NUMBER*INVEN_Y_NUMBER);
		
		int nMinIndex = m_pINFInvenItem->GetScrollStep()*INVEN_X_NUMBER;
		int nMaxIndex = (m_pINFInvenItem->GetScrollStep()*INVEN_X_NUMBER)+(INVEN_X_NUMBER*INVEN_Y_NUMBER);

		if(pItemInfo->Wear>WEAR_NOT_ATTACHED && pItemInfo->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
		{
			SetSingleWearIconInfo(pItemInfo);
		}
//		else if( pItemInfo->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER && 
//			pItemInfo->ItemWindowIndex < POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER+INVEN_X_NUMBER*INVEN_Y_NUMBER)
		else if( pItemInfo->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET+nMinIndex &&
			pItemInfo->ItemWindowIndex < POS_ITEMWINDOW_OFFSET+nMaxIndex)
		{
			if(IS_COUNTABLE_ITEM(pItemInfo->Kind))
			{
				if(pItemInfo->CurrentCount>0)
					SetSingleInvenIconInfo(pItemInfo);
#ifdef _DEBUG
				else
				{
					DBGOUT("CINFInvenExtend::SetAllIconInfo(), Do not rendering : item[%d] count == 0 \n", pItemInfo->ItemNum);
				}
#endif
			}
			else
			{
				SetSingleInvenIconInfo(pItemInfo); 
//				if(!IsTradingItem(pItemInfo))
//				{
//					SetSingleInvenIconInfo(pItemInfo); 
//				}
			}
			if(pItemInfo->CurrentCount<0)
			{
				char buf[256];
				wsprintf(buf,"DB ERROR : item(%d) count = %d", pItemInfo->ItemNum, pItemInfo->CurrentCount);
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
			}
		}
		it++;
	}

	// 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정
	if(g_pGameMain && 
		(ITEM_INVEN_POS == nSelectType) && 
		nTempUniqueNum)
	{
		INVEN_DISPLAY_INFO* pTemp;
		pTemp = GetInvenDisplayInfoToUniqueNum(nTempUniqueNum);

		if(pTemp)
		{
			g_pGameMain->SetSelectItem(pTemp, ptPos, nSelectType);
		}
	}
	// end 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정

//	map<int, ITEM_GENERAL*>::iterator it = g_pStoreData->m_mapItemInInventory.begin();
//	while(it != g_pStoreData->m_mapItemInInventory.end())
//	{
//		ITEM_GENERAL* pItem = it->second;
//		if(pItem->Wear>WEAR_NOT_ATTACHED && pItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
//		{
//			SetSingleWearIconInfo(pItem);
//		}
///		else if( pItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER && 
//			pItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER+INVEN_X_NUMBER*INVEN_Y_NUMBER)
//		{
//			if(IS_COUNTABLE_ITEM(pItem->Kind))
//			{
//				if(pItem->CurrentCount>0)
//					SetSingleInvenIconInfo(pItem);
//#ifdef _DEBUG
//				else
//				{
//					DBGOUT("CINFInvenExtend::SetAllIconInfo(), Do not rendering : item[%d] count == 0 \n", pItem->ItemNum);
//				}
//#endif
//			}
//			else
//			{
//				if(!IsTradingItem(pItem)) {
//					SetSingleInvenIconInfo(pItem); }
//			}
//			if(pItem->CurrentCount<0)
//			{
//				char buf[256];
//				wsprintf(buf,"DB ERROR : item(%d) count = %d", pItem->ItemNum, pItem->CurrentCount);
//				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
//			}
//		}
//		it++;
//	}
}

BOOL CINFInvenExtend::IsTradingItem(ITEM_GENERAL* pItem)
{
	FLOG( "CINFInvenExtend::IsTradingItem(ITEM_GENERAL* pItem)" );
	if( ((CINFGameMain*)m_pParent)->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
	{
		vector<CItemInfo*>::iterator it = ((CINFGameMain*)m_pParent)->m_pTrade->m_vecMyStore.begin();
		while(it != ((CINFGameMain*)m_pParent)->m_pTrade->m_vecMyStore.end())
		{
			if((*it)->UniqueNumber == pItem->UniqueNumber)
			{
				return TRUE;
			}
			it++;
		}

	}
	return FALSE;
}

void CINFInvenExtend::InitInvenIconInfo()
{
	FLOG( "CINFInvenExtend::InitInvenIconInfo()" );
	for(int i=0;i<INVEN_NUMBER;i++)
	{
		SAFE_DELETE(m_pInvenDisplayInfo[i]);
	}
}

void CINFInvenExtend::InitWearIconInfo()
{
	FLOG( "CINFInvenExtend::InitWearIconInfo()" );

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수 변경.
	//for(int i=0;i<WEAR_ITEM_NUMBER;i++)
	for(int i=0;i<MAX_EQUIP_POS;i++)
	{
		SAFE_DELETE(m_pWearDisplayInfo[i]);
	}
}

//void CINFInvenExtend::SetSingleInvenIconInfo(ITEM_GENERAL* pItem)
//{
//	FLOG( "CINFInvenExtend::SetSingleInvenIconInfo(ITEM_GENERAL* pItem)" );
//	char buf[20];
//	wsprintf(buf, "%08d",pItem->ItemNum);
//	int index = (pItem->ItemWindowIndex-POS_ITEMWINDOW_OFFSET) - m_nInvenCurrentScroll*INVEN_X_NUMBER;// POS_ITEMWINDOW_OFFSET은 WindowIndex가 100부터시작임을 나타냄
//	if(index >= 0 && index < INVEN_NUMBER)
//	{
//		if(m_pInvenDisplayInfo[index])
//		{
//			DBGOUT("ERROR CINFInvenExtend::SetSingleInvenIconInfo(ITEM_GENERAL* pItem) : [index:%d]가 지워지지 않았다. \n", index);
//		}
//		SAFE_DELETE(m_pInvenDisplayInfo[index]);
//		m_pInvenDisplayInfo[index] = new INVEN_DISPLAY_INFO;
//		memset(m_pInvenDisplayInfo[index], 0x00, sizeof(INVEN_DISPLAY_INFO));
//		strcpy(m_pInvenDisplayInfo[index]->IconName, buf);
//		ITEM *itemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
//		if(itemInfo)
//			strcpy(m_pInvenDisplayInfo[index]->Name, itemInfo->ItemName);
//		m_pInvenDisplayInfo[index]->pItem = (ITEM_BASE*)pItem;
//	}
//}

void CINFInvenExtend::SetSingleInvenIconInfo(CItemInfo* pItemInfo)
{
	char buf[20];
	

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	
	//wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);			// 2005-08-22 by ispark
	ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemInfo->GetShapeItemNum() );

	int tempSourceIndex = NULL;
	if( pShapeItem )
	{
		tempSourceIndex = pShapeItem->SourceIndex;
	}
	else
	{
		tempSourceIndex = pItemInfo->ItemInfo->SourceIndex;
	}
	wsprintf(buf, "%08d", tempSourceIndex );

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


//	int index = (pItemInfo->ItemWindowIndex-POS_ITEMWINDOW_OFFSET) - m_nInvenCurrentScroll*INVEN_X_NUMBER;// POS_ITEMWINDOW_OFFSET은 WindowIndex가 100부터시작임을 나타냄
	int index = (pItemInfo->ItemWindowIndex-POS_ITEMWINDOW_OFFSET) 
		- (m_pINFInvenItem->GetScrollStep()*INVEN_X_NUMBER);// POS_ITEMWINDOW_OFFSET은 WindowIndex가 100부터시작임을 나타냄
	if(index >= 0 && index < INVEN_NUMBER)
	{
		if(m_pInvenDisplayInfo[index])
		{
//			DBGOUT("ERROR CINFInvenExtend::SetSingleInvenIconInfo(ITEM_GENERAL* pItem) : [index:%d]가 지워지지 않았다. \n", index);
		}
		SAFE_DELETE(m_pInvenDisplayInfo[index]);
		m_pInvenDisplayInfo[index] = new INVEN_DISPLAY_INFO;
		memset(m_pInvenDisplayInfo[index], 0x00, sizeof(INVEN_DISPLAY_INFO));
		strcpy(m_pInvenDisplayInfo[index]->IconName, buf);
		ITEM *pITEM = pItemInfo->GetItemInfo();
		if(pITEM && strlen(pITEM->ItemName) > 0)
			strcpy(m_pInvenDisplayInfo[index]->Name, pITEM->ItemName);
		m_pInvenDisplayInfo[index]->pItem = (ITEM_BASE*)pItemInfo;
	}
}
//void CINFInvenExtend::SetSingleWearIconInfo(ITEM_GENERAL* pItem)
//{
//	FLOG( "CINFInvenExtend::SetSingleWearIconInfo(ITEM_GENERAL* pItem)" );
//	char buf[20];
//	wsprintf(buf, "%08d",pItem->ItemNum);
//	if(pItem->Wear>WEAR_NOT_ATTACHED && pItem->ItemWindowIndex<POS_ITEMWINDOW_OFFSET)
//	{
//		SAFE_DELETE(m_pWearDisplayInfo[pItem->ItemWindowIndex]);
//		m_pWearDisplayInfo[pItem->ItemWindowIndex] = new INVEN_DISPLAY_INFO;
//		memset(m_pWearDisplayInfo[pItem->ItemWindowIndex], 0x00, sizeof(INVEN_DISPLAY_INFO));
//		strcpy(m_pWearDisplayInfo[pItem->ItemWindowIndex]->IconName, buf);
//		ITEM *itemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
//		if(itemInfo)
//			strcpy(m_pWearDisplayInfo[pItem->ItemWindowIndex]->Name, itemInfo->ItemName);
//		m_pWearDisplayInfo[pItem->ItemWindowIndex]->pItem = pItem;
//	}
//}

void CINFInvenExtend::SetSingleWearIconInfo(CItemInfo* pItemInfo)
{
	char buf[20];
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	//wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);			// 2005-08-22 by ispark
	ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemInfo->GetShapeItemNum() );
	int tempSourceIndex = NULL;
	if( pShapeItem )
	{
		tempSourceIndex = pShapeItem->SourceIndex;
	}
	else
	{
		tempSourceIndex = pItemInfo->ItemInfo->SourceIndex;
	}
	wsprintf(buf, "%08d", tempSourceIndex );

	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


	if(pItemInfo->Wear>WEAR_NOT_ATTACHED && pItemInfo->ItemWindowIndex<POS_ITEMWINDOW_OFFSET)
	{
		SAFE_DELETE(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]);
		m_pWearDisplayInfo[pItemInfo->ItemWindowIndex] = new INVEN_DISPLAY_INFO;
		memset(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex], 0x00, sizeof(INVEN_DISPLAY_INFO));
		strcpy(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]->IconName, buf);
		ITEM *pITEM = pItemInfo->GetItemInfo();
		if(pITEM)
			strcpy(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]->Name, pITEM->ItemName);
		m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]->pItem = (ITEM_BASE*)pItemInfo;
	}
}

// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
void CINFInvenExtend::SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y, BOOL bShowMyEq/*=TRUE*/)
{
	FLOG( "CINFInvenExtend::SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y)" );
	if(m_pCurrentItemInfo != pItemInfo) 
	{
		if(pItemInfo) 
		{
			CItemInfo* pItem = g_pStoreData->FindItemInInventoryByUniqueNumber(pItemInfo->pItem->UniqueNumber);
			// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
			//((CINFGameMain*)m_pParent)->SetItemInfo(pItemInfo->pItem->UniqueNumber, pItemInfo->pItem->ItemNum, x, y, 0); 
			//CINFGameMain*)m_pParent)->SetItemInfo(pItemInfo->pItem->UniqueNumber, pItemInfo->pItem->ItemNum, x, y, 0, bShowMyEq); 
		    ((CINFGameMain*)m_pParent)->SetItemInfo(pItemInfo->pItem->UniqueNumber, pItemInfo->pItem->ItemNum, x, y, 0, bShowMyEq,0,FALSE,TRUE); 		// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}	
		else 
		{
			((CINFGameMain*)m_pParent)->SetItemInfo( 0, 0, 0, 0, 0, TRUE); 
		}
		m_pCurrentItemInfo = pItemInfo;
	}	
}

// 2012-06-14 by isshin 아템미리보기
void CINFInvenExtend::SetEnemyItemInfo(CItemInfo *pItemInfo, int x, int y, BOOL bShowMyEq/*=TRUE*/)
{
	FLOG( "CINFInvenExtend::SetEnemyItemInfo(CItemInfo *pItemInfo, int x, int y)" );
		
	if(pItemInfo) 
	{
		CItemInfo* pItem = g_pStoreData->FindItemInEnemyByUniqueNumber(pItemInfo->UniqueNumber);			
		((CINFGameMain*)m_pParent)->SetEnemyItemInfo(pItemInfo->UniqueNumber, pItemInfo->ItemNum, x, y, 0, bShowMyEq); 
	}
	else
	{
		((CINFGameMain*)m_pParent)->SetEnemyItemInfo( 0, 0, 0, 0, 0, TRUE); 
	}			
}// end 2012-06-14 by isshin 아템미리보기

int CINFInvenExtend::WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFInvenExtend::WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	// 일반 상태와 동일하게 처리
	return WndProc(uMsg, wParam, lParam, TRUE);	

	// 인벤토리 캐릭터 로테이션
	//ProcessRotationUnitWnd(uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_RBUTTONDOWN:
		
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

/*			int nWindowPosY = POS_LEFT_WINDOW_Y;
			if(nWindowPosY < SIZE_ROLL_Y )
				nWindowPosY = SIZE_ROLL_Y;
			if(nWindowPosY > LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
				nWindowPosY = LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
*/			if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
				pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*INVEN_X_NUMBER &&
				pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
				pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*INVEN_Y_NUMBER)
			{
				// 2005-12-10 by ispark, 기본 6라인 넘어가지 않으면
//				if(GetScrollLine() > 0) 
//				{
//					if((int)wParam<0)
//					{
//						m_nInvenCurrentScroll++;
//						if(m_nInvenCurrentScroll > GetScrollLine())
//						{
////							m_nInvenCurrentScroll = GetScrollLine();
////							m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
//						}
//						else
//						{
////							m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
//						}
//
//						SetAllIconInfo();
//						SetItemInfo(NULL, 0, 0);
//					}
//					else
//					{
//						m_nInvenCurrentScroll--;
//						if(m_nInvenCurrentScroll < 0)
//						{
////							m_nInvenCurrentScroll = 0;
////							m_nInvenCurrentScrollPosition = 0;
//						}
//						else
//						{
////							m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
//						}
//
//						SetAllIconInfo();
//						SetItemInfo(NULL, 0, 0);
//					}
//				}
				return INF_MSGPROC_BREAK;
			}
			break;
		}
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;		

		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
						
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			

			CItemInfo* pSelectItem = NULL;
//			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
//			{
//				pSelectItem = (CItemInfo*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem); 
//			}
//			// 2006-07-26 by ispark, 인벤용 아이템이 아니라면 나간다.
//			if(((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType != ITEM_INVEN_POS)
//			{
//				break;
//			}
//			//////////////////////////////////////////////////////////////////////////
//			// 아이템 -> 장착창
//			if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROW_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROW_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_PROW_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_PROW_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_PROW);
//			}
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_PROWIN_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_PROWIN_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_PROWIN);
//			}
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X &&
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_PROWOUT_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_PROWOUT);
//			}
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_WINGIN_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_WINGIN_Y+INVEN_SLOT_SIZE)
//			{
//				// 2006-07-26 by ispark, 추가
//				SendChangeWearWindowPos(POS_WINGIN);
//			}
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_WINGOUT_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_WINGOUT);
//			}
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_CENTER_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_CENTER_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_CENTER_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_CENTER_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_CENTER);
//			}
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_REAR_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_REAR_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_REAR_Y && 
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_REAR_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_REAR);
//			}
//			//
//			//////////////////////////////////////////////////////////////////////////			
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_ATTACHMENT_Y &&
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_ATTACHMENT);
//			}
//			// 2006-03-30 by ispark
//			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PET_X && 
//				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PET_X+INVEN_SLOT_SIZE &&
//				pt.y>CITY_SHOP_INVEN_START_Y+POS_PET_Y &&
//				pt.y<CITY_SHOP_INVEN_START_Y+POS_PET_Y+INVEN_SLOT_SIZE)
//			{
//				SendChangeWearWindowPos(POS_PET);
//			}

//			else if(pSelectItem && 
//					pt.x>CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X && 
//					pt.x<CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*INVEN_X_NUMBER && 
//					pt.y>CITY_SHOP_INVEN_START_Y+INVEN_ITEM_SLOT_START_Y && 
//					pt.y<CITY_SHOP_INVEN_START_Y+INVEN_ITEM_SLOT_START_Y+INVEN_SLOT_INTERVAL*INVEN_Y_NUMBER)
//			{
//				int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
//				int j = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
//				int nWindowPosition = j*INVEN_X_NUMBER+i+m_nInvenCurrentScroll*INVEN_X_NUMBER+POS_ITEMWINDOW_OFFSET;
//				if(	nWindowPosition != pSelectItem->ItemWindowIndex &&
//					pSelectItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET &&
//					pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
//					i >= 0 && i < INVEN_X_NUMBER &&
//					pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE &&
//					pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
//					j >= 0 && j < INVEN_Y_NUMBER &&
//					pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE )
//				{	// 아이템 -> 아이템
//					if( pSelectItem->ItemWindowIndex < nWindowPosition )
//					{
//						INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = m_pInvenDisplayInfo[pSelectItem->ItemWindowIndex];
//						for(int i=pSelectItem->ItemWindowIndex+1; i<= nWindowPosition; i++)
//						{
//							CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
//							if(it != g_pStoreData->m_mapItemWindowPosition.end())
//							{
//								CItemInfo* pSwapItem = it->second;
//								pSwapItem->ItemWindowIndex = i-1;
//								g_pStoreData->m_mapItemWindowPosition[i-1] = pSwapItem;
//								SetSingleInvenIconInfo(pSwapItem);
//							} else {
//								pSelectItem->ItemWindowIndex = i-1;
//								g_pStoreData->m_mapItemWindowPosition[i-1] = pSelectItem;
//								SetAllIconInfo();
//								SetSelectItem(NULL);
//								return INF_MSGPROC_NORMAL;
//							}
//						}
//						pSelectItem->ItemWindowIndex = nWindowPosition;
//						g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
//						SetSingleInvenIconInfo(pSelectItem);
//						SetAllIconInfo();
//					}
//					else if( pSelectItem->ItemWindowIndex > nWindowPosition )
//					{
//						INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = m_pInvenDisplayInfo[pSelectItem->ItemWindowIndex];
//						for(int i=pSelectItem->ItemWindowIndex-1; i >= nWindowPosition; i--)
//						{
//							CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
//							if(it != g_pStoreData->m_mapItemWindowPosition.end())
//							{
//								CItemInfo* pSwapItem = it->second;
//								pSwapItem->ItemWindowIndex = i+1;
//								g_pStoreData->m_mapItemWindowPosition[i+1] = pSwapItem;
//								SetSingleInvenIconInfo(pSwapItem);
//							} else {
//								DBGOUT("ERROR : CINFInvenExtend::WndProc() Item List crashed!!!!!\n");
//								SetSelectItem(NULL);
//								return INF_MSGPROC_NORMAL;// 아이템 리스트가 깨진경우
//							}
//						}
//						pSelectItem->ItemWindowIndex = nWindowPosition;
//						g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
//						SetSingleInvenIconInfo(pSelectItem);
//						SetAllIconInfo();
//					}
//				}
//				// 장착창 -> 아이템
//				else if(pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
//				{
//					// 2004-12-10 by jschoi
//					if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
//					{
//						// 칼라 상점내에서 아이템 교체 금지
//						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
//						break;
//					}
//					// 2006-08-02 by ispark, 개인상점에서 아이템 교체 금지
//					if(g_pInterface->m_pBazaarShop)
//					{
//						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0000,COLOR_ERROR);
//						break;
//					}
//					if(IsAbleReleaseItem(pSelectItem,pSelectItem->ItemWindowIndex))
//					{
//						// send item windowNumber (socket)
//						MSG_FC_ITEM_CHANGE_WINDOW_POSITION* pMsg;
//						char buffer[SIZE_MAX_PACKET];
//						*(MessageType_t*)buffer = T_FC_ITEM_CHANGE_WINDOW_POSITION;
//						pMsg = (MSG_FC_ITEM_CHANGE_WINDOW_POSITION*)(buffer+SIZE_FIELD_TYPE_HEADER);
//						pMsg->CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
//						pMsg->FromItemUniqueNumber = pSelectItem->UniqueNumber;
//						pMsg->FromItemWindowIndex = pSelectItem->ItemWindowIndex;
//						pMsg->ToItemUniqueNumber = 0;
//						// 빈자리 번호로 세팅한다.
//						int count=0;
//						for(int i=0;i<WEAR_ITEM_NUMBER;i++)
//						{
//							if(m_pWearDisplayInfo[i])
//								count++;
//						}
//						i=POS_ITEMWINDOW_OFFSET+g_pStoreData->m_mapItemWindowPosition.size();//-count-1;
//						pMsg->ToItemWindowIndex = i;
//						// 상점에서 장착아이템을 인벤토리로 내릴 때 해당 스킬을 여기서 해제해 줌..서버 요청
//						if(pSelectItem)
//						{
//							g_pShuttleChild->m_pSkill->DeleteSkillFromWearItem(pSelectItem->Kind);
//						}
//						g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
////						g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark, 메세지 응답을 기다린다.
//					}
//					else
//					{
//						char buf[128];
//						wsprintf(buf,STRMSG_C_ITEM_0007);//"요구 스탯으로 인하여 해당 아이템을 해제 할 수 없습니다."
//						g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
//					}
//
//				}
//			}
			if(pSelectItem && 
					pt.x>CITY_BASE_NPC_BOX_START_X+INVEN_GARBAGE_START_X && 
					pt.x<CITY_BASE_NPC_BOX_START_X+INVEN_GARBAGE_START_X+INVEN_GARBAGE_SIZE &&
					pt.y>CITY_SHOP_INVEN_START_Y+INVEN_GARBAGE_START_Y && 
					pt.y<CITY_SHOP_INVEN_START_Y+INVEN_GARBAGE_START_Y + INVEN_GARBAGE_SIZE
					&& g_pD3dApp->GetMultiSelectItem() <= 0)		// 2007-03-02 by bhsohn 다중 선택 추가 보안)
			{

				if( !((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE) &&
					!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE_NUM))
				{
					char buf[256];
					ITEM *pITEM = pSelectItem->GetItemInfo();
					if(pITEM)
					{
						if(pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
						{
							((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(STRMSG_C_ITEM_0008, _MESSAGE);//"장착된 아이템은 버릴 수 없습니다."
							pSelectItem = NULL;
						}
						else
						{
							if(IS_COUNTABLE_ITEM(pSelectItem->Kind) && pSelectItem->CurrentCount>1)
							{
								wsprintf(buf, STRMSG_C_ITEM_0009, pITEM->ItemName);//"아이템 %s 몇개를 버리시겠습니까?"
								((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(buf,
									_Q_ITEM_DELETE_NUM, (DWORD)pSelectItem, pSelectItem->CurrentCount);
							}
							else
							{
								wsprintf(buf, STRMSG_C_ITEM_0010, pITEM->ItemName);//"아이템 %s 를(을)  버리시겠습니까?"
								((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE);
							}
							// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
							//m_pDeleteItem = pSelectItem;
							SetDeleteItemInfo(pSelectItem);
						}
					}
				}
			}			
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if(((CINFGameMain*)m_pParent)->m_pTrade->m_bTrading)
				return INF_MSGPROC_NORMAL;
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
			if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
				i >= 0 && i < INVEN_X_NUMBER &&
				pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
			{
				int j = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				if( pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
					j >= 0 && j < INVEN_Y_NUMBER &&
					pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
				{
					// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
					//if(!m_pDeleteItem && m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i])
					if(0 == m_struDeleteItem.UniqueNumber && m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i])
					{
						// set tooltip
						CItemInfo* pItemInfo = (CItemInfo*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem;
						ITEM *pITEM = pItemInfo->GetItemInfo();
//						ITEM *item = g_pDatabase->GetServerItemInfo( pItem->ItemNum);
						if(pITEM)
						{
// 2008-06-03 by dgwoo 상점이 열려있는 상태에서 더블클릭으로 아이템 교체를 금지.
//							if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
//							{
//								// 칼라 상점내에서 아이템 교체 금지
//								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
//							}
//							// 2006-08-03 by ispark, 개인상점에서는 아이템 사용 금지
//							else if(g_pInterface->m_pBazaarShop)
//							{
//								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0001,COLOR_ERROR);
//							}
							if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100,COLOR_ERROR);
							}
							else
							{
								switch(pITEM->Position)
								{
								case POS_INVALID_POSITION:
									{
										// 2007-10-17 by bhsohn 다중 선택후 아이템 사용버그수정
										// 다중 선택 항목 초기화
										g_pD3dApp->DelMultiItemList(TRUE);
										if(g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_STORE_PUT_COUNTABLE_ITEM ))
										{
											break;
										}
										// end 2007-10-17 by bhsohn 다중 선택후 아이템 사용버그수정
		
										SendUseItem((ITEM_BASE*)pItemInfo);
									}
									break;
								case POS_PROWIN:
								case POS_PROWOUT:
								case POS_WINGIN:
								case POS_WINGOUT:
								case POS_PROW:
								case POS_CENTER:
								case POS_REAR:
								case POS_ACCESSORY_UNLIMITED:
								case POS_ACCESSORY_TIME_LIMIT :	// 2006-03-31 by ispark
								case POS_PET:					// 2010-06-15 by shcho&hslee 펫시스템
									{
										SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
										SendChangeWearWindowPos(pITEM->Position);
									}
									break;
								default:
									{
										char buf[128];
										wsprintf(buf,STRMSG_C_ITEM_0011,pItemInfo->ItemNum);//"item [ %08d ] 는 착용 할 수 없는 아이템입니다."
										g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
									}
								}
							}
						}
						SetSelectItem(NULL);
						((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					}
				}
			}
		}
		break;
	// 2007-02-12 by bhsohn Item 다중 선택 처리
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_CONTROL:
				{
					g_pD3dApp->OnCtrlBtnClick(TRUE);					
				}
				break;
			}
		}
		break;
	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_CONTROL:
				{					
					g_pD3dApp->OnCtrlBtnClick(FALSE);
				}
				break;
			}		
		}
		break;
		// end 2007-02-12 by bhsohn Item 다중 선택 처리
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFInvenExtend::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven/*= FALSE*/)
{
	FLOG( "CINFInvenExtend::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );	

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nCnt = 0;	
	for(nCnt = 0;nCnt < m_vecWndOrder.size();nCnt++)
	{
		int nWndOrderTmp = m_vecWndOrder[nCnt];
		switch(nWndOrderTmp)
		{
		case INVEN_ITEM_WND:
			{
				if(m_pINFInvenItem && (INF_MSGPROC_BREAK == m_pINFInvenItem->WndProc(uMsg, wParam, lParam, bShowInven)))
				{
					// 아이템 창
					return INF_MSGPROC_BREAK;
				}
			}
			break;
		case INVEN_EQ_WND:
			{
				if(m_pINFInvenEquip && (INF_MSGPROC_BREAK == m_pINFInvenEquip->WndProc(uMsg, wParam, lParam, bShowInven)))
				{
					// 장착 창
					return INF_MSGPROC_BREAK;
				}
			}
			break;
		}
	}	
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리	
	//if( m_pINFInvenItem && (INF_MSGPROC_BREAK == m_pINFInvenItem->WndProc(uMsg, wParam, lParam, bShowInven)) ||
#else
	m_pINFInvenItem->WndProc(uMsg, wParam, lParam, bShowInven);
	if(m_pINFInvenEquip && INF_MSGPROC_BREAK == m_pINFInvenEquip->WndProc(uMsg, wParam, lParam, bShowInven))
	{
		return INF_MSGPROC_BREAK;
	}																										
#endif
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
		}
		break;		
	case WM_MOUSEMOVE:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFInvenExtend::DeleteSelectItem(int count)
{
	FLOG( "CINFInvenExtend::DeleteSelectItem(int count)" );
	// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
//	if(m_pDeleteItem )//&& m_pDeleteItem->Count >= count)
//	{
//		if(IS_COUNTABLE_ITEM(m_pDeleteItem->Kind) && m_pDeleteItem->CurrentCount < count)
//		{
//			m_pDeleteItem = NULL;
//			return;
//		}		
//		MSG_FC_ITEM_THROW_AWAY_ITEM sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_FC_ITEM_THROW_AWAY_ITEM;
//		sMsg.Amount = count;// 갯수 세팅 필요
//		sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
//		sMsg.ItemUniqueNumber = m_pDeleteItem->UniqueNumber;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//	}
//	m_pDeleteItem = NULL;
	
	if(0 == m_struDeleteItem.UniqueNumber)
	{
		return;
	}
	
	CItemInfo* pDeleteItem = g_pStoreData->FindItemInInventoryByUniqueNumber( m_struDeleteItem.UniqueNumber );
	if(NULL == pDeleteItem)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_ERROR_0025,COLOR_ERROR);// "해당 아이템이 없습니다."
		InitDeleteItemInfo();
		return;
	}
				
	if(IS_COUNTABLE_ITEM(pDeleteItem->Kind) && pDeleteItem->CurrentCount < count)
	{
		//m_pDeleteItem = NULL;
		InitDeleteItemInfo();
			return;
	}		
	MSG_FC_ITEM_THROW_AWAY_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_ITEM_THROW_AWAY_ITEM;
	sMsg.Amount = count;// 갯수 세팅 필요
	sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.ItemUniqueNumber = pDeleteItem->UniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		
	InitDeleteItemInfo();
	// end 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
}

void CINFInvenExtend::SendUseItem(ITEM_BASE* pItem)
{
	FLOG( "CINFInvenExtend::SendUseItem(ITEM_BASE* pItem)" );
	switch(pItem->Kind)
	{
	case ITEMKIND_ENERGY:
		{
			bool bSend = false;
			ITEM * pIteminfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
			CAppEffectData * pEff = NULL; 
			// 2009-04-21 by bhsohn 아이템 DesParam추가
			//switch(pIteminfo->DestParameter1)
			switch(pIteminfo->ArrDestParameter[0])
			{
			case DES_CASH_STAT_ALL_INITIALIZE:
				{// 전체 스탯 초기화
					MSG_FC_ITEM_USE_ENERGY sMsg;
					sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENERGY, (char*)&sMsg, sizeof(sMsg) );
					((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					bSend = true;
				}
				break;
			case DES_CASH_STAT_PART_INITIALIZE:
				{// 부분 스탯 초기화
					// 2005-03-28 by jschoi
					bSend = true;
					// 2006-04-17 by ispark, 같은 메세지가 있는지 확인해서 처리한다.
					if(g_pGameMain->m_pInfWindow->SearchMsgBox(_Q_PART_STAT_RESET))
						break;
					g_pGameMain->m_pInfWindow->RadioClear();
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0001);//"공격"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_050517_0001);//"방어"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0003);//"연료"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0004);//"정신"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0005);//"회피"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0006);//"쉴드"

					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_STAT_0007,_Q_PART_STAT_RESET,0,0,0,pItem->UniqueNumber);//"초기화를 원하는 스탯을 선택하세요."

				}
				break;
			case DES_GRADUAL_EP_UP:		// 에너지류, 현재 EP 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
			case DES_IN_TIME_EP_UP:		// 에너지류, 현재 EP 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
			case DES_IMMEDIATE_EP_UP:	// 에너지류, 현재 EP 즉시 VALUE만큼 올려줌
				{
//					if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.EP > g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CurrentEP)
//					{
//						// 현재 사운드 없음
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_USEITEM_REPAIR_HP, g_pShuttleChild->m_vPos, FALSE);
//						pEff = new CAppEffectData(g_pD3dApp->m_pShuttleChild,RC_EFF_USEITEM_REPAIR_EP,D3DXVECTOR3(0,0,0));
//						pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
//						MSG_FC_CHARACTER_SHOW_EFFECT sMsg;
//						sMsg.EffectIndex = RC_EFF_USEITEM_REPAIR_EP;
//						g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SHOW_EFFECT, (char*)&sMsg, sizeof(sMsg) );
//					}
//					else
//					{
//						bSend = true;
//					}					
				}
				break;
			case DES_GRADUAL_SP_UP:		// 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
			case DES_IN_TIME_SP_UP:		// 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
			case DES_IMMEDIATE_SP_UP:	// 에너지류, 현재 SP를 즉시 VALUE만큼 올려줌
				break;
			case DES_GRADUAL_HP_UP:		// 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel
			case DES_IN_TIME_HP_UP:		// 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
			case DES_IMMEDIATE_HP_UP:	// 에너지류, 현재 HP를 즉시 VALUE만큼 올려줌
				{
//					if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.HP > g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CurrentHP)
//					{
//						int nEffNum = RC_EFF_USEITEM_REPAIR_HP;
//						switch(pItem->ItemNum)
//						{
//						case  ITEM_NUM_APLUS_GRADE_HP_KIT:		nEffNum = RC_EFF_USEITEM_REPAIR_HPA;	break;
//						case  ITEM_NUM_S_GRADE_HP_KIT:			nEffNum = RC_EFF_USEITEM_REPAIR_HPS;	break;
//						}
//
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_USEITEM_REPAIR_HP, g_pShuttleChild->m_vPos, FALSE);
//						pEff = new CAppEffectData(g_pD3dApp->m_pShuttleChild,nEffNum,D3DXVECTOR3(0,0,0));
//						pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
//						MSG_FC_CHARACTER_SHOW_EFFECT sMsg;
//						sMsg.EffectIndex = nEffNum;
//						g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SHOW_EFFECT, (char*)&sMsg, sizeof(sMsg) );
//					}
//					else
//					{
//						bSend = true;
//					}
				}
				break;
			case DES_GRADUAL_DP_UP:		// 에너지류, 현재 DP 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
			case DES_IN_TIME_DP_UP:		// 에너지류, 현재 DP 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
			case DES_IMMEDIATE_DP_UP:	// 에너지류, 현재 DP 즉시 VALUE만큼 올려줌
				{
//					if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.DP > g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CurrentDP)
//					{
//						int nEffNum = RC_EFF_USEITEM_REPAIR_DP;
//						switch(pItem->ItemNum)
//						{
//						case  ITEM_NUM_APLUS_GRADE_DP_KIT:		nEffNum = RC_EFF_USEITEM_REPAIR_DPA;	break;
//						case  ITEM_NUM_S_GRADE_DP_KIT:			nEffNum = RC_EFF_USEITEM_REPAIR_DPS;	break;
//						}
//
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_USEITEM_REPAIR_DP, g_pShuttleChild->m_vPos, FALSE);
//						pEff = new CAppEffectData(g_pD3dApp->m_pShuttleChild,nEffNum,D3DXVECTOR3(0,0,0));
//						pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
//						MSG_FC_CHARACTER_SHOW_EFFECT sMsg;
//						sMsg.EffectIndex = nEffNum;
//						g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SHOW_EFFECT, (char*)&sMsg, sizeof(sMsg) );
//					}
//					else
//					{
//						bSend = true;
//					}
				}
				break;
			// 2007-12-05 by bhsohn 포탈 사용시, 편대 비행 해제
			case DES_WARP:
			case DES_WARP_OUTPOST:
				{				
					if(g_pShuttleChild 						
						&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
					{
						if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER )
						{
							// 2008-06-17 by bhsohn 편대 관련 처리
							// 편대장은 자유비행으로 변경 
//							g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(FLIGHT_FORM_NONE);
//							
//							PARTYINFO partyTmp = g_pShuttleChild->m_pClientParty->GetPartyInfo();
//							partyTmp.bFormationFlyingType = FLIGHT_FORM_NONE;
//							g_pShuttleChild->m_pClientParty->SetPartyInfo(partyTmp);

							// 2008-06-17 by bhsohn 편대 관련 처리
							// 강제로 자유 비행으로
							g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
							// end 2008-06-17 by bhsohn 편대 관련 처리

						}
						else
						{
							// 2007-12-27 by bhsohn 편대 비행시 편대원이 포탈사용하고 다시 왔을 시 이상동작하는 버그 수정
							// 편대 비행 해제
							g_pShuttleChild->CheckFormationMoveDelete();
						}
						
					}					
				}
				break;
			// end 2007-12-05 by bhsohn 포탈 사용시, 편대 비행 해제
			}
			if(!bSend)
			{
				MSG_FC_ITEM_USE_ENERGY sMsg;
				sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
				sMsg.ItemUniqueNumber = pItem->UniqueNumber;
				g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENERGY, (char*)&sMsg, sizeof(sMsg) );
				((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			}
		}
		break;
 //2011-10-06 by jhahn 파트너 성장형 시스템
	case ITEMKIND_PET_SOCKET_ITEM:
		{	
			ITEM* teml = g_pGameMain->m_pInfSkill->FindItemIcon(pItem->ItemNum);

 			if (teml)
 			{
 				MSG_FC_ITEM_CANCEL_PET_SOCKET_ITEM sMsg;				
 				sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
 				sMsg.ItemUniqueNumber = pItem->UniqueNumber;
 				g_pFieldWinSocket->SendMsg( T_FC_ITEM_CANCEL_PET_SOCKET_ITEM, (char*)&sMsg, sizeof(sMsg) );
 				g_pShuttleChild->GetPetManager()->SetSocketOnOff( FALSE); 				
 			}
 			else
 			{
 				MSG_FC_ITEM_USE_PET_SOCKET_ITEM sMsg;				
 				sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
 				sMsg.ItemUniqueNumber = pItem->UniqueNumber;
 				g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_PET_SOCKET_ITEM, (char*)&sMsg, sizeof(sMsg) );
 				g_pShuttleChild->GetPetManager()->SetSocketOnOff( TRUE);
 			}			
		}  

		break;
//end 2011-10-06 by jhahn 파트너 성장형 시스템
	case ITEMKIND_CARD:
		{
			ITEM * pIteminfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
			// 2006-02-10 by ispark, 킬마크 가드일 경우 그냥 리턴
			// 2006-08-14 by ispark, 삭제
			// 2006-08-22 by ispark, 베트남 본 서버는 제외
			// 2007-10-04 by bhsohn 킬마크 경험치 관련 처리
//#if defined(_DEBUG) || defined(LANGUAGE_KOREA) || defined(LANGUAGE_ENGLISH) || (defined(LANGUAGE_VIETNAM) && defined(_TEST_SERVER)) || defined(LANGUAGE_CHINA)
//#else
//			if(COMPARE_BIT_FLAG(pIteminfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
//			{
//				return;
//			}
//
//#endif
			if(NULL == pIteminfo)
			{
				return;
			}
			// end 2007-10-04 by bhsohn 킬마크 경험치 관련 처리
			// 2009-04-21 by bhsohn 아이템 DesParam추가
			//switch(pIteminfo->DestParameter1)
			switch(pIteminfo->ArrDestParameter[0])
			{	
			case DES_CASH_NORMAL_RESTORE:	// 일반 부활 카드
				{
					// 나에게 부활 카드 사용한다.
					MSG_FC_ITEM_USE_CARDITEM_RESTORE sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_RESTORE, (char*)&sMsg, sizeof(sMsg) );
				}
				break;
			case DES_CASH_SUPER_RESTORE:	// 슈퍼 부활 카드
				{
					// 나를 포함한 누군가에게 부활 카드를 사용한다.
					if(g_pGameMain && g_pGameMain->m_pInfWindow)
					{
						g_pGameMain->m_bChangeMousePoint = TRUE;
						g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CARD_0006, _Q_SUPER_RESTORE,0,0,0,pItem->UniqueNumber );//"부활을 원하는 유닛을 입력하시오."
					}			
				}
				break;
			case DES_CASH_GUILD_ALL_MEMBERS_SUMMON:	// 여단원 전체 소환 카드
				{
					// 여단원 전체 소환
					MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_GUILDSUMMON, (char*)&sMsg, sizeof(sMsg) );
				}
				break;
			case DES_CASH_GUILD_MEMBER_SUMMON:		// 여단원 한 명 소환 카드
				{
					// 선택된 여단원 소환	
					if(g_pGameMain && g_pGameMain->m_pInfWindow)
					{
						// 2008-04-04 by bhsohn Ep3 커뮤니티 창
						//g_pGameMain->m_pCommunity->SetCommunityType(2);
						//g_pGameMain->m_pCommunity->SetCommunityType(COMMUNITY_GUILD);						
						
						//g_pGameMain->LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
						//g_pGameMain->OnClickCommunity();
						//g_pGameMain->m_bChangeMousePoint = TRUE;

						g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CARD_0007, _Q_MEMBER_SUMMON,0,0,0,pItem->UniqueNumber );//"소환을 원하는 유닛을 입력하시오."
					}
				}
				break;
			case DES_CASH_GUILD:
				{
					if(g_pGameMain && g_pGameMain->m_pInfWindow)
					{
						// 2008-04-04 by bhsohn Ep3 커뮤니티 창
						//g_pGameMain->m_pCommunity->SetCommunityType(2);
						//g_pGameMain->m_pCommunity->SetCommunityType(COMMUNITY_GUILD);

						
						//g_pGameMain->LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
						//g_pGameMain->OnClickCommunity();
						//g_pGameMain->m_bChangeMousePoint = TRUE;

						MSG_FC_ITEM_USE_CARDITEM_GUILD sMsg;
						sMsg.ItemUniqueNumber = pItem->UniqueNumber;
						g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_GUILD, (char*)&sMsg, sizeof(sMsg) );
					}
				}
				break;
			case DES_CASH_MONSTER_SUMMON:
				{
					MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON sMsg;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON, (char*)&sMsg, sizeof(sMsg) );
				}
				break;
			case DES_CASH_CHANGE_CHARACTERNAME:
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CARD_0008, _Q_USEITEM_NAME_CHANGE,0,0,0, pItem->UniqueNumber);	//"새캐릭터 이름을 입력해 주십시오(\\r변경시 로그오프 됩니다\\r)"
				}
				break;
				// 2008-12-30 by bhsohn 지도자 채팅 제한 카드 기획안
			case DES_CHAT_BLOCK:
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_081230_0209, _Q_USEITEM_LEADER_BAN_CHAT,0,0,0, pItem->UniqueNumber);	//"새캐릭터 이름을 입력해 주십시오(\\r변경시 로그오프 됩니다\\r)"
				}
				break;
				// end 2008-12-30 by bhsohn 지도자 채팅 제한 카드 기획안
			case DES_KILLMARK_EXP:
				{
					char chMsg[256] = {0,};					
					// 2008-12-30 by bhsohn 킬마크 관련 스트링 변경
					//wsprintf(chMsg, STRMSG_C_060814_0001, pItem->ItemInfo->ItemName);
					wsprintf(chMsg, STRMSG_C_081230_0207, pItem->ItemInfo->ItemName);

					g_pGameMain->m_pInfWindow->AddMsgBox( chMsg, _Q_USEITEM_KILL_MARK,0,0,0, pItem->UniqueNumber);	// "%s 킬 마크를 EXP로 변환하겠습니까?"
				}
				break;
				// 2007-08-07 by bhsohn 스피커 아이템 추가
			case DES_CHAT_ALL_INFLUENCE:
				{	
					// 스피커 아이템 창 띠움
					DoModalChatAllInfluence(pItem->UniqueNumber);
				}
				break;
				// end 2007-08-07 by bhsohn 스피커 아이템 추가
			// 2007-11-21 by dgwoo 캐릭터 변경 카드.
			case DES_CASH_CHANGE_PILOTFACE:
				{
					if(g_pD3dApp->m_bCharacter)
					{
						g_pGameMain->m_pChangeCharactor->SetActive(TRUE,pItem->UniqueNumber);
					}
					else
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_071121_0100,COLOR_SYSTEM);//"해당 아이템은 캐릭터 상태에서만 사용할 수 있습니다."						
					}
				}
				break;
			// 2010. 10. 15 by jskim 시즈모드시 필드창고 아이템 사용 되는 버그 수정
			case DES_FIELD_STORE:
				{
					if(g_pShuttleChild->m_bAttackMode == _SIEGE)
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090925_0402,COLOR_SYSTEM);//"필드 창고를 이용 할 수 없습니다."
						break;
					}
				}
			// end 2010. 10. 15 by jskim 시즈모드시 필드창고 아이템 사용 되는 버그 수정
			default:
				{
					// 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
					if( m_bShopConcurrent + CINFInvenExtend::SHOP_CONCURRENT_DELAY < timeGetTime() )
					{
						m_bShopConcurrent = timeGetTime();

						MSG_FC_ITEM_USE_ITEM sMsg;
						memset(&sMsg,0x00,sizeof(sMsg));
						char buffer[SIZE_MAX_PACKET];
						sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
						sMsg.ItemUniqueNumber = pItem->UniqueNumber;
						int nType = T_FC_ITEM_USE_ITEM;
						memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
						memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
						((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					}
					// end 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
				}
				break;
			}
		}
		break;
	case ITEMKIND_INFLUENCE_BUFF:
		{
			// 2006-04-24 by ispark
			MSG_FC_ITEM_USE_INFLUENCE_BUFF sMsg;
			sMsg.ItemUniqueNumber = pItem->UniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_INFLUENCE_BUFF, (char*)&sMsg, sizeof(sMsg) );
		}
		break;
	case ITEMKIND_INFLUENCE_GAMEEVENT:
		{
			// 2006-04-24 by ispark
			MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT sMsg;
			sMsg.ItemUniqueNumber = pItem->UniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_INFLUENCE_GAMEEVENT, (char*)&sMsg, sizeof(sMsg) );
		}
		break;
	case ITEMKIND_BULLET:
		{
//			if(g_pD3dApp->m_pShuttleChild->m_pItemPrimary || g_pD3dApp->m_pShuttleChild->m_pItemSecondary)
			if(g_pShuttleChild->m_pPrimaryWeapon || g_pShuttleChild->m_pSecondaryWeapon)
			{
				bool bSend = false;
				// 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정
				BOOL bPrimaryFullWeapon, bSecondaryFullWeapon;
				bPrimaryFullWeapon = bSecondaryFullWeapon = FALSE;
				// end 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정

				CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_PROWOUT);
				if(it != g_pStoreData->m_mapItemWindowPosition.end())
				{
					ITEM * p = it->second->GetItemInfo();//g_pDatabase->GetServerItemInfo(it->second->ItemNum);
					ITEM * p2 = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
					// 2007-08-07 by dgwoo 1,2형 모두 채우는 총알 아이템 RECHARGE_BULLET_ALL_CALIBER == p2->Caliber
					if( p && p2 && (RECHARGE_BULLET_ALL_CALIBER == p2->Caliber || p->Caliber == p2->Caliber))
					{
						// 2007-10-15 by bhsohn 총알 아이템 추가 처리
						int nMaxCharge = p->Charging;
						nMaxCharge = g_pGameMain->GetMaxBulletItem(p->Charging);

						//if( it->second->CurrentCount == p->Charging )
						if( it->second->CurrentCount == nMaxCharge )
						{
							// 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정
							bPrimaryFullWeapon = TRUE;
							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0012,COLOR_SYSTEM);//"이미 총알이 장전되어 있습니다."
							//break;
						}
						else
						{
							bSend = true;
						}
					}
					//					if(p && p2 && p->Caliber == p2->Caliber && it->second->CurrentCount != p->Charging)
					//					{// 구경으로 검색
					//						bSend = true;
					//					}
				}
				//if(!bSend)
				{
					it = g_pStoreData->m_mapItemWindowPosition.find(POS_WINGOUT);
					if(it != g_pStoreData->m_mapItemWindowPosition.end())
					{
						ITEM * p = it->second->GetItemInfo();//g_pDatabase->GetServerItemInfo(it->second->ItemNum);
						ITEM * p2 = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
						// 2007-08-07 by dgwoo 1,2형 모두 채우는 총알 아이템 RECHARGE_BULLET_ALL_CALIBER == p2->Caliber
						if( p && p2 && (RECHARGE_BULLET_ALL_CALIBER == p2->Caliber || p->Caliber == p2->Caliber))
						{
							// 2007-10-15 by bhsohn 총알 아이템 추가 처리
							int nMaxCharge = p->Charging;
							nMaxCharge = g_pGameMain->GetMaxBulletItem(p->Charging);

							//if( it->second->CurrentCount == p->Charging )
							if( it->second->CurrentCount == nMaxCharge )
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0013,COLOR_SYSTEM);//"이미 탄두가 장전되어 있습니다."
								// 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정
								bSecondaryFullWeapon = TRUE;
								//break;
							}
							else
							{
								bSend = true;
							}
	//						if(p && p2 && p->Caliber == p2->Caliber && it->second->CurrentCount != p->Charging)
	//						{// 구경으로 검색
	//							bSend = true;
	//						}
						}
					}
				}

				// 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정
				bSend = true; 
				if((TRUE == bPrimaryFullWeapon)
					&&(TRUE == bSecondaryFullWeapon))
				{
					bSend = false;					
				}
				// end 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정
				
				if(bSend)
				{
					MSG_FC_ITEM_USE_ITEM sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					char buffer[SIZE_MAX_PACKET];
					sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					int nType = T_FC_ITEM_USE_ITEM;
					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
					g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
					((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
				}
				// 2007-08-07 by bhsohn 총알 아이템 1형 찼을 시, 2형 충전 안되는 버그 수정
				else if((TRUE != bPrimaryFullWeapon)&&(TRUE != bSecondaryFullWeapon))
				{
//					g_pD3dApp->m_pChat->CreateChatChild("해당 아이템을 장전할 아이템이 없거나, 장전공간이 부족합니다.",COLOR_SYSTEM);
//					g_pD3dApp->m_pChat->CreateChatChild("아이템 장전을 실패하였습니다.",COLOR_SYSTEM);
//					g_pD3dApp->m_pChat->CreateChatChild("이미 장착되어 있습니다.",COLOR_SYSTEM);
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0014,COLOR_SYSTEM);//"이 탄환을 장전할 무기가 없습니다."
				}
			}
			else
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0015,COLOR_SYSTEM);//"장착된 아이템이 없습니다."
//				g_pD3dApp->m_pChat->CreateChatChild("아이템 장전을 실패하였습니다.",COLOR_SYSTEM);
			}
		}
		break;
	case ITEMKIND_ENCHANT:
	case ITEMKIND_GAMBLE:
		{
//			if(!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_USE_ENCHANT_ITEM_CARD))
//			{
//				((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox("재련할 아이템을 클릭하시오",_Q_USE_ENCHANT_ITEM_CARD);
//				m_bEnchantState = TRUE;
//				m_pEnchantItem = (CItemInfo *)pItem;
//			}
		}
		break;
	case ITEMKIND_TANK:
		break;
//  2006-08-10 by dgwoo 판도라의 상자 이벤트
	case ITEMKIND_RANDOMBOX:
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_USE_RANDOMBOX,T_FC_ITEM_USE_RANDOMBOX,pSRandombox,Sendbuf);
			pSRandombox->ItemUID	= pItem->UniqueNumber;
			g_pD3dApp->m_pFieldWinSocket->Write(Sendbuf, MSG_SIZE(MSG_FC_ITEM_USE_RANDOMBOX));
			g_pD3dApp->m_bRequestEnable = FALSE;			// 응답 메세지 기다리기
		}
		break;
// 2006-10-02 by ispark, 스킬보조아이템
	case ITEMKIND_SKILL_SUPPORT_ITEM:
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM, T_FC_ITEM_USE_SKILL_SUPPORT_ITEM, pSkillSupportItem, Sendbuf);
			pSkillSupportItem->ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
			pSkillSupportItem->ItemUniqueNumber = pItem->UniqueNumber;
			g_pD3dApp->m_pFieldWinSocket->Write(Sendbuf, MSG_SIZE(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM));
		}
		break;

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	case ITEMKIND_ACCESSORY_UNLIMITED:
		{
			MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM msg;
			msg.InvokingWearItemUID	= pItem->UniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_INVOKING_WEAR_ITEM, (char*)(&msg), sizeof( MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM ) );
		}
		break;
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	default:
		{
			char buf[128];
			wsprintf(buf,STRMSG_C_ITEM_0016,pItem->ItemInfo->ItemName);//"아이템 [ %s ]는 사용 할 수 없는 아이템입니다."
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
		break;
	}
}

void CINFInvenExtend::SendChangeWearWindowPos(int nWindowPosition)
{
	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
	if(nSelItemCnt > 0)
	{
		return;
	}
	// end 2007-03-02 by bhsohn 다중 선택 추가 보안


	FLOG( "CINFInvenExtend::SendChangeWearWindowPos(int nWindowPosition)" );
	if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
	{
		// 칼라 상점내에서 아이템 교체 금지
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
		return;
	}
	// 2006-08-02 by ispark, 개인상점에서 아이템 교체 금지
	if(g_pInterface->m_pBazaarShop)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0000,COLOR_ERROR);
		return;
	}

// 2006-07-26 by ispark, 선택 아이템 재 정리
	CItemInfo* pSelectItem = NULL;
	if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
	{
		pSelectItem = (CItemInfo*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem);
	}
//	if(m_pSelectItem && m_pSelectItem->ItemWindowIndex != nWindowPosition)
	if(pSelectItem && pSelectItem->ItemWindowIndex != nWindowPosition)
	{
		// 2008-07-23 by dgwoo 착용할수 있는 포지션값과 같을때만 서버로 교체 프로토콜을 보낸다.
		if(pSelectItem->GetItemInfo()->Position != nWindowPosition)
		{
			return;
		}
		// 2004-12-10 by jschoi
		// 교체하면 장착 아이템을 사용할 수 없는 경우는 교체 불가 메시지 출력만 한다.
		CItemInfo* pWearItemInfo = NULL;
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(nWindowPosition);
		if(it != g_pStoreData->m_mapItemWindowPosition.end())
		{
			pWearItemInfo = it->second;
		}
		if(	m_pWearDisplayInfo[nWindowPosition] == NULL || 
			(pWearItemInfo && IsAbleChangeItem(pSelectItem,pWearItemInfo,nWindowPosition))  )
		{
			// 2007-07-16 by dgwoo 튜토리얼 맵에서 서버로 장착 정보를 보내지 않고 강제 셋팅.
			if(g_pTutorial->IsTutorialMode() &&
				g_pTutorial->GetLesson() == L6)
			{// 
				g_pStoreData->TutorialCompulsionItemSetting(pSelectItem);

			}
			else
			{
				// 2009-01-12 by bhsohn 일본 추가 수정사항
				g_pStoreData->RqChangeItemPos(TRUE);
				// end 2009-01-12 by bhsohn 일본 추가 수정사항

				MSG_FC_ITEM_CHANGE_WINDOW_POSITION* pMsg;
				char buffer[SIZE_MAX_PACKET];
				*(MessageType_t*)buffer = T_FC_ITEM_CHANGE_WINDOW_POSITION;
				pMsg = (MSG_FC_ITEM_CHANGE_WINDOW_POSITION*)(buffer+SIZE_FIELD_TYPE_HEADER);
				pMsg->CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
				pMsg->FromItemUniqueNumber = pSelectItem->UniqueNumber;
				pMsg->FromItemWindowIndex = pSelectItem->ItemWindowIndex;
				if(m_pWearDisplayInfo[nWindowPosition])
				{
					pMsg->ToItemUniqueNumber = m_pWearDisplayInfo[nWindowPosition]->pItem->UniqueNumber;
				} else 
				{
					pMsg->ToItemUniqueNumber = 0;
				}
				pMsg->ToItemWindowIndex = nWindowPosition;

				// 빈 슬롯에 아이템을 장착할 때, 아이템을 교체할 때 여기서 해제해 줌... 서버 요청
				if(m_pWearDisplayInfo[nWindowPosition])
				{
					g_pShuttleChild->m_pSkill->DeleteSkillFromWearItem(m_pWearDisplayInfo[nWindowPosition]->pItem->Kind);
				}
				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
				g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark, 메세지 응답을 기다린다. // 2012-11-15 by jhahn, 락을 걸어 OK메세지 확인
	
			}
		}
		else
		{
			char buf[128];
			wsprintf(buf,STRMSG_C_ITEM_0007);//"요구 스탯으로 인하여 해당 아이템을 장착 할 수 없습니다."
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
	}
}


void CINFInvenExtend::SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo, POINT *pIconPos/*=NULL*/)
{
	FLOG( "CINFInvenExtend::SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)" );

	// 2006-07-27 by ispark, 아이콘 시작 위치에서 마우스 포인터 간격 넣기
	POINT ptIcon;
	if(NULL == pIconPos)
	{
		ptIcon.x = m_nRenderMoveIconIntervalWidth;
		ptIcon.y = m_nRenderMoveIconIntervalHeight;
	}
	else
	{
		ptIcon = (*pIconPos);
	}	
	

	((CINFGameMain*)m_pParent)->SetSelectItem(pDisplayInfo, ptIcon, ITEM_INVEN_POS);

//	m_bSelectWearItem = FALSE;
//	if(pDisplayInfo && !m_pDeleteItem)
//	{
//		m_pSelectIcon = ((CINFGameMain*)m_pParent)->m_pIcon->FindIcon(pDisplayInfo->IconName);
//		m_pSelectItem = (CItemInfo*)pDisplayInfo->pItem;
////		m_nSelectItemWindowPosition = ((CItemInfo*)pDisplayInfo->pItem)->ItemWindowIndex;
//		g_pGameMain->m_pQuickSlot->m_pSelectItem = (ITEM_BASE*)m_pSelectItem;
//
//		// set tooltip
//        POINT pt;
//		char buf[128];
//        GetCursorPos( &pt );
//        ScreenToClient( g_pD3dApp->GetHwnd(), &pt );
//		CheckMouseReverse(&pt);
//
//		if(IS_COUNTABLE_ITEM(pDisplayInfo->pItem->Kind))
////		if(pDisplayInfo->pItem->Kind == ITEMKIND_ENERGY || pDisplayInfo->pItem->Kind == ITEMKIND_MINE)
//		{
//			ITEM *item = g_pDatabase->GetServerItemInfo(pDisplayInfo->pItem->ItemNum);
//			if(item)
//				wsprintf(buf, STRMSG_C_TOOLTIP_0013,item->ItemName,((ITEM_GENERAL*)pDisplayInfo->pItem)->CurrentCount);
//		}
//		else
//		{
//			ITEM *item = g_pDatabase->GetServerItemInfo(pDisplayInfo->pItem->ItemNum);
//			if(item)
//				wsprintf(buf, "%s",item->ItemName);
//		}
//		((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, pDisplayInfo->Name);
//	}
//	else
//	{
////		m_nSelectItemWindowPosition = -1;
//		// 2006-05-30 by ispark, 인벤토리 아이템을 선택을 한것이 아니라면 -1을 함부로 주면 안됀다.
//		if(g_pGameMain->m_pQuickSlot->m_pSelectItem)
//			g_pGameMain->m_pQuickSlot->m_nSelectSlotNumber = -1;
//		g_pGameMain->m_pQuickSlot->m_pSelectItem = NULL;
//		m_pSelectItem = NULL;
//		m_pSelectIcon = NULL;
//	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)
/// \brief		다중 선택 처리
/// \author		bhsohn
/// \date		2007-02-12 ~ 2007-02-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)
{	
	POINT ptIcon;
	ptIcon.x = m_nRenderMoveIconIntervalWidth;
	ptIcon.y = m_nRenderMoveIconIntervalHeight;
	((CINFGameMain*)m_pParent)->SetMultiSelectItem(pDisplayInfo, 
									((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem, ptIcon);
}

void CINFInvenExtend::DeleteEnchantItem(UINT nUniqueNumber)
{
	FLOG( "CINFInvenExtend::DeleteEnchantItem(UINT nUniqueNumber)" );
	m_pEnchantItem = NULL;
/*	if(m_pEnchantItem)
	{
		map<int, ITEM_GENERAL*>::iterator it = g_pD3dApp->m_pShuttleChild->m_mapItem.begin();
		while(it != g_pD3dApp->m_pShuttleChild->m_mapItem.end())
		{
			ITEM_GENERAL* pItem = it->second;
			
			if(pItem->UniqueNumber == m_pEnchantItem->UniqueNumber)
			{
				g_pD3dApp->m_pShuttleChild->m_mapItem.erase(it);//pItem->ItemWindowIndex);
				SAFE_DELETE(pItem);
				g_pD3dApp->m_pShuttleChild->ResortingItem();
				g_pD3dApp->m_pShuttleChild->SetShuttleData(5);
				SetAllIconInfo();
				break;
			}
			it++;
		}
		m_pEnchantItem = NULL;
	}
#ifdef _DEBUG
	else
	{
		DBGOUT("ERROR : void CINFInvenExtend::DeleteEnchantItem(UINT nUniqueNumber) m_pEnchantItem == NULL\n");
	}
#endif
*/
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::IsAbleSetItem(CItemInfo* pItemInfo,int nWindowPosition)
/// \brief		해당 아이템을 장착할 수 있는가?
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenExtend::IsAbleSetItem(CItemInfo* pItemInfo,int nWindowPosition)
{
//	if(nWindowPosition != POS_REAR)
//	{
//		return TRUE;
//	}
	GEAR_STAT sGearStat = g_pShuttleChild->m_myShuttleInfo.TotalGearStat;
	ITEM* pItem = pItemInfo->GetRealItemInfo();
	// 2009-04-21 by bhsohn 아이템 DesParam추가
// 	SetGearStatByItem(sGearStat,pItem->DestParameter1,pItem->ParameterValue1);
// 	SetGearStatByItem(sGearStat,pItem->DestParameter2,pItem->ParameterValue2);
// 	SetGearStatByItem(sGearStat,pItem->DestParameter3,pItem->ParameterValue3);
// 	SetGearStatByItem(sGearStat,pItem->DestParameter4,pItem->ParameterValue4);
	int nArrParamCnt = 0;
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		SetGearStatByItem(sGearStat,pItem->ArrDestParameter[nArrParamCnt],
									pItem->ArrParameterValue[nArrParamCnt]);
	}	
	// end 2009-04-21 by bhsohn 아이템 DesParam추가
	
	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
	if(it != g_pStoreData->m_mapItemWindowPosition.end())
	{
		CItemInfo* pWearItemInfo = it->second;
		if(	pWearItemInfo->GetRealItemInfo()->ReqGearStat.AttackPart > sGearStat.AttackPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DefensePart > sGearStat.DefensePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DodgePart > sGearStat.DodgePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.FuelPart > sGearStat.FuelPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.SoulPart > sGearStat.SoulPart )
		{
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::IsAbleReleaseItem(CItemInfo* pItemInfo,int nWindowPosition)
/// \brief		해당 아이템을 해제할 수 있는가?
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenExtend::IsAbleReleaseItem(CItemInfo* pItemInfo,int nWindowPosition)
{
	if(nWindowPosition == POS_REAR)
	{
		return TRUE;
	}
	
	GEAR_STAT sGearStat = g_pShuttleChild->m_myShuttleInfo.TotalGearStat;
	ITEM* pItem = pItemInfo->GetRealItemInfo();
	
    int nArrParamCnt = 0;
	
// 2013-05-09 by ssjung 외형변경 킷 강화시스템 아이템 요구스텟 이 모자랄때 아머를 벗지 못하게 만듬 
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	int nStatLevel = pItemInfo->FixedTermShape.nStatLevel;
	if((0 < nStatLevel && pItemInfo->FixedTermShape.bActive) || SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL == nStatLevel)
//	if((0 < nStatLevel && !pItemInfo->FixedTermShape.bIsEnd) || 10 == nStatLevel)
	{
		for(nArrParamCnt = DES_ATTACK_PART; nArrParamCnt <= DES_DODGE_PART; ++nArrParamCnt)
		{
			ReleaseGearStatByItem(sGearStat,nArrParamCnt,nStatLevel);
		}
	}
	else
#endif
// end 2013-05-09 by ssjung 외형변경 킷 강화시스템 아이템 요구스텟 이 모자랄때 아머를 벗지 못하게 만듬 
	{
		// 2009-04-21 by bhsohn 아이템 DesParam추가
	// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter1,pItem->ParameterValue1);
	// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter2,pItem->ParameterValue2);
	// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter3,pItem->ParameterValue3);
	// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter4,pItem->ParameterValue4);
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			ReleaseGearStatByItem(sGearStat,pItem->ArrDestParameter[nArrParamCnt],
											pItem->ArrParameterValue[nArrParamCnt]);
		}
	}
	// end 2009-04-21 by bhsohn 아이템 DesParam추가

	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
	if(it != g_pStoreData->m_mapItemWindowPosition.end())
	{
		CItemInfo* pWearItemInfo = it->second;
		if(	pWearItemInfo->GetRealItemInfo()->ReqGearStat.AttackPart > sGearStat.AttackPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DefensePart > sGearStat.DefensePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DodgePart > sGearStat.DodgePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.FuelPart > sGearStat.FuelPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.SoulPart > sGearStat.SoulPart )
		{
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::IsAbleChangeItem(CItemInfo* pSetItemInfo,CItemInfo* pReleaseItemInfo,int nWindowPosition)
/// \brief		해당 아이템을 교체할 수 있는가?
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenExtend::IsAbleChangeItem(CItemInfo* pSetItemInfo,CItemInfo* pReleaseItemInfo,int nWindowPosition)
{
//	if(nWindowPosition != POS_REAR)
//	{
//		return TRUE;
//	}
	GEAR_STAT sGearStat = g_pShuttleChild->m_myShuttleInfo.TotalGearStat;
	ITEM* pSetItem = pSetItemInfo->GetRealItemInfo();
	ITEM* pReleaseItem = pReleaseItemInfo->GetRealItemInfo();
	// 2009-04-21 by bhsohn 아이템 DesParam추가
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter1,pSetItem->ParameterValue1);
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter2,pSetItem->ParameterValue2);
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter3,pSetItem->ParameterValue3);
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter4,pSetItem->ParameterValue4);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter1,pReleaseItem->ParameterValue1);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter2,pReleaseItem->ParameterValue2);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter3,pReleaseItem->ParameterValue3);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter4,pReleaseItem->ParameterValue4);
	// 2009-04-21 by bhsohn 아이템 DesParam추가
// 2013-07-25 by ssjung 외형변경 킷 강화시스템 아이템 요구스텟 이 모자랄때 아머를 벗지 못하게 만듬 (추가 예외처리)
	int nArrParamCnt = 0;
	int nStatLevel = 0;
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	nStatLevel = pSetItemInfo->FixedTermShape.nStatLevel;
	if((0 < nStatLevel && pSetItemInfo->FixedTermShape.bActive) || SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL == nStatLevel)
		//	if((0 < nStatLevel && !pItemInfo->FixedTermShape.bIsEnd) || 10 == nStatLevel)
	{
		for(nArrParamCnt = DES_ATTACK_PART; nArrParamCnt <= DES_DODGE_PART; ++nArrParamCnt)
		{
			SetGearStatByItem(sGearStat,nArrParamCnt,nStatLevel);
		}
	}
	else
#endif
// end 2013-07-25 by ssjung 외형변경 킷 강화시스템 아이템 요구스텟 이 모자랄때 아머를 벗지 못하게 만듬 (추가 예외처리)
	{
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			SetGearStatByItem(sGearStat,pSetItem->ArrDestParameter[nArrParamCnt],
										pSetItem->ArrParameterValue[nArrParamCnt]);
		}
	}
// 2013-07-25 by ssjung 외형변경 킷 강화시스템 아이템 요구스텟 이 모자랄때 아머를 벗지 못하게 만듬 (추가 예외처리)
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	nStatLevel = pReleaseItemInfo->FixedTermShape.nStatLevel;
	if((0 < nStatLevel && pReleaseItemInfo->FixedTermShape.bActive) || SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL == nStatLevel)
		//	if((0 < nStatLevel && !pItemInfo->FixedTermShape.bIsEnd) || 10 == nStatLevel)
	{
		for(nArrParamCnt = DES_ATTACK_PART; nArrParamCnt <= DES_DODGE_PART; ++nArrParamCnt)
		{
			ReleaseGearStatByItem(sGearStat,nArrParamCnt,nStatLevel);
		}
	}
	else
#endif
// end 2013-07-25 by ssjung 외형변경 킷 강화시스템 아이템 요구스텟 이 모자랄때 아머를 벗지 못하게 만듬 (추가 예외처리)
	{
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			ReleaseGearStatByItem(sGearStat,pReleaseItem->ArrDestParameter[nArrParamCnt],
											pReleaseItem->ArrParameterValue[nArrParamCnt]);
		}	
	}
	// end 2009-04-21 by bhsohn 아이템 DesParam추가

	ITEM* pItem = NULL;
	if( nWindowPosition == POS_REAR )
	{
		pItem = pSetItemInfo->GetRealItemInfo();
	}
	else
	{
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
		if(it != g_pStoreData->m_mapItemWindowPosition.end())
		{
			pItem = it->second->GetRealItemInfo();
		}
		else
		{
			return TRUE;
		}
	}

	if(	pItem->ReqGearStat.AttackPart > sGearStat.AttackPart ||
		pItem->ReqGearStat.DefensePart > sGearStat.DefensePart ||
		pItem->ReqGearStat.DodgePart > sGearStat.DodgePart ||
		pItem->ReqGearStat.FuelPart > sGearStat.FuelPart ||
		pItem->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
		pItem->ReqGearStat.SoulPart > sGearStat.SoulPart )
	{
		return FALSE;
	}

//	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
//	if(it != g_pStoreData->m_mapItemWindowPosition.end())
//	{
//		CItemInfo* pWearItemInfo = it->second;
//		if(	pWearItemInfo->GetRealItemInfo()->ReqGearStat.AttackPart > sGearStat.AttackPart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DefensePart > sGearStat.DefensePart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DodgePart > sGearStat.DodgePart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.FuelPart > sGearStat.FuelPart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.SoulPart > sGearStat.SoulPart )
//		{
//			return FALSE;
//		}
//	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::SetGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
/// \brief		아이템에 의해 스텟을 추가한다.
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::SetGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
{
	switch(nDestParam)
	{
	case DES_ATTACK_PART:				// 공격 디바이스 파트
		sGearStat.AttackPart += fDestValue;
		break;
	case DES_DEFENSE_PART:				// 내구 디바이스 파트
		sGearStat.DefensePart += fDestValue;
		break;
	case DES_FUEL_PART:					// 연료 디바이스 파트
		sGearStat.FuelPart += fDestValue;
		break;
	case DES_SOUL_PART:					// 감응 디바이스 파트
		sGearStat.SoulPart += fDestValue;
		break;
	case DES_SHIELD_PART:				// 방어 디바이스 파트
		sGearStat.ShieldPart += fDestValue;
		break;
	case DES_DODGE_PART	:				// 회피 디바이스 파트
		sGearStat.DodgePart += fDestValue;
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::ReleaseGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
/// \brief		아이템에 의해 스텟을 해제한다.
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ReleaseGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
{
	switch(nDestParam)
	{
	case DES_ATTACK_PART:				// 공격 디바이스 파트
		sGearStat.AttackPart -= fDestValue;
		break;
	case DES_DEFENSE_PART:				// 내구 디바이스 파트
		sGearStat.DefensePart -= fDestValue;
		break;
	case DES_FUEL_PART:					// 연료 디바이스 파트
		sGearStat.FuelPart -= fDestValue;
		break;
	case DES_SOUL_PART:					// 감응 디바이스 파트
		sGearStat.SoulPart -= fDestValue;
		break;
	case DES_SHIELD_PART:				// 방어 디바이스 파트
		sGearStat.ShieldPart -= fDestValue;
		break;
	case DES_DODGE_PART	:				// 회피 디바이스 파트
		sGearStat.DodgePart -= fDestValue;
		break;
	}
}

int	CINFInvenExtend::GetScrollLine()
{
	int nInvenTotalNum = g_pStoreData->m_mapItemUniqueNumber.size();
	int nWearItemNum = GetWearItemNum();
	//int ntemp = int((nInvenTotalNum - nWearItemNum) / INVEN_X_NUMBER) - 6;
	int ntemp = int((nInvenTotalNum - nWearItemNum) / INVEN_X_NUMBER);
	if(0 != ((nInvenTotalNum - nWearItemNum) % INVEN_X_NUMBER))
		ntemp++;

	if(ntemp < 0)
	{
		ntemp = 0;
	}
	return ntemp;
}

int	CINFInvenExtend::GetScrollLineNumber(float nLength)
{
	int ntemp = GetScrollLine();
	float fLineNum = (nLength * ntemp) / INVEN_SCROLL_LINE_MOVE_LENGTH;

	return (int)fLineNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::GetWearItemNum()
/// \brief		장착된 아이템 갯수 확인
/// \author		ispark
/// \date		2005-12-08 ~ 2005-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFInvenExtend::GetWearItemNum()
{
	int count = 0;
	for(int i = 0; i < SIZE_MAX_POS; i++)
	{
		if(m_pWearDisplayInfo[i])
		{
			count++;
		}
	}
	
	return count;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::GetScrollLineNumber(int nLength)
/// \brief		현재 스크롤 위치 라인 위치 가져오기
/// \author		ispark
/// \date		2005-12-08 ~ 2005-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFInvenExtend::GetScrollLineInterval(int nScrollLineNum)
{
	int ntemp = GetScrollLine();
	float fLineNum = 0.0f;
	if(ntemp)
		fLineNum = (INVEN_SCROLL_LINE_MOVE_LENGTH * nScrollLineNum) / ntemp;

	return fLineNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInvenExtend::SetScrollEndLine()
/// \brief		스크롤을 마지막 라인으로 옮기기
/// \author		ispark
/// \date		2006-06-21 ~ 2006-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::SetScrollEndLine()
{
	//m_nInvenCurrentScroll = GetScrollLine();
	//m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
	m_pINFInvenItem->SetScrollEndLine(); // 마지막라인으로
	SetAllIconInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::InitInven()
/// \brief		인벤토리 초기 상태, 스크롤위치와 라인 아이템 셋팅
/// \author		ispark
/// \date		2005-12-12 ~ 2005-12-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::InitInven()
{
//	m_nInvenCurrentScroll = 0;
//	m_nInvenCurrentScrollPosition = 0;
	int nCurrentLine = GetScrollLine();
	m_pINFInvenItem->SetMaxScrollStep(nCurrentLine);

	SetAllIconInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInvenExtend::ReSetScrollCurrentLine()
/// \brief		현재 스크롤 라인을 체크를 해서 재 셋팅한다.
/// \author		ispark
/// \date		2006-09-26 ~ 2006-09-26
/// \warning	
///
/// \param		
/// \return		
//////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ReSetScrollCurrentLine()
{
	UpdateInvenScrollMax();	// 인벤위치 갱신

	SetAllIconInfo();

	// 전보다 인벤 라인이 작으면
//	if(m_nInvenCurrentScroll > nCurrentLine)
//	{
//		m_nInvenCurrentScroll = nCurrentLine;
//		m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
//	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int GetInvenFreeSize()
/// \brief		인벤의 비어있는 양을 알려준다.
/// \author		bhsohn
/// \date		2007-02-22 ~ 2007-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFInvenExtend::GetInvenFreeSize()
{
	// 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
	//int nFreeSize = (CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1) 
	// 2009. 12. 17 by jskim 캐쉬(인벤/창고 확장) 재수정
// 	CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
// 	int nFreeSize = (CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1) 
	int nFreeSize = (CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1) 
	//end 2009. 12. 17 by jskim 캐쉬(인벤/창고 확장) 재수정
	//end 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
					- g_pStoreData->GetTotalUseInven();	
	return nFreeSize;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInvenExtend::ClearMultiSeletItem()
/// \brief		인벤의 비어있는 양을 알려준다.
/// \author		// 2007-03-02 by bhsohn 다중 선택 추가 보안
/// \date		2007-03-05 ~ 2007-03-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ClearMultiSeletItem()
{
	int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
	if(nSelItemCnt > 0)
	{
		SetMultiSelectItem(NULL);	// 다중 처리 제거
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL IsInvenHave(int nItemNum)
/// \brief		인벤에 있는지 여부를 판단한다. 
/// \author		// 2007-03-09 by bhsohn 다중 선택 창고 부분 보안
/// \date		2007-03-09 ~ 2007-03-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenExtend::IsInvenHave(int nItemNum)
{
	BOOL bHave = FALSE;
	for(int nCnt = 0; nCnt < INVEN_NUMBER; nCnt++)
	{
		if(NULL == m_pInvenDisplayInfo[nCnt])
		{
			continue;
		}
		if(nItemNum == m_pInvenDisplayInfo[nCnt]->pItem->ItemNum )
		{
			bHave = TRUE;
			break;
		}
	}
	return bHave;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
/// \date		2007-06-20 ~ 2007-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::InitDeleteItemInfo()
{
	memset(&m_struDeleteItem, 0x00, sizeof(structDelItemInfo));
}


void CINFInvenExtend::SetDeleteItemInfo(CItemInfo* pSelectItem)
{
	if(NULL == pSelectItem)
	{
		InitDeleteItemInfo();
		return;
	}
	m_struDeleteItem.UniqueNumber = pSelectItem->UniqueNumber;
	m_struDeleteItem.CurrentCount = pSelectItem->CurrentCount;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn 스피커 아이템 추가
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::DoModalChatAllInfluence(UID64_t		UniqueNumber)
{	
	if(g_pGameMain && g_pGameMain->m_pInfWindow)
	{		
		g_pGameMain->m_bChangeMousePoint = TRUE;
		g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070810_0201, 
												_Q_USEITEM_SPEAKER_MSG,
												0,
												0,
												0, 
												UniqueNumber);	//"새캐릭터 이름을 입력해 주십시오(\\r변경시 로그오프 됩니다\\r)"
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤 보일지 여부
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ShowInven(POINT *pItem, POINT *pEq, BOOL bClick/*=FALSE*/, BOOL bShop/*=FALSE*/)
{
//	POINT ptItem, ptEq;
//
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//	ptItem.x = INVEN_EQ_STApEqRT_X;
//	ptItem.y = INVEN_EQ_START_Y;
//
//	ptEq.x = 0;
//	ptEq.y = INVEN_EQ_START_Y;
	BOOL bShowEq = FALSE;
	if(!bShop)
	{
		if(m_bShowEqWnd)
		{
			bShowEq = TRUE;
		}		
		// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
		else
		{
			// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			//g_INFCnt++;
			g_pD3dApp->AddINFCnt();
			// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		}
		// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	}
	else 
	{
		if(m_bShowEqShopWnd)
		{
			bShowEq = TRUE;
		}
		// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
		else
		{
			// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			//g_INFCnt++;
			g_pD3dApp->AddINFCnt();
			// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		}
		// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	}

	// 장비 인벤
	if(pEq && bShowEq)
	{
		POINT ptEqPos = (*pEq);
		if(bClick && (!bShop))
		{
			if(m_bEqSetPos)
			{
				ptEqPos = m_pEqIvenNormalPos;
			}					
		}
		else if(bShop)
		{
			if(m_bEqShopSetPos)
			{
				ptEqPos = m_pEqIvenShopPos;
			}
		}
		m_pINFInvenEquip->ShowWnd(TRUE, &ptEqPos);
		g_pInterface->SetWindowOrder(WNDInvenWnd);
	}
	else
	{
		if(m_pINFInvenEquip->IsShowWnd())
		{
			if(bClick && !bShop)
			{
				m_bEqSetPos = TRUE;
				POINT ptItemIvenPos = m_pINFInvenEquip->GetBkPos();
				m_pEqIvenNormalPos.x = ptItemIvenPos.x;
				m_pEqIvenNormalPos.y = ptItemIvenPos.y;
			}
			else if(bShop)
			{				
				m_bEqShopSetPos = TRUE;
				POINT ptItemIvenPos = m_pINFInvenEquip->GetBkPos();
				m_pEqIvenShopPos.x = ptItemIvenPos.x;
				m_pEqIvenShopPos.y = ptItemIvenPos.y;
			}
		}
		m_pINFInvenEquip->ShowWnd(FALSE, NULL);
	}
#endif

	// 아이템 인벤
	if(pItem)
	{
		POINT ptEqPos = (*pItem);
		if(bClick && m_bItemSetPos)
		{
			ptEqPos = m_pItemIvenPos;
		}
		m_pINFInvenItem->ShowWnd(TRUE, &ptEqPos);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pINFInvenEquip->ShowWnd(TRUE, &ptEqPos);												  
#endif
	}
	else
	{
		if(bClick && m_pINFInvenItem->IsShowWnd())
		{
			m_bItemSetPos = TRUE;
			POINT ptItemEqPos = m_pINFInvenItem->GetBkPos();
			m_pItemIvenPos.x = ptItemEqPos.x;
			m_pItemIvenPos.y = ptItemEqPos.y;
		}
		m_pINFInvenItem->ShowWnd(FALSE, NULL);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pINFInvenEquip->ShowWnd(FALSE, NULL);													  
#endif
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::SetBkPos(POINT ptBkPos)
{
	if(m_pINFInvenItem)
	{
		m_pINFInvenItem->SetBkPos(ptBkPos);	
	}
	if(m_pINFInvenEquip)
	{
		m_pINFInvenEquip->SetBkPos(ptBkPos);
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤 보일지 여부
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ShowAllInven(BOOL bShow)
{	
	if(bShow)
	{
		POINT ptPos = m_pINFInvenEquip->GetBkPos();
		m_pINFInvenEquip->ShowWnd(TRUE, &ptPos);

		ptPos = m_pINFInvenItem->GetBkPos();
		m_pINFInvenItem->ShowWnd(TRUE, &ptPos);
		g_pInterface->SetWindowOrder(WNDInvenWnd);
		
	}
	else
	{
		m_pINFInvenEquip->ShowWnd(NULL, NULL);
		m_pINFInvenItem->ShowWnd(NULL, NULL);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInvenExtend::ShowInvenAndMirror(BOOL i_bInven,BOOL i_bMirror /* = FALSE */)
/// \brief		인벤토리 또는 미러 이미지 창 보일지 안보일지 셋팅.
/// \author		dgwoo
/// \date		2008-11-24 ~ 2008-11-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ShowInvenAndMirror(BOOL i_bInven,BOOL i_bMirror /* = FALSE */)
{
	if(i_bInven)
	{
		POINT ptPos = m_pINFInvenItem->GetBkPos();
		m_pINFInvenItem->ShowWnd(TRUE, &ptPos);
		g_pInterface->SetWindowOrder(WNDInvenWnd);
	}
	else
	{
		m_pINFInvenItem->ShowWnd(NULL,NULL);
	}

	if(i_bMirror)
	{
		POINT ptPos = m_pINFInvenEquip->GetBkPos();
		m_pINFInvenEquip->ShowWnd(TRUE, &ptPos);
	}
	else
	{
		m_pINFInvenEquip->ShowWnd(NULL,NULL);
	}

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤 보일지 여부
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenExtend::IsShowInven()
{
	return m_pINFInvenItem->IsShowWnd();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INVEN_DISPLAY_INFO* CINFInvenExtend::GetInvenDisplayInfo(int nIdx)
{
	if(nIdx >= INVEN_NUMBER)
	{
		return NULL;
	}
	return m_pInvenDisplayInfo[nIdx];

}

// 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정
INVEN_DISPLAY_INFO* CINFInvenExtend::GetInvenDisplayInfoToUniqueNum(UID64_t nUniqueNum)
{
	for(int i = 0 ; i < INVEN_NUMBER ; i++)
	{
		if(!m_pInvenDisplayInfo[i])
		{
			continue;
		}
		if(m_pInvenDisplayInfo[i] && nUniqueNum == m_pInvenDisplayInfo[i]->pItem->UniqueNumber)
		{
			return m_pInvenDisplayInfo[i];
		}
	}

	return NULL;
}
// end 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::SetWearPosition(int iWearPosition)
{
	if(NULL == m_pINFInvenEquip)
	{
		return;
	}
	m_pINFInvenEquip->SetWearPosition(iWearPosition);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::RenderMirror(POINT *pMirrorPos/*=NULL*/)
{
	if(NULL == m_pINFInvenEquip)
	{
		return;
	}
	m_pINFInvenEquip->RenderMirror(pMirrorPos);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenExtend::IsEquipInvenShow()
{
	if(NULL == m_pINFInvenEquip)
	{
		return FALSE;
	}
	return m_pINFInvenEquip->IsShowWnd();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXMATRIX	CINFInvenExtend::GetInvenMatInven()
{
	if(NULL == m_pINFInvenEquip)
	{
		D3DXMATRIX MatInven;
		D3DXMatrixIdentity(&MatInven);
		return MatInven;
	}
	return m_pINFInvenEquip->GetInvenMatInven();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INVEN_DISPLAY_INFO* CINFInvenExtend::GetWearDisplayInfo(int nIdx)
{

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수 변경.
	//if(nIdx < 0 || nIdx >= WEAR_ITEM_NUMBER)
	if(nIdx < 0 || nIdx >= MAX_EQUIP_POS)
	{
		return NULL;
	}

	return m_pWearDisplayInfo[nIdx];
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤의 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID64_t	 CINFInvenExtend::GetDeleteItemUID()
{
	return m_struDeleteItem.UniqueNumber;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		SPI폰트
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CD3DHanFont* CINFInvenExtend::GetFontSpi()
{
	return m_pFontSpi;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		SPI폰트
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CD3DHanFont* CINFInvenExtend::GetFontWeight(int nIdx)
{
	return m_pFontWeight[nIdx];
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		스크롤 갱신
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::UpdateInvenScrollMax()
{
	int nCurrentLine = GetScrollLine();
	m_pINFInvenItem->SetMaxScrollStep(nCurrentLine);
	m_pINFInvenItem->SetScrollEndLine();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		스크롤 갱신
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::ShowEqInven()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	BOOL bShow = m_pINFInvenEquip->IsShowWnd();
	bShow^=TRUE;
	if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
	{
		m_bShowEqShopWnd = bShow;	// 상점에서의 보이는지 여부
	}
	else
	{
		m_bShowEqWnd = bShow;
	}
	
	if(bShow)
	{
		POINT ptPos = m_pINFInvenEquip->GetBkPos();		
		if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
		{	
			ptPos.x = INVEN_EQ_SHOP_START_X;
			ptPos.y = INVEN_EQ_SHOP_START_Y;
			if(m_bEqShopSetPos)
			{
				ptPos = m_pEqIvenShopPos;
			}
		}
		else if(m_bEqSetPos)
		{
			ptPos = m_pEqIvenNormalPos;			
		}		
		m_pINFInvenEquip->ShowWnd(TRUE, &ptPos);
		
		g_pInterface->SetWindowOrder(WNDInvenWnd);
	}
	else
	{
		if(m_pINFInvenEquip->IsShowWnd())
		{
			if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
			{
				m_bEqShopSetPos = TRUE;
				POINT ptItemIvenPos = m_pINFInvenEquip->GetBkPos();
				m_pEqIvenShopPos.x = ptItemIvenPos.x;
				m_pEqIvenShopPos.y = ptItemIvenPos.y;
			}
			else
			{
				m_bEqSetPos = TRUE;
				POINT ptItemIvenPos = m_pINFInvenEquip->GetBkPos();
				m_pEqIvenNormalPos.x = ptItemIvenPos.x;
				m_pEqIvenNormalPos.y = ptItemIvenPos.y;
			}
		}

		m_pINFInvenEquip->ShowWnd(NULL, NULL);
	}
#endif

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		우선 순위 갱신
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::SetWndOrder(int nWndIdx)
{
	// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
	if(g_pInterface && g_pInterface->m_pCityBase)
	{
		GUI_BUILDINGNPC* pBuilding = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
		if(pBuilding )
		{
			// 상점이 떠있으면 안된다.
			return;
		}
	}
	// end 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	deque<int>	 vecWndOrderTmp;
	vecWndOrderTmp.clear();
	int nCnt = 0;	
	for(nCnt = 0;nCnt < m_vecWndOrder.size();nCnt++)
	{
		int nWndOrderTmp = m_vecWndOrder[nCnt];
		if(nWndOrderTmp == nWndIdx)
		{
			vecWndOrderTmp.push_front(nWndOrderTmp);
		}
		else
		{
			vecWndOrderTmp.push_back(nWndOrderTmp);
		}
	}
	m_vecWndOrder = vecWndOrderTmp;
#endif

	g_pInterface->SetWindowOrder(WNDInvenWnd);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		우선 순위 갱신
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////// 
POINT CINFInvenExtend::GetEqInvenBkPos()
{
	return m_pINFInvenEquip->GetBkPos();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		우선 순위 갱신
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////// 
void CINFInvenExtend::SetInvenPosInfo(structInvenPosInfo* pstruInvenPosInfo)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pItemIvenPos	= pstruInvenPosInfo->ptItemIvenPos;
// 	m_pEqIvenNormalPos	= pstruInvenPosInfo->ptEqIvenPos;
// 	m_pEqIvenShopPos = pstruInvenPosInfo->ptEqIvenShopPos;
	m_bItemSetPos	= pstruInvenPosInfo->bItemSetPos;		
// 	m_bEqSetPos		= pstruInvenPosInfo->bEqSetPos;		
// 	m_bShowEqWnd	= pstruInvenPosInfo->bShowEqWnd;		
// 	m_bShowEqShopWnd= pstruInvenPosInfo->bShowEqShopWnd;		
// 	m_bEqShopSetPos= pstruInvenPosInfo->bEqShopSetPos;		
#else	 
	m_pItemIvenPos	= pstruInvenPosInfo->ptItemIvenPos;
	m_pEqIvenNormalPos	= pstruInvenPosInfo->ptEqIvenPos;
	m_pEqIvenShopPos = pstruInvenPosInfo->ptEqIvenShopPos;
	m_bItemSetPos	= pstruInvenPosInfo->bItemSetPos;		
	m_bEqSetPos		= pstruInvenPosInfo->bEqSetPos;		
	m_bShowEqWnd	= pstruInvenPosInfo->bShowEqWnd;		
	m_bShowEqShopWnd= pstruInvenPosInfo->bShowEqShopWnd;		
	m_bEqShopSetPos= pstruInvenPosInfo->bEqShopSetPos;		
#endif

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		우선 순위 갱신
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////// 
structInvenPosInfo CINFInvenExtend::GetInvenPosInfo()
{
	structInvenPosInfo	struInfo;
	memset(&struInfo, 0x00, sizeof(structInvenPosInfo));

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	struInfo.ptItemIvenPos = m_pItemIvenPos;
	//struInfo.ptEqIvenPos = m_pEqIvenNormalPos;
	struInfo.bItemSetPos = m_bItemSetPos;		
// 	struInfo.bEqSetPos = m_bEqSetPos;		
// 	struInfo.bShowEqWnd = m_bShowEqWnd;		
// 	struInfo.bShowEqShopWnd = m_bShowEqShopWnd;
// 	struInfo.ptEqIvenShopPos = m_pEqIvenShopPos;	 
// 	struInfo.bEqShopSetPos = m_bEqShopSetPos;													  
#else		   
	struInfo.ptItemIvenPos = m_pItemIvenPos;
	struInfo.ptEqIvenPos = m_pEqIvenNormalPos;
	struInfo.bItemSetPos = m_bItemSetPos;		
	struInfo.bEqSetPos = m_bEqSetPos;		
	struInfo.bShowEqWnd = m_bShowEqWnd;		
	struInfo.bShowEqShopWnd = m_bShowEqShopWnd;
	struInfo.ptEqIvenShopPos = m_pEqIvenShopPos;	 
	struInfo.bEqShopSetPos = m_bEqShopSetPos;
#endif

	return struInfo;

}

// 2010. 04. 01 by ckPark 리소스 변경 시스템시 팩토리나 연구소에서 아이템 회수한 후 변경이 안되는 문제 해결
void	CINFInvenExtend::UpdateIcon( CItemInfo* pItemInfo )
{
	SetSingleInvenIconInfo( pItemInfo );
	SetSingleWearIconInfo( pItemInfo );
}
// end 2010. 04. 01 by ckPark 리소스 변경 시스템시 팩토리나 연구소에서 아이템 회수한 후 변경이 안되는 문제 해결


// 2010-06-15 by shcho&hslee 펫시스템 - 장착아이템 슬롯 정보 포인터 리턴.
INVEN_DISPLAY_INFO *CINFInvenExtend :: rtnPtr_AttachmentItemInfo ( const INT a_nPosKind )
{

	if ( a_nPosKind <= POS_NONE || a_nPosKind >= MAX_EQUIP_POS )
		return NULL;

	return m_pWearDisplayInfo[a_nPosKind];

}
// End 2010-06-15 by shcho&hslee 펫시스템 - 장착아이템 슬롯 정보 포인터 리턴.


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::RenderLuckyMechine()
{
	GUI_BUILDINGNPC* pCurrentBuildingNPC = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
	if(NULL == pCurrentBuildingNPC)
	{
		return;
	}
	else if(BUILDINGKIND_LUCKY != pCurrentBuildingNPC->buildingInfo.BuildingKind)
	{
		return;
	}
	CMapCityShopIterator itLucky = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LUCKY);
	if(itLucky == g_pInterface->m_pCityBase->m_mapCityShop.end())
	{		
		return;
	}			
	CINFLuckyMachine* pINFLuckyMachine = ((CINFLuckyMachine*)itLucky->second);
	if(NULL == pINFLuckyMachine)
	{
		return;
	}
	pINFLuckyMachine->RenderYouLucky();
}
							  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
POINT CINFInvenExtend::GetBkSize()
{
	return m_pINFInvenItem->GetBkSize();	
};

BOOL CINFInvenExtend::GetInvenMove()
{
	return m_pINFInvenItem->GetMove();
};
#endif


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메뉴 버튼 
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenExtend::OnClickItemMenuListWnd(BOOL bShow, POINT ptPoint, INT nItemNum, UID64_t uItemUniNum)
{
	if(NULL == m_pItemMenuList)
	{
		return;
	}	
	m_pItemMenuList->ShowWnd(bShow, nItemNum, uItemUniNum, &ptPoint);		
	
}

int CINFInvenExtend::WndProcItemMenuListWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if(NULL == m_pItemMenuList)
	{
		return INF_MSGPROC_NORMAL;
	}
	return m_pItemMenuList->WndProc(uMsg, wParam, lParam);
}

void CINFInvenExtend::RenderItemMenuListWnd()
{
	if(NULL == m_pItemMenuList)
	{
		return;
	}
	m_pItemMenuList->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-03-18 by bhsohn 팩토리 조합 추가
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
POINT CINFInvenExtend::GetBkPos()
{
	POINT ptPos = {0,0};
	if(m_pINFInvenItem)
	{
		ptPos = m_pINFInvenItem->GetBkPos();
	}
	return ptPos;
}