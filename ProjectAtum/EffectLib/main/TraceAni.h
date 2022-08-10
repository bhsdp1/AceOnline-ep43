// TraceAni.h: interface for the CTraceAni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACEANI_H__5E5475D8_C91D_45F0_99AB_6C3DFA865C98__INCLUDED_)
#define AFX_TRACEANI_H__5E5475D8_C91D_45F0_99AB_6C3DFA865C98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "effect.h"

/*
struct TRACEANI_INDEX
{
	WORD _0,_1,_2,_3;
};
*/
struct TraceData : public Effect
{
	char		m_strName[20];				//이름
	UINT		m_nNumberOfTrace;			// 생성될 잔상박스의 개수 
	float		m_fCreateTick;				//생성 간격(작을수록 촘촘한 잔상이 생성된다.)
	//D3DXVECTOR3 m_vCreatePos;				//생성 좌표(상대좌표) : CEffectInfo로 넘어감
	float		m_fHalfSize;				// 잔상의 가로 길이의 절반
	UINT		m_nTextureNumber;			// 텍스쳐 개수
	float		m_fTextureAnimationTime;	// 텍스쳐 애니메이션 타임
	char		m_strTextureName[20][20];		// 텍스쳐는 최대 20개만 쓸수 있다.
	int			m_nNumberOfCross;				// 크로스 개수(1 ~ 4)

	BOOL		m_bAlphaBlendEnable;
	DWORD		m_dwSrcBlend;
	DWORD		m_dwDestBlend;
	DWORD		m_nTextureRenderState;
	BOOL		m_bZbufferEnable;
	BOOL		m_bZWriteEnable;
	
};

class CTraceAni;
class CEffectPlane : public Effect
{
	friend CTraceAni;
public:
	CEffectPlane(CTraceAni*	pParent, int nIBIndex);
	~CEffectPlane();

//	D3DXVECTOR3 &GetReferencePos() { return m_vPos; }
//	TRACEANI_INDEX &GetReferenceTraceAniIndex() { return m_TraceAniIndex; }

	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	VOID SetFirstIndex(BOOL bSet);
	void SetEndIndex();
	D3DXVECTOR3 GetPos() { return m_vPos; }

public:
	CTraceAni				*m_pParent;				// 부모 객체
protected:
	D3DXVECTOR3				m_vPos ;				// 유닛좌표를 계산한 절대좌표(ZEnable시에 거리계산에 사용), parent의 m_vPos참조
	int						m_nIBIndex;				// 0 ~ 15 사이 값
	LPDIRECT3DVERTEXBUFFER9 m_pVB;					// 버텍스버퍼(4*32)
	BOOL					m_bIsFirstIndex;
	BOOL					m_bZbufferEnable;

};

class CEffectInfo;
class CEffectPlane;
class CTraceAni : public TraceData
{
public:
	float					m_fCurrentCreateTick;		// 생성대기 시간
	int						m_nCurrentNumberOfTrace;	// 현재까지 생성된 잔상개수(MAX=m_nNumberOfTrace)
	int						m_nCurrentTextureNumber;	// 현재 세팅된 텍스쳐 번호
//	LPDIRECT3DVERTEXBUFFER8 m_pVB;					// 버텍스버퍼
	float					m_fCurrentTextureTick;		// 텍스쳐 애니메이션 대기시간
	BOOL					m_bCreateTrace;				// 이 값이 FALSE이면 더이상 잔상을 만들지 않는다.
	CEffectInfo				*m_pParent;					// 이펙트 정보 파일
	CEffectPlane			**m_pEffectPlane;		// CEffectPlane 버퍼(16 X m_nNumberOfTrace)
//	LPDIRECT3DINDEXBUFFER8	*m_pIB;					// 인덱스 버퍼, 16 X m_nNumberOfTrace, CEffectPlane와 1:1매치
	int						m_nCurrentCreateIndex;	// 현재 생성할 VB,Plane 인덱스(0 ~ m_nNumberOfTrace)
	int						m_nCurrentRenderIndex;	// 현재 렌더링이 시작되는 인덱스
	int						m_nCurrentDeleteIndex;	// 지워질 경우 지워지는 인덱스
	D3DXVECTOR3				m_vOldPos;
	D3DXVECTOR3				m_vCurrentPos;
	BOOL					m_bRestored;

	float					m_fRadius;
public:
	CTraceAni();
	~CTraceAni();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	void TraceEnable(BOOL bEnable) { m_bCreateTrace = bEnable; }
	void SetTexture(char* strTexture) {};
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) {};
/*
public:
	float					m_fCurrentCreateTick;		// 생성대기 시간
	int						m_nCurrentNumberOfTrace;	// 현재까지 생성된 잔상개수(MAX=m_nNumberOfTrace)
	int						m_nCurrentTextureNumber;	// 현재 세팅된 텍스쳐 번호
	D3DXVECTOR3				m_vCurrentPos ;				// 삭제됨
	LPDIRECT3DVERTEXBUFFER8 m_pVBTrace[2];				// 버텍스버퍼
	float					m_fCurrentTextureTick;		// 텍스쳐 애니메이션 대기시간
	BOOL					m_bCreateTrace;				// 이 값이 FALSE이면 더이상 잔상을 만들지 않는다.
//	CTraceAniRender*		m_pRenderer;				// 렌더러
	CEffectInfo*			m_pParent;					// 이펙트 정보 파일
	DWORD					m_dwStateBlock;

	BOOL					m_bRestored;
public:
	CTraceAni();
	~CTraceAni();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	void TraceEnable(BOOL bEnable) { m_bCreateTrace = bEnable; }
	void SetTexture(char* strTexture) {};
	void SetTexture(LPDIRECT3DTEXTURE8 pTexture) {};
*/
};

#endif // !defined(AFX_TRACEANI_H__5E5475D8_C91D_45F0_99AB_6C3DFA865C98__INCLUDED_)
