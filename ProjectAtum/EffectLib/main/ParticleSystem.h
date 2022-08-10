// ParticleSystem.h: interface for the CParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLESYSTEM_H__96E094F7_D630_44BD_A363_2C624A4665ED__INCLUDED_)
#define AFX_PARTICLESYSTEM_H__96E094F7_D630_44BD_A363_2C624A4665ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
/*
struct PARTICLE_VERTEX
{
    D3DXVECTOR3 p; // The position.
	float		s; // The PointSize
    D3DCOLOR    c; // The color.
};
struct SPRITE_VERTEX
{
    D3DXVECTOR3 p; // The position.
    D3DCOLOR    c; // The color.
	float		tu,tv;
};

#define D3DFVF_PARTICLE_VERTEX (D3DFVF_XYZ|D3DFVF_PSIZE|D3DFVF_DIFFUSE)
#define D3DFVF_SPRITE_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

enum { PARTICLE_TYPE, SPRITE_TYPE };
enum { EMITTER_POINT, EMITTER_CIRCLE, EMITTER_SPHERE, EMITTER_AREA };
enum { TEXTURE_SIZE_RANDOM, TEXTURE_SIZE_STEP, TEXTURE_SIZE_FIX };
enum { TEXTURE_ANIMATION_NONE=-1, TEXTURE_ANIMATION_RANDOM, TEXTURE_ANIMATION_STEP };

struct ComParticle
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		if(pPtr1->m_fDistance > pPtr2->m_fDistance)
		{
			return true;
		}
		return false;
	}
};
*/
class CEffectInfo;
class CObjectAni;
class CTraceAni;
class CParticleSystem;
class CParticle : public Effect
{
public:
	CParticle(int nParticleType);
	~CParticle();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	BOOL Tick(float fElapsedTime);

	int			m_nParticleType;// 파티클 종류(PARTICLE, SPRITE)
	float		m_fSize;		// 크기
	float		m_fLifeTime;	// 파티클의 라이프 타임
	float		m_fCurrentLifeTime;	// 현재 파티클의 라이프 타임

	D3DXCOLOR	m_cColor;		// 색
	D3DXCOLOR	m_cColorStep;	// 색 변화율
	D3DXVECTOR3	m_vPos;			// 위치
	D3DXVECTOR3 m_vVel;			// 위치 변화 가속도

	D3DXVECTOR3 m_vDir;			// 위치 변화 속도
//	D3DXVECTOR3 m_vArea;		// 영역 좌표(2)
	BOOL		m_bCreateRandom;// 영역에서 랜덤하게 생성될 경우 TRUE
	float		m_fCircleForce;	// 원심력
//	float		m_fCreateDensity;// 생성 밀도(가속도의 랜덤 최대각)
//	float		m_fEmitRadius;	// 발산되는 각도 
	float		m_fRotateAngle;	// 회전 속도
	float		m_fTextureAnimationTime;// 텍스쳐 애니메이션 시간
//	int			m_nEmitterType;	// 발산기 타입
	int			m_nTextureAnimationType;// 텍스쳐 애니메이션 타입(랜덤,순차,고정)
	int			m_nTextureType;	// 텍스쳐 타입

	float		m_fTextureSizeVel;	// 텍스쳐가 변화하는 속도
	float		m_fTextureStartSize;// 텍스쳐의 최초 사이즈
	float		m_fTextureSizeMax;	// 텍스쳐 최대 사이즈
	float		m_fTextureSizeMin;	// 텍스쳐 최소 사이즈
	int			m_nTextureSizeChangeType;// 텍스쳐 사이즈 변화 타입
	float		m_fCurrentTextureAnimationTime;// 현재 텍스쳐 애니메이션 시간
	int			m_nTextureNumber;

	float		m_fGravity;		// 중력

	int			m_nPersistence;
//	float		m_fDistance;
	float		m_fColorChangeStartTime;

	float		m_fCurrentRotateAngle;
	float		m_fCurrentRotateTime;

	D3DXVECTOR3	m_vObjTarget;
	D3DXVECTOR3	m_vObjUp;
//	LPDIRECT3DVERTEXBUFFER8	m_pVBParticle;
	D3DXVECTOR3 m_vStartPos;
	D3DXMATRIX	m_mOldParentMatrix;
	CParticleSystem* m_pParent;
	// object particle
	CObjectAni*	m_pObjectAni;
	CTraceAni*	m_pTraceAni;
	float		m_fCullRadius;
};

struct ParticleData
{
//	DWORD	m_dwStateBlock;		// 렌더링 상태 블럭
	char	m_strName[20];		// 파티클 이름
	BOOL	m_bLoop;			// 루프를 돌때 TRUE
	float	m_fDelayTime;		// 루프시에 기다리는 시간
	float	m_fCurrentDelayTime;// 현재 루스 시간
	DWORD	m_dwDestBlend;		// DEST알파블렌딩 값
	DWORD	m_dwSrcBlend;		// SOURCE알파블렌딩 값
	int		m_nEmitMass;		// 한번에 발산되는 양
	float	m_fEmitTime;		// 파티클을 발산하는 텀
	float	m_fCurrentEmitTime;	// 현재 발산기의 시간(위의 텀을 계산하기 위해 사용)
	float	m_fGravity;			// 중력
//	float	m_fLifeTime;		
	float	m_fEmitLifeTime;	// 발산시스템의 LifeTime
	float	m_fCurrentEmitLifeTime;// 발산시스템의 현재 LifeTime;
	float	m_fParticleLifeTime;// 파티클의 LifeTime
	D3DXCOLOR m_cStartColor;	// 파티클의 시작 컬러값
	D3DXCOLOR m_cColorVel;		// 파티클의 컬러 변화율
	D3DXVECTOR3 m_vPos;			// 생성 좌표
	D3DXVECTOR3 m_vVel;			// 가속도
	float	m_fTextureSizeVel;	// 텍스쳐가 변화하는 속도
	float	m_fTextureStartSize;// 텍스쳐의 최초 사이즈
	float	m_fTextureSizeMax;	// 텍스쳐 최대 사이즈
	float	m_fTextureSizeMin;	// 텍스쳐 최소 사이즈
	float	m_fTick;			// 파티클의 계산 텀
	int		m_nTextureSizeChangeType;// 텍스쳐 사이즈 변화 타입
	float	m_fCurrentTick;		// 파티클의 계산 텀을 계산하기 위해 사용

	D3DXVECTOR3 m_vDir;			// 파티클 생성 방향
	D3DXVECTOR3 m_vArea;		// 생성 영역
	BOOL	m_bCreateRandom;	// 생성 영역에서 랜덤하게 생성될때 TRUE
	float	m_fCircleForce;		// 원심력(가속)
	float	m_fCreateDensity;	// 생성시의 밀도(발산되는 각도와 가속도의 벌어지는 랜덤폭)
	float	m_fEmitRadius;		// 생성시 원이나 구로 생성될때 반지름
	float	m_fRotateAngle;		// 공전(회전)할 경우 회전력
	float	m_fTextureAnimationTime;// 텍스쳐 애니메이션 시간
	int		m_nEmitterType;		// 발산기의 타입(점, 원,구, 영역)
	int		m_nTextureAnimationType;// 텍스쳐 애니메이션 타입(랜덤, 순차)
	int		m_nParticleType;	// 파티클 타입(파티클, 스프라이트)
	float	m_fEmitAngle;		// Circle생성시 파티클을 한번에 생성할 경우 회전율
	int		m_nTextureNumber;	// 텍스쳐 개수
	char	m_strTextureName[20][20];// 텍스쳐 이름, 20개 이상은 쓰지 말것.!
	int		m_nPersistence;
	BOOL	m_bZbufferEnable;
	float	m_fColorChangeStartTime;
//	BOOL	m_bColorLoop;
	int		m_nColorLoop;

	int		m_nObjCreateTargetType;
	int		m_nObjCreateUpType;
	int		m_nObjMoveTargetType;
	BOOL	m_bZWriteEnable;
	
};

class CParticleSystem  : public ParticleData
{
public:
	CParticleSystem();
	~CParticleSystem();
	
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void EmitterStop() { m_fCurrentEmitLifeTime = 0; }
	void SetTexture(char* strTexture, int index = 0);// by dhkwon, 030917

	DWORD	m_dwStateBlock;		// 렌더링 상태 블럭

	vector<Effect*>			m_vecParticle;

	CEffectInfo*				m_pParent;
	
	float		m_fRadius;

};
#endif // !defined(AFX_PARTICLESYSTEM_H__96E094F7_D630_44BD_A363_2C624A4665ED__INCLUDED_)
