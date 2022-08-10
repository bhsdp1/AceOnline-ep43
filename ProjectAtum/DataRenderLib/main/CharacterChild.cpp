// CharacterChild.cpp: implementation of the CCharacterChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterChild.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "AtumSound.h"
#include "EnemyData.h"
#include "ChatMoveData.h"
#include "AtumApplication.h"
#include "FieldWinSocket.h"
#include "Chat.h"
#include "Cinema.h"								// 2005-07-25 by ispark
#include "Background.h"
#include "SceneData.h"
#include "Camera.h"
#include "Weapon.h"
#include "INFGameMain.h"						// 2005-07-25 by ispark
#include "Skill.h"								// 2005-08-02 by ispark
#include "AtumDatabase.h"
#include "WeaponShieldData.h"
#include "CharacterRender.h"					// 2005-07-21 by ispark
#include "Frustum.h"
#include "ObjectChild.h"
#include "ClientParty.h"
#include "ObjRender.h"
#include "SkillEffect.h"
#include "dxutil.h"
#include "Interface.h"
#include "INFCityBase.h"
#include "INFTarget.h"
#include "INFMissionInfo.h"
#include "PkNormalTimer.h"						// 2005-08-02 by ispark
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "EffectRender.h"
#include "StoreData.h"	
#include "CustomOptimizer.h"

// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
#include "INFCityInfinityField.h"
#include "INFGameArena.h"
// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

// 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
#include "INFInvenExtend.h"
// end 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정


#include "PetManager.h"			// 2010-06-15 by shcho&hslee 펫시스템 - 펫 데이터를 가져오는 함수
#include "INFItemInfo.h"
#include "WeaponItemInfo.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define CHARACTER_HEIGHT				7.087f

// 2009-02-05 by bhsohn 카메라 깨지는 현상 처리
// #define CHARACTER_CAMERA_DISTANCE		30.0f	// 초기거리
// #define CHARACTER_CAMERA_HEIGHT			30.0f	// 초기높이
// #define CHARACTER_CAMERA_WHEEL_DISTANCE 6.0f	// 휠 간격
// end 2009-02-05 by bhsohn 카메라 깨지는 현상 처리

#define CHARACTER_WALK					10.0f	// 걷기
#define CHARACTER_MAN_RUN				35.7f	// 남자 뛰기 46->25->30->33		// 2006-11-02 by ispark, 다른것으로 바꿈
#define CHARACTER_WOMAN_RUN				33.0f	// 여자 뛰기 46->25->30->33		// 2006-11-02 by ispark, 다른것으로 바꿈

#define CHARACTER_SHOP_SERCH_DISTANCE	50.0f	// 샵검색

// 2007-07-27 by bhsohn 프리스카 맵만 상점 인식 범위 증가
#define CHARACTER_SHOP_SERCH_FREESKA_DISTANCE	100.0f	// 샵검색

#define CHARACTER_PICKING_LENGTH		10000.0f;// Picking 허용 거리
//////////////////////////////////////////////////////////////////////////

CCharacterChild::CCharacterChild()
{
	D3DXMatrixIdentity(&m_mMatrix);
	D3DXMatrixIdentity(&m_mPickMatrix);

	g_pCharacterChild = this;
	m_pCharacterInfo = NULL;
	m_pPickingInfo = NULL;					// 2005-07-26 by ispark Picking 이펙트
	m_pCharacterRender = g_pD3dApp->m_pCharacterRender;

	m_dwState = _INITIALIZING;
//	m_dwPartType = _SHUTTLE;
	m_bRButtonState = FALSE;
	m_bPickMove = FALSE;

	m_pVBShadow = NULL;

	m_fCurrentTime = 0.0f;
	m_fCharacterSpeed = 0.0f;
	m_fCharacterAddSpeed = .0f;
	m_hyBodyCondition = 0;
	m_nUnitNum = 0;
	m_pSelectObject = NULL;					// 2006-04-08 by ispark
	m_pShopObject = NULL;
	m_bEnterShopState = FALSE;

	m_fOldLength = 0.0f;					// 2005-07-29 by ispark
	m_bMouseMove = FALSE;					// 2005-08-16 by ispark
	m_bMouseWheel = FALSE;					// 2005-08-17 by ispark

	m_bCharacterRender = TRUE;
	m_nStartEventType = 0;

	m_bStartPositionEvent = FALSE;
	m_pSelBazaarShop = NULL;					// 2006-07-29 by ispark
	m_fCharacterWarpErrTime = 0.0f;				// 2006-08-07 by ispark

	m_pMoveChatShop = NULL;

//	m_fCharacterMoveRate = 0.0f;			// 2006-11-02 by ispark

	// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
	m_bSetSpeed = FALSE;
	m_bPetWearItem = FALSE;
	// end 2010-11-29 by jskim, 펫 장착시 이동속도 증가

	// 2012-06-25 by jhahn, 펫 장착시 이동속도 증가 기능추가
	PetoptionSpeed	= NULL;
	//end 2012-06-25 by jhahn, 펫 장착시 이동속도 증가 기능추가

	// 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
	m_fAdminModeSpeed = 0.0f;
	// END 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
}

CCharacterChild::~CCharacterChild()
{
	SAFE_RELEASE(m_pVBShadow);
	SAFE_DELETE(m_pMoveChatShop);
}

HRESULT CCharacterChild::InitDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		SAFE_DELETE(m_pCharacterInfo);
	}
	if(m_pPickingInfo)
	{
		m_pPickingInfo->InvalidateDeviceObjects();
		m_pPickingInfo->DeleteDeviceObjects();
		SAFE_DELETE(m_pPickingInfo);
	}
	if(m_pMoveChatShop)
		m_pMoveChatShop->InitDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::RestoreDeviceObjects()
{
	RestoreShadow();
	if(m_pMoveChatShop)
		m_pMoveChatShop->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
	if(m_pMoveChatShop)
		m_pMoveChatShop->InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
//	if(m_pCharacterInfo)
//		m_pCharacterInfo->DeleteDeviceObjects();
//	if(m_pPickingInfo)
//		m_pPickingInfo->DeleteDeviceObjects();
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		SAFE_DELETE(m_pCharacterInfo);
	}
	if(m_pPickingInfo)
	{
		m_pPickingInfo->InvalidateDeviceObjects();
		m_pPickingInfo->DeleteDeviceObjects();
		SAFE_DELETE(m_pPickingInfo);
	}
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->DeleteDeviceObjects();
		SAFE_DELETE(m_pMoveChatShop);
	}
	return S_OK;
}
// 2010-06-08 by dgwoo, 펫시스템 추가. 
void CCharacterChild::SetPartner(char* i_szPartner)
{
	//int nPartner = atoi(i_szPartner);
	//CreateWearItem( WEAR_ITEM_KIND_PET, nPartner, FALSE);
}
// 2010-06-08 by dgwoo, 펫시스템 추가. 
void CCharacterChild::Render()
{
	FLOG( "CShuttleChild::Render()" );
//	if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL ||
//		m_bIsCameraPattern == TRUE)
	// 2005-08-30 by ispark	
//	g_pShuttleChild->RenderMirror();

	if(m_bCharacterRender)
	{
		g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

//		g_pD3dDev->LightEnable( 1, FALSE );
//		g_pD3dDev->LightEnable( 2, FALSE );
//		g_pD3dDev->LightEnable( 3, FALSE );
//		g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
		g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
		
//		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATER);
		g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x00000088 );
	

		
//		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		
		m_pCharacterRender->Render();	
		g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
	}
}

void CCharacterChild::Tick(float fElapsedTime)
{
	//////////////////////////////////////////////////////////////////////////
	// CShuttleChild->Tick()중에서 꼭 필요한 함수들... 
	// 캐릭터일때도 돌아야 하는 함수들
	// 2005-08-02 by ispark

	// 파티 Tick
	if(g_pShuttleChild->m_pClientParty->IsParty())
	{
		g_pShuttleChild->m_pClientParty->Tick(fElapsedTime);
	}
	
	g_pShuttleChild->m_pPkNormalTimer->Tick(fElapsedTime);

	g_pShuttleChild->UnitInfoBarSmoothRepair(fElapsedTime);
	g_pShuttleChild->UsedItemTick(fElapsedTime);
//	g_pShuttleChild->m_pSkill->Tick(fElapsedTime);			// 2005-08-08 by ispark
	// 클라이언트 타이머 계산
	g_pShuttleChild->m_timeProcess.Tick(fElapsedTime);
	g_pShuttleChild->TickBurn(fElapsedTime);				// 2005-08-04 by ispark
	g_pShuttleChild->m_pSkill->Tick(fElapsedTime);			// 2005-12-01 by ispark

	//////////////////////////////////////////////////////////////////////////
	// 현재 상점 이용중이가? 이용중이면 Picking 무시
	// 2006-06-08 by ispark, 상점 체크 변경
	m_bEnterShopState = FALSE;
//	GUI_BUILDINGNPC* pTempBase = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
//	if(pTempBase)
//	{
//		if( m_pShopObject )
//		{
//			m_bPickMove = FALSE;
//			m_bEnterShopState = TRUE;
//			m_dwState = _STAND;
//			m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
////			float fDistance = D3DXVec3Length(&(m_vPos - m_pShopObject->m_vPos));
////			if(fDistance>CHARACTER_SHOP_SERCH_DISTANCE)
////			{
////				m_bPickMove = TRUE;
////				g_pInterface->m_pCityBase->SendLeaveEnterBuilding( g_pInterface->m_pCityBase-> GetCurrentEnterBuildingIndex(), -1 );
////			}
//		}		
//	}
	// 2006-06-13 by ispark, 캐릭터 상태가 _NCITYIN 아니어야 한다. 캐릭터가 처음에 한번은 _NCITYIN 처리해야 한다.
	if(g_pD3dApp->m_dwGameState == _SHOP && m_dwState != _NCITYIN)
	{
		m_bPickMove = FALSE;
		m_bEnterShopState = TRUE;
		m_dwState = _STAND;
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
		 // 2010-06-08 by dgwoo, 펫시스템 추가. 
		//ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
		 // 2010-06-08 by dgwoo, 펫시스템 추가. 
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
  		// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, BODYCON_CHARACTER_MODE_STOP);
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, BODYCON_CHARACTER_MODE_STOP);
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);

	}
	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	//g_pGameMain->m_pMissionInfo->SetEnterShop(m_bEnterShopState);
	g_pGameMain->SetMissionEnterShop(m_bEnterShopState);

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 이벤트 오브젝트
	CheckEventTypeObject(fElapsedTime);

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 이동
	CheckState();
	Move_Character(fElapsedTime);

	CheckShadowPos();
	CheckAniTime(fElapsedTime);
	g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);

	// 스크린상의 좌표를 구한다.
	// 2005-07-29 by ispark
	// 캐릭터 키 높이로 수정
	D3DXVECTOR3 vPos = m_vPos;
	vPos.y += GetCharacterHeight(m_nUnitNum) + 1.0f;

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 카메라 셋팅
	if(g_pShuttleChild->m_bIsCameraPattern != TRUE)
		CameraMoveTick();

//	DBGOUT("m_vPos %f,  %f,  %f\n",m_vPos.x,m_vPos.y,m_vPos.z);
	g_pD3dApp->CalcObjectSourceScreenCoords(vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		g_pShuttleChild->m_nObjScreenX, g_pShuttleChild->m_nObjScreenY, g_pShuttleChild->m_nObjScreenW);
	g_pShuttleChild->m_pMoveChat->Tick();

	// 2006-09-04 by ispark, 개인상점 오픈 말풍선
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->Tick();
	}
}

void CCharacterChild::CheckAniTime(float fElapsedTime)
{
	FLOG( "CShuttleChild::CheckAniTime()" );

	m_pCharacterInfo->Tick(fElapsedTime);
	m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
	m_fCurrentTime = m_pCharacterInfo->GetCurrentCharacterAnimationTime();

	m_pPickingInfo->Tick(fElapsedTime);
//	sprintf( g_pD3dApp->m_strDebug, "0x%016I64x %.3f",GetCurrentBodyCondition(), m_fCurrentTime );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CShuttleChild::Move_Character(float fElapsedTime)
/// \brief		캐릭터 이동
/// \author		ispark
/// \date		2005-07-13 ~ 2005-07-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::Move_Character(float fElapsedTime)
{
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 이동
	// Picking에 의한 처리
	D3DXVECTOR3 vPos = m_vPos;
	vPos.y += CHARACTER_HEIGHT;			// 충돌검사로 인한 높이 값을 올린다.

	CHARACTER myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
	//if( COMPARE_RACE( myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) )
	// end 2010-11-29 by jskim, 펫 장착시 이동속도 증가
	//{	// 관리자 모드의 이동 속도 변경 처리.
		float fTempSpeed = m_fCharacterAddSpeed;
		
		const float a_fMinAddSpeed = .0f;
		const float a_fMaxAddSpeed = 150.0f;
		
	// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
	//if ( GetAsyncKeyState ( VK_ADD ) )				// 키패드 '+'로 속도 증가.
	//	m_fCharacterAddSpeed += 1.0f;
	//else if ( GetAsyncKeyState ( VK_SUBTRACT ) )	// 키패드 '-'로 속도 감소.
	//	m_fCharacterAddSpeed -= 1.0f;
	//else if ( GetAsyncKeyState ( VK_DECIMAL ) )
	//	m_fCharacterAddSpeed += 50.0f;
	//else if ( GetAsyncKeyState ( VK_DIVIDE ) )		// 키배드 '/'로 초기화.
	//	m_fCharacterAddSpeed = .0f;
	if( COMPARE_RACE( myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) )
	{
		// 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
		if ( GetAsyncKeyState ( VK_ADD ) )				// 키패드 '+'로 속도 증가.
			m_fAdminModeSpeed += 1.0f;
		else if ( GetAsyncKeyState ( VK_SUBTRACT ) )	// 키패드 '-'로 속도 감소.
			m_fAdminModeSpeed -= 1.0f;
		else if ( GetAsyncKeyState ( VK_DECIMAL ) )
			m_fAdminModeSpeed += 50.0f;
		else if ( GetAsyncKeyState ( VK_DIVIDE ) )		// 키배드 '/'로 초기화.
			m_fAdminModeSpeed = .0f;				
		// END 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
	}
	// end end 2010-11-29 by jskim, 펫 장착시 이동속도 증가

	// 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
	{
		float fcharSpeed = GetParamFactor_DesParam( g_pShuttleChild->m_paramFactor, DES_RARE_ITEM_PARTNER_SPEED );
		if(0 != fcharSpeed)
		{
			m_fCharacterAddSpeed = GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * fcharSpeed;			
		}
		else
		{
			m_fCharacterAddSpeed = 0.0f;			
		}		
		if(m_fAdminModeSpeed > 0.0f)
		{
			m_fCharacterAddSpeed += m_fAdminModeSpeed;
		}
	}
	// END 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
		
	m_fCharacterAddSpeed = max ( m_fCharacterAddSpeed , a_fMinAddSpeed );
	m_fCharacterAddSpeed = min ( m_fCharacterAddSpeed , a_fMaxAddSpeed );
				
	// 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
	// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
// 	if( m_bPetWearItem )
// 	{
// 		if( m_bSetSpeed == FALSE )
// 		{
// 		// 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
// 			//m_fCharacterAddSpeed += GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * 0.1f;
// 			float fcharSpeed = GetParamFactor_DesParam( g_pShuttleChild->m_paramFactor, DES_RARE_ITEM_PARTNER_SPEED );
// 			m_fCharacterAddSpeed += GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * fcharSpeed;			
// 
// 		// end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
// 			m_bSetSpeed = TRUE;
// 		}
// 	}
// 	else
// 	{
// 		m_fCharacterAddSpeed = 0.0f;
// 		m_bPetWearItem = FALSE;
// 	}
	// end 2010-11-29 by jskim, 펫 장착시 이동속도 증가	
	// END 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선

	// 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
// 2012-06-25 by jhahn, 펫 장착시 이동속도 증가 기능추가
#ifdef S_OPTION_PARTNER__JHAHN		
// 	tPET_CURRENTINFO *pPetCurrentInfo = NULL;
// 	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;
// 	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );
// 	BOOL runSpeed = FALSE;
// 	
// 			
// 		if ( pInvenDisplayInfo )
// 		{
// 			pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData ( pInvenDisplayInfo->pItem->UniqueNumber );
// 			psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData ( pPetCurrentInfo->PetIndex , pPetCurrentInfo->PetLevel );			
// 			CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	
// 
// 			for(int i =0 ; i<6 ;i++)
// 			{
// 				CItemInfo* SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(pPetCurrentInfo->PetSocketItemUID[i] ); 
// 				
//  				if(SoketItem == NULL)
// 					continue;
// 				
// 				else if( SoketItem->GetItemInfo() && SoketItem->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_SPEED )
//  				{
//  					runSpeed = TRUE;
// 					break;
//  				}	
// 				else
// 					runSpeed = FALSE;
// 			}
// 			if (runSpeed == FALSE)
// 			{
// 				PetoptionSpeed = NULL;
// 			}
// 			
// 			if ((pPetCurrentInfo->PetLevel != PetoptionSpeed) && runSpeed)
// 			{
// 				int TempLevel;
// 				if (pPetCurrentInfo->PetLevel / 5 <= 0)
// 				{
// 					TempLevel = 1 ;						
// 				}
// 				else 
// 				{
// 					TempLevel = pPetCurrentInfo->PetLevel;
// 				}
// 				m_fCharacterAddSpeed = (((GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * 0.1f ) + (pPetCurrentInfo->PetLevel/5 ) * 2.1f) );		
// 				PetoptionSpeed = pPetCurrentInfo->PetLevel;
// 			}
// 		
// 		}
// 		else
// 		{
// 
// 			PetoptionSpeed = NULL;
// 			runSpeed = FALSE;
// 		}
		// END 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
#endif
//end 2012-06-25 by jhahn, 펫 장착시 이동속도 증가 기능추가
	
	
	g_cCustomOptimizer.FastMove( m_fCharacterAddSpeed );

	if ( fTempSpeed != m_fCharacterAddSpeed )
	{
		// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
// 		if ( g_pD3dApp->m_pChat )
// 		{
// 			char szTemp[256] = {0, };
// 			sprintf ( szTemp , "Change Character Add MoveSpeed : (%.2f)" , m_fCharacterAddSpeed );
// 			g_pD3dApp->m_pChat->CreateChatChild ( szTemp , COLOR_SYSTEM );
// 		}
		if( COMPARE_RACE( myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) )
		{
			if ( g_pD3dApp->m_pChat )
			{
				char szTemp[256] = {0, };
				sprintf ( szTemp , "Change Character Add MoveSpeed : (%.2f)" , m_fCharacterAddSpeed );
				
				g_pD3dApp->m_pChat->CreateChatChild ( szTemp , COLOR_SYSTEM );
			}
		}
		// end 2010-11-29 by jskim, 펫 장착시 이동속도 증가
			CBodyConditionInfo *pcGetBodyCondition = m_pCharacterInfo->FindBodyConditionInfo ( BODYCON_CHARACTER_MODE_RUN );

			if ( pcGetBodyCondition )
			{

				float fSetFrame = 1.0f + ( 2.0f * ( m_fCharacterAddSpeed / a_fMaxAddSpeed ) );


				pcGetBodyCondition->SetFrameSpeed ( fSetFrame );
			}
	}

	if(m_dwState == _STAND)
	{
		m_fCharacterSpeed = 0.0f;
	}
	if(m_bPickMove)
	{
		D3DXMATRIX  matTemp;
		double dRadian = 0.0f;
		float dot = 0.0f;
		D3DXVECTOR3 vCrossVec;
		D3DXVECTOR3 vNewDir = m_vPickDir;

		D3DXVec3Cross(&vCrossVec, &m_vVel, &vNewDir);
		D3DXVec3Normalize(&vNewDir, &vNewDir);
		dot = D3DXVec3Dot(&m_vVel, &vNewDir);

		if(m_dwState != _RUN)
		{
			if(1.0f > dot)
			{
				if(0.0f > vCrossVec.y)
				{
					//오른쪽 회전  
					dRadian -= ACOS(dot);
				}
				else	
				{
					// 왼쪽 회전
					dRadian += ACOS(dot);
				}

				D3DXMatrixRotationY(&matTemp, (float)dRadian);
				D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
			}
			// 일단 테스트로 뛰는 행동을 준다.
			// 2005-07-29 by ispark
			// Picking 허용 거리를 새로 셋팅
			m_fOldLength = CHARACTER_PICKING_LENGTH;
			m_dwState = _RUN;
			// 2005-07-29 by ispark
			// 뛰기 바디 컨디션을 WndProc에서 여기로 옮겼다.
			m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_RUN);
			// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
			ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_RUN);
			// 2010-06-08 by dgwoo, 펫시스템 추가.
		}
		// 펫 관련 - 캐릭터 이동 (마우스 우 클릭)시 Tick() 지연되는 문제 수정. 2010. 10. 08. by jskim.
		if(m_dwState == _RUN)
		{
			D3DXVECTOR3 vMovingVec;
			vMovingVec = m_vPicking - m_vPos;
			FLOAT lenght = D3DXVec3Length(&vMovingVec);

			// 2005-07-29 by ispark
			// 보정 거리 수정 5.0f -> 10.0f
			if(10.0f >= lenght)
			{
				// 한번더 회전 검사후 거리 0.5에서 정지 시킨다.
				if(1.0f > dot && 0.5f < lenght)
				{
					if(0.0f > vCrossVec.y)
					{
						//오른쪽 회전  
						dRadian -= ACOS(dot);
					}
					else	
					{
						// 왼쪽 회전
						dRadian += ACOS(dot);
					}

					D3DXMatrixRotationY(&matTemp, (float)dRadian);
					D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
				}
			}
			
			if((0.5f > lenght) || (m_fOldLength < lenght))
				{
//					g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가. 
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
					//ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가. 
					m_dwState = _STAND;
					m_bPickMove = FALSE;
//					m_vPos = m_vPicking;
				}
			else
			{
				// 캐릭터는  Picking때만 움직인다.
				CheckMoveRate(fElapsedTime);
				m_vPos += (m_vNextPos - m_vPos) * fElapsedTime;
				m_fOldLength = lenght;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 오브젝트 충돌
	// 여기서 오브젝트는 상점 내부이다.
	// 캐릭터는 오브젝트 위에 존재한다는 정의로 시작한다. 오브 높이 산출
	// 2006-07-14 by ispark, 캐릭터 워프 중에는 오브젝트 충돌 검사를 안한다.
	if(m_dwState == _WARP)
	{
		// 워프 중에 이동
		m_vPos += m_vMoveVel * fElapsedTime;
	}
	else if(!m_bStartPositionEvent)						// 2006-07-20 by ispark, 시작 위치가 이벤트 위치가 아니라면
	{
		float fTempHeight = 0.0f;
		float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
		
		COLLISION_RESULT GroundCollResult;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - vUp),&vSide);
	//	fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist - CHARACTER_HEIGHT;
		GroundCollResult = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos);

		fTempHeight = GroundCollResult.fDist - CHARACTER_HEIGHT;

		if(GroundCollResult.vNormalVector.y > 0.7f)
		{
			if(fTempHeight<m_vPos.y)
				m_vPos.y += -fTempHeight;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 이동 범위 제한
	// 다음 계산한 오브젝트와의 거리로 허용 범위를 계산한다.
	// _NCITYIN 처음 상점에 들어올때는 이동 범위 제한 검사를 안하다.
	// 할경우 캐릭터를 오브젝트 위에 띄우는데 문제가 생김
	// 서버로 부터 Y값을 제외한 값만 받고 클라이언트에서 Y값을 계산
	// 그러나 상점 높이가 변할 수 있는 관계로 꽤 높은 값으로 설정
	// 캐릭터가 나타나는 위치에는 위쪽으로 오브젝트가 없다는 존재하에 설정
	// 2006-07-14 by ispark, 캐릭터 워프 중에는 오브젝트 충돌 검사를 안한다.
	if(m_dwState != _NCITYIN && m_dwState != _WARP)
	{
		float fCollResult;
		COLLISION_RESULT CollResult;
		D3DXMATRIX matArray[2];
		D3DXVECTOR3 vCollSide,vVel,vFrontPos, vDownPos;
		
		vFrontPos = vPos;			// 사람 키높이를 생각하자
		vDownPos = vPos;			// 절벽를 계산하기 위해서 내 위치 보다 더 앞쪽 좌표 입력
		vDownPos += m_vVel * 5;

		D3DXVec3Cross(&vCollSide,&m_vUp,&m_vVel);
		
		D3DXMatrixLookAtLH(&matArray[0],&vFrontPos,&(vFrontPos + m_vVel),&m_vUp);		// 앞
		D3DXMatrixLookAtLH(&matArray[1],&vDownPos,&(vDownPos - m_vUp),&vCollSide);		// 아래
		
		float fFrontMove = m_fCharacterSpeed*fElapsedTime;
		float size[2] = {7 + fFrontMove, CHARACTER_HEIGHT + 4.0f};	// 유닛 크기	앞, 아래
		
		// 오브젝트와 충돌 처리 2004.06.18 jschoi
		
		if(	/*m_bCollObjectCheck && */
			g_pScene &&
			g_pScene->m_byMapType != MAP_TYPE_TUTORIAL &&
			g_pScene->m_pObjectRender)
		{
			// 정면
			CollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[0], vPos);	
			// 2005-07-29 by ispark
			// 오브젝트안에서 나오지 못하는 버그 예외 처리(나오게 함)
//			DBGOUT("방향 %f %f %f\n", m_vVel.x, m_vVel.y, m_vVel.z);
//			DBGOUT("노말 %.2f %.2f %.2f\n", CollResult.vNormalVector.x, CollResult.vNormalVector.y, CollResult.vNormalVector.z);
//			DBGOUT("거리 %f\n", CollResult.fDist);
			float dot = D3DXVec3Dot(&m_vVel, &CollResult.vNormalVector);
			if(0.0f >= dot)
			{
//				DBGOUT("방향 %f %f %f\n", m_vVel.x, m_vVel.y, m_vVel.z);
//				DBGOUT("노말 %f %f %f\n", CollResult.vNormalVector.x, CollResult.vNormalVector.y, CollResult.vNormalVector.z);
//				if(CollResult.fDist < size[0] && m_dwState != _STAND)
				// 2005-08-04 by ispark
				// 경사 각도로 계산
				if((CollResult.fDist < size[0] && CollResult.vNormalVector.y <= 0.7f) && m_dwState != _STAND)
				{
					// size[0] 안에 있으면 충돌
					m_vPos = vPos;
					m_vPos.y -= CHARACTER_HEIGHT;

					m_dwState = _STAND;
					m_bPickMove = FALSE;
//					g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가. 
				}
			}

			// 절벽
			fCollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[1], m_vPos).fDist - CHARACTER_HEIGHT;
			if(fCollResult > size[1] && m_dwState != _STAND)
			{
				// size[1] 허용 수치보다 크면 정지
				m_vPos = vPos;
				m_vPos.y -= CHARACTER_HEIGHT;

				m_dwState = _STAND;
				m_bPickMove = FALSE;
//				g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
				m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
				ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
				// 2010-06-08 by dgwoo, 펫시스템 추가.
			}
			else if(m_vPos.y < vPos.y - CHARACTER_HEIGHT - 5.0f)		// 지금 떨어지는 위치인가
			{															// 5.0f은 경사면이 아닌것을 확인하기 위해서
				// 허용 수치이면 부드럽게 떨어진다.
				// 2005-08-04 by ispark
				// 앞쪽 방향은 조금 움직인다.
				D3DXVECTOR3 vVel = m_vNextPos - vPos;
				D3DXVec3Normalize(&vVel, &vVel);
				m_vPos = vPos + (vVel) * fElapsedTime;
				m_vPos.y = vPos.y - CHARACTER_HEIGHT - (CHARACTER_HEIGHT * fElapsedTime * 6.0f);
			}
		}
	}
	else if(m_dwState == _NCITYIN)
	{
		// Move_Character()에서 처음으로 시작해야 하는 부분
		// _NCITYIN 이라면 _STAND로 바꾼다. 
		// _NCITYIN 한번만 사용
		m_dwState = _STAND;
		m_bPickMove = FALSE;
		m_bStartPositionEvent = FALSE;
		// 카메라 초기 셋팅
		// InitCharacterData()에서 하지 않는 이유는 착륙후 카메라 위치에 오차가 생김
		g_pCamera->SetCamDistance(CHARACTER_CAMERA_DISTANCE);		// 거리
		g_pD3dApp->SetCamPosInit();
	}

	//////////////////////////////////////////////////////////////////////////
	// 이벤트 지역 체크 - Shop
	int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
	int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
	if((g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType != EVENT_TYPE_ENTER_BUILDING &&
		g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType != EVENT_TYPE_ENTER_BUILDING_BAZAAR) &&
		m_dwState != _STAND)
	{
		m_vPos = vPos;
		m_vPos.y -= CHARACTER_HEIGHT;
		
		m_dwState = _STAND;
		m_bPickMove = FALSE;
//		g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
		// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
		ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
		// 2010-06-08 by dgwoo, 펫시스템 추가.

	}

	m_bBazaarEventPos = FALSE;
	if(g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType == EVENT_TYPE_ENTER_BUILDING_BAZAAR)
	{
		m_bBazaarEventPos = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	SetMatrix_Move_Character(fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	// ShuttleChild와 같이 입력해야 한다.
	g_pShuttleChild->m_vPos = m_vNextPos = m_vPos;
	g_pShuttleChild->m_vVel = m_vVel;
	g_pShuttleChild->m_vUp = m_vUp;
	g_pShuttleChild->m_vSideVel;

	// 2005-07-28 by ispark
	// Move 패킷을 보낼때 Up 벡터를 Metrix에서 가져온다. 
	g_pShuttleChild->SetMatrix_Move_Ground(fElapsedTime);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CShuttleChild::SetMatrix_Move_Character(float fElapsedTime)
/// \brief		캐릭터 메트릭스 셋팅
/// \author		ispark
/// \date		2005-07-13 ~ 2005-07-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::SetMatrix_Move_Character(float fElapsedTime)
{
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
	D3DXVec3Cross(&m_vVel,&m_vSideVel,&m_vUp);

	D3DXMatrixLookAtRH(&m_mMatrix, &m_vPos, &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix );

	D3DXMATRIX matTemp;
	D3DXMatrixScaling(&matTemp, CHARACTER_SCALE, CHARACTER_SCALE, CHARACTER_SCALE);
	m_mMatrix = matTemp * m_mMatrix;
}

void CCharacterChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CCharacterChild::WndProc()" );
	// 2006-07-14 by ispark, 워프시 프로시저 금지
	if(m_dwState == _WARP)
		return;
	
	float fElapsedTime = g_pD3dApp->GetElapsedTime();

	if(!g_pD3dApp->m_bChatMode)// && m_bMoveInputType == 0)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			{
				switch(lParam)
				{
				case WM_KEYDOWN_LPARAM_Z:
					{
						if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() == TRUE ||
							g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
						{
							// 2008-10-14 by bhsohn VOIP개선							
//							COPYDATASTRUCT	copyData;
//							memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//							AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK, 0);
//							copyData.dwData = (DWORD)voIP;
//							copyData.lpData = NULL;
//							copyData.cbData = 0;
//							::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
							g_pGameMain->ChangeVOIPMode();
						}
					}
					break;
				}

//				switch(wParam)
//				{
//				case VK_ADD:
//					{
//						m_fCharacterMoveRate += 0.1f;
//					}
//					break;
//				case VK_SUBTRACT:
//					{
//						m_fCharacterMoveRate -= 0.1f;
//					}
//					break;
//				}
			}
		case WM_KEYUP:
			{
				switch(lParam)
				{
				case WM_KEYUP_LPARAM_Z:
					{
						// 2008-10-14 by bhsohn VOIP개선							
//						if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() == TRUE ||
//							g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
//						{
//							COPYDATASTRUCT	copyData;
//							memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//							AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK_CANCEL, 0);
//							copyData.dwData = (DWORD)voIP;
//							copyData.lpData = NULL;
//							copyData.cbData = 0;
//							::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//						}
						// end 2008-10-14 by bhsohn VOIP개선							
					}
				}
				break;
			}
			break;
		case WM_MBUTTONDOWN:
			{				
			}
			break;
		case WM_MBUTTONUP:
			{
			}
			break;
		case WM_MOUSEWHEEL:
			{
				// 2005-08-18 by ispark
				// 캐릭터 카메라 휠
				m_bMouseWheel = TRUE;
				float nDist = g_pCamera->GetCamDistance();
				float fCollDist = g_pCamera->GetCollDistance();			// 2005-08-19 by ispark

				if((int)wParam > 0)
				{
					// 2005-08-19 by ispark
					// 충돌 거리 있을 때 카메라 뒤로 가기 계산 안함
					if(fCollDist != 0 && fCollDist < nDist)
					{
						break;
					}

					float fDistMax = CHARACTER_CAMERA_DISTANCE_MAX;					
#ifdef _DEBUG
					fDistMax = 600.0f;
#endif
					if(nDist + CHARACTER_CAMERA_WHEEL_DISTANCE > fDistMax)
					{
						nDist = fDistMax;
						g_pCamera->SetCamDistance(nDist);
					}
					else
					{
						g_pCamera->SetCamDistance(nDist + CHARACTER_CAMERA_WHEEL_DISTANCE);
					}
				}
				else
				{
					// 2005-08-19 by ispark
					// 가까이 갈 때만 충돌 거리 적용(실제 거리 삭제)
					if(fCollDist != 0 && fCollDist < nDist)
					{
						if(fCollDist < CHARACTER_CAMERA_DISTANCE_MIN)
						{
							g_pCamera->SetCamDistance(fCollDist);
							break;
						}

						nDist = fCollDist;
					}

					if(nDist - CHARACTER_CAMERA_WHEEL_DISTANCE < CHARACTER_CAMERA_DISTANCE_MIN)
					{
						nDist = CHARACTER_CAMERA_DISTANCE_MIN;
						g_pCamera->SetCamDistance(nDist);
					}
					else
					{
						g_pCamera->SetCamDistance(nDist - CHARACTER_CAMERA_WHEEL_DISTANCE);
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
				CheckMouseReverse(&pt);
	
				// Pick
				// 2005-08-25 by ispark, 팝업 메뉴 띄우상태라면
				// 2006-06-20 by ispark, 서버로 부터 응답을 기다리는 중이라면 캐릭터가 움직이지 못하게 한다.
				// (인터페이스 창을 누르면 Picking이 되기 때문이다.) 
				// 2007-12-17 by bhsohn 아이템 정보가 다 안오면 게임 대기상태로
				//if(g_pGameMain->m_bMenuLock || g_pD3dApp->m_bRequestEnable == FALSE)
				if(g_pGameMain->m_bMenuLock || g_pD3dApp->IsLockMode() == FALSE)
					break;

				// 2005-09-14 by ispark
				// 기본인터페이스 밑줄 클릭을 막는다.
				if(g_pInterface->m_bShowInterface && 
					(pt.x >= 0 && pt.x <= g_pD3dApp->GetBackBufferDesc().Width) &&
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
					(pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 70) || // #define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			70 -> INFGameMainChat에 있음
#else				 
					(pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 39) ||
#endif
					((pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 51) && 
					(pt.x >= g_pD3dApp->GetBackBufferDesc().Width / 2 - 117 &&
					pt.x <= g_pD3dApp->GetBackBufferDesc().Width / 2 + 117)))
				{
					break;
				}
					
				m_dwState = _STAND;
				
				COLLISION_RESULT Coll_Result_Pick;
				Coll_Result_Pick = g_pScene->m_pObjectRender->CheckPickMesh(pt.x, pt.y);

				// 2007-07-27 by bhsohn 프리스카 맵만 상점 인식 범위 증가
				float fSearchLength = CHARACTER_SHOP_SERCH_DISTANCE;
				// 2009. 10. 14 by jskim 프리스카 제거 
// 				if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
// 				{
// 					fSearchLength = CHARACTER_SHOP_SERCH_FREESKA_DISTANCE;
// 				}
				//end 2009. 10. 14 by jskim 프리스카 제거 
				//if(!CheckEnterShopNPC(&pt, CHARACTER_SHOP_SERCH_DISTANCE, TRUE))
				if(!CheckEnterShopNPC(&pt, fSearchLength, TRUE))
				{
					// Picking을 바닥으로 제한
	//				DBGOUT("벡터 %.2f %.2f %.2f\n", Coll_Result_Pick.vPicking.x, Coll_Result_Pick.vPicking.y, Coll_Result_Pick.vPicking.z);
	//				DBGOUT("노말 %.2f %.2f %.2f\n", Coll_Result_Pick.vNormalVector.x, Coll_Result_Pick.vNormalVector.y, Coll_Result_Pick.vNormalVector.z);
					if(Coll_Result_Pick.vNormalVector.y > 0.7f)
					{
						m_bPickMove = TRUE;
						m_vPicking = Coll_Result_Pick.vPicking;
						D3DXVECTOR3 vPosTemp = m_vPos;
						D3DXVECTOR3 vPickTemp = m_vPicking;
						// Picking 위치 셋팅
						SetMatrix_Picking(m_vPicking, Coll_Result_Pick.vNormalVector);
						m_pPickingInfo->SetSingleBodyConditionMatrix(BODYCON_LANDED_MASK, m_mPickMatrix);
						
						vPosTemp.y = vPickTemp.y = 0.0f;
						m_vPickDir = vPickTemp - vPosTemp;
					}
				}
			}
			break;
		case WM_LBUTTONUP:
			{
			}
			break;
		case WM_RBUTTONDOWN:
			{
				if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
				{
					g_pCamera->m_bIsCamControl = TRUE;
					POINT pt;
					pt.x = LOWORD(lParam);
					pt.y = HIWORD(lParam);
					CheckMouseReverse(&pt);

					m_pOldMousePoint = pt;
				}
				
				m_bRButtonState = TRUE;
			}
			break;
		case WM_RBUTTONUP:
			{
				// 카메라 전환 모드 해제
				if(g_pD3dApp->m_pCamera->m_bIsCamControl)
				{
					g_pD3dApp->m_pCamera->m_bIsCamControl = FALSE;
				}
				m_bRButtonState = FALSE;
			}
			break;
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				// 2007-07-27 by bhsohn 프리스카 맵만 상점 인식 범위 증가
				//CheckEnterShopNPC(&pt, CHARACTER_SHOP_SERCH_DISTANCE);
				float fSearchLength = CHARACTER_SHOP_SERCH_DISTANCE;
				// 2009. 10. 14 by jskim 프리스카 제거 
				//if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
				//{
				//	fSearchLength = CHARACTER_SHOP_SERCH_FREESKA_DISTANCE;
				//}				
				//end 2009. 10. 14 by jskim 프리스카 제거  
				CheckEnterShopNPC(&pt, fSearchLength);
				
				// end 2007-07-27 by bhsohn 프리스카 맵만 상점 인식 범위 증가

				

				m_bMouseMove = TRUE;

//				if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
//				{
//					if( g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL &&
//						g_pD3dApp->m_pCamera->m_bIsCamControl && 
//						m_bRButtonState)
//					{
//						float fDirY = 0.0f, fDirX = 0.0f;
//						fDirY = m_pOldMousePoint.y - pt.y;
//						fDirX = m_pOldMousePoint.x - pt.x;
//						if(fDirY != 0)
//						{
//							// 캐릭터 카메라 회전시 오브젝트를 통과를 하는데 그이유는 마우스 이동이 커서 그렇다.
//							// 그래서 제한을 두었다.
//							if(20 < fDirY)
//								fDirY = 20;
//							if(-20 > fDirY)
//								fDirY = -20;
//						}
//						g_pCamera->SetAngleY(fDirY);
//						
//						if(fDirX != 0)
//						{
//							if(20 < fDirX)
//								fDirX = 20;
//							if(-20 > fDirX)
//								fDirX = -20;					
//						}
//						g_pCamera->SetAngleX(fDirX);
//					}
//				}
//				m_pOldMousePoint = pt;
			}
			break;
		}

	}
}

void CCharacterChild::CheckShadowPos()
{
	// 여기서 수치는 그림자를 뿌릴 사이즈를 뜻한다. 원점에서 대각선 방향으로 거리이다.
	m_vShadowPos[0] = m_vPos + CHARACTER_SHADOW_SCALE * (m_vVel - m_vSideVel);
	m_vShadowPos[1] = m_vPos + CHARACTER_SHADOW_SCALE * (m_vVel + m_vSideVel);
	m_vShadowPos[2] = m_vPos - CHARACTER_SHADOW_SCALE * (m_vVel - m_vSideVel);
	m_vShadowPos[3] = m_vPos - CHARACTER_SHADOW_SCALE * (m_vVel + m_vSideVel);

	for(int i = 0 ; i < 4 ; i++)
	{
		m_vShadowPos[i].y += CHARACTER_HEIGHT;
		float fTempHeight = 0.0f;
		float fObjHeight = -DEFAULT_COLLISION_DISTANCE;

		D3DXMATRIX matTemp;
		D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matTemp,&m_vShadowPos[i],&(m_vShadowPos[i] - vUp),&vSide);
		fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vShadowPos[i]).fDist - CHARACTER_HEIGHT;

		if(fTempHeight <= 1.0f && fTempHeight >= -1.0f)
		{
			m_vShadowPos[i].y += -fTempHeight;
		}
		else
		{
			if(fTempHeight > 0) 
			{
				m_vShadowPos[i].y += -fTempHeight;
			}
			else
			{
				float fRealObjectHeight = m_vShadowPos[i].y - fTempHeight;
				if( fRealObjectHeight > m_vShadowPos[i].y )
				{
					m_vShadowPos[i].y = fRealObjectHeight;
				}
			}
		}

		// 그림자는 Z버퍼를 사용하므로 그림자 높이를 0.1f만큼 올린다.
		if(g_pD3dApp->m_nDephbias == -1)
		{
			m_vShadowPos[i].y -= (CHARACTER_HEIGHT - 0.1f);	
		}
		else
		{
			m_vShadowPos[i].y -= CHARACTER_HEIGHT;
		}
	}

	SPRITEVERTEX* pV;
	m_pVBShadow->Lock( 0, 0, (void**)&pV,	0 );
	pV[0].p = m_vShadowPos[3]; 
	pV[1].p = m_vShadowPos[0]; 
	pV[2].p = m_vShadowPos[2]; 
	pV[3].p = m_vShadowPos[1];
	m_pVBShadow->Unlock();
}

void CCharacterChild::CheckMoveRate(float fElapsedTime)
{
	if(m_dwState == _WALK)
	{
		m_fCharacterSpeed = CHARACTER_WALK;
	}
	else if(m_dwState == _RUN)
	{
//		// 2006-02-09 by ispark, 남자, 여자 움직임 속도 다름
//		if(g_pShuttleChild->m_myShuttleInfo.PilotFace < 100)
//		{
//			m_fCharacterSpeed = CHARACTER_WOMAN_RUN;		
//		}
//		else
//		{
//			m_fCharacterSpeed = CHARACTER_MAN_RUN;		
//		}
		// 2006-11-02 by ispark, 캐릭터 별로 움직임으로 변경
		m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace);

		m_fCharacterSpeed += m_fCharacterAddSpeed;

// 		m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace) + m_fCharacterMoveRate;
	}

	m_vNextPos += m_vVel * m_fCharacterSpeed;
}

void CCharacterChild::InitEffect()
{
	FLOG( "CCharacterChild::InitEffect()" );
	char strFileName[16];
//	CItemInfo* pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//	// 2005-07-20 by ispark
//	// 캐릭터 렌더링 번호를 입력한다. 여기에 이펙트 포함
////	ChangeUnitCharacterInfo((pItem?pItem->ItemNum:0), g_pShuttleChild->m_myShuttleInfo.PilotFace, GetCurrentBodyCondition(), FALSE);
	int nTemp = ::GetUnitNum(0, 0, g_pShuttleChild->m_myShuttleInfo.PilotFace, TRUE);

//	nTemp = 20000100;
	if(nTemp != m_nUnitNum)
	{
		m_nUnitNum = nTemp;
		wsprintf( strFileName, "%08d", m_nUnitNum );
		LoadCharacterEffect(&m_pCharacterInfo, strFileName);
		if(m_pCharacterInfo)
		{
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_HUMAN_ANIMATION_TIME);
			m_pCharacterInfo->SetBodyCondition(m_hyBodyCondition);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Picking 이펙트6
	wsprintf(strFileName, "%08d", PICKING_OBJECT_NUMBER);
	LoadCharacterEffect(&m_pPickingInfo, strFileName);
	if(m_pPickingInfo)
	{
		m_pPickingInfo->SetAllBodyConditionMatrix(m_mPickMatrix);
		m_pPickingInfo->SetBodyCondition(BODYCON_LANDED_MASK);
	}
}

void CCharacterChild::InitCharacterData()
{
	D3DXVECTOR3 vTempPos;
	m_vUp = D3DXVECTOR3(0, 1, 0);
	vTempPos.x = g_pGround->m_projectInfo.sXSize*TILE_SIZE/2.0f+rand()%100;
	vTempPos.y = m_vPos.y;
	vTempPos.z = g_pGround->m_projectInfo.sYSize*TILE_SIZE/2.0f+rand()%100;
	D3DXVec3Normalize(&m_vVel,&(vTempPos-m_vPos));
	// 2007-07-04 by dgwoo 프리스카 맵에서만 바라보는 방향을 반대 방향으로 변경.
	// 2009. 10. 14 by jskim 프리스카 제거 
// 	if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
// 	{
// 		m_vVel = -m_vVel;
// 	}
	//end 2009. 10. 14 by jskim 프리스카 제거 
	DBGOUT("CCharacterChild,m_vPos(%.2f, %.2f, %.2f)\n", m_vPos.x, m_vPos.y, m_vPos.z);
	DBGOUT("CCharacterChild,m_vVel(%.2f, %.2f, %.2f)\n", m_vVel.x, m_vVel.y, m_vVel.z);
	D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);        

	m_dwState = _NCITYIN;
	g_pShuttleChild->ChangeUnitState( _LANDED );
	m_bPickMove = FALSE;										// 처음에는 Picking 상태 아님
	m_bCharacterRender = TRUE;
	m_bBazaarEventPos = FALSE;
		
	InitEffect();
	g_pShuttleChild->InitItemPoint();									// 2006-07-21 by ispark
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );	// 2005-08-19 by ispark
	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 높이 셋팅
//	D3DXVECTOR3 vPos = m_vPos;
//	vPos.y += CHARACTER_HEIGHT;

	// 초기에는 서 있는 애니메이션
//	g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHARACTER_MODE_STOP);
	m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
	// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
	ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
	// 2010-06-08 by dgwoo, 펫시스템 추가.

	// 초기화
//	float fTempHeight = 0.0f;
//	float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
//	
//	D3DXMATRIX matTemp;
//	D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
//	D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - vUp),&vSide);
//	fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist - CHARACTER_HEIGHT;
//	
//	if(fTempHeight<m_vPos.y)
//	m_vPos.y += -fTempHeight;

	m_vNextPos = m_vPos;
	//////////////////////////////////////////////////////////////////////////
	// CShuttleChild 초기화
	// 착륙전 시점이 1인칭이였으면 3인칭으로 변환 
	if(g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS)
	{
		g_pD3dApp->m_pCamera->SetCamType(CAMERA_TYPE_NORMAL);
		g_pShuttleChild->m_bLandingOldCamtypeFPS = TRUE;
	}
	
	g_pShuttleChild->m_vPos = m_vPos;
	g_pShuttleChild->m_vVel = m_vVel;

	g_pShuttleChild->InitCharacterToShuttleData();				// 2005-11-10 by ispark

	//////////////////////////////////////////////////////////////////////////
	// 기타
	g_pInterface->m_pGameMain->InitShowWindow();				// 2005-08-05 by ispark

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	{
		//g_pInterface->m_bShowInterface = TRUE;						// 2005-08-30 by ispark	
		//BOOL bShowInter = !g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_HIDE);
		//g_pInterface->SetShowInterface(bShowInter);		
	}

	// 핵 검사를 위한 좌표 초기화
	g_pD3dApp->m_vShuttleOldPos = m_vPos;


//	// 2006-02-09 by ispark, 남자, 여자 움직임 속도 다름
//	if(g_pShuttleChild->m_myShuttleInfo.PilotFace < 100)
//	{
//		m_fCharacterSpeed = CHARACTER_WOMAN_RUN;		
//	}
//	else
//	{
//		m_fCharacterSpeed = CHARACTER_MAN_RUN;		
//	}
	// 2006-11-02 by ispark, 캐릭터 별로 움직임으로 변경
	m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace);

	m_nStartEventType = EVENT_TYPE_NOEVENT;
	m_nSelBazaarType = 0;
	m_ShopEnemyClientIndex = 0;
	m_fCharacterWarpErrTime	= 0.0f;
}

void CCharacterChild::LoadCharacterEffect(CCharacterInfo** pCharacterInfo, char *strFileName)
{
	if((*pCharacterInfo))
	{
		(*pCharacterInfo)->InvalidateDeviceObjects();
		(*pCharacterInfo)->DeleteDeviceObjects();
		SAFE_DELETE(*pCharacterInfo);
	}
	(*pCharacterInfo) = new CCharacterInfo();
	if((*pCharacterInfo)->Load(strFileName))
	{
		(*pCharacterInfo)->InitDeviceObjects();
		(*pCharacterInfo)->RestoreDeviceObjects();
	}
	else
	{
		SAFE_DELETE(*pCharacterInfo);
		DBGOUT("WARNING!! [EFFECT ERROR] Can't Find Effect File : %s \n",strFileName);
	}
}

HRESULT CCharacterChild::RestoreShadow()
{
	DWORD	dwColor = 0xFFFFFFFF;
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITEVERTEX ),
		0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &m_pVBShadow,NULL ) ) )
	{
		g_pD3dApp->CheckError(CLIENT_ERR_SHUTTLE_EFFECTRESTORE);
		return E_FAIL;
	}
	SPRITEVERTEX* v;
	m_pVBShadow->Lock( 0, 0, (void**)&v, 0 );
	v[0].p = D3DXVECTOR3(0,0,0);  v[0].color=dwColor;	v[0].tu=0; v[0].tv=1;	
	v[1].p = D3DXVECTOR3(0,0,0);  v[1].color=dwColor;	v[1].tu=0; v[1].tv=0;	
	v[2].p = D3DXVECTOR3(0,0,0);  v[2].color=dwColor;	v[2].tu=1; v[2].tv=1;	
	v[3].p = D3DXVECTOR3(0,0,0);  v[3].color=dwColor;	v[3].tu=1; v[3].tv=0;	
	m_pVBShadow->Unlock();

	return S_OK;
}

void CCharacterChild::FineObjectTakeOff()
{
	if(m_bEnterShopState == TRUE)
		return;
	CObjectChild * pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_TAKEOFF_PATTERN_START, m_vPos, 10240.0f);
	
	if(pCurrentObjEvent)
	{
		CObjectChild* pObj = g_pScene->FindObjectByIndex(pCurrentObjEvent->m_sEventIndexFrom);
		
		if(pObj)
		{
		    m_vPos = pObj->m_vPos;
			// 기어 모드 변환
			g_pD3dApp->m_bCharacter = FALSE;
			g_pInterface->m_pGameMain->InitWarp();
			g_pShuttleChild->m_vPos = m_vPos;
			g_pShuttleChild->InitShuttleData(TRUE);
			g_pShuttleChild->m_vPos.y = m_vPos.y;
			g_pShuttleChild->m_bWarpLink = TRUE;
		}
	}
	
}

void CCharacterChild::CheckState()
{
	if(g_pShuttleChild->m_dwState == _WARP)
	{
		EVENT_POINT UnitPoint, CameraPoint;
		BOOL bResult = FALSE;
//		if(g_pShuttleChild->m_pCinemaUnitPattern != NULL)
//		{
//			bResult = g_pShuttleChild->m_pCinemaUnitPattern->SkillTick();
//			UnitPoint = g_pShuttleChild->m_pCinemaUnitPattern->GetCurrentCinemaPoint();
//			m_vPos = UnitPoint.vPosition;				// 위치
//			m_vVel = UnitPoint.vDirection;				// 방향
//			m_vUp = UnitPoint.vUpVector;
//			m_vTargetPosition = UnitPoint.vTarget;			
//		}

		if(g_pShuttleChild->m_bIsCameraPattern == TRUE)
		{
			bResult = g_pShuttleChild->m_pCinemaCamera->SkillTick();
			D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
			CameraPoint = g_pShuttleChild->m_pCinemaCamera->GetCurrentCinemaPoint();
			vCameraPos = CameraPoint.vPosition;
			vCameraVel = m_vPos - CameraPoint.vPosition;
			vCameraUp = CameraPoint.vUpVector;
			g_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
			
		}
//		m_fEventCheckTime = 2.0f;
		if(bResult)	// 카메라 패턴이 끝나던 아니면 유닛 패턴이 끝나면 종료한다.
		{
			switch(g_pShuttleChild->m_nEventType)
			{
			case EVENT_GAME_END:
				{
					g_pShuttleChild->SetSelectCharacterItem();
					g_pD3dApp->SendFieldSocketCharacterGameEnd(FALSE);	// 2015-09-26 Future, standardized Game closing
				}
				break;
			}
//			m_bEventReady = FALSE; WARP_OK에서 한다.
			
		}
	}
}

void CCharacterChild::SetMatrix_Picking(D3DXVECTOR3 vPos, D3DXVECTOR3 vNorPos)
{
	// 오브젝트 좌표가 Z방향으로 90도 회전 되어 있는 상태이다.
	// 이부분은 툴에서 문제가 있다.
	// 여기서 vNorPos를 90도 회전 시킨 것이 vDir(방향) 값이 된다.
//	D3DXMATRIX matTemp;
//	D3DXVECTOR3 vDir;
//	D3DXMatrixRotationZ(&matTemp, D3DXToRadian(90));
//	D3DXVec3TransformCoord(&vDir, &vNorPos, &matTemp);

	// LookAtRH에서 Up값을 x축 기준으로 주었다.
	vPos.y += 0.3f;
	D3DXMatrixLookAtRH(&m_mPickMatrix, &vPos, &(vPos + vNorPos), &D3DXVECTOR3(1, 0, 0));
	D3DXMatrixInverse(&m_mPickMatrix, NULL, &m_mPickMatrix );
}

BOOL CCharacterChild::CheckEnterShopNPC(POINT *pt, float fLength, BOOL nFlag)
{
	int nShopIndex = 0;
	BOOL bCheckShopObj = FALSE;

	// 2005-12-26 by ispark
	// 거래중이면 상점을 열지 못하게 한다.
	if(g_pGameMain->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
		return FALSE;
	
	// 2005-11-11 by ispark
	// 리턴이 TRUE이면 상점 사용
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return TRUE;

	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt->x,pt->y);
	COLLISION_OBJ_RESULT stObject;
	CObjectChild * NPCtempObj = NULL;
	m_pSelectObject = NULL;

	stObject = g_pScene->m_pObjectRender->GetPickObject(pt->x, pt->y);
	
	if(stObject.pObject)
	{
		float fDistanceScene = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
//		DBGOUT("ObjNum %d, Pick EventType %d(dis = %f)\n", stObject.pObject->m_nCode, stObject.pObject->m_bEventType, fDistanceScene);
		if(stObject.pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<100.0f)
		{
//			m_nSelectObjectIndex = tempObj->m_pObjectInfo->RenderIndex; // 성공	
			m_pSelectObject = stObject.pObject; // 성공	
			
//			float fDistance = D3DXVec3Length(&(m_vPos - tempObj->m_vPos)) - tempObj->m_pObjMesh->m_fRadius;
			float fDistance = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
			if( fDistance<fLength && nFlag)
			{
				nShopIndex = stObject.pObject->m_sEventIndexTo;
#ifdef CUSTOM_OPTIMIZER_HSSON
				g_cCustomOptimizer.m_nMyInfiShopIndex = stObject.pObject->m_sEventIndexTo;
#endif
			}

			bCheckShopObj = TRUE;
		}
	}

	// 2006-09-14 by ispark, 두번째 검사
	NPCtempObj = g_pScene->m_pObjectRender->GetPickObjectShopNPC(pt->x, pt->y);
	if(NPCtempObj && bCheckShopObj == FALSE)
	{
		
		float fDistanceScene = D3DXVec3Length(&(m_vPos - NPCtempObj->m_vPos));
		if(NPCtempObj->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<10.0f)	// 2012-10-10 by mspark, 다른 층의 건물 클릭되는 문제 수정 - 기존 100.0f에서 10.0f로 변경
		{
			m_pSelectObject = NPCtempObj; // 성공	
			
			float fDistance = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
			if( fDistance<fLength && nFlag)
			{
				nShopIndex = NPCtempObj->m_sEventIndexTo;
			}
		}
	}
		
	// 2007-08-21 by bhsohn 캐쉬 샹점 사용 못하게 막음	
	if(( CASH_SHOP_BUILDING_INDEX == nShopIndex)&& (g_pD3dApp->GetTestServerFlag() == TRUE ))
	{
		return TRUE;
	}	
	// end 2007-08-21 by bhsohn 캐쉬 샹점 사용 못하게 막음	

	// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
	if(nShopIndex && !g_pD3dApp->IsPossibleShopOpen())
	{
		return TRUE;
	}
	// end 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경


	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	if( nShopIndex )
	{
		BUILDINGNPC* pBuilding = g_pDatabase->GetServerBuildingNPCInfo( nShopIndex );
		if( pBuilding )
		{
			// 인피니티 팝업창이 떠있을 경우 인피니티 상점클릭이 중복되지 않도록 한다
			if( pBuilding->BuildingKind == BUILDINGKIND_INFINITY )
			{
				if( g_pD3dApp->m_pInterface->m_pInfinityPopup 
					&& g_pD3dApp->m_pInterface->m_pInfinityPopup->IsShowWnd() )
				{
					return TRUE;
				}

				if( g_pD3dApp->m_pInterface->IsArenaGameState( ARENA_GAME_STATE_TEAMINFO ) )
				{
					return TRUE;
				}
			}

			if( pBuilding->BuildingKind == BUILDINGKIND_ARENA )
			{
				if( g_pD3dApp->m_pInterface->m_pInfinityPopup 
					&& g_pD3dApp->m_pInterface->m_pInfinityPopup->IsShowWnd() )
				{
					return TRUE;
				}
			}
		}
	}
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	// 2013-03-21 by bhsohn 창고 주위에 개인상점 캐릭터 선택 시 개인상점이 연결되지 않고 창고가 오픈되는 현상.
	if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL,0))
	{			
		//현재 자신의 세력이 일반군이면 개인상점을 비활성화 한다.
		// 캐릭터 체크
		CEnemyData* pEnemy = g_pScene->GetEnemyCharaterID(vPos1);
		m_pSelBazaarShop = NULL;
		if(pEnemy)
		{
			float fDistanceScene = D3DXVec3Length(&(m_vPos - pEnemy->m_vPos));
			
			if(fDistanceScene <= REQUERED_BAZAAR_DISTANCE)
			{
				return g_pInterface->SendBazaarRequestItemList(pEnemy, nFlag);
			}
		}
	}
	// END 2013-03-21 by bhsohn 창고 주위에 개인상점 캐릭터 선택 시 개인상점이 연결되지 않고 창고가 오픈되는 현상.


	// 2006-09-14 by ispark, 상점 메세지
//*--------------------------------------------------------------------------*//
	if(nShopIndex)
	{
		// 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
		// 아이템을 사용해서 상점을 열었으면 일정시간 상점을 못 열게 한다.
		if( g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent + CINFInvenExtend::SHOP_CONCURRENT_DELAY < timeGetTime() )
		{
			g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent = timeGetTime();

			m_pShopObject = stObject.pObject;
			
			MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING sMsg;
			sMsg.nBuildingIndex0 = nShopIndex;
			//		sMsg.SendShopItemList = FALSE;
			g_pFieldWinSocket->SendMsg( T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING, (char*)&sMsg, sizeof(sMsg) );				
			g_pD3dApp->m_bRequestEnable = FALSE;			// 20 06-06-21 by ispark, 메세지 응답을 기다린다.
			return TRUE;
		}
		// end 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
	}
	
//*--------------------------------------------------------------------------*//

	// 2013-03-21 by bhsohn 창고 주위에 개인상점 캐릭터 선택 시 개인상점이 연결되지 않고 창고가 오픈되는 현상.
	// 2006-08-07 by dgwoo 현재 자신의 세력이 일반군이면 개인상점을 비활성화 한다.
// 	if(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL,0))
// 		return FALSE;
// 
// 	// 2006-07-29 by ispark, 캐릭터 체크
// 	CEnemyData* pEnemy = g_pScene->GetEnemyCharaterID(vPos1);
// 	m_pSelBazaarShop = NULL;
// 	if(pEnemy)
// 	{
// 		float fDistanceScene = D3DXVec3Length(&(m_vPos - pEnemy->m_vPos));
// 
// 		if(fDistanceScene <= REQUERED_BAZAAR_DISTANCE)
// 		{
// 			return g_pInterface->SendBazaarRequestItemList(pEnemy, nFlag);
// 		}
// 	}
	// END 2013-03-21 by bhsohn 창고 주위에 개인상점 캐릭터 선택 시 개인상점이 연결되지 않고 창고가 오픈되는 현상.

	return FALSE;
}

CObjectChild *CCharacterChild::GetEventObject(float fDistance)
{
	CObjectChild * pObjectRes = NULL;
	vectorCObjectChildPtr::iterator itObj = g_pScene->m_vectorRangeObjectPtrList.begin();
	while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC)
		{
			float fLengthTemp = D3DXVec3Length(&(m_vPos - pObject->m_vPos));
			if(fLengthTemp > fDistance)
			{
				itObj++;
				continue;
			}
			
			if(pObjectRes == NULL)
			{
				pObjectRes = pObject;
			}

			float fLengthTemp1 = D3DXVec3Length(&(m_vPos - pObject->m_vPos));
			float fLengthTemp2 = D3DXVec3Length(&(m_vPos - pObjectRes->m_vPos));
			
			if(fLengthTemp2>fLengthTemp1)
			{
				pObjectRes = pObject;
			}
			
		}
		itObj++;
	}

	return pObjectRes;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterChild::CameraMoveTick()
/// \brief		캐릭터 마우스 움직임 Tick()
/// \author		ispark
/// \date		2005-08-16 ~ 2005-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::CameraMoveTick()
{
	float fDirY = 0.0f, fDirX = 0.0f;
	POINT pt;

	m_bCharacterRender = TRUE;

	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);

	if(m_bRButtonState && m_bMouseMove)
	{
//		CheckMouseReverse(&pt);
		// 일반, 외곽에서 마우스 움직임일 때
		if(pt.y <= 0)
			fDirY = 10.0f;
		else if(pt.y >= g_pD3dApp->GetBackBufferDesc().Height-1)
			fDirY = -10.0f;
		else
			fDirY = m_pOldMousePoint.y - pt.y;
		
		if(pt.x <= 0)
			fDirX = -10.0f;
		else if(pt.x >= g_pD3dApp->GetBackBufferDesc().Width-1)
			fDirX = 10.0f;
		else
			fDirX = pt.x - m_pOldMousePoint.x;
		
		m_pOldMousePoint = pt;
	}
	else if(m_bRButtonState)
	{
		// 외곽처리 문제 오른쪽 버튼눌렀을 때만 처리, 마우스 움직임이 없음
		if(pt.y <= 0)
			fDirY = 10.0f;
		else if(pt.y >= g_pD3dApp->GetBackBufferDesc().Height-1)
			fDirY = -10.0f;

		if(pt.x <= 0)
			fDirX = -10.0f;
		else if(pt.x >= g_pD3dApp->GetBackBufferDesc().Width-1)
			fDirX = 10.0f;
	}
	
	g_pCamera->SetAngleX(fDirX);
	g_pCamera->SetAngleY(fDirY);

	// 이벤트 오브젝트오 인해 카메라가 움직이지만 회전을 하며 안돼는 상황
	BOOL bWarp = (m_nStartEventType != EVENT_TYPE_NOEVENT) ? TRUE : FALSE;

	if(m_bPickMove == TRUE || m_bMouseWheel == TRUE || bWarp)
		g_pCamera->SetCamMove(m_bRButtonState, m_bMouseWheel, bWarp);

	// 카메라 거리가 최소허용치보다 작으면 캐릭터 렌더링 안함
	float fCollDist = g_pCamera->GetCollDistance();
	if(fCollDist != 0 && 
		fCollDist < CHARACTER_CAMERA_DISTANCE_MIN)
		m_bCharacterRender = FALSE;

	m_bMouseMove = FALSE;
	m_bMouseWheel = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::CheckEventTypeObject(float fElapsedTime)
/// \brief		캐릭터 전용 이벤트 오브젝트 체크
/// \author		ispark
/// \date		2006-07-14 ~ 2006-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::CheckEventTypeObject(float fElapsedTime)
{
	if(g_pScene->m_pObjectRender)
	{
		CObjectChild* pCurrentObjEvent = (CObjectChild *)g_pScene->m_pObjectRender->CheckCollMeshEvent(40.0f);
		
		if(!pCurrentObjEvent)
		{
			return;
		}

		// 시작이라면
		if(m_dwState == _NCITYIN)
		{
			m_bStartPositionEvent = TRUE;
			return;
		}

		// 타임 체크
		// 에러 타임(10초)을 초과 했는지
		m_fCharacterWarpErrTime -= fElapsedTime;
		if(m_fCharacterWarpErrTime > 0.0f)
		{
			return;
		}
		else
		{
			m_fCharacterWarpErrTime = 0.0f;
		}

		float fEventLength = D3DXVec3Length(&D3DXVECTOR3(pCurrentObjEvent->m_vPos - m_vPos));
		
		switch(pCurrentObjEvent->m_bEventType)
		{
		case EVENT_TYPE_CHARACTERMODE_WARP:
			{
				if(m_nStartEventType == EVENT_TYPE_CHARACTERMODE_WARP_TARGET)
				{
					// 워프 시작
					// 이벤트가 특정 거리 안에 드는지
					if(fEventLength > 15.0f)
					{
						break;
					}

//					m_dwState = _STAND;
					m_bPickMove = FALSE;

					m_vMoveVel = D3DXVECTOR3(0, 0, 0);
					m_fCharacterWarpErrTime = 10.0f;
					SendRequestEventObjectWarpIn(pCurrentObjEvent);
					m_nStartEventType = EVENT_TYPE_NOEVENT;
				}
				else if(m_nStartEventType == EVENT_TYPE_NOEVENT)
				{
					// 타겟 이벤트 오브젝트 방향을 찾는다.
					SetMoveVelByEventObject(EVENT_TYPE_CHARACTERMODE_WARP_TARGET, 500.0f);
					m_nStartEventType = EVENT_TYPE_CHARACTERMODE_WARP;
					m_dwState = _WARP;

					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가.
				}
			}
			break;
		case EVENT_TYPE_CHARACTERMODE_WARP_TARGET:
			{
				if(m_nStartEventType == EVENT_TYPE_CHARACTERMODE_WARP)
				{
					// 워프 끝
					// 이벤트 오브젝트 높이보다 높아야 한다.
					if(pCurrentObjEvent->m_vPos.y > m_vPos.y)
					{
						break;
					}

					m_dwState = _STAND;
					m_nStartEventType = -1;								// 이벤트 타입 무시
				}
				else if(m_nStartEventType == EVENT_TYPE_NOEVENT)
				{
					// 이벤트가 특정 거리 안에 드는지
					if(fEventLength > 10.0f)
					{
						break;
					}

					// 워프 이벤트 오브젝트 방향을 찾는다.
					SetMoveVelByEventObject(EVENT_TYPE_CHARACTERMODE_WARP, 500.0f);
					m_nStartEventType = EVENT_TYPE_CHARACTERMODE_WARP_TARGET;
					m_dwState = _WARP;
					m_bPickMove = FALSE;

					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, 펫시스템 추가.
				}
				else if(m_nStartEventType == -1)
				{
					// 워프를 한 후 일정 범위를 넘어가기 전까지는 이벤트 오브젝트를 무시한다.
					float fEventLength = D3DXVec3Length(&D3DXVECTOR3(pCurrentObjEvent->m_vPos - m_vPos));
					if(fEventLength > 30.0f)
					{
						m_nStartEventType = EVENT_TYPE_NOEVENT;
					}
				}
			}
			break;
			// 2007-12-14 by dgwoo 캐릭터 모드에서 바로 워프 탈수있도록 이벤트 추가.
		case EVENT_TYPE_CHARACTERMODE_DIRECTLY_WARP:
			{
				m_bPickMove = FALSE;
				m_vMoveVel = D3DXVECTOR3(0, 0, 0);
				m_fCharacterWarpErrTime = 10.0f;
				SendRequestEventObjectWarpIn(pCurrentObjEvent);
				m_nStartEventType = EVENT_TYPE_NOEVENT;
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::SendRequestEventObjectWarpIn(CObjectChild *pObj)
/// \brief		캐릭터 워프 인
/// \author		ispark
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::SendRequestEventObjectWarpIn(CObjectChild *pObj)
{
	MSG_FC_EVENT_REQUEST_OBJECT_EVENT sMsg;
	sMsg.ObjectType = pObj->m_nCode;
	sMsg.ObjectPosition = pObj->m_vPos;
	g_pFieldWinSocket->SendMsg( T_FC_EVENT_REQUEST_OBJECT_EVENT, (char*)&sMsg, sizeof(sMsg) );
	DBGOUT("Character Request Warp : MSG_FC_EVENT_REQUEST_OBJECT_EVENT(%d)\n",pObj->m_nCode);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CCharacterChild::SetMoveVelByEventObject(BYTE bObjectType, float fDist)
/// \brief		특정 이벤트 오브젝트 방향을 정한다.
/// \author		ispark
/// \date		2006-07-20 ~ 2006-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CCharacterChild::SetMoveVelByEventObject(BYTE bObjectType, float fDist)
{
	// 이벤트 오브젝트를 찾는다.
	CObjectChild * pNextTargetObjEvent = g_pScene->FindEventObjectByTypeAndPosition(bObjectType, m_vPos, fDist);
	// 이동 방향을 결정한다.
	m_vMoveVel = pNextTargetObjEvent->m_vPos - m_vPos;
	m_vMoveVel.x = m_vMoveVel.z = 0.0f;
	D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);

	m_vMoveVel.y *= CHARACTER_MAN_RUN;
	return m_vMoveVel;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::DeleteChatMoveShop()
/// \brief		
/// \author		ispark
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::DeleteChatMoveShop()
{
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->InvalidateDeviceObjects();
		m_pMoveChatShop->DeleteDeviceObjects();
		SAFE_DELETE(m_pMoveChatShop);
	}
}
// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 
void CCharacterChild::ChangeWearItemBodyConditionAllProcess(BodyCond_t i_nBodyCondition)
{
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_PET, i_nBodyCondition);
}
// 2010-06-08 by dgwoo, 펫시스템 추가. 아래의 함수로 통합. 