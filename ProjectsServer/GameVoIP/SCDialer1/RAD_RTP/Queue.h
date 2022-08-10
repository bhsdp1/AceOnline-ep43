// Queue.h: interface for the CQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEUE_H__B64962C5_F995_495B_9675_6C82B7CCE2F8__INCLUDED_)
#define AFX_QUEUE_H__B64962C5_F995_495B_9675_6C82B7CCE2F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
using namespace std;

class CQueue  
{
public:
	enum {DEFAULT_MAX_SIZE = 2048};

//protected:
	CRITICAL_SECTION	m_Lock;
	const int			m_iMaxSize;

	BYTE*				m_pBuffer;
	int					m_nfront;
	int					m_nback;
	int					m_nTotalBytes;

	
public:
	CQueue(int max_size);
	virtual ~CQueue();

	//Queue에서 남아있는 공간의 size를 return.
	int SpaceFree();

	//Queue에서 사용중인 공간의 size를 return.
	int SpaceUsed();

	//insert한 count만큼의 data를 Insert하고 성공하면,
	//그 수맡큼을 return하고, 실패하면 0을 return.
	int InsertFront(byte *data, int count);

	//Queue에서 max수 맡큼의 data을 extract하고,
	//빼낸 수만큼을 return ,실패하면 0을 return.
	int ExtractBack(byte *data, int max);

	//Queue값을 Clear.
	void Clear();
};

#endif // !defined(AFX_QUEUE_H__B64962C5_F995_495B_9675_6C82B7CCE2F8__INCLUDED_)
