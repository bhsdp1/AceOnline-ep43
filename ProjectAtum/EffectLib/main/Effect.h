#if !defined(AFX_EFFECT_H_)
#define AFX_EFFECT_H_

#define D3DFVF_PARTICLE_VERTEX (D3DFVF_XYZ|D3DFVF_PSIZE|D3DFVF_DIFFUSE)
#define D3DFVF_SPRITE_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define FIRST_BODYCONDITION 0x00000001 // 64bit flag
#define VEL_COLOR_CHANGE	fElapsedTime * 50.0f

// object particle moving type
#define OBJ_MOVE_TYPE0	0
#define OBJ_MOVE_TYPE1	1
#define OBJ_MOVE_TYPE2	2
#define OBJ_MOVE_TYPE3	3

enum { PARTICLE_OBJECT_TYPE, PARTICLE_SPRITE_TYPE, PARTICLE_TRACE_TYPE };
enum { EMITTER_POINT, EMITTER_CIRCLE, EMITTER_SPHERE, EMITTER_AREA };
enum { TEXTURE_SIZE_RANDOM, TEXTURE_SIZE_STEP, TEXTURE_SIZE_FIX };
enum { TEXTURE_ANIMATION_NONE=-1, TEXTURE_ANIMATION_RANDOM, TEXTURE_ANIMATION_STEP };
enum { EFFECT_TYPE_OBJECT, EFFECT_TYPE_SPRITE, EFFECT_TYPE_PARTICLE, EFFECT_TYPE_TRACE };


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
struct EffectFileHeader
{
	DWORD	dwProductID;
	DWORD	dwVersion;
	DWORD	dwEffectType;
};

struct Effect
{
	DWORD dwType;
	float fDistance;
};

struct CompareEffect
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		if(pPtr1->fDistance > pPtr2->fDistance)
		{
			return true;
		}
		return false;
	}
};

//DWORD LoadEffect(char* strName, DWORD dwEffectType, char* pEffect);
//void SaveEffect(char* strName, DWORD dwEffectType, char* pEffect);

#endif