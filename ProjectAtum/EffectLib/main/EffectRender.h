// EffectRender.h: interface for the CEffectRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTRENDER_H__7B0E8CE2_67C8_4A53_97BE_F7589D2FFB62__INCLUDED_)
#define AFX_EFFECTRENDER_H__7B0E8CE2_67C8_4A53_97BE_F7589D2FFB62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"
#include "effect.h"

#define TEX_EFFECT_NUM 100
#define OBJ_EFFECT_NUM 300

class CObjectAni;
class CSpriteAni;
class CParticleSystem;
class CParticle;
class CObjectAni;
class CTraceAni;
class CEffectPlane;
class CSkinnedMesh;
class CGameData;
class CCharacterInfo;
class CAppEffectData;

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
class CEffectInfo;
typedef struct
{
	int nWindowInvenIdx;
	char chEffectName[32];
} structInvenParticleInfo;

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
typedef struct
{
	int LoadingPriority;
	char chEffectName[32];
} LoadingPriorityInfo;
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

class CEffectRender : public CAtumNode  
{
public:
	CEffectRender();
	virtual ~CEffectRender();

	void Render();
//	void RenderSun();
//	void RenderCloud();
	void RenderCharacterInfo(CCharacterInfo* pChar, BOOL bAlpha = FALSE, int nAlphaValue = SKILL_OBJECT_ALPHA_NONE);
	void ObjectAniRender(CObjectAni* pEffect, BOOL bAlpha = FALSE, int nAlphaValue = SKILL_OBJECT_ALPHA_NONE);
	void SpriteAniRender(CSpriteAni* pEffect);
	void ParticleSystemRender(CParticleSystem* pEffect);
	int ParticleRender(CParticleSystem* pParticleSystem, CParticle* p,D3DXVECTOR3 vAxis,int nOldTextureIndex);
//	void ParticleRender(CParticleSystem* pEffect);
	void ObjectParticleRender(CObjectAni* pEffect, CParticle* pParticle);
	void TraceAniRender( CTraceAni* pEffect );
	void EffectPlaneRender( CEffectPlane *pEffect );
	void RenderZEnable();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void Tick(float fElapsedTime);

	int GetEmptyTextureIndex();
	int LoadTexture(char* strName);
//	int GetEmptyObjectIndex();
//	int LoadObject(char* strName);
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//CSkinnedMesh* LoadObject(char* strName);
	CSkinnedMesh* LoadObject(char* strName, int LoadingPriority = _NOTHING_PRIORITY);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	
	void LoadObjectToMap(char* strName);
	DataHeader* FindEffectInfo(char* strName);
	DataHeader* FindObjectInfo(char* strName);
	DWORD LoadEffect(char* strName, DWORD dwEffectType, char* pEffect);
	// by dhkwon, 030923
	BOOL AddFontTexture(char* strText, LPDIRECT3DTEXTURE9 pTexture );
	BOOL DeleteFontTexture(char* strText);

	void DevideZBufferEnableEffect();
	void DevideCharacterEffect(CCharacterInfo* pChar, BOOL bAlpha = FALSE, int nAlphaValue = SKILL_OBJECT_ALPHA_NONE);

	BOOL CheckAlphaRender(CAppEffectData* pEffect, DWORD dwType);

	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
	void ParticleAlphaRender(D3DXVECTOR3 posPaticlePos);	
	int InvenParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex, float fUnitScaling, D3DXMATRIX* pmatPaticlePos, D3DXMATRIX* pmatShttlePos);
	void RenderParticleInvenVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling);
	void ResetContentInvneParticle();
	void AddInvenPaticleName(int nInvenIdx, char* pEffectName);
	CEffectInfo* GetEffectInfo(char* pEffectName, int nWindowInvenIdx);
	CEffectInfo* GetCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx);
	CEffectInfo* GetObjEffectInfo(char* pObjName);

	// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
	void RenderParticleEnemyItemVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling, UID32_t TargetCharcterUID);
	void ResetContentEnemyItemParticle();
	void AddEnemyItemPaticleName(int nInvenIdx, char* pEffectName);
	CEffectInfo* GetEnemyCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx);
	CEffectInfo* GetEnemyEffectInfo(char* pEffectName, int nWindowInvenIdx, UID32_t TargetCharcterUID);
	// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

	LPDIRECT3DVERTEXBUFFER9	m_pVB1;
	LPDIRECT3DVERTEXBUFFER9 m_pVB2[2];			
	LPDIRECT3DVERTEXBUFFER9 m_pVB4[4];			
	LPDIRECT3DVERTEXBUFFER9 m_pVB8[8];			
	LPDIRECT3DVERTEXBUFFER9 m_pVB16[16];

	float					m_fTextureCheckTime;
	CGameData*				m_pTexEffectData;
	CGameData*				m_pEffectData;
	CGameData*				m_pObjectData;
	LPDIRECT3DTEXTURE9		m_pTexture[TEX_EFFECT_NUM];
	int						m_nTextureRenderCount[TEX_EFFECT_NUM];
	//CGameData* pObjEffectData[];
//	CSkinnedMesh*			m_pObjEffectMesh[OBJ_EFFECT_NUM];
	map<string, int>		m_mapTexNameToIndex;
//	map<string, int>		m_mapObjNameToIndex;
	map<string, CSkinnedMesh*>	m_mapObjNameToMesh;
	vector<Effect*>			m_vecZEnableEffect;
	vector<string>			m_vecLoadObj;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	vector<LoadingPriorityInfo>	m_vecLoadingPriority;		
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	// by dhkwon, 030923
	map<string, LPDIRECT3DTEXTURE9> m_mapTextToTexture;
	map<string, int> m_mapTextRenderCount;

	BOOL					m_bZBufferTemp;
	D3DLIGHT9				m_light2;

	int						m_nParticleEffectCountPerSecond;
	int						m_nSpriteEffectCountPerSecond;
	int						m_nObjectEffectCountPerSecond;
	int						m_nTraceEffectCountPerSecond;

private:
	vector<structInvenParticleInfo>			m_vecInvenParticleInfo;
	vector<structInvenParticleInfo>			m_vecEnemyItemParticleInfo;		// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
};

#endif // !defined(AFX_EFFECTRENDER_H__7B0E8CE2_67C8_4A53_97BE_F7589D2FFB62__INCLUDED_)
