// ObjectAniData.h: interface for the CObjectAniData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTANIDATA_H__DA6D75DA_6683_44D3_9028_A4A6AE8112B7__INCLUDED_)
#define AFX_OBJECTANIDATA_H__DA6D75DA_6683_44D3_9028_A4A6AE8112B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "characterinfo.h"

struct ObjectAniData : public Effect
{
	char	m_strName[20];
	char	m_strObjectFile[20];
	float	m_fScale;
	int		m_nObjectAniType;
	float	m_fTextureAniVel;
	float	m_fObjectAniVel;
	float	m_fTick;
	BOOL	m_bZbufferEnable;
	D3DXCOLOR m_cColor;
	D3DXCOLOR m_cColorStep;
	float	m_fColorChangeStartTime;
	BOOL	m_bAlphaBlending;
	int		m_nSrcBlend;
	int		m_nDestBlend;
	int		m_nTextureRenderState;
	BOOL	m_bLightMapUse;
	BOOL	m_bLightMapAlphaBlending;
	int		m_nLightMapSrcBlend;
	int		m_nLightMapDestBlend;
	int		m_nLightMapRenderState;
	BOOL	m_bAnimationLoop;
	float	m_fStartTime;
	float	m_fEndTime;
	BOOL	m_bObjectAnimationLoop;
//	BOOL	m_bColorLoop;
	int		m_nColorLoop;
	BOOL	m_bUseEnvironmentLight;
	BOOL	m_bAlphaTestEnble;
	int		m_nAlphaTestValue;
	BOOL	m_bZWriteEnable;
};

class CObjectAni : public ObjectAniData
{
public:
	CObjectAni();
	~CObjectAni();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void SetTexture(char* strTexture);// by dhkwon, 030917
	void SetObjectAniStartTime(float fStartObjectAniTime);
	float GetRadius() { return m_fRadius; }

//	FLOAT		m_fDistance;
	int			m_nCurrentTextureType;
	int			m_nTotalTextureNumber;
	FLOAT		m_fCurrentTick;
	FLOAT		m_fCurrentTextureAniTime;
	FLOAT		m_fCurrentObjectAniTime;
	BOOL		m_bRestored;
	float		m_fCurrentBillboardRotateAngle;
	float		m_fCurrentBillboardRotateTime;
	float		m_fCurrentRandomUpAngleX;
	float		m_fCurrentRandomUpAngleZ;
	char		m_strTextureFile[20];
	
	float		m_fRadius;
//	CObjectAniRender* m_pRenderer;
	CEffectInfo* m_pParent;

	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int			m_LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

};

#endif // !defined(AFX_OBJECTANIDATA_H__DA6D75DA_6683_44D3_9028_A4A6AE8112B7__INCLUDED_)
