// EffectRender.cpp: implementation of the CEffectRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "EffectRender.h"
#include "ObjectChild.h"
#include "GameDataLast.h"
#include "SpriteAniData.h"
#include "ParticleSystem.h"
#include "ObjectAniData.h"
#include "TraceAni.h"
#include "EnemyData.h"
#include "MonsterData.h"
#include "SkinnedMesh.h"
#include "SceneData.h"
#include "Camera.h"
#include "Background.h"
#include "Weapon.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "ItemData.h"
#include "dxutil.h"
#include "SunData.h"
#include "D3DHanFont.h"
#include "ObjRender.h"
#include "SkillEffect.h"
#include "TutorialSystem.h"
#include "Frustum.h"
#include "INFGameMain.h"	// 2008-08-22 by bhsohn EP3 인벤토리 처리
#include "MeshInitThread.h"  // 2009. 11. 23 by jskim 리소스 로딩 구조 변경

#include "Chat.h"

extern LPDIRECT3DDEVICE9	g_pD3dDev;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectRender::CEffectRender()
{
	FLOG( "CEffectRender()" );
	m_pVB1 = NULL;
	memset(m_pVB2,0x00,4*2);
	memset(m_pVB4,0x00,4*4);
	memset(m_pVB8,0x00,4*8);
	memset(m_pVB16,0x00,4*16);

	m_pTexEffectData = NULL;
	memset(m_pTexture, 0x00, TEX_EFFECT_NUM*sizeof(DWORD));
	memset(m_nTextureRenderCount, 0x00, TEX_EFFECT_NUM*sizeof(int));
//	memset(m_pObjEffectMesh, 0x00, OBJ_EFFECT_NUM*sizeof(DWORD));
	m_fTextureCheckTime = 300.0f;// 5분에 한번씩 이펙트 사용여부 검사

	m_pEffectData = NULL;
	m_pObjectData = NULL;
	

	m_bZBufferTemp = FALSE;
	m_nParticleEffectCountPerSecond = 0;
	m_nSpriteEffectCountPerSecond = 0;
	m_nObjectEffectCountPerSecond = 0;
	m_nTraceEffectCountPerSecond = 0;

	DBGOUT_EFFECT("-------------------- Effect wear item postion --------------------\n");
	DBGOUT_EFFECT("1-1Type Weapon		:	0\n");
	DBGOUT_EFFECT("1-2Type Weapon		:	1\n");
	DBGOUT_EFFECT("2-1Type Weapon		:	2\n");
	DBGOUT_EFFECT("2-2Type Weapon		:	3\n");
	DBGOUT_EFFECT("Front(Rader)			:	4\n");
	DBGOUT_EFFECT("Center(Armor)		:	5\n");
	DBGOUT_EFFECT("Option(ATTACH)		:	6\n");
	DBGOUT_EFFECT("Back(Engine)			:	7\n");
	DBGOUT_EFFECT("-------------------- wear item postion --------------------\n");

	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
	// 인벤 파티클 초기화
	m_vecInvenParticleInfo.clear();
	m_vecEnemyItemParticleInfo.clear();		// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
}

CEffectRender::~CEffectRender()
{
	FLOG( "~CEffectRender()" );
	SAFE_RELEASE(m_pVB1);
	int i;
	for(i=0;i<2;i++)
		SAFE_RELEASE(m_pVB2[i]);
	for(i=0;i<4;i++)
		SAFE_RELEASE(m_pVB4[i]);
	for(i=0;i<8;i++)
		SAFE_RELEASE(m_pVB8[i]);
	for(i=0;i<16;i++)
		SAFE_RELEASE(m_pVB16[i]);

	SAFE_DELETE(m_pTexEffectData);
	SAFE_DELETE(m_pEffectData);
	SAFE_DELETE(m_pObjectData);
	for(i=0;i<TEX_EFFECT_NUM;i++)
		SAFE_RELEASE(m_pTexture[i]);
//	for(i=0;i<OBJ_EFFECT_NUM;i++)
//		SAFE_DELETE(m_pObjEffectMesh[i]);
	m_vecEnemyItemParticleInfo.clear();			// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
}
/*
int CEffectRender::GetEmptyObjectIndex()
{
	FLOG( "CEffectRender::GetEmptyObjectIndex()" );
	int index = 0;
	for(int i=0;i<OBJ_EFFECT_NUM;i++)
	{
		if(!m_pObjEffectMesh[i])
			return i;
	}
	return -1;

}
*/

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
//CSkinnedMesh* CEffectRender::LoadObject(char* strName)
CSkinnedMesh* CEffectRender::LoadObject(char* strName, int LoadingPriority)
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
{
	FLOG( "CEffectRender::LoadObject(char* strName)" );
	if(strlen(strName)<=0)
	{
		return NULL;
	}
	map<string,CSkinnedMesh*>::iterator it = m_mapObjNameToMesh.find(strName);
	if(it == m_mapObjNameToMesh.end())
	{
		m_vecLoadObj.push_back(strName);

		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		LoadingPriorityInfo temp;
		temp.LoadingPriority = LoadingPriority;
		strncpy(temp.chEffectName,strName,32);
		m_vecLoadingPriority.push_back(temp);
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
//		char strPath[MAX_PATH];
//		CGameData* pMeshData = new CGameData();
//		g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_EFFECT, strName );
//		if(pMeshData->SetFile(strPath,FALSE, NULL, 0))
//		{
//			CSkinnedMesh* pMesh = new CSkinnedMesh(FALSE);
//			pMesh->InitDeviceObjects();
//			pMesh->LoadMeshHierarchyFromMem( pMeshData );
//			pMesh->m_nRenderCount = 2;
//			m_mapObjNameToMesh[strName] = pMesh;
//			delete pMeshData;
//			pMeshData = NULL;
//			return pMesh;
//		}
//		else
//		{
//			DBGOUT("이펙트 OBJECT 파일이 없습니다.(%s)",strName);
//			return NULL;
//		}
	}
	else
	{
		return it->second;
	}
	return NULL;
}
void CEffectRender::LoadObjectToMap(char* strName)
{
	FLOG( "CEffectRender::LoadObject(char* strName)" );
	if(strlen(strName)<=0)
	{
		return ;
	}
	map<string,CSkinnedMesh*>::iterator it = m_mapObjNameToMesh.find(strName);
	if(it == m_mapObjNameToMesh.end())
	{
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// 		char strPath[MAX_PATH];
// 		CGameData* pMeshData = new CGameData();
// 		g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_EFFECT, strName );
// 		if(pMeshData->SetFile(strPath,FALSE, NULL, 0))
// 		{
// #ifdef _DEBUG
// 			// 보안 코드
// 			char buf[64];
// 			memset( buf, 0x00, sizeof(buf) );
// 			strncpy( buf, strName, 8 );
// 			if(pMeshData->Find(buf) == NULL)
// 			{
// 				SAFE_DELETE(pMeshData);
// //				delete pMeshData;
// //				pMeshData = NULL;
// 				g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
// 				DBGOUT("Resource File Error(%s)\n",strName);
// 				return ;
// 			}
// #endif
// 			CSkinnedMesh* pMesh = new CSkinnedMesh(TRUE);
// 			pMesh->InitDeviceObjects();
// 			pMesh->LoadMeshHierarchyFromMem( pMeshData );	
// 			pMesh->m_nRenderCount = 2;
// 			m_mapObjNameToMesh[strName] = pMesh;
// 			SAFE_DELETE(pMeshData);
// //			delete pMeshData;
// //			pMeshData = NULL;
// 			return ;
// 		}
// 		else
// 		{
// 			
// 			DBGOUT("Can't Find Effect OBJECT File.(%s)\n",strName);
// 			return ;
// 		}
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		int  tempPriority = _NOTHING_STEP;
		vector<LoadingPriorityInfo>::iterator it1 = m_vecLoadingPriority.begin();
		while(it1 != m_vecLoadingPriority.end())
		{
			if(!strcmp((*it1).chEffectName, strName))
			{
				tempPriority = (*it1).LoadingPriority;
				break;
			}				
			it1++;
		}			
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		CSkinnedMesh* pMesh = new CSkinnedMesh(TRUE);
		pMesh->InitDeviceObjects();
		pMesh->m_nRenderCount = 2;
		m_mapObjNameToMesh[strName] = pMesh;

		if(g_pD3dApp->m_dwGameState != _GAME || tempPriority == _NOTHING_STEP)
		{
			char strPath[MAX_PATH];
			CGameData* pMeshData = new CGameData();
			g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_EFFECT, strName );
			if(pMeshData->SetFile(strPath,FALSE, NULL, 0))
			{
#ifdef _DEBUG
				// 보안 코드
				char buf[64];
				memset( buf, 0x00, sizeof(buf) );
				strncpy( buf, strName, 8 );
				if(pMeshData->Find(buf) == NULL)
				{
					// 2015-06-30 Future, added error Output on Effect loading error
					char errMsg[512];
					sprintf_s(errMsg, 512, STRERR_C_RESOURCE_0001, pMeshData->GetZipFilePath());
					g_pD3dApp->NetworkErrorMsgBox(errMsg);
					SAFE_DELETE(pMeshData);
					DBGOUT("Resource File Error(%s)\n",strName);
					return ;
				}
#endif
				pMesh->LoadMeshHierarchyFromMem( pMeshData );
				SAFE_DELETE(pMeshData);
				return ;
			}
			else
			{
				char errStr[512];
				sprintf_s(errStr, 512, STRERR_C_RESOURCE_0012, strPath);
				g_pD3dApp->m_pChat->CreateChatChild(errStr, COLOR_ERROR);
				SAFE_DELETE(pMeshData);
				DBGOUT("Can't Find Effect OBJECT File.(%s)\n",strName);
				return ;
			}
		}
		else
		{	
			structLoadingGameInfo* LoadingGameInfo = new structLoadingGameInfo;
			strcpy(LoadingGameInfo->MeshName, strName);
			LoadingGameInfo->MeshType			= _EFFECT_TYPE;
			LoadingGameInfo->pSkinnedMesh		= pMesh;	
			LoadingGameInfo->LoadingPriority	= tempPriority;
			EnterCriticalSection(&g_pD3dApp->m_cs);
			g_pD3dApp->m_pMeshInitThread->QuePushGameData( LoadingGameInfo );
			LeaveCriticalSection(&g_pD3dApp->m_cs);
		}
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	}
}
/*
int CEffectRender::LoadObject(char* strName)
{
	FLOG( "CEffectRender::LoadObject(char* strName)" );
	map<string,int>::iterator it = m_mapObjNameToIndex.find(strName);
	if(it == m_mapObjNameToIndex.end())
	{
		char strPath[MAX_PATH];
		CGameData* pMeshData = new CGameData();
		g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_EFFECT, strName );
		pMeshData->SetFile(strPath,FALSE, NULL, 0);
		int index = GetEmptyObjectIndex();
		if(index>=0)
		{
			m_pObjEffectMesh[index] = new CSkinnedMesh(FALSE);
			m_pObjEffectMesh[index]->InitDeviceObjects();
			m_pObjEffectMesh[index]->LoadMeshHierarchyFromMem( pMeshData );
			m_pObjEffectMesh[index]->m_nRenderCount = 2;
			m_mapObjNameToIndex[strName] = index;
			delete pMeshData;
			pMeshData = NULL;
			return index;
		}
		else
		{
			DBGOUT("ERROR  CEffectRender::LoadObject, OBJ_EFFECT_NUM 포인터 버퍼 개수가 부족합니다.\n");
			return -1;
		}
	}
	else
	{
		return it->second;
	}
}
*/
int CEffectRender::GetEmptyTextureIndex()
{
	FLOG( "CEffectRender::GetEmptyTextureIndex()" );
	int index = 0;
	for(int i=0;i<TEX_EFFECT_NUM;i++)
	{
		if(!m_pTexture[i])
			return i;
	}
	return -1;
}

DataHeader* CEffectRender::FindEffectInfo(char* strName)
{
	FLOG( "CEffectRender::FindEffectInfo(char* strName)" );
	if(!m_pEffectData)
		return NULL;
	DataHeader * pDataHeader = m_pEffectData->GetStartPosition();
	while(pDataHeader)
	{
		// 8글자만 비교한다. 확장자는 제외시킨다.
		if(strncmp(strName, pDataHeader->m_FileName,8)==0)
		{
			return pDataHeader;
		}
		pDataHeader = m_pEffectData->GetNext();
	}
	return NULL;
}

DataHeader* CEffectRender::FindObjectInfo(char* strName)
{
	FLOG( "CEffectRender::FindObjectInfo(char* strName)" );
	if(!m_pObjectData)
		return NULL;
	DataHeader * pDataHeader = m_pObjectData->GetStartPosition();
	while(pDataHeader)
	{
		// 8글자만 비교한다. 확장자는 제외시킨다.
		if(strncmp(strName, pDataHeader->m_FileName,8)==0)
		{
			return pDataHeader;
		}
		pDataHeader = m_pObjectData->GetNext();
	}
	return NULL;
}

int CEffectRender::LoadTexture(char* strName)
{
	FLOG( "CEffectRender::LoadTexture(char* strName)" );
	if(!m_pTexEffectData)
		return -1;
	if(strlen(strName) <= 0)
	{
		return -1;
	}
	map<string,int>::iterator it = m_mapTexNameToIndex.find(strName);
	if(it == m_mapTexNameToIndex.end())
	{
		DataHeader * pDataHeader = m_pTexEffectData->GetStartPosition();
		while(pDataHeader)
		{
			if(strncmp(strName, pDataHeader->m_FileName,8)==0)
			{
				int index = GetEmptyTextureIndex();
				if(index<0)
				{
					DBGOUT("ERROR, CEffectRender::LoadTexture, GetEmptyTextureIndex() < 0\n");
					return -1;
				}
				// load texture from memory
				HRESULT hr=D3DXCreateTextureFromFileInMemory( g_pD3dDev, pDataHeader->m_pData,  pDataHeader->m_DataSize,  &m_pTexture[index]);
				if(hr!=D3D_OK)
				{
					m_pTexture[index] = NULL;
					DBGOUT("ERROR, CEffectRender::LoadTexture, D3DXCreateTextureFromFileInMemory() != D3D_OK\n");
					return -1;
				}
				m_nTextureRenderCount[index] = 2;
				m_mapTexNameToIndex[strName] = index;
				return index;
			}
			pDataHeader = m_pTexEffectData->GetNext();
		}
	} 
	else
	{
		return it->second;
	}
	DBGOUT("ERROR, CEffectRender::LoadTexture (%s)\n", strName);
	return -1;
}

void CEffectRender::Tick(float fElapsedTime)
{
	FLOG( "CEffectRender::Tick(float fElapsedTime)" );
	vector<string>::iterator it = m_vecLoadObj.begin();
	while(it != m_vecLoadObj.end() )
	{
		LoadObjectToMap((char*)(*it).c_str());
		it++;
	}
	m_vecLoadObj.clear();
	// texture delete : 60초에 한번씩
	m_fTextureCheckTime -= fElapsedTime;
	if(m_fTextureCheckTime>0.0f)
		return;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//m_fTextureCheckTime = 300.0f;
	if( g_pD3dApp->IsEmptyLoadingGameDataList() )
	{
	m_fTextureCheckTime = 300.0f;
	}
	else
	{
		m_fTextureCheckTime = 30.0f;
		return;
	}
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	
	for(int i=0;i<TEX_EFFECT_NUM;i++)
	{
		if(m_nTextureRenderCount[i] == 0)
			continue;
//		m_nTextureRenderCount[i]--;
		if(m_nTextureRenderCount[i]==2)// if(m_nTextureRenderCount[i] == 1)
		{
			map<string,int>::iterator it = m_mapTexNameToIndex.begin();
			while(it != m_mapTexNameToIndex.end())
			{
				if(it->second == i)
				{
					m_mapTexNameToIndex.erase(it);
					break;
				}
				it++;
			}
			m_nTextureRenderCount[i] = 0;
			SAFE_RELEASE(m_pTexture[i]);
		}
		else// if(m_nTextureRenderCount[i] >= 2)
		{
			m_nTextureRenderCount[i] = 2;
		}
	}
/*	for(i=0;i<OBJ_EFFECT_NUM;i++)
	{
		if(m_pObjEffectMesh[i] && m_pObjEffectMesh[i]->m_nRenderCount)
		{
			if(m_pObjEffectMesh[i]->m_nRenderCount == 2)// if(m_nObjRenderCount[i] == 1)
			{
				map<string,int>::iterator it = m_mapObjNameToIndex.begin();
				while(it != m_mapObjNameToIndex.end())
				{
					if(it->second == i)
					{
						m_mapObjNameToIndex.erase(it);
						break;
					}
					it++;
				}
				m_pObjEffectMesh[i]->InvalidateDeviceObjects();
				m_pObjEffectMesh[i]->DeleteDeviceObjects();
				SAFE_DELETE(m_pObjEffectMesh[i]);
			}
			else// if(m_nObjRenderCount[i] >= 2)
			{
				m_pObjEffectMesh[i]->m_nRenderCount = 2;
			}
		}
	}*/
	map<string, CSkinnedMesh*>::iterator itMesh = m_mapObjNameToMesh.begin();
	while(itMesh != m_mapObjNameToMesh.end())
	{
		if(itMesh->second->m_nRenderCount == 2)
		{
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			itMesh->second->DeleteLoadingGameData();
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			itMesh->second->InvalidateDeviceObjects();
			itMesh->second->DeleteDeviceObjects();
			SAFE_DELETE(itMesh->second);
			itMesh = m_mapObjNameToMesh.erase(itMesh);
			continue;
		}
		itMesh++;
	}
}
/*
void CEffectRender::RenderSun()
{
	FLOG( "CEffectRender::RenderSun()" );
	CCharacterInfo* pChar;
	// Sun
	g_pD3dDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	if(g_pD3dApp->m_pScene->m_pSunData)
	{
		pChar = g_pD3dApp->m_pScene->m_pSunData->m_pCharacterInfo;
		if(pChar)
		{
			D3DXMATRIX mat;
			D3DXVECTOR3 vPos = g_pCamera->GetEyePt();
			D3DXVECTOR3 vVel =  g_pD3dApp->m_pScene->m_pSunData->m_vPos - g_pCamera->GetEyePt();
			D3DXVECTOR3 vSide = g_pCamera->GetCross();
			D3DXVECTOR3 vUp;
			D3DXVec3Cross(&vUp,&vVel,&vSide);
			D3DXMatrixLookAtLH(&mat,&vPos,&(vPos + vVel),&vUp);
			COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMesh(mat,vPos);
			if(collResult.fDist == 10000)
			{
			RenderCharacterInfo(pChar);
			}
		}
	}
}
*/
void CEffectRender::DevideCharacterEffect(CCharacterInfo* pChar, BOOL bAlpha, int nAlphaValue)
{
	FLOG( "CEffectRender::DevideCharacterEffect(CCharacterInfo* pChar)" );
	int nParticleEffectCountPerSecond=0,nSpriteEffectCountPerSecond=0,nObjectEffectCountPerSecond=0,nTraceEffectCountPerSecond=0;
	set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
	while(itCurrent != pChar->m_vecCurrentBodyCondition.end())
	{
		map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);
		if(itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
			while(itEffect != pBody->m_vecEffect.end())
			{
				// effect rendering
				CEffectInfo* pEffectInfo = (*itEffect);
				pEffectInfo->m_nAlphaValue = nAlphaValue;

// 2010. 07. 23 by dhkwon, jskim 지연 시작 이팩트가 먼저 랜더 되는 버그 수정
				if(pEffectInfo->m_fCurrentTime>0)
				{
					itEffect++;
					continue;
				}
				
				switch(pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_OBJECT:
					{
						if(pEffectInfo->m_pEffect)
						{
							CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);	
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fRadius);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(!pEffect->m_bAlphaBlending && bResult == TRUE)
							{
// 2006-11-16 by ispark
//								if(bWeapon == TRUE)
//									ObjectAniRender(pEffect, TRUE);
//								else if(g_pShuttleChild->m_bInvenRender && !g_pD3dApp->m_bCharacter)
//									ObjectAniRender(pEffect, TRUE);
//								else
									ObjectAniRender(pEffect, bAlpha, nAlphaValue);
							}
							else if(pEffect->m_bZbufferEnable && bResult == TRUE)
							{
#ifdef _DEBUG
								if((UINT)pEffect == (UINT)0xfdfdfdfd)
								{
									DBGOUT("\n\n\nCRITICAL ERROR\n\n\n");
								}
#endif
								m_vecZEnableEffect.push_back((Effect*)pEffect);
							}
							nObjectEffectCountPerSecond++;
						}
					}
					break;
				case EFFECT_TYPE_SPRITE:
					{
						if(pEffectInfo->m_pEffect)
						{
							CSpriteAni* pEffect = (CSpriteAni*)pEffectInfo->m_pEffect;
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fTextureSize);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(pEffect->m_bZbufferEnable && bResult == TRUE)
							{
#ifdef _DEBUG
								if((UINT)pEffect == (UINT)0xfdfdfdfd)
								{
									DBGOUT("\n\n\nCRITICAL ERROR\n\n\n");
								}
#endif
								m_vecZEnableEffect.push_back((Effect*)pEffect);
							}
							nSpriteEffectCountPerSecond++;
						}
					}
					break;
				case EFFECT_TYPE_PARTICLE:
					{
						// 2008-02-26 by bhsohn 적유닛 바디 컨디션 안나오는 버그 수정
//						if(pEffectInfo->m_pEffect &&
//							// 2007-10-16 by dgwoo 이펙트가 자신의 것이며 캐릭터 상태에선 파티클을 발생시키지 않는다.
//							// 새로 들어간 무기 파티클 이펙트가 캐릭터 모드에서도 발생하여 처리.
//							// 파티클과 오브젝트간의 관계를 변경할 필요성이 있음.
//							(pChar->m_bShuttleChildEffect ))
//							// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
//							//g_pD3dApp->m_bCharacter == FALSE))
						// 적유닛 바디 컨디션 안나오는 버그 수정
						if(pEffectInfo->m_pEffect)
						{
							CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fRadius);
//							DBGOUT("%s File : Len(%f:%f), Render(%d)\n", pEffect->m_strName, D3DXVec3Length(&D3DXVECTOR3(g_pShuttleChild->m_vPos - vPos)), pEffect->m_fRadius, bResult);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(pEffect->m_bZbufferEnable && bResult == TRUE)
							{
#ifdef _DEBUG
								vector<Effect*>::iterator itP = pEffect->m_vecParticle.begin();
								while( itP != pEffect->m_vecParticle.end() )
								{
									if((UINT)(*itP) == (UINT)0xfdfdfdfd)
									{
										DBGOUT("\n\n\nCRITICAL ERROR\n\n\n");
									}
									itP++;
								}
#endif
								// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
//								m_vecZEnableEffect.insert(m_vecZEnableEffect.end(),
//										pEffect->m_vecParticle.begin(), pEffect->m_vecParticle.end());
								// 케릭터 모드시에는 파티클 이펙트를 안그린다.

								// 2007-12-17 by bhsohn 마을 파티클 문제 처리
//								if(g_pD3dApp->m_bCharacter == FALSE)
//								{
//									m_vecZEnableEffect.insert(m_vecZEnableEffect.end(),
//										pEffect->m_vecParticle.begin(), pEffect->m_vecParticle.end());
//								}								
								// end 2007-12-17 by bhsohn 마을 파티클 문제 처리

								// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리								
								// 인벤 이펙트는 무조건 그려야한다.
								vector<Effect*>::iterator itEffectParticle = pEffect->m_vecParticle.begin();
								while( itEffectParticle != pEffect->m_vecParticle.end() )
								{
									Effect* pEffectParticle = (*itEffectParticle);
									int  nInvenIdx = ((CParticle*)pEffectParticle)->m_pParent->m_pParent->m_nInvenWeaponIndex;
									// 2007-12-17 by bhsohn 마을 파티클 문제 처리
									if(g_pD3dApp->m_bCharacter == TRUE )
									{
										if(nInvenIdx == 0)
										{
											// 캐릭터 모드고 인벤 파티클을 뿌리지 않는다.
											m_vecZEnableEffect.push_back((Effect*)pEffectParticle);
										}
									}
									else
									{
										// 캐릭터 모드고 인벤 파티클을 뿌리지 않는다.
										m_vecZEnableEffect.push_back((Effect*)pEffectParticle);
									}
									// end 2007-12-17 by bhsohn 마을 파티클 문제 처리

									// 2008-08-22 by bhsohn EP3 인벤토리 처리
									//if(nInvenIdx > 0 &&  g_pShuttleChild->m_bInvenRender == TRUE)
									if(nInvenIdx > 0 &&  g_pGameMain->IsEquipInvenShow() == TRUE)
									{							
										AddInvenPaticleName(nInvenIdx - 1, ((CParticle*)pEffectParticle)->m_pParent->m_strName);
									}						
									
									itEffectParticle++;
								}

							}
							nParticleEffectCountPerSecond+=pEffect->m_vecParticle.size();
						}
					}
					break;
				case EFFECT_TYPE_TRACE:
					{
						if(pEffectInfo->m_pEffect)
						{
							CTraceAni* pEffect = (CTraceAni*)pEffectInfo->m_pEffect;
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fRadius);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(pEffect->m_bZbufferEnable && bResult == TRUE)
							{
								for(int i=0;i<pEffect->m_nNumberOfTrace;i++)
								{
									if((Effect*)pEffect->m_pEffectPlane[i])
									{
#ifdef _DEBUG
										if((UINT)pEffect->m_pEffectPlane[i] == (UINT)0xfdfdfdfd)
										{
											DBGOUT("\n\n\nCRITICAL ERROR\n\n\n");
										}
#endif
										m_vecZEnableEffect.push_back((Effect*)pEffect->m_pEffectPlane[i]);
									}
								}
							}
							nTraceEffectCountPerSecond++;
						}
/*						if(pEffectInfo->m_pEffect)
						{
							CTraceAni* pEffect = (CTraceAni*)pEffectInfo->m_pEffect;
							if(pEffect->m_bZbufferEnable)
							{
								m_vecZEnableEffect.push_back((Effect*)pEffect);
							}
							nTraceEffectCountPerSecond++;
						}
*/					}
					break;
				}
				itEffect++;
			}//if(!Effect.end())
		}//if(!bodyCondition.end())
		itCurrent++;
	}//end()
	m_nParticleEffectCountPerSecond += nParticleEffectCountPerSecond;
	m_nSpriteEffectCountPerSecond += nSpriteEffectCountPerSecond;
	m_nObjectEffectCountPerSecond += nObjectEffectCountPerSecond;
	m_nTraceEffectCountPerSecond += nTraceEffectCountPerSecond;
}

void CEffectRender::DevideZBufferEnableEffect()
{
	FLOG( "CEffectRender::DevideZBufferEnableEffect()" );
	CCharacterInfo* pChar;
	m_nParticleEffectCountPerSecond = 0;
	m_nSpriteEffectCountPerSecond = 0;
	m_nObjectEffectCountPerSecond = 0;
	m_nTraceEffectCountPerSecond = 0;

	// Object - Normal
//	if(g_pD3dApp->m_bDegree == 2) // 고사양
//	{
//		vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorRangeObjectPtrList.begin());
//		while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
//		{
//			CObjectChild * pObj = *itObj;
//			pChar = pObj->m_pCharacterInfo;
//			if(pChar)
//			{
//				DevideCharacterEffect(pChar);
//			}
//	//		m_bZBufferTemp = FALSE;
//			itObj++;
//		}
//	}
//	else
//	{
	float fDistanceToCamera;
	float fDistance;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCulledObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCulledObjectPtrList.end())
	{
		CObjectChild * pObj = *itObj;
		pChar = pObj->m_pCharacterInfo;
		
		fDistanceToCamera = D3DXVec3Length(&(pObj->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));		
//		fDistance = (g_pScene->m_fOrgFogEndValue/(10-g_pSOption->sTerrainEffectRender));
		
		// 2007-07-11 by dgwoo 튜토리얼 맵에서 챕터(1_1 ~ 1_5)의 링을 챕터마다 다르게 그려줘야한다.
		if(g_pTutorial->IsTutorialMode() == TRUE &&				// 튜토리얼 모드 이면서.
//			g_pTutorial->GetLesson() == L1 &&					// 레슨 1이면서.
			pObj->m_nCode == TUTORIAL_GATE &&					// 게이트 오브젝트면서.
			!g_pTutorial->IsEnableTutorialGate(pObj->m_vPos))	// 현재 사용해야할 게이트라면.
		{// 
			int i = 0;
		}
		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
		else if(pObj->IsShowNode()) // else 
		{// 튜토리얼 모드가 아닐시.
			// 2005-05-10 by jschoi - 이펙트 옵션 삭제
			fDistance = g_pScene->m_fFogEndValue + pObj->m_pObjMesh->m_fRadius;

			if(fDistanceToCamera < fDistance)
			{
					
				if(pChar)
				{
					DevideCharacterEffect(pChar);
				}
			}
		}
		
		itObj++;
	}

	if(g_pGround && g_pGround->m_pObjectEvent)
	{
		CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
		while(pObjEvent)
		{
			pChar = pObjEvent->m_pCharacterInfo;
			if(pChar)
			{
				// 2007-07-11 by dgwoo 튜토리얼맵에서 그리지 말아야할 링(이펙트가 있다)
				if(g_pTutorial->IsTutorialMode() == TRUE &&				// 튜토리얼 모드 이면서.
//					g_pTutorial->GetLesson() == L1 &&					// 레슨 1이면서.
					pObjEvent->m_nCode == TUTORIAL_GATE &&					// 게이트 오브젝트면서.
					!g_pTutorial->IsEnableTutorialGate(pObjEvent->m_vPos))	// 현재 사용해야할 게이트라면.
				{
					int i = 0;
				}
				// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
				else if(pObjEvent->IsShowNode()) // else
				{
					DevideCharacterEffect(pChar);
				}
				// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템
			}
			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}
	// ShuttleChild 와 CharacterChild
	// 2005-07-26 by ispark
	if(g_pD3dApp->m_bCharacter == FALSE)
	{

//		if(g_pShuttleChild && COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_GAMEMASTER)==FALSE)
		if(g_pShuttleChild)
		{
			pChar = g_pD3dApp->m_pShuttleChild->m_pCharacterInfo;
			if(pChar &&
				g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
			{
				// 2006-11-16 by ispark, 알파 체크
				if(g_pShuttleChild->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
				{
					ALPHA_CHARACTERINFO stAlphaInfo;
					stAlphaInfo.pCharInfo = pChar;
					stAlphaInfo.nAlphaValue = g_pShuttleChild->m_nAlphaValue;
					g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
				}
				else
				{
					DevideCharacterEffect(pChar);
				}
			}

			// 2004-10-12 by jschoi
			vector<SkillEffectInfo>::iterator itEffect = g_pShuttleChild->m_pSkillEffect->m_vecSkillEffect.begin();
			while(itEffect != g_pShuttleChild->m_pSkillEffect->m_vecSkillEffect.end())
			{
				if(itEffect->pCharacterInfo)
				{
					DevideCharacterEffect(itEffect->pCharacterInfo);
				}
				itEffect++;
			}
			
			// 2006-12-04 by ispark, 체프 사출
			vector<CItemData*>::iterator itItem = g_pShuttleChild->m_pChaffData.begin();
			while(itItem != g_pShuttleChild->m_pChaffData.end())
			{
				if((*itItem)->m_pCharacterInfo)
				{
					DevideCharacterEffect((*itItem)->m_pCharacterInfo);
				}
				itItem++;
			}
			// 2007-02-12 by dgwoo 스캔 오브젝트
			itItem = g_pScene->m_vecScanData.begin();
			while(itItem != g_pScene->m_vecScanData.end())
			{
				if((*itItem)->m_pCharacterInfo)
				{
					DevideCharacterEffect((*itItem)->m_pCharacterInfo);
				}
				itItem++;
			}
			
		}
	}
	else
	{
//		pChar = g_pD3dApp->m_pShuttleChild->m_pCharacterInfo;
//		if(pChar)
//		{
//			DevideCharacterEffect(pChar);
//		}
		// 캐릭터 이펙트
		// Picking 이펙트
		if(g_pCharacterChild &&
			g_pCharacterChild->GetbPickMove())
		{
			pChar = g_pCharacterChild->m_pPickingInfo;
			if(pChar)
			{
				DevideCharacterEffect(pChar);
			}			
		}
	}

	CVecEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_vecEnemyRenderList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_vecEnemyRenderList.end())
	{
		// 2005-07-26 by ispark
		// 적이 캐릭터인지 판단
// 2011-07-20 by jhahn	인피3차 시네마플레이중 타캐릭터 안보이기
		if((*itEnemy)->m_bRender == FALSE )
		{
			itEnemy++;
			continue;
		}
//end 2011-07-20 by jhahn	인피3차 시네마플레이중 타캐릭터 안보이기
		if((*itEnemy)->m_bEnemyCharacter == FALSE)
		{
			// 적 기어 이펙트
			pChar = (*itEnemy)->m_pCharacterInfo;
			// 2008-07-23 by dgwoo Gm아머의 경우 이펙트가 안나오는 버그 수정.
			//if(pChar && (*itEnemy)->m_dwPartType != _ADMIN)
			if(pChar)
			{
				// 2006-11-16 by ispark, 알파 체크
				if((*itEnemy)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
				{
					ALPHA_CHARACTERINFO stAlphaInfo;
					stAlphaInfo.pCharInfo = pChar;
					stAlphaInfo.nAlphaValue = (*itEnemy)->m_nAlphaValue;
					g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
				}
				else
				{
					DevideCharacterEffect(pChar);
				}
			}

			// 2004-10-12 by jschoi
			vector<SkillEffectInfo>::iterator itEffect = (*itEnemy)->m_pSkillEffect->m_vecSkillEffect.begin();
			while(itEffect != (*itEnemy)->m_pSkillEffect->m_vecSkillEffect.end())
			{
				if(itEffect->pCharacterInfo)
				{
					// 2006-11-16 by ispark, 알파 체크
					if((*itEnemy)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = itEffect->pCharacterInfo;
						stAlphaInfo.nAlphaValue = (*itEnemy)->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
#ifdef C_USER_EFFECT_CONTROL	// 2012-10-23 by jhahn, 이펙트 옵션 조절
					else if(g_pSOption->sTerrainEffectRender > 0) // 2012-06-01 by jhahn, 이펙트 콤보수정
//2013-04-10 by ssjung 이펙트 옵션이 없을때 이펙트가 인비지블 상태에서 보이는 버그수정
#else
					else
//2013-04-10 by ssjung 이펙트 옵션이 없을때 이펙트가 인비지블 상태에서 보이는 버그수정
#endif
					{
						DevideCharacterEffect(itEffect->pCharacterInfo);
					}
				}
				itEffect++;
			}

			// 2006-12-04 by ispark, 체프 사출
			vector<CItemData*>::iterator itItem = (*itEnemy)->m_pChaffData.begin();
			while(itItem != (*itEnemy)->m_pChaffData.end())
			{
				if((*itItem)->m_pCharacterInfo)
				{
					// 2006-11-16 by ispark, 알파 체크
					if((*itEnemy)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = pChar;
						stAlphaInfo.nAlphaValue = (*itEnemy)->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
#ifdef C_USER_EFFECT_CONTROL	// 2012-10-23 by jhahn, 이펙트 옵션 조절

					else if(g_pSOption->sTerrainEffectRender > 0) // 2012-06-01 by jhahn, 이펙트 콤보수정
//2013-04-10 by ssjung 이펙트 옵션이 없을때 이펙트가 인비지블 상태에서 보이는 버그수정
#else
					else
//2013-04-10 by ssjung 이펙트 옵션이 없을때 이펙트가 인비지블 상태에서 보이는 버그수정
#endif
					{
						DevideCharacterEffect((*itItem)->m_pCharacterInfo);
					}
				}
				itItem++;
			}
		}
		else
		{
			// 적 캐릭터 이펙트
		}

		itEnemy++;
	}
//	if(g_pD3dApp->m_bDegree == 2) // 고사양
//	{
//		CMapMonsterIterator itMonster = g_pD3dApp->m_pScene->m_mapMonsterList.begin();
//		while(itMonster != g_pD3dApp->m_pScene->m_mapMonsterList.end())
//		{
//			pChar = (itMonster->second)->m_pCharacterInfo;
//			if(pChar)
//			{
//				DevideCharacterEffect(pChar);
//			}
//			itMonster++;
//		}
//	}
//	else
//	{
		CVecMonsterIterator itMonster = g_pD3dApp->m_pScene->m_vecMonsterRenderList.begin();
		while(itMonster != g_pD3dApp->m_pScene->m_vecMonsterRenderList.end())
		{
			pChar = (*itMonster)->m_pCharacterInfo;
			if(pChar)
			{
				DevideCharacterEffect(pChar);
			}


			// 2004-10-12 by jschoi
			vector<SkillEffectInfo>::iterator itEffect = (*itMonster)->m_pSkillEffect->m_vecSkillEffect.begin();
			while(itEffect != (*itMonster)->m_pSkillEffect->m_vecSkillEffect.end())
			{
				if(itEffect->pCharacterInfo)
				{
					DevideCharacterEffect(itEffect->pCharacterInfo);
				}
				itEffect++;
			}

			itMonster++;
		}
//	}
	// Weapon, 총알 이펙트
	if(g_pD3dApp->m_pScene->m_pWeaponData)
	{
		CWeapon * pWeapon = (CWeapon*)g_pD3dApp->m_pScene->m_pWeaponData->m_pChild;
		while(pWeapon)
		{
			pChar = pWeapon->m_pCharacterInfo;
			if(pChar)
			{
				// 2006-07-18 by ispark, 잔존 메모릭을 찾기 위한 DBGOUT
//				if(pWeapon->m_pItemData && (pChar->m_nCurrentBodyCondition == BODYCON_HIT_MASK || pChar->m_nCurrentBodyCondition == BODYCON_FIRE_MASK || pChar->m_nCurrentBodyCondition == BODYCON_BULLET_MASK))
//				{
//					DBGOUT("Effect Check MemoryRick(%d) 0x%x\n", pWeapon->m_pItemData->ItemNum, pChar->m_nCurrentBodyCondition);
//				}
				if( 
					// 2007-02-01 by dgwoo 침묵스킬을 맞았을때 자신을 타겟하고있는 2형무기는 보이지 않는다.
					g_pShuttleChild->GetSkillMissileWarning() &&				//침묵스킬이 발동죽이면.
					pWeapon->m_pTarget == g_pShuttleChild &&					//타겟이 자신일때
					pWeapon->m_pItemData != NULL &&								
					IS_SECONDARY_WEAPON(pWeapon->m_pItemData->Kind)				//2형무기일때만. 
// 2007-02-01 by dgwoo 아래 조건이 들어갈 이유를 찾지 못하겠음 ㅡㅡ;
//					IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE &&
//					g_pShuttleChild->m_bAttackMode == _SIEGE &&
//					pWeapon->m_pTarget == g_pShuttleChild &&
//					COMPARE_BODYCON_BIT(pWeapon->m_pCharacterInfo->m_nCurrentBodyCondition, BODYCON_HIT_MASK)
					)
				{
					
					// rendering하지 않는다.
				}
				else
				{
					DevideCharacterEffect(pChar);
				}
				
			}
			pWeapon = (CWeapon *)pWeapon->m_pNext;
		}
	}
	// Item
	if(g_pD3dApp->m_pScene->m_pItemData)
	{
		CItemData * pItem = (CItemData*)g_pD3dApp->m_pScene->m_pItemData->m_pChild;
		while(pItem)
		{
			pChar = pItem->m_pCharacterInfo;
			if(pChar && pItem->m_bIsRender)
			{
				DevideCharacterEffect(pChar);
			}
			pItem = (CItemData *)pItem->m_pNext;
		}
	}
	if( //g_pScene->m_byMapType != MAP_TYPE_CITY && 
		(g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _SHOP) &&
		g_pScene->m_byWeatherType != WEATHER_FOGGY &&			// 안개낀
		g_pScene->m_byWeatherType != WEATHER_RAINY &&			// 비
		g_pScene->m_byWeatherType != WEATHER_SNOWY &&			// 눈
//		g_pScene->m_byWeatherType != WEATHER_CLOUDY &&			// 구름낀
		g_pScene->m_pSunData &&
		g_pScene->m_pSunData->m_pCharacterInfo &&
		IsSunRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))	
	{
		pChar = g_pD3dApp->m_pScene->m_pSunData->m_pCharacterInfo;
		if(pChar)
		{
			DevideCharacterEffect(pChar);
		}
	}

	if(g_pD3dApp->m_pEffectList)
	{
		CAppEffectData * pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
		while(pEffect)
		{
			// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
			if( !pEffect->m_bRender )
			{
				pEffect = (CAppEffectData*)pEffect->m_pNext;
				continue;
			}
			// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
			
			pChar = pEffect->m_pCharacterInfo;
			// 2006-11-16 by ispark, 알파값 따로 저장
			
			// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
			BOOL bEffectRender = FALSE;
			// 2012. 02. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
			//( pEffect->m_nType/1000000 == 7 || pEffect->m_nType/10000000 == 1 || pEffect->m_nType/100000   == 1)
			if( pEffect->m_nType/1000000 == 7 || pEffect->m_nType/10000000 == 1 || pEffect->m_nType/100000   == 1 || pEffect->m_nType/1000000 == 4)
			// 2012. 02. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
			{
				bEffectRender = TRUE;
			}
			// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정

			if(pChar)
			{
				if( pEffect->m_pParent && 
					(pEffect->m_pParent->m_dwPartType == _ENEMY  || pEffect->m_pParent->m_dwPartType == _ADMIN )&&
					// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
					//(pEffect->m_nType/1000000 == 7 || pEffect->m_nType/10000000 == 1 || pEffect->m_nType / 100000   == 1))
					bEffectRender )
					// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
				{// 테스트(장착 아이템일경우)
					// 2005-07-29 by ispark
					// 적 캐릭터일 때 찍지마
					if(((CEnemyData *)pEffect->m_pParent)->m_bEnemyCharacter == FALSE)
					{
						// 2006-11-16 by ispark, 알파 검사
						if(CheckAlphaRender(pEffect, pEffect->m_pParent->m_dwPartType))
						{
							ALPHA_CHARACTERINFO stAlphaInfo;
							stAlphaInfo.pCharInfo = pChar;
							stAlphaInfo.nAlphaValue = ((CEnemyData *)pEffect->m_pParent)->m_nAlphaValue;
							g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
						}
						else if(((CEnemyData *)pEffect->m_pParent)->m_bIsRender &&
						(((CAtumData*)pEffect->m_pParent)->m_bDegree != 0 ||	// LOW가 아니거나
						// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
						//pEffect->m_nType/100000 == 78) )						// Skill Effect인 경우
						pEffect->m_nType/100000 == 78) ||						// Skill Effect인 경우
						bEffectRender )
						// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
						{
#ifdef C_USER_EFFECT_CONTROL	// 2012-10-23 by jhahn, 이펙트 옵션 조절
						// 2012-06-01 by jhahn, 이펙트 콤보수정
							if((g_pSOption->sTerrainEffectRender <= 1) && (((CEnemyData *)pEffect->m_pParent)->m_pWingIn))
							{
								((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_bRender = FALSE;
							}
							else if(((CEnemyData *)pEffect->m_pParent)->m_pWingIn)
							{
								((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_bRender = TRUE;
							}
						//end 2012-06-01 by jhahn, 이펙트 콤보수정
							

							if(g_pSOption->sTerrainEffectRender > 1 ) // 2012-06-01 by jhahn, 이펙트 콤보수정
#endif
								DevideCharacterEffect(pChar); // 적
						}
					}
					else
					{
						// 2006-06-29 by ispark, 악세사리 경우는 캐릭터 상태에서도 렌더링한다.
						if((((CEnemyData *)pEffect->m_pParent)->m_pContainer &&
							((CEnemyData *)pEffect->m_pParent)->m_pContainer->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pAccessories &&
							((CEnemyData *)pEffect->m_pParent)->m_pAccessories->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pWingIn &&
							((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_pCharacterInfo == pChar) ||
							// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner &&
							((CEnemyData *)pEffect->m_pParent)->m_pPartner->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner1 &&
							((CEnemyData *)pEffect->m_pParent)->m_pPartner1->m_pCharacterInfo == pChar))
							// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정							
						{
							if(((CEnemyData *)pEffect->m_pParent)->m_bIsRender)
							{
								DevideCharacterEffect(pChar);
							}
						}
					}
				}
				else
				{
					// 2006-11-16 by ispark, 알파 검사
					if(pEffect->m_pParent && 
						CheckAlphaRender(pEffect, _SHUTTLE))
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = pChar;
						stAlphaInfo.nAlphaValue = g_pShuttleChild->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else if(g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS &&
						g_pShuttleChild == pEffect->m_pParent)
					{
#ifdef C_USER_EFFECT_CONTROL	// 2012-10-23 by jhahn, 이펙트 옵션 조절
						if( (g_pShuttleChild->m_pWeapon1_1_1 && 
							pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon1_1_2 && 
							pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon2_1_1 && 
							pChar == g_pShuttleChild->m_pWeapon2_1_1->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon2_1_2 && 
							pChar == g_pShuttleChild->m_pWeapon2_1_2->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon1_2 &&
							pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo)	&&							
							g_pSOption->sTerrainEffectRender <= 1)
#else
						if( (g_pShuttleChild->m_pWeapon1_1_1 && 
							pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon1_1_2 && 
							pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon2_1_1 && 
							pChar == g_pShuttleChild->m_pWeapon2_1_1->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon2_1_2 && 
							pChar == g_pShuttleChild->m_pWeapon2_1_2->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon1_2 &&
							pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo))
#endif

							// 2012-06-01 by jhahn, 이펙트 콤보수정
							
						{
							DevideCharacterEffect(pChar);
						}
					}
					else
					{
						// 2005-07-13 by ispark
						// 캐릭터 렌더링시에는 무기를 그리지 않는다.
						// DevideCharacterEffect(pChar);
						if(g_pD3dApp->m_bCharacter)
						{
							// 2007-10-01 by bhsohn 인벤토리 파티클이펙트 문제 처리
//							if(g_pShuttleChild != pEffect->m_pParent)
//							{
								DevideCharacterEffect(pChar);
//							}
//							if(g_pShuttleChild != pEffect->m_pParent)
//							{
//								// 자신의 기체가 아니면 무조건 그린다.
//								DevideCharacterEffect(pChar);
//							}
//							else if((g_pShuttleChild->m_pContainer && g_pShuttleChild->m_pContainer->m_pCharacterInfo == pChar) 
//								||(g_pShuttleChild->m_pPet &&g_pShuttleChild->m_pPet->m_pCharacterInfo == pChar) 
//								||(g_pShuttleChild->m_pWingIn &&g_pShuttleChild->m_pWingIn->m_pCharacterInfo == pChar))
//							{
//								// 자신의 기체는 마크등만 그린다. 
//								DevideCharacterEffect(pChar);
//							}							
							// end 2007-10-01 by bhsohn 인벤토리 파티클이펙트 문제 처리
						}
						else
						{
							DevideCharacterEffect(pChar);	
						}
					}
				}
			}
			pEffect = (CAppEffectData*)pEffect->m_pNext;
		}
	}
}

void CEffectRender::Render()
{
	FLOG( "CEffectRender::Render()" );
	
	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	// 2006-11-16 by ispark, 알파 레더링 데이터 초기화
	g_pScene->m_vecAlphaEffectRender.clear();
	m_vecZEnableEffect.clear();
// 2008-03-19 by dgwoo 렉 최소화 작업.
//	if(g_pSOption->sTerrainEffectRender <=0)
//		return;
	DevideZBufferEnableEffect();	
	RenderZEnable();
	CCharacterInfo* pChar;	

	m_bZBufferTemp = FALSE;

	// effect gathering
	// object, character, ememy, monster, weapon, item
	

	// Object - Normal
//	if(g_pD3dApp->m_bDegree == 2) // 고사양
//	{
//		vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorRangeObjectPtrList.begin());
//		while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
//		{
///			CObjectChild * pObj = *itObj;
//			pChar = pObj->m_pCharacterInfo;
//			if(pChar)
//			{
//				RenderCharacterInfo(pChar);
//			}
//	//		m_bZBufferTemp = FALSE;
//			itObj++;
//		}
//	}
//	else
//	{


	// Object - Normal
	float fDistanceToCamera;
	float fDistance;
	
	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCulledObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCulledObjectPtrList.end())
	{
		CObjectChild * pObj = *itObj;
		fDistanceToCamera = D3DXVec3Length(&(pObj->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));		
		//		fDistance = (g_pScene->m_fOrgFogEndValue/(10-g_pSOption->sTerrainEffectRender));
		// 2007-07-11 by dgwoo 튜토리얼 맵에서 챕터(1_1 ~ 1_5)의 링을 챕터마다 다르게 그려줘야한다.
		if(g_pTutorial->IsTutorialMode() == TRUE &&				// 튜토리얼 모드 이면서.
//			g_pTutorial->GetLesson() == L1 &&					// 레슨 1이면서.
			pObj->m_nCode == TUTORIAL_GATE &&					// 게이트 오브젝트면서.
			!g_pTutorial->IsEnableTutorialGate(pObj->m_vPos))	// 현재 사용해야할 게이트라면.
		{// 
			int i = 0;
		}
		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
		else if(pObj->IsShowNode())
		{// 튜토리얼 모드가 아닐시.
			// 2005-05-10 by jschoi - 이펙트 옵션 삭제
			fDistance = g_pScene->m_fFogEndValue + pObj->m_pObjMesh->m_fRadius;

			if(fDistanceToCamera < fDistance)
			{
				pChar = pObj->m_pCharacterInfo;
				if(pChar)
				{
					RenderCharacterInfo(pChar);
				}
			}
		}


		
		itObj++;
	}
	
	
	if(g_pGround && g_pGround->m_pObjectEvent)
	{
		CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
		while(pObjEvent)
		{
			pChar = pObjEvent->m_pCharacterInfo;
			if(pChar)
			{
				BOOL bResult;
				// 2006-05-17 by ispark
				if(g_pD3dApp->m_dwGameState == _GAME)
				{
					bResult = g_pFrustum->CheckSphere( pChar->m_vPos.x, pChar->m_vPos.y, pChar->m_vPos.z, pObjEvent->m_pObjMesh->m_fRadius );
				}
				else
				{
					bResult = TRUE;
				}
				if(bResult == TRUE)
				{
					// 2007-07-11 by dgwoo 튜토리얼 모드에서 그리지 말아야할 링이 있다.
					if(g_pTutorial->IsTutorialMode() == TRUE &&				// 튜토리얼 모드 이면서.
//						g_pTutorial->GetLesson() == L1 &&					// 레슨 1이면서.
						pObjEvent->m_nCode == TUTORIAL_GATE &&					// 게이트 오브젝트면서.
						!g_pTutorial->IsEnableTutorialGate(pObjEvent->m_vPos))	// 현재 사용해야할 게이트라면.
					{
						int i = 0;
					}
					// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
					else if(pObjEvent->IsShowNode()) // else	
					{
						RenderCharacterInfo(pChar);
					}

				}
			}
			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}

	// ShuttleChild 와 CharacterChild
	// 2005-07-26 by ispark
	if(g_pD3dApp->m_bCharacter == FALSE)
	{
//		if(g_pShuttleChild && COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_GAMEMASTER)==FALSE)
		if(g_pShuttleChild)
		{
			pChar = g_pD3dApp->m_pShuttleChild->m_pCharacterInfo;
			if(pChar)
			{
//				// 2006-11-16 by ispark, 알파 체크
//				if(g_pShuttleChild->m_nAlphaValue < 255)
//				{
//					g_pScene->m_vecAlphaEffectRender.push_back(pChar);
//				}
//				else
				{
				RenderCharacterInfo(pChar);
				}
			}

			// 2004-10-12 by jschoi
			vector<SkillEffectInfo>::iterator itEffect = g_pShuttleChild->m_pSkillEffect->m_vecSkillEffect.begin();
			while(itEffect != g_pShuttleChild->m_pSkillEffect->m_vecSkillEffect.end())
			{
				if(itEffect->pCharacterInfo)
				{
					RenderCharacterInfo(itEffect->pCharacterInfo);
				}
				itEffect++;
			}

			// 2006-12-04 by ispark, 체프 사출
			vector<CItemData*>::iterator itItem = g_pShuttleChild->m_pChaffData.begin();
			while(itItem != g_pShuttleChild->m_pChaffData.end())
			{
				if((*itItem)->m_pCharacterInfo)
				{
					RenderCharacterInfo((*itItem)->m_pCharacterInfo);
				}
				itItem++;
			}
		}
	}
	else
	{
//		pChar = g_pD3dApp->m_pShuttleChild->m_pCharacterInfo;
//		if(pChar)
//		{
//			RenderCharacterInfo(pChar);
//		}

		// 캐릭터 이펙트
		if(g_pCharacterChild &&
			g_pCharacterChild->GetbPickMove())
		{
			pChar = g_pCharacterChild->m_pPickingInfo;
			if(pChar)
			{
				RenderCharacterInfo(pChar);
			}			
		}
	}
	
	// Enemy
	// 2005-07-26 by ispark
	CVecEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_vecEnemyRenderList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_vecEnemyRenderList.end())
	{
		// 2005-07-26 by ispark
		// 적이 캐릭터인지 판단
		if((*itEnemy)->m_bEnemyCharacter == FALSE)
		{
			// 적 기어 이펙트
			pChar = (*itEnemy)->m_pCharacterInfo;
			// 2008-07-23 by dgwoo Gm아머의 경우 이펙트가 안나오는 버그 수정.
			//if(pChar && (*itEnemy)->m_dwPartType != _ADMIN)
			if(pChar)
			{
				// 2006-11-16 by ispark, 알파 체크
				if((*itEnemy)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
				{
					ALPHA_CHARACTERINFO stAlphaInfo;
					stAlphaInfo.pCharInfo = pChar;
					stAlphaInfo.nAlphaValue = (*itEnemy)->m_nAlphaValue;
					g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
				}
				else
				{
					m_bZBufferTemp = (*itEnemy)->m_bZBuffer;
					RenderCharacterInfo(pChar);
					m_bZBufferTemp = FALSE;
				}
			}

			// 2004-10-12 by jschoi
			vector<SkillEffectInfo>::iterator itEffect = (*itEnemy)->m_pSkillEffect->m_vecSkillEffect.begin();
			while(itEffect != (*itEnemy)->m_pSkillEffect->m_vecSkillEffect.end())
			{
				if(itEffect->pCharacterInfo)
				{
					// 2006-11-16 by ispark, 알파 체크
					if((*itEnemy)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = itEffect->pCharacterInfo;
						stAlphaInfo.nAlphaValue = (*itEnemy)->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else
					{
						RenderCharacterInfo(itEffect->pCharacterInfo);
					}
				}
				itEffect++;
			}

			// 2006-12-04 by ispark, 체프 사출
			vector<CItemData*>::iterator itItem = (*itEnemy)->m_pChaffData.begin();
			while(itItem != (*itEnemy)->m_pChaffData.end())
			{
				if((*itItem)->m_pCharacterInfo)
				{
					// 2006-11-16 by ispark, 알파 체크
					if((*itEnemy)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = pChar;
						stAlphaInfo.nAlphaValue = (*itEnemy)->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else
					{
						RenderCharacterInfo((*itItem)->m_pCharacterInfo);
					}
				}
				itItem++;
			}
		}
		else
		{
			// 적 캐릭터 이펙트
		}

		itEnemy++;
	}

	// monster
//	if(g_pD3dApp->m_bDegree == 2) // 고사양
//	{
//		CMapMonsterIterator itMonster = g_pD3dApp->m_pScene->m_mapMonsterList.begin();
//		while(itMonster != g_pD3dApp->m_pScene->m_mapMonsterList.end())
//		{
//			pChar = (itMonster->second)->m_pCharacterInfo;
//			if(pChar)
//			{
//				RenderCharacterInfo(pChar);
//			}
//			itMonster++;
//		}
//	}
//	else
//	{
		CVecMonsterIterator itMonster = g_pD3dApp->m_pScene->m_vecMonsterRenderList.begin();
		while(itMonster != g_pD3dApp->m_pScene->m_vecMonsterRenderList.end())
		{
			pChar = (*itMonster)->m_pCharacterInfo;
			if(pChar)
			{
				RenderCharacterInfo(pChar);
			}

			// 2004-10-12 by jschoi
			vector<SkillEffectInfo>::iterator itEffect = (*itMonster)->m_pSkillEffect->m_vecSkillEffect.begin();
			while(itEffect != (*itMonster)->m_pSkillEffect->m_vecSkillEffect.end())
			{
				if(itEffect->pCharacterInfo)
				{
					RenderCharacterInfo(itEffect->pCharacterInfo);
				}
				itEffect++;
			}

			itMonster++;
		}
//	}
	// Weapon
	if(g_pD3dApp->m_pScene->m_pWeaponData)
	{
		CWeapon * pWeapon = (CWeapon*)g_pD3dApp->m_pScene->m_pWeaponData->m_pChild;
		while(pWeapon)
		{
			
			pChar = pWeapon->m_pCharacterInfo;
			if(pChar)
			{
// 2007-08-07 by dgwoo 아래 if문은 뭘까 --? 저렇게 변경.
//				if( IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE &&
//					g_pShuttleChild->m_bAttackMode == _SIEGE &&
//					pWeapon->m_pTarget == g_pShuttleChild &&
//					COMPARE_BODYCON_BIT(pWeapon->m_pCharacterInfo->m_nCurrentBodyCondition, BODYCON_HIT_MASK))
				if(	g_pShuttleChild->GetSkillMissileWarning() &&				//침묵스킬이 발동죽이면.
					pWeapon->m_pTarget == g_pShuttleChild &&					//타겟이 자신일때
					pWeapon->m_pItemData != NULL &&								
					IS_SECONDARY_WEAPON(pWeapon->m_pItemData->Kind))				//2형무기일때만. 
				{
					// rendering하지 않는다.
				}
				else
				{
					RenderCharacterInfo(pChar);
				}
			}
			pWeapon = (CWeapon *)pWeapon->m_pNext;
		}
	}
	// Item
	if(g_pD3dApp->m_pScene->m_pItemData)
	{
		CItemData * pItem = (CItemData*)g_pD3dApp->m_pScene->m_pItemData->m_pChild;
		while(pItem)
		{
			pChar = pItem->m_pCharacterInfo;
			if(pChar && pItem->m_bIsRender)
			{
				RenderCharacterInfo(pChar);
			}
			pItem->RenderItemName(); // 아이템 이름이 있을 경우 렌더링
			pItem = (CItemData *)pItem->m_pNext;
		}
	}
	if( //g_pScene->m_byMapType != MAP_TYPE_CITY && 
		(g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _SHOP) &&
		g_pScene->m_byWeatherType != WEATHER_FOGGY &&			// 안개낀
		g_pScene->m_byWeatherType != WEATHER_RAINY &&			// 비
		g_pScene->m_byWeatherType != WEATHER_SNOWY &&			// 눈
//		g_pScene->m_byWeatherType != WEATHER_CLOUDY &&			// 구름낀
		g_pScene->m_pSunData &&
		g_pScene->m_pSunData->m_pCharacterInfo &&
		IsSunRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))	
	{
		pChar = g_pD3dApp->m_pScene->m_pSunData->m_pCharacterInfo;
		if(pChar)
		{
			RenderCharacterInfo(pChar);
		}
	}
	// 적기의 미사일이나 총알등을 찍는 루틴. 
	if(g_pD3dApp->m_pEffectList)
	{
		CAppEffectData * pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
		while(pEffect)
		{
			pChar = pEffect->m_pCharacterInfo;
			if(pChar)
			{
				if( pEffect->m_pParent && 
					(pEffect->m_pParent->m_dwPartType == _ENEMY  || pEffect->m_pParent->m_dwPartType == _ADMIN )&&
					(pEffect->m_nType/1000000 == 7 || pEffect->m_nType/10000000 == 1))
				{// 테스트(장착 아이템일경우)
					// 2005-07-29 by ispark
					// 적 캐릭터일 때 찍지마
					if(((CEnemyData *)pEffect->m_pParent)->m_bEnemyCharacter == FALSE)
					{
						if(((CEnemyData *)pEffect->m_pParent)->m_bIsRender &&
							(((CAtumData*)pEffect->m_pParent)->m_bDegree != 0 ||	// LOW가 아니거나
							pEffect->m_nType/100000 == 78) &&						// Skill Effect인 경우
							(((CEnemyData *)pEffect->m_pParent)->m_bySkillStateFlag == CL_SKILL_NONE ||					// 2006-12-12 by ispark, 위장, 인비지블이 아니고
							(((CEnemyData *)pEffect->m_pParent)->m_bySkillStateFlag != CL_SKILL_NONE &&					// 2006-12-11 by ispark, 위장, 인비지블이라면
							(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, ((CEnemyData *)pEffect->m_pParent)->m_infoCharacter.CharacterInfo.InfluenceType) || // 2006-12-12 by ispark, 같은 세력 이거나
							!(((CEnemyData *)pEffect->m_pParent)->IsPkEnable())))))
						{
							RenderCharacterInfo(pChar);
						}
					}
					else
					{
						// 2006-06-29 by ispark, 악세사리 경우는 캐릭터 상태에서도 렌더링한다.
						if((((CEnemyData *)pEffect->m_pParent)->m_pContainer &&
							((CEnemyData *)pEffect->m_pParent)->m_pContainer->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pAccessories &&
							((CEnemyData *)pEffect->m_pParent)->m_pAccessories->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pWingIn &&
							((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_pCharacterInfo == pChar) ||
							// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner &&
							((CEnemyData *)pEffect->m_pParent)->m_pPartner->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner1 &&
							((CEnemyData *)pEffect->m_pParent)->m_pPartner1->m_pCharacterInfo == pChar))
							// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정							
						{
							if(((CEnemyData *)pEffect->m_pParent)->m_bIsRender)
							{
								DevideCharacterEffect(pChar);
							}
						}
					}
				}
				else
				{
					if(g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS &&
						g_pShuttleChild == pEffect->m_pParent)
					{
						if( (g_pShuttleChild->m_pWeapon1_1_1 && 
							pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) || 
							(g_pShuttleChild->m_pWeapon1_1_2 && 
							pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_2 &&
							pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo) ||
							pEffect->m_nType == RC_EFF_AGEAR_FIRE)
						{
							RenderCharacterInfo(pChar);
						}
					}
					else
					{
						// 2005-07-27 by ispark
						// 캐릭터 렌더링시에는 무기를 그리지 않는다.
						// DevideCharacterEffect(pChar);
						if(g_pD3dApp->m_bCharacter)
						{
//							if(g_pShuttleChild != pEffect->m_pParent)
							{
								RenderCharacterInfo(pChar);
							}
						}
						else
						{
							RenderCharacterInfo(pChar);	
						}					
					}
				}
			}
			pEffect = (CAppEffectData*)pEffect->m_pNext;
		}
	}

	// 2005-01-04 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );	
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
}

void CEffectRender::RenderCharacterInfo(CCharacterInfo* pChar, BOOL bAlpha, int nAlphaValue)
{
	FLOG( "CEffectRender::RenderCharacterInfo(CCharacterInfo* pChar)" );
	set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
	while(itCurrent != pChar->m_vecCurrentBodyCondition.end())
	{
		map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);
		if(itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
			while(itEffect != pBody->m_vecEffect.end())
			{
				// effect rendering
				CEffectInfo* pEffectInfo = (*itEffect);

				switch(pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_OBJECT:
					{
						if(pEffectInfo->m_pEffect)
						{
							CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
//							if(pEffect->m_bZbufferEnable)
//							{// Z buffer enable effect loading
//								m_vecZEnableEffect.push_back((Effect*)pEffect);
//							}
//							else 
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);	
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fRadius);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if( !pEffect->m_bZbufferEnable && 
								pEffect->m_bAlphaBlending && 
								bResult == TRUE)
							{
								ObjectAniRender(pEffect, bAlpha, nAlphaValue);
							}
						}
					}
					break;
				case EFFECT_TYPE_SPRITE:
					{
						if(pEffectInfo->m_pEffect)
						{
							CSpriteAni* pEffect = (CSpriteAni*)pEffectInfo->m_pEffect;
//							if(pEffect->m_bZbufferEnable)
//							{// Z buffer enable effect loading
//								m_vecZEnableEffect.push_back((Effect*)pEffect);
//							}
//							else 
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);	
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fTextureSize);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(!pEffect->m_bZbufferEnable && 
								bResult == TRUE)
							{
								SpriteAniRender(pEffect);
							}
						}
					}
					break;
				case EFFECT_TYPE_PARTICLE:
					{
						if(pEffectInfo->m_pEffect)
						{
							CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;
//							if(pEffect->m_bZbufferEnable)
//							{// Z buffer enable effect loading
//								m_vecZEnableEffect.insert(m_vecZEnableEffect.end(),
//									pEffect->m_vecParticle.begin(), pEffect->m_vecParticle.end());
//							}
//							else 
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);	
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fRadius);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(!pEffect->m_bZbufferEnable && 
								bResult == TRUE)
							{
								ParticleSystemRender(pEffect);
							}
						}
					}
					break;
				case EFFECT_TYPE_TRACE:
					{
						if(pEffectInfo->m_pEffect)
						{
							CTraceAni* pEffect = (CTraceAni*)pEffectInfo->m_pEffect;
//							if(pEffect->m_bZbufferEnable)
//							{// Z buffer enable effect loading
//								m_vecZEnableEffect.insert(m_vecZEnableEffect.end(),
//									pEffect->m_vecParticle.begin(), pEffect->m_vecParticle.end());
//							}
//							else 
							// 2006-05-17 by ispark
							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z, 
											pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y, 
											pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);	
//							BOOL bResult = g_pFrustum->CheckSphere( pEffect->m_pParent->m_pParent->m_mMatrix._41, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._42, 
//																	pEffect->m_pParent->m_pParent->m_mMatrix._43,  
//																	pEffect->m_fRadius);
							BOOL bResult = g_pFrustum->CheckSphere( vPos.x, 
																	vPos.y, 
																	vPos.z,  
																	pEffect->m_fRadius);
							// 2006-05-23 by ispark
							if(g_pD3dApp->m_dwGameState != _GAME)
							{
								bResult = TRUE;
							}
							if(!pEffect->m_bZbufferEnable && 
								bResult == TRUE)
							{
								TraceAniRender(pEffect);
							}
						}
					}
					break;
				}
				itEffect++;
			}//if(!Effect.end())
		}//if(!bodyCondition.end())
//		}//if(0)
		itCurrent++;
	}//end()

}

void CEffectRender::SpriteAniRender(CSpriteAni* pEffect)
{
	FLOG( "CEffectRender::SpriteAniRender(CSpriteAni* pEffect)" );
	DWORD dwSrc,dwDest,dwColorOp;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
	g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);

	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable);

	if( pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable );
	}
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bAlphaBlending );
	if(pEffect->m_bAlphaBlending)
	{
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nDestBlend);
	}
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	g_pD3dDev->SetFVF( D3DFVF_SPRITE_VERTEX );

	// set light
	D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
	m_light2 = g_pD3dApp->m_pScene->m_light2;
	m_light2.Direction  = vAxis;
 	g_pD3dDev->SetLight( 2, &m_light2 );
	g_pD3dDev->LightEnable( 2, TRUE );

	D3DXMATRIX matScale;
//	D3DXVECTOR3 pos = pEffect->m_pParent->m_pParent->m_pParent->m_vPos + pEffect->m_pParent->m_vPos;
	D3DXVECTOR3 pos = pEffect->m_pParent->m_vPos;
	D3DXVec3TransformCoord( &pos, &pos, &pEffect->m_pParent->m_pParent->m_mMatrix);
	 
	D3DXMatrixScaling(&matScale, pEffect->m_fTextureSize,pEffect->m_fTextureSize,pEffect->m_fTextureSize);
	matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();
	if((pEffect->m_pParent && pEffect->m_pParent->m_fBillboardRotateAngle > 0 )|| 
	   pEffect->m_fCurrentRotateAngle != 0)
	{
		D3DXMATRIX matRotate;
//		vAxis = pEffect->m_pParent->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
//		vAxis = pos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentRotateAngle );
		matScale *= matRotate;
	}
	matScale._41 = pos.x;
	matScale._42 = pos.y;
	matScale._43 = pos.z;

	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, pEffect->m_cColor.r, 
		pEffect->m_cColor.g, pEffect->m_cColor.b, pEffect->m_cColor.a);

	g_pD3dDev->SetTransform( D3DTS_WORLD, &matScale );
	if(pEffect->m_nTextureVertexBufferType == 0)
		g_pD3dDev->SetStreamSource( 0, m_pVB4[pEffect->m_nSpriteType],0, sizeof(SPRITE_VERTEX) );
	else if(pEffect->m_nTextureVertexBufferType == 1)
		g_pD3dDev->SetStreamSource( 0, m_pVB8[pEffect->m_nSpriteType],0, sizeof(SPRITE_VERTEX) );
	else if(pEffect->m_nTextureVertexBufferType == 2)
		g_pD3dDev->SetStreamSource( 0, m_pVB16[pEffect->m_nSpriteType],0, sizeof(SPRITE_VERTEX) );
	else if(pEffect->m_nTextureVertexBufferType == 3)
		g_pD3dDev->SetStreamSource( 0, m_pVB2[pEffect->m_nSpriteType],0, sizeof(SPRITE_VERTEX) );
	else if(pEffect->m_nTextureVertexBufferType == 4)
		g_pD3dDev->SetStreamSource( 0, m_pVB1,0, sizeof(SPRITE_VERTEX) );
	g_pD3dDev->SetMaterial( &mtrl );
	// set texture
	if(pEffect->m_pParent && pEffect->m_pParent->m_pTexture ) 
	{
		g_pD3dDev->SetTexture(0, pEffect->m_pParent->m_pTexture ); 
	}
	else
	{
		int index = LoadTexture(pEffect->m_strTextureFile);
		if(index>=0)
		{
			g_pD3dDev->SetTexture( 0, m_pTexture[index]);
			if(m_nTextureRenderCount[index] == 2)
				m_nTextureRenderCount[index]++;
		}
	}
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	if(strlen(pEffect->m_strLightMapFile)>0)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,pEffect->m_bLightMapAlphaBlending );
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nLightMapSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nLightMapDestBlend);
		g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nLightMapRenderState);
//		Set texture
		if(pEffect->m_pParent && pEffect->m_pParent->m_pTexture ) {
			g_pD3dDev->SetTexture(0, pEffect->m_pParent->m_pTexture ); }
		else
		{
			int index = LoadTexture(pEffect->m_strLightMapFile);
			if(index>=0)
			{
				g_pD3dDev->SetTexture( 0, m_pTexture[index]);
				if(m_nTextureRenderCount[index] == 2)
					m_nTextureRenderCount[index]++;
			}
		}
		g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	}
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);

	g_pD3dDev->LightEnable( 2, FALSE );
	if( pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}
}


void CEffectRender::ParticleSystemRender(CParticleSystem* pEffect)
{
	FLOG( "CEffectRender::ParticleSystemRender(CParticleSystem* pEffect)" );
//	g_pD3dDev->ApplyStateBlock( pEffect->m_dwStateBlock );
	switch(pEffect->m_nParticleType)
	{
	case PARTICLE_OBJECT_TYPE:
		{
			for(int i=0; i<pEffect->m_vecParticle.size(); i++)
			{
				ObjectParticleRender(((CParticle*)pEffect->m_vecParticle[i])->m_pObjectAni, (CParticle*)pEffect->m_vecParticle[i]);
			}
		}
		break;
	case PARTICLE_SPRITE_TYPE:
		{
			g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			g_pD3dDev->SetFVF( D3DFVF_SPRITE_VERTEX );
		//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

//			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
//			g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,         0x08 );
//			g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );

//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable);
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, pEffect->m_dwSrcBlend );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, pEffect->m_dwDestBlend );
			g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
			g_pD3dDev->SetStreamSource( 0, m_pVB1,0, sizeof(SPRITE_VERTEX) );
			// set light
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = vAxis;
 			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
			int nOldTextureIndex = -1;
			for(int i=0; i<pEffect->m_vecParticle.size(); i++)
			{
				CParticle* p = (CParticle*)pEffect->m_vecParticle[i];
				nOldTextureIndex = ParticleRender(pEffect, p, vAxis, nOldTextureIndex);
			}
			g_pD3dDev->LightEnable( 2, FALSE );
		}
		break;
	case PARTICLE_TRACE_TYPE:
		{
			for(int i=0; i<pEffect->m_vecParticle.size(); i++)
			{
				//TraceAniRender(((CParticle*)pEffect->m_vecParticle[i])->m_pObjectAni, (CParticle*)pEffect->m_vecParticle[i]);
			}
		}
		break;
	}

}

int CEffectRender::ParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex)
{
	FLOG( "CEffectRender::ParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex)" );
	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, p->m_cColor.r, p->m_cColor.g,p->m_cColor.b, p->m_cColor.a);
	pos = p->m_vPos;//실제 좌표
	
	// 2010. 03. 18 by jskim 몬스터변신 카드
	float tempScale = 0.0f;
	if(p->m_pParent->m_pParent->m_pParent->m_MonsterTransformer &&
		p->m_pParent->m_pParent->m_pParent->m_MonsterTransScale > 0)
	{
		tempScale = p->m_fSize * p->m_pParent->m_pParent->m_pParent->m_MonsterTransScale;
	}
	else
	{
		tempScale = p->m_fSize;
	}
	//D3DXMatrixScaling(&matScale, p->m_fSize,p->m_fSize,p->m_fSize);	
	D3DXMatrixScaling(&matScale, tempScale,tempScale,tempScale);
	// 2010. 03. 18 by jskim 몬스터변신 카드
	
	matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();
	// 2004.2.16 파티클 랜덤 회전
	if(pParticleSystem->m_pParent && p->m_fCurrentRotateAngle != 0 )//pParticleSystem->m_pParent->m_fBillboardRotateAngle > 0)
	{
		D3DXMATRIX matRotate;
//			vAxis = pParticleSystem->m_pParent->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
//			vAxis = p->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis( &matRotate, &vAxis, p->m_fCurrentRotateAngle );
		matScale *= matRotate;
	}
	matScale._41 = pos.x;
	matScale._42 = pos.y;
	matScale._43 = pos.z;
//		matScale._41 += pParticleSystem->m_pParent->m_vPos.x;
//		matScale._42 += pParticleSystem->m_pParent->m_vPos.y;
//		matScale._43 += pParticleSystem->m_pParent->m_vPos.z;

	g_pD3dDev->SetTransform( D3DTS_WORLD, &matScale );
	// set texture
	
	int index = 0;
	if(pParticleSystem->m_pParent && pParticleSystem->m_pParent->m_pTexture) {
		g_pD3dDev->SetTexture( 0, pParticleSystem->m_pParent->m_pTexture); }
	else if(nOldTextureIndex == -1 || pParticleSystem->m_nTextureNumber != 1)
	{
		index = LoadTexture(pParticleSystem->m_strTextureName[p->m_nTextureType]);
		if(index>=0 && nOldTextureIndex != index)
		{
			g_pD3dDev->SetTexture( 0, m_pTexture[index]);
			if(m_nTextureRenderCount[index] == 2)
				m_nTextureRenderCount[index]++;
			nOldTextureIndex = index;
		}
	}
	g_pD3dDev->SetMaterial( &mtrl );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	return nOldTextureIndex;

}

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
void CEffectRender::RenderParticleInvenVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling)
{
	if(m_vecInvenParticleInfo.empty())
	{
		return;
	}
	int  nSize = m_vecInvenParticleInfo.size();
	int nRender = 0;

	vector<structInvenParticleInfo>::iterator itInvenEffect = m_vecInvenParticleInfo.begin();
	
	while(itInvenEffect != m_vecInvenParticleInfo.end())
	{
		structInvenParticleInfo struParticleInfo = (*itInvenEffect);

		if(struParticleInfo.nWindowInvenIdx != nMatIndex)
		{
			itInvenEffect++;
			continue;
		}
		CEffectInfo*  pEffectInfo = GetEffectInfo(struParticleInfo.chEffectName, struParticleInfo.nWindowInvenIdx);
		if(NULL == pEffectInfo)
		{
			itInvenEffect++;
			continue;
		}
		
		CParticleSystem* pParticleSystem = (CParticleSystem*)pEffectInfo->m_pEffect;						
		
		if(NULL == pParticleSystem)
		{
			itInvenEffect++;
			continue;
		}

		{
			g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			g_pD3dDev->SetFVF( D3DFVF_SPRITE_VERTEX );
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pParticleSystem->m_bZbufferEnable );
//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
			if( pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pParticleSystem->m_bZWriteEnable );
			}
			
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, pParticleSystem->m_dwSrcBlend );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, pParticleSystem->m_dwDestBlend );

			g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

			g_pD3dDev->SetStreamSource( 0, m_pVB1,0, sizeof(SPRITE_VERTEX) );
			// set light
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = vAxis;
			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
			
			// 실제로 파티클 이펙트 랜더링 해주는 곳			
			int nOldTextureIndex = -1;
			int i =0;
			for(i=0; i<pParticleSystem->m_vecParticle.size(); i++)
			{
				CParticle* p = (CParticle*)pParticleSystem->m_vecParticle[i];
				nOldTextureIndex = InvenParticleRender(pParticleSystem, p, vAxis, nOldTextureIndex, fUnitScaling, &matPos, &matShuttlePos);
			}			
			
			g_pD3dDev->LightEnable( 2, FALSE );
			if( pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			}
			// 파티클 환경 종료
			{
				g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
				g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
			}

			nRender++;
		}
		itInvenEffect++;
	}
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
void CEffectRender::RenderParticleEnemyItemVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling, UID32_t TargetCharcterUID)
{
	if(m_vecEnemyItemParticleInfo.empty())
	{
		return;
	}
	int  nSize = m_vecEnemyItemParticleInfo.size();
	int nRender = 0;
	
	vector<structInvenParticleInfo>::iterator itInvenEffect = m_vecEnemyItemParticleInfo.begin();
	
	while(itInvenEffect != m_vecEnemyItemParticleInfo.end())
	{
		structInvenParticleInfo struParticleInfo = (*itInvenEffect);
		
		if(struParticleInfo.nWindowInvenIdx != nMatIndex)
		{
			itInvenEffect++;
			continue;
		}
		CEffectInfo*  pEffectInfo = GetEnemyEffectInfo(struParticleInfo.chEffectName, struParticleInfo.nWindowInvenIdx, TargetCharcterUID);
		if(NULL == pEffectInfo)
		{
			itInvenEffect++;
			continue;
		}
		
		CParticleSystem* pParticleSystem = (CParticleSystem*)pEffectInfo->m_pEffect;						
		
		if(NULL == pParticleSystem)
		{
			itInvenEffect++;
			continue;
		}
		
		{
			g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			g_pD3dDev->SetFVF( D3DFVF_SPRITE_VERTEX );
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pParticleSystem->m_bZbufferEnable );
			//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
			if( pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pParticleSystem->m_bZWriteEnable );
			}
			
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, pParticleSystem->m_dwSrcBlend );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, pParticleSystem->m_dwDestBlend );
			
			g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
			
			g_pD3dDev->SetStreamSource( 0, m_pVB1,0, sizeof(SPRITE_VERTEX) );
			// set light
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = vAxis;
			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
			
			// 실제로 파티클 이펙트 랜더링 해주는 곳			
			int nOldTextureIndex = -1;
			int i =0;
			for(i=0; i<pParticleSystem->m_vecParticle.size(); i++)
			{
				CParticle* p = (CParticle*)pParticleSystem->m_vecParticle[i];
				nOldTextureIndex = InvenParticleRender(pParticleSystem, p, vAxis, nOldTextureIndex, fUnitScaling, &matPos, &matShuttlePos);
			}			
			
			g_pD3dDev->LightEnable( 2, FALSE );
			if( pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			}
			// 파티클 환경 종료
			{
				g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
				g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
			}
			
			nRender++;
		}
		itInvenEffect++;
	}
}
// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
void CEffectRender::ResetContentInvneParticle()
{
	// 인벤 파티클 초기화
	m_vecInvenParticleInfo.clear();
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
void CEffectRender::ResetContentEnemyItemParticle()
{	
	m_vecEnemyItemParticleInfo.clear();
}
// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

int CEffectRender::InvenParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex, float fUnitScaling, D3DXMATRIX* pmatPaticlePos, D3DXMATRIX* pmatShttlePos)
{
	FLOG( "CEffectRender::ParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex)" );
	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, p->m_cColor.r, p->m_cColor.g,p->m_cColor.b, p->m_cColor.a);
	
	float fSize = p->m_fSize;
	pos = p->m_vPos;//실제 좌표
	if(pmatPaticlePos && pmatShttlePos)
	{
		// 스케일링
		//fSize *= 0.0074f;
		fSize *= (fUnitScaling/2);
		
		
		// 현재 월드에 대한 정보		
		D3DXVECTOR3 vInvenShuttle, vParticlePos, vObjInvenPos;	// 실제 그려질 파티클 위치
		D3DXVECTOR3 vDirWorldVel; // 월드 방향벡터 
		D3DXVECTOR3 vDirInvenVel; // 인벤 방향벡터 
		D3DXVECTOR3 vWeaponInvenVel; // 무기 방향벡터

		vWeaponInvenVel = D3DXVECTOR3(0,0,0);
		D3DXVECTOR3 vParCenter = D3DXVECTOR3(0,0,0);// 파티클의 중심점			
		D3DXVECTOR3 vParResultDir = D3DXVECTOR3(0,0,0);// 파티클의 중심점			
		
		// 인벤토리내에 기체의 위치
		{
			vInvenShuttle.x = pmatShttlePos->_41;
			vInvenShuttle.y = pmatShttlePos->_42;
			vInvenShuttle.z = pmatShttlePos->_43;
		}

		// 인벤토리내 무기 오브젝트 위치
		{		
			vObjInvenPos.x = pmatPaticlePos->_41;
			vObjInvenPos.y = pmatPaticlePos->_42;
			vObjInvenPos.z = pmatPaticlePos->_43;	
		}
				
		// 파티클에 실질적으로 위치할 위치
		{
			vParticlePos.x = pmatPaticlePos->_41;
			vParticlePos.y = pmatPaticlePos->_42;
			vParticlePos.z = pmatPaticlePos->_43;
		}
		
		// 인벤 방향 벡터
		{			
			vDirInvenVel.x = pmatPaticlePos->_31;
			vDirInvenVel.y = pmatPaticlePos->_32;
			vDirInvenVel.z = pmatPaticlePos->_33;
			vDirInvenVel *= -1;
			D3DXVec3Normalize( &vDirInvenVel, &vDirInvenVel);			
		}
		
		// 월드 좌표 방향 벡터
		{
			vDirWorldVel.x = pParticleSystem->m_pParent->m_pParent->m_mMatrix._31;
			vDirWorldVel.y = pParticleSystem->m_pParent->m_pParent->m_mMatrix._32;
			vDirWorldVel.z = pParticleSystem->m_pParent->m_pParent->m_mMatrix._33;
			vDirWorldVel *= -1;
			D3DXVec3Normalize( &vDirWorldVel, &vDirWorldVel);			
			
		}
		// 월드 좌표에 실질적인 파티클의 중심점		
		{
			vParCenter.x = pParticleSystem->m_pParent->m_pParent->m_mMatrix._41;
			vParCenter.y = pParticleSystem->m_pParent->m_pParent->m_mMatrix._42;
			vParCenter.z = pParticleSystem->m_pParent->m_pParent->m_mMatrix._43;
		}			
		
		D3DXVECTOR3 vCrossWorldAxisY;// Y 축을 구한다.
		D3DXVECTOR3 vCrossWorldAxisX;// X 축을 구한다.
		float fAngleRadianY =0.0f;
		float fAngleRadianX =0.0f;
		{
			D3DXVECTOR3 vTmpWeaponVel = p->m_vPos - vParCenter;
			D3DXVec3Normalize( &vTmpWeaponVel, &vTmpWeaponVel);		
			

			D3DXVec3Cross(&vCrossWorldAxisY, &vDirWorldVel, &vTmpWeaponVel); 
			D3DXVec3Normalize(&vCrossWorldAxisY,&vCrossWorldAxisY); // 중심축이다. 
			
			D3DXVec3Cross(&vCrossWorldAxisX, &vDirWorldVel, &vCrossWorldAxisY); 
			D3DXVec3Normalize(&vCrossWorldAxisX,&vCrossWorldAxisX); // 중심축이다. 

			fAngleRadianX = GetRadianVectorBetweenVector(vCrossWorldAxisX, vTmpWeaponVel);
			fAngleRadianY = GetRadianVectorBetweenVector(vDirWorldVel, vTmpWeaponVel);
		}

		{
			D3DXMATRIX matRotateY;
			D3DXMATRIX matRotateX;
			D3DXMATRIX matRotate;

			D3DXMatrixIdentity( &matRotateY); 
			D3DXMatrixIdentity( &matRotateX);
			D3DXMatrixRotationAxis(&matRotateY, &vCrossWorldAxisY, fAngleRadianY); 
			D3DXMatrixRotationAxis(&matRotateX, &vCrossWorldAxisX, fAngleRadianX); 
			matRotate = matRotateX * matRotateY;
			// 실질적인 이펙트 벡터를 구한다.
			D3DXVec3TransformCoord( &vParResultDir, &vDirInvenVel, &matRotate ); 
			D3DXVec3Normalize(&vParResultDir,&vParResultDir); // 중심축이다. 			
		}
		
		{			
			// 월드좌표에 파티클 거리
			D3DXVECTOR3 vParVel = vParCenter - p->m_vPos;
			float fParticleLength = D3DXVec3Length(&vParVel);			
			
			//fParticleLength *= 0.025f;				
			fParticleLength *= fUnitScaling;			
			
			vParResultDir*= fParticleLength;
			vParticlePos += vParResultDir;
		}

//		{
//			//p->m_vDir;		
//			// 월드좌표에 파티클 거리
//			D3DXVECTOR3 vParVel = vParCenter - p->m_vPos;
//			float fParticleLength = D3DXVec3Length(&vParVel);
//			D3DXVec3Normalize( &vParVel, &vParVel);
//			
//			//fParticleLength *= 0.025f;				
//			fParticleLength *= fUnitScaling;
//			
//			vParVel*= fParticleLength;
//			vParticlePos += vParVel;
//		}
		pos	= vParticlePos;
	}
	
	D3DXMatrixScaling(&matScale, fSize,fSize,fSize);
	
	//matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();	
	
	// 2004.2.16 파티클 랜덤 회전
	if(pParticleSystem->m_pParent && p->m_fCurrentRotateAngle != 0 )//pParticleSystem->m_pParent->m_fBillboardRotateAngle > 0)
	{
		D3DXMATRIX matRotate;
//			vAxis = pParticleSystem->m_pParent->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
//			vAxis = p->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis( &matRotate, &vAxis, p->m_fCurrentRotateAngle );
		matScale *= matRotate;
	}
	matScale._41 = pos.x;
	matScale._42 = pos.y;
	matScale._43 = pos.z;
//		matScale._41 += pParticleSystem->m_pParent->m_vPos.x;
//		matScale._42 += pParticleSystem->m_pParent->m_vPos.y;
//		matScale._43 += pParticleSystem->m_pParent->m_vPos.z;

	g_pD3dDev->SetTransform( D3DTS_WORLD, &matScale );
	// set texture
	
	int index = 0;
	if(pParticleSystem->m_pParent && pParticleSystem->m_pParent->m_pTexture) {
		g_pD3dDev->SetTexture( 0, pParticleSystem->m_pParent->m_pTexture); }
	else if(nOldTextureIndex == -1 || pParticleSystem->m_nTextureNumber != 1)
	{
		index = LoadTexture(pParticleSystem->m_strTextureName[p->m_nTextureType]);
		if(index>=0 && nOldTextureIndex != index)
		{
			g_pD3dDev->SetTexture( 0, m_pTexture[index]);
			if(m_nTextureRenderCount[index] == 2)
				m_nTextureRenderCount[index]++;
			nOldTextureIndex = index;
		}
	}
	g_pD3dDev->SetMaterial( &mtrl );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	return nOldTextureIndex;
}


void CEffectRender::EffectPlaneRender( CEffectPlane *pEffect )
{
	if(pEffect->m_pParent->m_nCurrentNumberOfTrace<=0)
		return;
	int index = LoadTexture(pEffect->m_pParent->m_strTextureName[pEffect->m_pParent->m_nCurrentTextureNumber]);

	DWORD dwSrc,dwDest,dwColorOp;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
	g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);

//	g_pD3dDev->ApplyStateBlock( pEffect->m_dwStateBlock );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_pParent->m_bZbufferEnable );
	if( pEffect->m_pParent->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pEffect->m_pParent->m_bZWriteEnable );
	}
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, pEffect->m_pParent->m_bAlphaBlendEnable );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, pEffect->m_pParent->m_dwSrcBlend );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, pEffect->m_pParent->m_dwDestBlend );
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_pParent->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	// 2005-01-03 by jschoi
//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat);
	g_pD3dDev->SetTransform( D3DTS_WORLD, &mat );
    g_pD3dDev->SetFVF( D3DFVF_SPRITE_VERTEX );
	if(index>=0)
	{
		if( pEffect->m_pParent && 
			pEffect->m_pParent->m_pParent && 
			pEffect->m_pParent->m_pParent->m_pTexture ) 
		{
			g_pD3dDev->SetTexture( 0, pEffect->m_pParent->m_pParent->m_pTexture);
		}
		else 
		{
			g_pD3dDev->SetTexture( 0, m_pTexture[index]);
		}
		if(m_nTextureRenderCount[index] == 2)
		{
			m_nTextureRenderCount[index]++;
		}
	}

//	g_pD3dDev->SetTexture( 0, NULL);
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, 1.0f,1.0f,1.0f, 1.0f);
	g_pD3dDev->SetMaterial( &mtrl );
	pEffect->Render();
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	if( pEffect->m_pParent->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}
}

void CEffectRender::TraceAniRender( CTraceAni* pEffect )
{
	FLOG( "CEffectRender::TraceAniRender( CTraceAni* pEffect )" );
	
	for(int i=0;i<pEffect->m_nNumberOfTrace;i++)
	{
		if(pEffect->m_pEffectPlane[i])
		{
			EffectPlaneRender(pEffect->m_pEffectPlane[i]);
		}
	}
	
/*	if(pEffect->m_nCurrentNumberOfTrace<=0)
		return;
	int index = LoadTexture(pEffect->m_strTextureName[pEffect->m_nCurrentTextureNumber]);

	DWORD dwSrc,dwDest,dwColorOp;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
	g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);

//	g_pD3dDev->ApplyStateBlock( pEffect->m_dwStateBlock );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, pEffect->m_bAlphaBlendEnable );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, pEffect->m_dwSrcBlend );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, pEffect->m_dwDestBlend );
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat);
	g_pD3dDev->SetTransform( D3DTS_WORLD, &mat );
    g_pD3dDev->SetVertexShader( D3DFVF_SPRITE_VERTEX );
	if(index>=0)
	{
		if(pEffect->m_pParent && pEffect->m_pParent->m_pTexture) 
		{
			g_pD3dDev->SetTexture( 0, pEffect->m_pParent->m_pTexture);
		}
		else 
		{
			g_pD3dDev->SetTexture( 0, m_pTexture[index]);
		}
		if(m_nTextureRenderCount[index] == 2)
			m_nTextureRenderCount[index]++;
	}
	D3DMATERIAL8 mtrl;
	D3DUtil_InitMaterial(mtrl, 1.0f,1.0f,1.0f, 1.0f);
	g_pD3dDev->SetMaterial( &mtrl );
	g_pD3dDev->SetStreamSource( 0, pEffect->m_pVBTrace[0], sizeof(SPRITE_VERTEX) );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0,pEffect->m_nCurrentNumberOfTrace*2 );
	g_pD3dDev->SetStreamSource( 0, pEffect->m_pVBTrace[1], sizeof(SPRITE_VERTEX) );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0,pEffect->m_nCurrentNumberOfTrace*2 );
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);
*/
}


void CEffectRender::ObjectAniRender(CObjectAni* pEffect, BOOL bAlpha, int nAlphaValue)
{
	FLOG( "CEffectRender::ObjectAniRender(CObjectAni* pEffect)" );
	if( strlen(pEffect->m_strName) == 0 )
	{
		return;
	}
	DWORD dwSrc,dwDest,dwColorOp;
	DWORD dwFogValue = FALSE;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
	g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);

    g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  pEffect->m_bAlphaTestEnble );
	if(pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHAREF, pEffect->m_nAlphaTestValue );
		g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	}

	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
	// 2008-07-30 by dgwoo 아겨 특정아머 부스터 사용시 이펙트가 아머뒤에 있을경우에도 보인다.
	// 2008-10-22 by bhsohn 부스터 쓰면서 공폭 사용시, 네모난 텍스처 생기는 버그 수정
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable);
	//g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);

	if( pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable );
	}
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bAlphaBlending );
	if(pEffect->m_bAlphaBlending)
	{
		g_pD3dDev->GetRenderState( D3DRS_FOGENABLE,  &dwFogValue );
		if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  FALSE );
		}
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nDestBlend);
	}
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	
	//*--------------------------------------------------------------------------*//
	// 2006-11-16 by ispark, 알파
	if(bAlpha)
	{
		g_pD3dApp->SetAlphaRenderState(nAlphaValue);
	}
	//*--------------------------------------------------------------------------*//

	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	if(pEffect->m_pParent)
	{
		pos = pEffect->m_pParent->m_vPos;//상대 좌표
	}
	else 
	{
		pos = D3DXVECTOR3(0,0,0);
	}
	// 2010. 03. 18 by jskim 몬스터변신 카드
	float tempScale = 0.0f;
	if(pEffect->m_pParent->m_pParent->m_MonsterTransformer &&
	   pEffect->m_pParent->m_pParent->m_MonsterTransScale > 0)
	{
		tempScale = pEffect->m_fScale * pEffect->m_pParent->m_pParent->m_MonsterTransScale;
	}
	else
	{
		tempScale = pEffect->m_fScale;
	}
	//D3DXMatrixScaling(&matScale, pEffect->m_fScale,pEffect->m_fScale,pEffect->m_fScale);
	D3DXMatrixScaling(&matScale, tempScale,tempScale,tempScale);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	if(pEffect->m_pParent && pEffect->m_pParent->m_bUseBillboard)
	{
		if( pEffect->m_pParent->m_bGroundBillboard == TRUE )
		{
			D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41, 
				pEffect->m_pParent->m_pParent->m_mMatrix._42,
				pEffect->m_pParent->m_pParent->m_mMatrix._43);
			D3DXMATRIX matTemp;
			D3DXVECTOR3 vVel = g_pD3dApp->m_pCamera->GetEyePt() - vPos;
			D3DXVECTOR3 vUp(0,1,0);
			if( vVel == vUp )
			{
				vVel = D3DXVECTOR3(1,0,0);
			}
			else
			{
				D3DXVec3Cross(&vVel, &vVel, &vUp );
				D3DXVec3Cross(&vVel, &vVel, &vUp );
			}
			D3DXVec3Normalize( &vVel, &vVel );
			D3DXMatrixLookAtLH( &matTemp, &(vPos), &(vPos + vVel ), &vUp);
			D3DXMatrixInverse( &matTemp, NULL, &matTemp );
			matScale = matTemp * matScale;
		}
		else
		{
			D3DXMATRIX matBillboard = g_pD3dApp->m_pCamera->GetBillboardMatrix();
			D3DXMATRIX matRotate;
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();//pos - g_pD3dApp->m_pCamera->GetEyePt();
			D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_pParent->m_fBillboardAngle );
			matBillboard *= matRotate;

			if(pEffect->m_pParent->m_fBillboardRotatePerSec>0)
			{
				D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentBillboardRotateAngle );
				matBillboard *= matRotate;
			} 
			if(pEffect->m_fCurrentRandomUpAngleX != 0)
			{
				D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentRandomUpAngleX );
				matBillboard *= matRotate;
			}
			matScale._41 = pEffect->m_pParent->m_vPos.x;
			matScale._42 = pEffect->m_pParent->m_vPos.y;
			matScale._43 = pEffect->m_pParent->m_vPos.z;
			
			matBillboard = matScale * matBillboard;

			matScale = matScale * pEffect->m_pParent->m_pParent->m_mMatrix;
			matBillboard._41 = matScale._41;
			matBillboard._42 = matScale._42;
			matBillboard._43 = matScale._43;
			matScale = matBillboard;
		}
		// set light
		if(!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = g_pD3dApp->m_pCamera->GetViewDir();
 			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
		}
	} else
	{
		D3DXMATRIX lookat;
		D3DXVECTOR3 up = pEffect->m_pParent->m_vUp;
		D3DXVECTOR3 target = pEffect->m_pParent->m_vTarget;
		if(pEffect->m_fCurrentRandomUpAngleX != 0 || pEffect->m_fCurrentRandomUpAngleZ != 0)
		{
			up = D3DXVECTOR3(0,1,0);
			target = pEffect->m_pParent->m_vPos;
			D3DXVECTOR3 vel = D3DXVECTOR3(0,0,1);
			D3DXMATRIX matRotateX, matRotateZ;
			D3DXVECTOR3 vAxis(1,0,0);// up vector 회전축(X축)
			D3DXMatrixRotationAxis( &matRotateX, &vAxis, pEffect->m_fCurrentRandomUpAngleX);
			D3DXVec3TransformCoord( &up, &up, &matRotateX );
			D3DXVec3TransformCoord( &vel, &vel, &matRotateX );
			vAxis = D3DXVECTOR3(0,0,1);// up vector 회전축(X축)
			D3DXMatrixRotationAxis( &matRotateZ, &vAxis, pEffect->m_fCurrentRandomUpAngleZ);
			D3DXVec3TransformCoord( &up, &up, &matRotateZ );
			D3DXVec3TransformCoord( &vel, &vel, &matRotateZ );
			target += vel;
		} 
		D3DXMatrixLookAtLH( &lookat, &pos, &target, &up);
		D3DXMatrixInverse( &lookat, NULL, &lookat );
		matScale = lookat * matScale;// * matScale;
		D3DXMATRIX mat = pEffect->m_pParent->m_pParent->m_mMatrix;
		mat._41 = 0;
		mat._42 = 0;
		mat._43 = 0;
		matScale *= mat;
		matScale._41 += pEffect->m_pParent->m_pParent->m_mMatrix._41;
		matScale._42 += pEffect->m_pParent->m_pParent->m_mMatrix._42;
		matScale._43 += pEffect->m_pParent->m_pParent->m_mMatrix._43;

		// set light
		if(!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = g_pD3dApp->m_pCamera->GetViewDir();
 			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
		}
	}
	CSkinnedMesh* pMesh = LoadObject(pEffect->m_strObjectFile);
	if(!pMesh)
	{
		if(!pEffect->m_bUseEnvironmentLight)
		{
			g_pD3dDev->LightEnable( 2, FALSE );
		}
		if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  dwFogValue );
		}
		if( pEffect->m_bZWriteEnable == FALSE)
		{
			g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		}
		return;
	}
	switch(pEffect->m_nObjectAniType)
	{
	case 0:// object animation
		{	//m_pObjEffectMesh[index]
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(1);
		}
		break;
	case 1:// texture animation
		{
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType+1);
		}
		break;
	case 2:// object + texture animation
		{
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType+1);
		}
		break;
	}

	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, pEffect->m_cColor.r, 
		pEffect->m_cColor.g, pEffect->m_cColor.b, pEffect->m_cColor.a);
	BOOL bTemp = pMesh->m_bMaterial;
	pMesh->m_bMaterial = TRUE;
	pMesh->m_material = mtrl;
//	절대 좌표 세팅
	pMesh->SetWorldMatrix(matScale);//*pEffect->m_pParent->m_pParent->m_mMatrix);

	if(!pEffect->m_bZbufferEnable && m_bZBufferTemp)
	{
		D3DXVECTOR3 vTempPos;
		vTempPos.x = matScale._41;
		vTempPos.y = matScale._42;
		vTempPos.z = matScale._43;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vPos,vVel,vUp,vSide;
		vPos = g_pD3dApp->m_pCamera->GetEyePt();
		D3DXVec3Normalize(&vUp,&g_pD3dApp->m_pCamera->GetUpVec());
		D3DXVec3Normalize(&vVel,&(vTempPos - vPos));
		D3DXVec3Cross(&vSide,&vUp,&vVel);
		D3DXVec3Cross(&vUp,&vVel,&vSide);

		D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos + vVel),&vUp);
		float fDist1,fDist2;
		fDist1 = D3DXVec3Length(&(vTempPos - vPos));
		fDist2 = g_pD3dApp->m_pScene->m_pObjectRender->CheckPickMesh(matTemp,vPos).fDist;
		if(fDist1 > fDist2)
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
		else
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE);
	}
	// by dhkwon, 030917
	LPDIRECT3DTEXTURE9 pOrgTex = NULL;
	if(pEffect->m_pParent && pEffect->m_pParent->m_pTexture ) {
		pOrgTex = pMesh->SetTexture(pEffect->m_pParent->m_pTexture, 0); }
	else if(pEffect->m_strTextureFile[0])
	{
		int i = LoadTexture(pEffect->m_strTextureFile);
		if(m_nTextureRenderCount[i] == 2)
			m_nTextureRenderCount[i]++;
		pOrgTex = pMesh->SetTexture(m_pTexture[i], 0);
	}
	DWORD dwLightColorOp = 0;
	if(pEffect->m_bLightMapUse)
	{
		g_pD3dDev->GetTextureStageState( 1, D3DTSS_COLOROP, &dwLightColorOp );
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState );
        g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );

		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	}

	// 2006-01-24 by ispark
	if((!g_pD3dApp->m_bCharacter &&									// 기어 이면서
		g_pD3dApp->m_pCamera->GetCamType() != CAMERA_TYPE_FPS &&	//  1인칭 모드가 아니구
		pEffect->m_pParent->m_nInvenWeaponIndex > 0) ||				// 인벤 무기 인덱스가 0 이상이면 렌더링 하지마라
		pEffect->m_pParent->m_nInvenWeaponIndex == 0)				// 인벤 무기 인덱스가 0이면 렌더링 
	{
		if(pEffect->m_bLightMapUse)
		{
			pMesh->Render(TRUE);
		}
		else
		{
			pMesh->Render();
		}
	}
		
	// 2008-08-22 by bhsohn EP3 인벤토리 처리	
	//if(pEffect->m_pParent->m_nInvenWeaponIndex > 0	&&  g_pShuttleChild->m_bInvenRender == TRUE)

// 2012-06-14 by isshin 아템미리보기
#ifdef C_ENEMY_INFO_ISSHIN
	if((pEffect->m_pParent->m_nInvenWeaponIndex > 0 && pEffect->m_pParent->m_nInvenWeaponIndex < ENEMY_WEAPON_INDEX_DUMMY)	&&  g_pGameMain->IsEquipInvenShow() == TRUE)	
#else
	if(pEffect->m_pParent->m_nInvenWeaponIndex > 0	&&  g_pGameMain->IsEquipInvenShow() == TRUE)
#endif
// end 2012-06-14 by isshin 아템미리보기
	{
		D3DXMATRIX pMatOldView, pMatOldProj, pMatPresView, pMatPresProj, pMatrix;
		D3DXMatrixIdentity(&pMatOldView);
		D3DXMatrixIdentity(&pMatOldProj);
		D3DXMatrixIdentity(&pMatPresView);
		D3DXMatrixIdentity(&pMatPresProj);
		D3DXMatrixIdentity(&pMatrix);		
		
		// 현재 뷰와 프로젝션을 가져온다
		g_pD3dDev->GetTransform( D3DTS_VIEW,       &pMatOldView );
		g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );	
		
		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatPresProj);
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
		
		if(pEffect->m_pParent->m_nInvenWeaponIndex > 4)
		{
			// 2008-08-22 by bhsohn EP3 인벤토리 처리
			//pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInven);
			pMesh->SetWorldMatrix(g_pGameMain->GetInvenMatInven());			
			pMesh->Render();		
		}
		else
			g_pShuttleChild->SetInvenMesh(pEffect->m_pParent->m_nInvenWeaponIndex - 1, pMesh);
		//			pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInvenWeaponSetPosition[pEffect->m_pParent->m_nInvenWeaponIndex - 1]);
		//		pMesh->Render();
		
		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatOldView );
		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatOldProj );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	}		

	

//	if(g_pShuttleChild->m_bInvenRender && bInven == TRUE && pEffect->m_pParent->m_nInvenWeaponIndex > 0)
//	{
//		//////////////////////////////////////////////////////////////////////////
//		//
//		D3DXMATRIX pMatOldView, pMatOldProj, pMatPresView, pMatPresProj, pMatrix;
//		D3DXMatrixIdentity(&pMatOldView);
//		D3DXMatrixIdentity(&pMatOldProj);
//		D3DXMatrixIdentity(&pMatPresView);
//		D3DXMatrixIdentity(&pMatPresProj);
//		D3DXMatrixIdentity(&pMatrix);		
//
//		// 현재 뷰와 프로젝션을 가져온다
//		g_pD3dDev->GetTransform( D3DTS_VIEW,       &pMatOldView );
//		g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );	
//		
//		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
//		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatPresProj);
//		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
//		
//		// 무기
//		if(pEffect->m_pParent->m_nInvenWeaponIndex > 4)
//		{
//			pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInven);
//			if(pEffect->m_bLightMapUse)	
//				pMesh->Render(TRUE);
//			else 
//				pMesh->Render();	
//		}
//		else
//			g_pShuttleChild->SetInvenMesh(pEffect->m_pParent->m_nInvenWeaponIndex - 1, pMesh);
////			pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInvenWeaponSetPosition[pEffect->m_pParent->m_nInvenWeaponIndex - 1]);
////		if(pEffect->m_bLightMapUse)	
////			pMesh->Render(TRUE);
////		else 
////			pMesh->Render();	
//		// 복구
//		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatOldView );
//		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatOldProj );
//		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
//		//
//		//////////////////////////////////////////////////////////////////////////		
//	}		

	if((pEffect->m_pParent && pEffect->m_pParent->m_pTexture) || pEffect->m_strTextureFile[0])
	{
		pMesh->SetTexture(pOrgTex, 0);
	}

	if(pMesh && pMesh->m_nRenderCount == 2)
		pMesh->m_nRenderCount++;
	if(pMesh)
		pMesh->m_bMaterial = bTemp;
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);
	if(!pEffect->m_bUseEnvironmentLight)
	{
		g_pD3dDev->LightEnable( 2, FALSE );
	}
	if(pEffect->m_bLightMapUse)
	{
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, dwLightColorOp );
	}
	if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
	{
		g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  dwFogValue );
	}
	if(pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	}
	if( pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}

}

void CEffectRender::ObjectParticleRender(CObjectAni* pEffect, CParticle* pParticle)
{
	FLOG( "CEffectRender::ObjectParticleRender(CObjectAni* pEffect, CParticle* pParticle)" );
	DWORD dwSrc,dwDest,dwColorOp;
	DWORD dwFogValue = FALSE;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
	g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);

    g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  pEffect->m_bAlphaTestEnble );
	if(pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHAREF, pEffect->m_nAlphaTestValue );
		g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	}

	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable);

	if( pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable );
	}
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bAlphaBlending );
	if(pEffect->m_bAlphaBlending)
	{
		g_pD3dDev->GetRenderState( D3DRS_FOGENABLE,  &dwFogValue );
		if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  FALSE );
		}
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nDestBlend);
	}
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	if(pParticle)
	{
//		pos = pData->m_pParent->m_vPos;
		pos = pParticle->m_vPos;//좌표
	}
	else 
	{
		pos = D3DXVECTOR3(0,0,0);
	}
	// 2010. 03. 18 by jskim 몬스터변신 카드
	//D3DXMatrixScaling(&matScale, pEffect->m_fScale,pEffect->m_fScale,pEffect->m_fScale);
	float tempScale = 0.0f;
	if(pEffect->m_pParent->m_pParent->m_MonsterTransformer &&
		pEffect->m_pParent->m_pParent->m_MonsterTransScale > 0)
	{
		tempScale = pEffect->m_fScale * pEffect->m_pParent->m_pParent->m_MonsterTransScale;
	}
	else
	{
		tempScale = pEffect->m_fScale;
	}
	
	D3DXMatrixScaling(&matScale, tempScale,tempScale,tempScale);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	if(pEffect->m_pParent && pEffect->m_pParent->m_bUseBillboard)
	{
/*		matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();
		D3DXMATRIX matRotate;
		D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();//pos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_pParent->m_fBillboardAngle );
		matScale *= matRotate;

		if(pEffect->m_pParent->m_fBillboardRotatePerSec>0)
		{
			D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentBillboardRotateAngle );
			matScale *= matRotate;
		} 
		if(pEffect->m_fCurrentRandomUpAngleX != 0)
		{
			D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentRandomUpAngleX );
			matScale *= matRotate;
		}
*/		D3DXMATRIX matBillboard = g_pD3dApp->m_pCamera->GetBillboardMatrix();
		D3DXMATRIX matRotate;
		D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();//pos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_pParent->m_fBillboardAngle );
		matBillboard *= matRotate;

		if(pEffect->m_pParent->m_fBillboardRotatePerSec>0)
		{
			D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentBillboardRotateAngle );
			matBillboard *= matRotate;
		} 
		if(pEffect->m_fCurrentRandomUpAngleX != 0)
		{
			D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentRandomUpAngleX );
			matBillboard *= matRotate;
		}
/*
		matScale._41 = pEffect->m_pParent->m_pParent->m_mMatrix._41;
		matScale._42 = pEffect->m_pParent->m_pParent->m_mMatrix._42;
		matScale._43 = pEffect->m_pParent->m_pParent->m_mMatrix._43;

		matScale._41 += pEffect->m_pParent->m_vPos.x;
		matScale._42 += pEffect->m_pParent->m_vPos.y;
		matScale._43 += pEffect->m_pParent->m_vPos.z;
*/		matScale._41 = pEffect->m_pParent->m_vPos.x;
		matScale._42 = pEffect->m_pParent->m_vPos.y;
		matScale._43 = pEffect->m_pParent->m_vPos.z;
		matBillboard = matScale * matBillboard;
		matScale = matScale * pEffect->m_pParent->m_pParent->m_mMatrix;
		matBillboard._41 = matScale._41;
		matBillboard._42 = matScale._42;
		matBillboard._43 = matScale._43;
		matScale = matBillboard;
		// set light
		if(!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = g_pD3dApp->m_pCamera->GetViewDir();
 			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
		}
	} else
	{
		D3DXVECTOR3 up,target;
		switch(pParticle->m_pParent->m_nObjMoveTargetType)
		{
		case OBJ_MOVE_TYPE0://파티클 이동방향을 가리킴
			{
				target = pParticle->m_vObjTarget;
				up = pParticle->m_vObjUp;
			}
			break;
		case OBJ_MOVE_TYPE1:// 유닛 방향으로 가리킴
			{
				target = pEffect->m_pParent ? pEffect->m_pParent->m_vTarget : D3DXVECTOR3(0,0,1);
				up = pEffect->m_pParent ?  pEffect->m_pParent->m_vUp : D3DXVECTOR3(0,1,0);
			}
			break;
		case OBJ_MOVE_TYPE2:// 랜덤으로 움직임
			{
				target = pParticle->m_vObjTarget;
				up = pParticle->m_vObjUp;
//				target.x = Random(1.0f, -1.0f);
//				target.y = Random(1.0f, -1.0f);
//				target.z = Random(1.0f, -1.0f);
//				D3DXVec3Normalize(&target,&target);
//				up = pEffect->m_pParent ?  pEffect->m_pParent->m_vUp : D3DXVECTOR3(0,1,0);
			}
			break;
		case OBJ_MOVE_TYPE3:// 빌보드
			{
				target = pParticle->m_vPos - g_pD3dApp->m_pCamera->GetViewDir();
				up = g_pD3dApp->m_pCamera->GetUpVec();
			}
			break;
		}
		D3DXMATRIX lookat;
//		D3DXVECTOR3 up = pEffect->m_pParent->m_vUp;
//		D3DXVECTOR3 target = pEffect->m_pParent->m_vTarget;
		if(pEffect->m_fCurrentRandomUpAngleX != 0 || pEffect->m_fCurrentRandomUpAngleZ != 0)
		{
			up = D3DXVECTOR3(0,1,0);
			target = pParticle->m_vPos;
			D3DXVECTOR3 vel = D3DXVECTOR3(0,0,1);
			D3DXMATRIX matRotateX, matRotateZ;
			D3DXVECTOR3 vAxis(1,0,0);// up vector 회전축(X축)
			D3DXMatrixRotationAxis( &matRotateX, &vAxis, pEffect->m_fCurrentRandomUpAngleX);
			D3DXVec3TransformCoord( &up, &up, &matRotateX );
			D3DXVec3TransformCoord( &vel, &vel, &matRotateX );
			vAxis = D3DXVECTOR3(0,0,1);// up vector 회전축(X축)
			D3DXMatrixRotationAxis( &matRotateZ, &vAxis, pEffect->m_fCurrentRandomUpAngleZ);
			D3DXVec3TransformCoord( &up, &up, &matRotateZ );
			D3DXVec3TransformCoord( &vel, &vel, &matRotateZ );
			target += vel;
		} 
		
		D3DXMatrixLookAtLH( &lookat, &D3DXVECTOR3(0,0,0), &target, &up);
//		D3DXMatrixLookAtLH( &lookat, &pos, &target, &up);
		D3DXMatrixInverse( &lookat, NULL, &lookat );
		matScale = lookat * matScale;// * matScale;
		D3DXMATRIX mat = pEffect->m_pParent->m_pParent->m_mMatrix;
		mat._41 = pos.x;
		mat._42 = pos.y;
		mat._43 = pos.z;
		matScale = matScale * mat;
		// set light
		if(!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction  = g_pD3dApp->m_pCamera->GetViewDir();
 			g_pD3dDev->SetLight( 2, &m_light2 );
			g_pD3dDev->LightEnable( 2, TRUE );
		}
	}

//	int index = LoadObject(pEffect->m_strObjectFile);
//	if(index<0)
//		return;
	CSkinnedMesh* pMesh = LoadObject(pEffect->m_strObjectFile);
	if(!pMesh)
	{
		if(!pEffect->m_bUseEnvironmentLight)
		{
		g_pD3dDev->LightEnable( 2, FALSE );
		}
		if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  dwFogValue );
		}
		if( pEffect->m_bZWriteEnable == FALSE)
		{
			g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		}
		return;
	}
	switch(pEffect->m_nObjectAniType)
	{
	case 0:// object animation
		{
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(1);
		}
		break;
	case 1:// texture animation
		{
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType+1);
		}
		break;
	case 2:// object + texture animation
		{
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType+1);
		}
		break;
	}

	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, pEffect->m_cColor.r, 
		pEffect->m_cColor.g, pEffect->m_cColor.b, pEffect->m_cColor.a);
	BOOL bTemp = pMesh->m_bMaterial;
	pMesh->m_bMaterial = TRUE;
	pMesh->m_material = mtrl;
//	절대 좌표 세팅
	pMesh->SetWorldMatrix(matScale);//*pEffect->m_pParent->m_pParent->m_mMatrix);
	if(!pEffect->m_bZbufferEnable && m_bZBufferTemp)
	{
		D3DXVECTOR3 vTempPos;
		vTempPos.x = matScale._41;
		vTempPos.y = matScale._42;
		vTempPos.z = matScale._43;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vPos,vVel,vUp,vSide;
		vPos = g_pD3dApp->m_pCamera->GetEyePt();
		D3DXVec3Normalize(&vUp,&g_pD3dApp->m_pCamera->GetUpVec());
		D3DXVec3Normalize(&vVel,&(vTempPos - vPos));
		D3DXVec3Cross(&vSide,&vUp,&vVel);
		D3DXVec3Cross(&vUp,&vVel,&vSide);

		D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos + vVel),&vUp);
		float fDist1,fDist2;
		fDist1 = D3DXVec3Length(&(vTempPos - vPos));
		fDist2 = g_pD3dApp->m_pScene->m_pObjectRender->CheckPickMesh(matTemp,vPos).fDist;
		if(fDist1 > fDist2)
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
		else
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE);
	}
	// by dhkwon, 030917
	LPDIRECT3DTEXTURE9 pOrgTex = NULL;
	if(pEffect->m_pParent && pEffect->m_pParent->m_pTexture ) {
		pOrgTex = pMesh->SetTexture(pEffect->m_pParent->m_pTexture, 0); }
	else if(pEffect->m_strTextureFile[0])
	{
		int i = LoadTexture(pEffect->m_strTextureFile);
		if(m_nTextureRenderCount[i] == 2)
			m_nTextureRenderCount[i]++;
		pOrgTex = pMesh->SetTexture(m_pTexture[i], 0);
	}
	DWORD dwLightColorOp = 0;
	if(pEffect->m_bLightMapUse)
	{
		g_pD3dDev->GetTextureStageState( 1, D3DTSS_COLOROP, &dwLightColorOp );
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState );
        g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
		// 2005-01-04 by jschoi
//		g_pD3dDev->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
//		g_pD3dDev->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
//		g_pD3dDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}
	if(pEffect->m_bLightMapUse)
	{
		pMesh->Render(TRUE);
//		pMesh->Render();
	}
	else
	{
		pMesh->Render();
	}
	if((pEffect->m_pParent && pEffect->m_pParent->m_pTexture) || pEffect->m_strTextureFile[0])
	{
		pMesh->SetTexture(pOrgTex, 0);
	}

//	if(m_nObjRenderCount[index] < 3)
//		m_nObjRenderCount[index]++;
	if(pMesh && pMesh->m_nRenderCount == 2)
		pMesh->m_nRenderCount++;
/*	if(pEffect->m_bLightMapUse)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bLightMapAlphaBlending );
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nLightMapSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nLightMapDestBlend);
		g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nLightMapRenderState);
		// by dhkwon, 030917
//		LPDIRECT3DTEXTURE8 pOrgTex = NULL;
		if(pEffect->m_strTextureFile[0])
		{
			int i = LoadTexture(pEffect->m_strTextureFile);
			if(m_nTextureRenderCount[i] == 2)
				m_nTextureRenderCount[i]++;
			pOrgTex = m_pObjEffectMesh[index]->SetTexture(m_pTexture[i], 0);
		}
		m_pObjEffectMesh[index]->Render();
		if(pEffect->m_strTextureFile[0])
		{
			m_pObjEffectMesh[index]->SetTexture(pOrgTex, 0);
		}
	}
*/
	if (pMesh)
		pMesh->m_bMaterial = bTemp;
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
	g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);
	if(!pEffect->m_bUseEnvironmentLight)
	{
	g_pD3dDev->LightEnable( 2, FALSE );
	}
	if(pEffect->m_bLightMapUse)
	{
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, dwLightColorOp );
	}
	if(pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	}
	if( pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}
}

#define DEBUG_COLOR		D3DCOLOR_ARGB(255,255,255,0)
int g_nOldDistance = 0;
void CEffectRender::RenderZEnable()
{
	// 2005-01-07 by jschoi
//	g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	FLOG( "CEffectRender::RenderZEnable()" );
	if(m_vecZEnableEffect.size() == 0 )
		return;
	// effect sorting
	if( g_pD3dApp->m_dwGameState != _GAME && 
		g_pD3dApp->m_dwGameState != _SELECT &&
		g_pD3dApp->m_dwGameState != _CREATE && 
		g_pD3dApp->m_dwGameState != _LOGO &&
		g_pD3dApp->m_dwGameState != _SHOP && 
		g_pD3dApp->m_dwGameState != _CITY )
//		g_pD3dApp->m_dwGameState != _WAITING)
	{
		DBGOUT_EFFECT("ZBufferEnable Effect Disable state , effect number : %d\n", m_vecZEnableEffect.size());
		m_vecZEnableEffect.clear();
		return;
	}
//	sort(m_vecZEnableEffect.begin(), m_vecZEnableEffect.end(), CompareEffect());
	// effect rendering
	//vector<Effect*>::iterator itEffect = m_vecZEnableEffect.begin();
#ifdef _DBGOUT_EFFECT
	int linePixel = 14;
	int lineNumber = 0;
#endif //_DBGOUT_EFFECT_endif
	for (auto pEffect : m_vecZEnableEffect)
	//while(itEffect != m_vecZEnableEffect.end())
	{
		// effect rendering
		//Effect* pEffect = *itEffect;
//		if( g_nOldDistance == pEffect->fDistance && 
//			pEffect->dwType == EFFECT_TYPE_PARTICLE)
//		{
//			DBGOUT("같은 거리의 이펙트 리턴\n");
//			itEffect++;
//			continue;
//		}
		g_nOldDistance = pEffect->fDistance;
#ifdef _DBGOUT_EFFECT
		char buf[256];
		switch(pEffect->dwType)
		{
		case EFFECT_TYPE_OBJECT:
			{
				sprintf(buf,"[OBJECT][obj:%s][%8.2f,%8.2f,%8.2f][%8.2f]",
					((CObjectAni*)pEffect)->m_strObjectFile,
					((CObjectAni*)pEffect)->m_pParent->m_vPos.x,
					((CObjectAni*)pEffect)->m_pParent->m_vPos.y,
					((CObjectAni*)pEffect)->m_pParent->m_vPos.z,
					pEffect->fDistance);
			}
			break;
		case EFFECT_TYPE_SPRITE:
			{
				sprintf(buf,"[SPRITE][tex:%s][%8.2f,%8.2f,%8.2f][%8.2f]",
					((CSpriteAni*)pEffect)->m_strTextureFile,
					((CSpriteAni*)pEffect)->m_pParent->m_vPos.x,
					((CSpriteAni*)pEffect)->m_pParent->m_vPos.y,
					((CSpriteAni*)pEffect)->m_pParent->m_vPos.z,
					pEffect->fDistance);
			}
			break;
		case EFFECT_TYPE_PARTICLE:
			{
				sprintf(buf,"[PARTIC][obj:%d][%8.2f,%8.2f,%8.2f][%8.2f][%8.2f]",
					((CParticle*)pEffect)->m_nTextureNumber,
					((CParticle*)pEffect)->m_vPos.x,
					((CParticle*)pEffect)->m_vPos.y,
					((CParticle*)pEffect)->m_vPos.z,
					pEffect->fDistance,
					D3DXVec3Length(&(g_pD3dApp->m_pCamera->GetEyePt()-((CParticle*)pEffect)->m_vPos)));

			}
			break;
		case EFFECT_TYPE_TRACE:
			{
				sprintf(buf,"[TRACE ][obj:%d][%8.2f]",
					((CEffectPlane*)pEffect)->m_pParent->m_strName,
					pEffect->fDistance);
			}
			break;
		}
		g_pD3dApp->m_pFontDebug->DrawText( 10, linePixel*lineNumber++, DEBUG_COLOR, buf, 0);

#endif //_DBGOUT_EFFECT_endif
		
		switch(pEffect->dwType)
		{
		case EFFECT_TYPE_OBJECT:
			{
				// 2006-12-06 by ispark, 알파값 SKILL_OBJECT_ALPHA_OTHER_INFLUENCE(0) 인것은 렌더링 안한다.
				if(((CObjectAni*)pEffect)->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)
					break;

				ObjectAniRender((CObjectAni*)pEffect);
			}
			break;
		case EFFECT_TYPE_SPRITE:
			{
				// 2006-12-06 by ispark, 알파값 SKILL_OBJECT_ALPHA_OTHER_INFLUENCE(0) 인것은 렌더링 안한다.
				if(((CSpriteAni*)pEffect)->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)
					break;

				SpriteAniRender((CSpriteAni*)pEffect);
			}
			break;
		case EFFECT_TYPE_PARTICLE:
			{
//				ParticleSystemRender((CParticleSystem*)pEffect);
				// 2006-12-06 by ispark, 알파값 SKILL_OBJECT_ALPHA_OTHER_INFLUENCE(0) 인것은 렌더링 안한다.
				if(((CParticle*)pEffect)->m_pParent->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)
					break;

				switch( ((CParticle*)pEffect)->m_pParent->m_nParticleType )
				{
				case PARTICLE_OBJECT_TYPE:
					{
						ObjectParticleRender(((CParticle*)pEffect)->m_pObjectAni, (CParticle*)pEffect);
					}
					break;
				case PARTICLE_SPRITE_TYPE:
					{
						g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
						g_pD3dDev->SetFVF( D3DFVF_SPRITE_VERTEX );
						g_pD3dDev->SetRenderState( D3DRS_ZENABLE, ((CParticle*)pEffect)->m_pParent->m_bZbufferEnable );
						//g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
						if( ((CParticle*)pEffect)->m_pParent->m_bZWriteEnable == FALSE)
						{
							g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, ((CParticle*)pEffect)->m_pParent->m_bZWriteEnable );
						}
						g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
						g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, ((CParticle*)pEffect)->m_pParent->m_dwSrcBlend );
						g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, ((CParticle*)pEffect)->m_pParent->m_dwDestBlend );
						g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
						g_pD3dDev->SetStreamSource( 0, m_pVB1,0, sizeof(SPRITE_VERTEX) );
						// set light
						D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
						m_light2 = g_pD3dApp->m_pScene->m_light2;
						m_light2.Direction  = vAxis;
 						g_pD3dDev->SetLight( 2, &m_light2 );
						g_pD3dDev->LightEnable( 2, TRUE );
						ParticleRender(((CParticle*)pEffect)->m_pParent, (CParticle*)pEffect, vAxis, -1);
						g_pD3dDev->LightEnable( 2, FALSE );
						if( ((CParticle*)pEffect)->m_pParent->m_bZWriteEnable == FALSE)
						{
							g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
						}
					}
					break;
				}
			}
			break;
		case EFFECT_TYPE_TRACE:
			{
				// 2006-12-06 by ispark, 알파값 SKILL_OBJECT_ALPHA_OTHER_INFLUENCE(0) 인것은 렌더링 안한다.
				if(((CEffectPlane*)pEffect)->m_pParent->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)
					break;

				EffectPlaneRender((CEffectPlane*)pEffect );
//				TraceAniRender((CTraceAni*)pEffect);
			}
			break;
		}
		//itEffect++;
	}//if(itEffect != m_vecZEnableEffect.end())
	m_vecZEnableEffect.clear();
	// 2005-01-07 by jschoi
//	g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

}

HRESULT CEffectRender::InitDeviceObjects()// load game data file(texture)
{
	FLOG( "CEffectRender::InitDeviceObjects()" );
	m_pTexEffectData = new CGameData;
	char strPath[MAX_PATH];
	strcpy(strPath,".\\Res-Tex\\spreff.tex");
	if (!m_pTexEffectData->SetFile(strPath, FALSE, NULL, 0))
	{
		SAFE_DELETE(m_pTexEffectData);
		char strErr[512];
		sprintf_s(strErr, 512, STRERR_C_RESOURCE_0001, strPath);
		g_pD3dApp->NetworkErrorMsgBox(strErr);
		return E_FAIL;
	}

	m_pEffectData = new CGameData;
	strcpy(strPath,".\\Res-Eff\\effectInfo.inf");
	if (!m_pEffectData->SetFile(strPath, FALSE, NULL, 0))
	{
		SAFE_DELETE(m_pTexEffectData);
		SAFE_DELETE(m_pEffectData);
		char strErr[512];
		sprintf_s(strErr, 512, STRERR_C_RESOURCE_0001, strPath);
		g_pD3dApp->NetworkErrorMsgBox(strErr);
		return E_FAIL;
	}

	m_pObjectData = new CGameData;
	strcpy(strPath,".\\Res-Eff\\objectInfo.inf");
	if (!m_pObjectData->SetFile(strPath, FALSE, NULL, 0))
	{
		SAFE_DELETE(m_pTexEffectData);
		SAFE_DELETE(m_pEffectData);
		SAFE_DELETE(m_pObjectData);
		char strErr[512];
		sprintf_s(strErr, 512, STRERR_C_RESOURCE_0001, strPath);
		g_pD3dApp->NetworkErrorMsgBox(strErr);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffectRender::RestoreDeviceObjects()// create vertex buffer
{
	FLOG( "CEffectRender::RestoreDeviceObjects()" );
	float hsx,hsy;
	int i,j;
	hsx = 0.5f;
	hsy = 0.5f;
	SPRITE_VERTEX* v;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITE_VERTEX ),
				0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB16[4*i+j] ,NULL) ) )
				return E_FAIL;
			m_pVB16[4*i+j]->Lock( 0, 0, (void**)&v, 0 );
			v[0].p = D3DXVECTOR3(-hsx,-hsy,0);	v[0].tu=0.25f*j;		v[0].tv=0.25f*(i+1);	v[0].c = 0xffffffff;
			v[1].p = D3DXVECTOR3(-hsx,hsy,0);	v[1].tu=0.25f*j;		v[1].tv=0.25f*i;		v[1].c = 0xffffffff;
			v[2].p = D3DXVECTOR3(hsx,-hsy,0);	v[2].tu=0.25f*(j+1);	v[2].tv=0.25f*(i+1);	v[2].c = 0xffffffff;
			v[3].p = D3DXVECTOR3(hsx,hsy,0);	v[3].tu=0.25f*(j+1);	v[3].tv=0.25f*i;		v[3].c = 0xffffffff;
			m_pVB16[4*i+j]->Unlock();
		}
	}
	for(i=0;i<2;i++)
	{
		for(j=0;j<4;j++)
		{
			if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITE_VERTEX ),
				0 , D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB8[4*i+j] ,NULL) ) )
				return E_FAIL;
			m_pVB8[2*i+j]->Lock( 0, 0, (void**)&v, 0 );
			v[0].p = D3DXVECTOR3(-hsx,-hsy,0);	v[0].tu=0.25f*j;		v[0].tv=0.5f*(i+1);		v[0].c = 0xffffffff;
			v[1].p = D3DXVECTOR3(-hsx,hsy,0);	v[1].tu=0.25f*j;		v[1].tv=0.5f*i;			v[1].c = 0xffffffff;
			v[2].p = D3DXVECTOR3(hsx,-hsy,0);	v[2].tu=0.25f*(j+1);	v[2].tv=0.5f*(i+1);		v[2].c = 0xffffffff;
			v[3].p = D3DXVECTOR3(hsx,hsy,0);	v[3].tu=0.25f*(j+1);	v[3].tv=0.5f*i;			v[3].c = 0xffffffff;
			m_pVB8[2*i+j]->Unlock();
		}
	}
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITE_VERTEX ),
				0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB4[2*i+j],NULL ) ) )
				return E_FAIL;
			m_pVB4[2*i+j]->Lock( 0, 0, (void**)&v, 0 );
			v[0].p = D3DXVECTOR3(-hsx,-hsy,0);	v[0].tu=0.5f*j;			v[0].tv=0.5f*(i+1);		v[0].c = 0xffffffff;
			v[1].p = D3DXVECTOR3(-hsx,hsy,0);	v[1].tu=0.5f*j;			v[1].tv=0.5f*i;			v[1].c = 0xffffffff;
			v[2].p = D3DXVECTOR3(hsx,-hsy,0);	v[2].tu=0.5f*(j+1);		v[2].tv=0.5f*(i+1);		v[2].c = 0xffffffff;
			v[3].p = D3DXVECTOR3(hsx,hsy,0);	v[3].tu=0.5f*(j+1);		v[3].tv=0.5f*i;			v[3].c = 0xffffffff;
			m_pVB4[2*i+j]->Unlock();
		}
	}
	for(i=0;i<2;i++)
	{
		if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITE_VERTEX ),
			0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB2[i],NULL ) ) )
			return E_FAIL;
		m_pVB2[i]->Lock( 0, 0, (void**)&v, 0 );
		v[0].p = D3DXVECTOR3(-hsx,-hsy,0);	v[0].tu=0.5f*i;		v[0].tv=1.0f;		v[0].c = 0xffffffff;
		v[1].p = D3DXVECTOR3(-hsx,hsy,0);	v[1].tu=0.5f*i;		v[1].tv=0.0f;		v[1].c = 0xffffffff;
		v[2].p = D3DXVECTOR3(hsx,-hsy,0);	v[2].tu=0.5f*(i+1);	v[2].tv=1.0f;		v[2].c = 0xffffffff;
		v[3].p = D3DXVECTOR3(hsx,hsy,0);	v[3].tu=0.5f*(i+1);	v[3].tv=0.0f;		v[3].c = 0xffffffff;
		m_pVB2[i]->Unlock();
	}
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITE_VERTEX ),
		0 , D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB1,NULL ) ) )
		return E_FAIL;
	m_pVB1->Lock( 0, 0, (void**)&v, 0 );
	v[0].p = D3DXVECTOR3(-hsx,-hsy,0);	v[0].tu=0.0f;	v[0].tv=1.0f;		v[0].c = 0xffffffff;
	v[1].p = D3DXVECTOR3(-hsx,hsy,0);	v[1].tu=0.0f;	v[1].tv=0.0f;		v[1].c = 0xffffffff;
	v[2].p = D3DXVECTOR3(hsx,-hsy,0);	v[2].tu=1.0f;	v[2].tv=1.0f;		v[2].c = 0xffffffff;
	v[3].p = D3DXVECTOR3(hsx,hsy,0);	v[3].tu=1.0f;	v[3].tv=0.0f;		v[3].c = 0xffffffff;
	m_pVB1->Unlock();
	DBGOUT("CEffectRender::RestoreDeviceObjects()\n");
	return S_OK;
}

HRESULT CEffectRender::InvalidateDeviceObjects()
{
	FLOG( "CEffectRender::InvalidateDeviceObjects()" );
	SAFE_RELEASE(m_pVB1);
	int i;
	for(i=0;i<2;i++)
		SAFE_RELEASE(m_pVB2[i]);
	for(i=0;i<4;i++)
		SAFE_RELEASE(m_pVB4[i]);
	for(i=0;i<8;i++)
		SAFE_RELEASE(m_pVB8[i]);
	for(i=0;i<16;i++)
		SAFE_RELEASE(m_pVB16[i]);
	return S_OK;
}

HRESULT CEffectRender::DeleteDeviceObjects()
{
	FLOG( "CEffectRender::DeleteDeviceObjects()" );
	SAFE_DELETE(m_pTexEffectData);
	SAFE_DELETE(m_pEffectData);
	SAFE_DELETE(m_pObjectData);
	for(int i=0;i<TEX_EFFECT_NUM;i++)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
	m_mapTexNameToIndex.clear();
/*	m_mapObjNameToIndex.clear();
	for(i=0;i<OBJ_EFFECT_NUM;i++)
	{
		if(m_pObjEffectMesh[i])
		{
			DbgOut("m_pObjEffectMesh[%d]", i);
			m_pObjEffectMesh[i]->InvalidateDeviceObjects();
			m_pObjEffectMesh[i]->DeleteDeviceObjects();
			SAFE_DELETE(m_pObjEffectMesh[i]);
			DbgOut("Deleted\n", i);
		}
	}
*/
	map<string, CSkinnedMesh*>::iterator itMesh = m_mapObjNameToMesh.begin();
	while(itMesh != m_mapObjNameToMesh.end())
	{
		// 2004-10-12 by jschoi 여기서 죽음 LostDevice
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		itMesh->second->DeleteLoadingGameData();
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		itMesh->second->InvalidateDeviceObjects();
		itMesh->second->DeleteDeviceObjects();
		SAFE_DELETE(itMesh->second);
		itMesh++;
	}
	m_mapObjNameToMesh.clear();

	map<string,LPDIRECT3DTEXTURE9>::iterator it = m_mapTextToTexture.begin();
	while(it != m_mapTextToTexture.end())
	{
		LPDIRECT3DTEXTURE9 pTexture = it->second;
		SAFE_RELEASE(pTexture);
		it++;
	}
	m_mapTextToTexture.clear();
	m_mapTextRenderCount.clear();
	
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_vecLoadingPriority.clear();
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	return S_OK;
}


// pEffect는 최대사이즈로 버퍼를 잡고 이 함수에 들어간다.
// 함수가 끝난 후에 내용을 사이즈만큼 복사하고, 버퍼는 삭제한다.
DWORD CEffectRender::LoadEffect(char* strName, DWORD dwEffectType, char* pEffect)
{
	FLOG( "CEffectRender::LoadEffect(char* strName, DWORD dwEffectType, char* pEffect)" );
	EffectFileHeader header;
	switch(dwEffectType)
	{
	case EFFECT_TYPE_OBJECT:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if(pDataHeader)
			{
				memcpy( (void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy( pEffect, pDataHeader->m_pData+sizeof(EffectFileHeader), sizeof(ObjectAniData)-sizeof(Effect));
			}
			
		}
		break;
	case EFFECT_TYPE_SPRITE:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if(pDataHeader)
			{
				memcpy( (void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy( pEffect, pDataHeader->m_pData+sizeof(EffectFileHeader), sizeof(SpriteAniData)-sizeof(Effect));
			}
		}
		break;
	case EFFECT_TYPE_PARTICLE:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if(pDataHeader)
			{
				memcpy( (void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy( pEffect, pDataHeader->m_pData+sizeof(EffectFileHeader), sizeof(ParticleData));
			}
		}
		break;
	case EFFECT_TYPE_TRACE:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if(pDataHeader)
			{
				memcpy( (void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy( pEffect, pDataHeader->m_pData+sizeof(EffectFileHeader), sizeof(TraceData));
			}
		}
		break;
	}
	return header.dwEffectType;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEffectRender::AddFontTexture(char* strText, LPDIRECT3DTEXTURE8 pTexture )
/// \brief		폰트 텍스쳐를 ADD한다.
/// \author		dhkwon
/// \date		2004-03-25 ~ 2004-03-25
/// \warning	
///
/// \param		
/// \return		BOOL ( TRUE:Add성공, FALSE:이미 존재)
///////////////////////////////////////////////////////////////////////////////
BOOL CEffectRender::AddFontTexture(char* strText, LPDIRECT3DTEXTURE9 pTexture )
{
	FLOG( "CEffectRender::AddFontTexture(char* strText, LPDIRECT3DTEXTURE9 pTexture )" );
	map<string,LPDIRECT3DTEXTURE9>::iterator it = m_mapTextToTexture.find(strText);
	if(it == m_mapTextToTexture.end())
	{
		m_mapTextToTexture[strText] = pTexture;
		m_mapTextRenderCount[strText] = 2;
		return TRUE;
	}
	else
	{
		map<string,int>::iterator it = m_mapTextRenderCount.find(strText);
		if(it != m_mapTextRenderCount.end())
		{
			(it->second) ++;
		}
		return FALSE;// 이미 존재함
	}
	return FALSE;
}

// 폰트에서 Texture를 지우지 않고, 이 함수를 통해 Texture가 지워진다.
// 곧, 폰트에서 Effect Render로 들어갈 Texture를 빼면 그 폰트의 Texture=NULL로 세팅한다.
BOOL CEffectRender::DeleteFontTexture(char* strText)
{
	FLOG( "CEffectRender::DeleteFontTexture(char* strText)" );
	map<string,LPDIRECT3DTEXTURE9>::iterator it = m_mapTextToTexture.find(strText);
	if(it != m_mapTextToTexture.end())
	{
		map<string,int>::iterator it2 = m_mapTextRenderCount.find(strText);
		if(it2 != m_mapTextRenderCount.end())
		{
			if((it2->second)>2)
			{
				it2->second --;
			}
			else
			{
				LPDIRECT3DTEXTURE9 pTexture = it->second;
				SAFE_RELEASE(pTexture);
				m_mapTextToTexture.erase(strText);
				m_mapTextRenderCount.erase(strText);
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEffectRender::CheckAlphaRender(CAppEffectData* pParent, DWORD dwType)
/// \brief		
/// \author		ispark
/// \date		2006-11-16 ~ 2006-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEffectRender::CheckAlphaRender(CAppEffectData * pEffect, DWORD dwType)
{
	CCharacterInfo * pChar = pEffect->m_pCharacterInfo;

	if(dwType == _SHUTTLE)
	{
		if( ((g_pShuttleChild->m_pWeapon1_1_1 && 
			pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) || 
			(g_pShuttleChild->m_pWeapon1_1_2 && 
			pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) || 
			(g_pShuttleChild->m_pWeapon2_1_1 && 
			pChar == g_pShuttleChild->m_pWeapon2_1_1->m_pCharacterInfo) || 
			(g_pShuttleChild->m_pWeapon2_1_2 && 
			pChar == g_pShuttleChild->m_pWeapon2_1_2->m_pCharacterInfo) || 
			(g_pShuttleChild->m_pWeapon1_2 &&
			pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo) ||
			(g_pShuttleChild->m_pEngine &&
			pChar == g_pShuttleChild->m_pEngine->m_pCharacterInfo) ||
			(g_pShuttleChild->m_pRadar &&
			pChar == g_pShuttleChild->m_pRadar->m_pCharacterInfo) ||
			(g_pShuttleChild->m_pContainer &&
			pChar == g_pShuttleChild->m_pContainer->m_pCharacterInfo) ||
			(g_pShuttleChild->m_pAccessories &&
			pChar == g_pShuttleChild->m_pAccessories->m_pCharacterInfo) ||
			(g_pShuttleChild->m_pWingIn &&
			pChar == g_pShuttleChild->m_pWingIn->m_pCharacterInfo) ||
			// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
			(g_pShuttleChild->m_pPartner &&
			pChar == g_pShuttleChild->m_pPartner->m_pCharacterInfo) ||
			(g_pShuttleChild->m_pPartner1 &&
			pChar == g_pShuttleChild->m_pPartner1->m_pCharacterInfo)) &&
			// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정 
			g_pShuttleChild->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
		{
			return TRUE;
		}
	}
	else if(dwType == _ENEMY ||
			dwType == _ADMIN)
	{
		CEnemyData * pEnemyData = (CEnemyData *)pEffect->m_pParent;
		if( ((pEnemyData->m_pWeapon1_1_1 && 
			pChar == pEnemyData->m_pWeapon1_1_1->m_pCharacterInfo) || 
			(pEnemyData->m_pWeapon1_1_2 && 
			pChar == pEnemyData->m_pWeapon1_1_2->m_pCharacterInfo) || 
			(pEnemyData->m_pWeapon2_1_1 && 
			pChar == pEnemyData->m_pWeapon2_1_1->m_pCharacterInfo) || 
			(pEnemyData->m_pWeapon2_1_2 && 
			pChar == pEnemyData->m_pWeapon2_1_2->m_pCharacterInfo) || 
			(pEnemyData->m_pWeapon1_2 &&
			pChar == pEnemyData->m_pWeapon1_2->m_pCharacterInfo) ||
			(pEnemyData->m_pEngine &&
			pChar == pEnemyData->m_pEngine->m_pCharacterInfo) ||
			(pEnemyData->m_pRadar &&
			pChar == pEnemyData->m_pRadar->m_pCharacterInfo) ||
			(pEnemyData->m_pContainer &&
			pChar == pEnemyData->m_pContainer->m_pCharacterInfo) ||
			(pEnemyData->m_pAccessories &&
			pChar == pEnemyData->m_pAccessories->m_pCharacterInfo) ||
			(pEnemyData->m_pWingIn &&
			pChar == pEnemyData->m_pWingIn->m_pCharacterInfo) ||
			// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
			(pEnemyData->m_pPartner &&
			pChar == pEnemyData->m_pPartner->m_pCharacterInfo) ||
			(pEnemyData->m_pPartner1 &&
			pChar == pEnemyData->m_pPartner1->m_pCharacterInfo)) &&
			// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정			
			pEnemyData->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
		{
			return TRUE;
		}		
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEffectRender::AddInvenPaticleName(int nInvenIdx, char* pEffectName)
/// \brief		인벤 이펙트 추가
/// \author		// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
/// \date		2007-11-08 ~ 2007-11-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEffectRender::AddInvenPaticleName(int nInvenIdx, char* pEffectName)
{
	structInvenParticleInfo struInvenParticleInfoTmp;
	memset(&struInvenParticleInfoTmp, 0x00, sizeof(structInvenParticleInfo));

	struInvenParticleInfoTmp.nWindowInvenIdx = nInvenIdx;
	strncpy(struInvenParticleInfoTmp.chEffectName, pEffectName, 32);

	m_vecInvenParticleInfo.push_back(struInvenParticleInfoTmp);
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
void CEffectRender::AddEnemyItemPaticleName(int nInvenIdx, char* pEffectName)
{
	structInvenParticleInfo struInvenParticleInfoTmp;
	memset(&struInvenParticleInfoTmp, 0x00, sizeof(structInvenParticleInfo));
	
	struInvenParticleInfoTmp.nWindowInvenIdx = nInvenIdx;
	strncpy(struInvenParticleInfoTmp.chEffectName, pEffectName, 32);
	
	m_vecEnemyItemParticleInfo.push_back(struInvenParticleInfoTmp);
}
// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEffectInfo* CEffectRender::GetEffectInfo(char* pEffectName, int nWindowInvenIdx)
/// \brief		인벤 이펙트 정보 가져온다.
/// \author		// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
/// \date		2007-11-08 ~ 2007-11-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CEffectInfo* CEffectRender::GetEffectInfo(char* pEffectName, int nWindowInvenIdx)
{
	if(NULL == g_pD3dApp->m_pEffectList)
	{
		return NULL;
	}
	CAppEffectData * pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;		
	while(pEffect)
	{
		CCharacterInfo* pChar = pEffect->m_pCharacterInfo;
		if(pChar)
		{
//			if( pEffect->m_pParent 
//				&& (pEffect->m_pParent->m_dwPartType == _SHUTTLE))
			{				
				CEffectInfo* pRtnParticle = GetCharInfo_To_Effect(pChar, pEffectName, nWindowInvenIdx);
				if(pRtnParticle )
				{
					return pRtnParticle;										
				}
			}			
			
		}				
		// 다음으로 넘어감.
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return NULL;
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
CEffectInfo* CEffectRender::GetEnemyEffectInfo(char* pEffectName, int nWindowInvenIdx, UID32_t TargetCharcterUID)
{
	if(NULL == g_pD3dApp->m_pEffectList)
	{
		return NULL;
	}

	CAppEffectData * pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;		
	while(pEffect)
	{
		// 2012-10-20 by bhsohn 인첸트 이펙트 메모리 버그 수정
		BOOL bContinue = FALSE;
		if( !pEffect->m_pParent )
		{
			bContinue = TRUE;
		}
		else if(!(pEffect->m_pParent->m_dwPartType == _ENEMY  || pEffect->m_pParent->m_dwPartType == _ADMIN ))
		{	
			bContinue = TRUE;
		}
		
		if(bContinue)
		{
			pEffect = (CAppEffectData*)pEffect->m_pNext;
			continue;			
		}
		// END 2012-10-20 by bhsohn 인첸트 이펙트 메모리 버그 수정

		if(((CEnemyData *)pEffect->m_pParent)->GetEnemyInfo().CharacterInfo.CharacterUniqueNumber == TargetCharcterUID)
		{
			CCharacterInfo* pChar = pEffect->m_pCharacterInfo;
			if(pChar)
			{							
				CEffectInfo* pRtnParticle = GetEnemyCharInfo_To_Effect(pChar, pEffectName, nWindowInvenIdx);
				if(pRtnParticle )
				{
					return pRtnParticle;										
				}
			}							
		}		
		// 다음으로 넘어감.
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return NULL;
}
// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEffectInfo* CEffectRender::GetCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx)
/// \brief		인벤 이펙트 정보 가져온다.
/// \author		// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
/// \date		2007-11-08 ~ 2007-11-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CEffectInfo* CEffectRender::GetCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx)
{
	set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
	while(itCurrent != pChar->m_vecCurrentBodyCondition.end())
	{		
		map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);		
		if(itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			// 캐릭터 보디 컨디션의 이펙트를 체크
			vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
			while(itEffect != pBody->m_vecEffect.end())
			{			
				// effect rendering
				CEffectInfo* pEffectInfo = (*itEffect);
				switch(pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_PARTICLE:
					{						
						if(pEffectInfo->m_pEffect 
							&& (0 == strncmp(pEffectName, pEffectInfo->m_strEffectName, strlen(pEffectName)))
							&& ((pEffectInfo->m_nInvenWeaponIndex-1) == nWindowInvenIdx))
						{
//							CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;						
//							return pEffect;
							return pEffectInfo;
						}
					}
				}			
				itEffect++;			
			}
		}
		itCurrent++;		
	}
	return NULL;
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
CEffectInfo* CEffectRender::GetEnemyCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx)
{
	set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
	while(itCurrent != pChar->m_vecCurrentBodyCondition.end())
	{		
		map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);		
		if(itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			// 캐릭터 보디 컨디션의 이펙트를 체크
			vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
			while(itEffect != pBody->m_vecEffect.end())
			{			
				// effect rendering
				CEffectInfo* pEffectInfo = (*itEffect);
				switch(pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_PARTICLE:
					{						
						if(pEffectInfo->m_pEffect 
							&& (0 == strncmp(pEffectName, pEffectInfo->m_strEffectName, strlen(pEffectName)))
							&& ((pEffectInfo->m_nInvenWeaponIndex-1-ENEMY_WEAPON_INDEX_DUMMY) == nWindowInvenIdx))
						{
							//							CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;						
							//							return pEffect;
							return pEffectInfo;
						}
					}
				}			
				itEffect++;			
			}
		}
		itCurrent++;		
	}
	return NULL;
}
// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

CEffectInfo* CEffectRender::GetObjEffectInfo(char* pObjName)
{
	CAppEffectData * pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
	CCharacterInfo* pChar;
	while(pEffect)
	{
		pChar = pEffect->m_pCharacterInfo;
		
		if(pChar)
		{
			set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
			while(itCurrent != pChar->m_vecCurrentBodyCondition.end())
			{
				map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);
				if(itBody != pChar->m_mapBodyCondition.end())
				{
					CBodyConditionInfo* pBody = itBody->second;
					// 캐릭터 보디 컨디션의 이펙트를 체크
					vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
					while(itEffect != pBody->m_vecEffect.end())
					{			
						// effect rendering
						CEffectInfo* pEffectInfo = (*itEffect);

						//if(0 == strncmp(pObjName, pEffectInfo->m_strEffectName, strlen(pObjName)))
						if(0 == strncmp(pObjName, pEffectInfo->m_strEffectName, 8))
						{
							return pEffectInfo;
						}								
						itEffect++;			
					}
				}
				itCurrent++;
			}

		}
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return NULL;
}
