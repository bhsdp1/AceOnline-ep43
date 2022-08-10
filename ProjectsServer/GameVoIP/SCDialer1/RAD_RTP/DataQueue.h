// DataQueue.h: interface for the CDataQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAQUEUE_H__53325E8D_5B20_47DB_A70F_A173E5488D49__INCLUDED_)
#define AFX_DATAQUEUE_H__53325E8D_5B20_47DB_A70F_A173E5488D49__INCLUDED_

#include <afxmt.h>
#include <afxtempl.h>
#include <mmsystem.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _G7231CODEC								// DataQueue.h
#define JITTERBUFFERSIZE	30
#endif

#ifdef _G729CODEC								// DataQueue.h
#define JITTERBUFFERSIZE	20
#endif


typedef struct 
{		
	unsigned long	sequenceNumber; 
	unsigned long	time;
	int				dataSize;
	BYTE*			lpData;		
}JittData;

class CDataQueue  
{
public:	
	unsigned long	GetHighSeqNuber(); 
	void DeleteDataFromBuffer(int count);
	int GetDataCount(void);
	JittData* GetDataFromBuffer(void);
	int SetDataToBuffer(JittData* lp);
	void InitializeVariable(void);
	void EmptyDataQueue(void);
	CDataQueue();
	virtual ~CDataQueue();

	void MixingOperation(short* src1, float rate1, short* src2, float rate2, short* des, int shortSize, float Rate);

private:
	CList < JittData*, JittData* >		m_BufferList;
	CRITICAL_SECTION		m_critical;

	// Lost Packet Recovery
	JittData		m_LastData;
	BOOL			m_bFirstPacket;
	int				m_nRecoveryCount;
	int				m_nReceiveCount;

};

#endif // !defined(AFX_DATAQUEUE_H__53325E8D_5B20_47DB_A70F_A173E5488D49__INCLUDED_)
