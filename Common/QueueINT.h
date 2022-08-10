// QueueINT.h: interface for the CQueueINT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEUEINT_H__7D90B033_C4E9_4EBC_AB79_43B916F7F3AB__INCLUDED_)
#define AFX_QUEUEINT_H__7D90B033_C4E9_4EBC_AB79_43B916F7F3AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef mt_vector<INT>			mtvectorINT;

class CQueueINT  
{
public:
	CQueueINT();
	~CQueueINT();

	BOOL IsValidDataNum(int i_nNum);
	BOOL InitQueueINT(int i_nMinNum, int i_nMaxNum);
	void ResetQueueINT(void);

	BOOL PopQueueINT(int *o_pNum);
	BOOL PushQueueINT(int i_nNum);

protected:
	mtvectorINT			m_mtvectorINTList;	
	int					m_nFrontIndex;
	int					m_nBackIndex;
	int					m_nQueueSize;
	int					m_nQueueMinNum;
	int					m_nQueueMaxNum;
};

#endif // !defined(AFX_QUEUEINT_H__7D90B033_C4E9_4EBC_AB79_43B916F7F3AB__INCLUDED_)
