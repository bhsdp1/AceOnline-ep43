// DataQueue.cpp: implementation of the CDataQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataQueue::CDataQueue()
{
	InitializeCriticalSection(&m_critical);
	if(!m_BufferList.IsEmpty())
	{
		m_BufferList.RemoveAll();
	}

	ZeroMemory(&m_LastData, sizeof(JittData));
	m_LastData.lpData = new BYTE[512];
	m_bFirstPacket = TRUE;
	m_nRecoveryCount = 0;
	m_nReceiveCount = 0;
}

CDataQueue::~CDataQueue()
{
	EmptyDataQueue();
	if(m_LastData.lpData)
	{
		delete[] m_LastData.lpData;
	}

	DeleteCriticalSection(&m_critical);
}

void CDataQueue::InitializeVariable()
{
	EmptyDataQueue();	
}

int CDataQueue::SetDataToBuffer(JittData *lp)
{
	int nRet = 0;

	EnterCriticalSection(&m_critical);	
	if(lp && lp->lpData) 
	{		
		if(m_BufferList.GetCount() >= JITTERBUFFERSIZE)
		{	
//			DbgOut("\n					Buffer is full.");
			delete[] lp->lpData;
			delete lp;			
		}
		else
		{			
			POSITION pos, posBack;
			posBack = pos = m_BufferList.GetTailPosition();
			if( pos == NULL || m_BufferList.GetCount() <= 0)			// List is empty.
			{			
				m_BufferList.AddTail(lp);
			}
			else
			{
				JittData* head = m_BufferList.GetHead();
				JittData* tmp = m_BufferList.GetPrev(pos);				
				
				if(tmp == NULL || pos == NULL)					// ListCount is 1.
				{
					if( tmp->sequenceNumber <= lp->sequenceNumber)
					{
						m_BufferList.AddTail(lp);						
					}					
					else if( tmp->sequenceNumber - lp->sequenceNumber == 1) 
					{
						m_BufferList.AddHead(lp);						
					}
					else
					{
						if( lp && lp->lpData)
						{
							delete[] lp->lpData;
						}
						else if(lp)
						{
							delete lp;
						}
					}
				}
				else
				{
					for(int i=0; i < m_BufferList.GetCount()-1; i++)
					{
						if( tmp->sequenceNumber <= lp->sequenceNumber)
						{
							m_BufferList.InsertAfter(posBack, lp);
							break;
						}
						else
						{
							POSITION tpos = pos;
							posBack = pos;
							tmp = m_BufferList.GetPrev(pos);
							
							if(tmp == head || pos == NULL)
							{
								if( tmp->sequenceNumber <= lp->sequenceNumber)
								{
									m_BufferList.InsertAfter(tpos, lp);
								}
								else if( tmp->sequenceNumber - lp->sequenceNumber == 1) 
								{
									m_BufferList.AddHead(lp);
								}
								else
								{
									if( lp && lp->lpData)
									{
										delete[] lp->lpData;
									}
									else if(lp)
									{
										delete lp;
									}
								}
								break;
							}
						}
					}
				}			
			}		
		}
	}

	nRet = m_BufferList.GetCount();

	LeaveCriticalSection(&m_critical);
	
	return nRet;
}

//#define _LOSTPACKET_RECOVERY

JittData* CDataQueue::GetDataFromBuffer()
{
	JittData* lp = NULL;
	JittData* temp = NULL;

	EnterCriticalSection(&m_critical);
	
	if(m_BufferList.IsEmpty() || m_BufferList.GetCount() <= 0 )
	{
		m_bFirstPacket = TRUE;
		TRACE("\n					Buffer is Empty.");				
	}
	else
	{

#ifndef _LOSTPACKET_RECOVERY
		lp = m_BufferList.RemoveHead();
#else
		temp = m_BufferList.GetHead();

		if( m_bFirstPacket)
		{
			lp = m_BufferList.RemoveHead();		
			
			m_LastData.dataSize = lp->dataSize;
			m_LastData.sequenceNumber = lp->sequenceNumber;
			m_LastData.time = lp->time;
			CopyMemory(m_LastData.lpData, lp->lpData, lp->dataSize);

			m_bFirstPacket = FALSE;
			m_nReceiveCount++;
		}		
		else if((temp->time - m_LastData.time) < 300)	// 240
		{
			//TRACE("\n					240 ÀÌ´Ù");
			lp = m_BufferList.RemoveHead();		
			
			m_LastData.dataSize = lp->dataSize;
			m_LastData.sequenceNumber = lp->sequenceNumber;
			m_LastData.time = lp->time;
			CopyMemory(m_LastData.lpData, lp->lpData, lp->dataSize);		

			m_nRecoveryCount = 0;
			m_nReceiveCount++;
		}
		else if((temp->time - m_LastData.time) < 500)	// 480
		{			
			m_nRecoveryCount++;		
			if(m_nRecoveryCount == 1 && m_nReceiveCount < 5000)
			{
				TRACE("\n					Lost Packet Recovery [1-1]");
				lp = new JittData;
				ZeroMemory(lp, sizeof(JittData));
				lp->dataSize = m_LastData.dataSize;
				lp->sequenceNumber = m_LastData.sequenceNumber;
				lp->time = m_LastData.time + 240 * m_nRecoveryCount;
				lp->lpData = new BYTE[lp->dataSize];

				CopyMemory(lp->lpData, temp->lpData, lp->dataSize);
				MixingOperation((short*)lp->lpData , (float)0.2, (short*)m_LastData.lpData, (float)0.2, (short*)lp->lpData, lp->dataSize/2, 0.5);				
			}
			else
			{
				TRACE("\n					Lost Packet Recovery [1-2]");
				lp = m_BufferList.RemoveHead();		
				
				m_LastData.dataSize = lp->dataSize;
				m_LastData.sequenceNumber = lp->sequenceNumber;
				m_LastData.time = lp->time;
				CopyMemory(m_LastData.lpData, lp->lpData, lp->dataSize);		
				
				m_nRecoveryCount = 0;				
			}
			
			m_nReceiveCount = 0;
		}
		else if((temp->time - m_LastData.time) < 800)	// 720
		{
			m_nReceiveCount = 0;
			m_nRecoveryCount++;
			if(m_nRecoveryCount == 1)
			{
				lp = new JittData;
				ZeroMemory(lp, sizeof(JittData));
				lp->dataSize = m_LastData.dataSize;
				lp->sequenceNumber = m_LastData.sequenceNumber;
				lp->time = m_LastData.time + 240 * m_nRecoveryCount;
				lp->lpData = new BYTE[lp->dataSize];

				CopyMemory(lp->lpData, temp->lpData, lp->dataSize);
				MixingOperation((short*)lp->lpData, 0.25, (short*)m_LastData.lpData, 0.5, (short*)lp->lpData, lp->dataSize/2, 0.75);
				TRACE("\n					Lost Packet Recovery [2-1]");
			}
			else if(m_nRecoveryCount == 2)
			{
				lp = new JittData;
				ZeroMemory(lp, sizeof(JittData));
				lp->dataSize = m_LastData.dataSize;
				lp->sequenceNumber = m_LastData.sequenceNumber;
				lp->time = m_LastData.time + 240 * m_nRecoveryCount;
				lp->lpData = new BYTE[lp->dataSize];

				CopyMemory(lp->lpData, temp->lpData, lp->dataSize);
				MixingOperation((short*)lp->lpData , 0.5, (short*)m_LastData.lpData, 0.25, (short*)lp->lpData, lp->dataSize/2, 0.75);
				TRACE("\n					Lost Packet Recovery [2-2]");
			}
			else
			{
				lp = m_BufferList.RemoveHead();		
				
				m_LastData.dataSize = lp->dataSize;
				m_LastData.sequenceNumber = lp->sequenceNumber;
				m_LastData.time = lp->time;
				CopyMemory(m_LastData.lpData, lp->lpData, lp->dataSize);		
				
				m_nRecoveryCount = 0;
				TRACE("\n					Lost Packet Recovery [2-3]");
			}			
		}
		else if((temp->time - m_LastData.time) < 1000)	// 960
		{	
			m_nReceiveCount = 0;
			m_nRecoveryCount++;
			if(m_nRecoveryCount == 1)
			{
				lp = new JittData;
				ZeroMemory(lp, sizeof(JittData));
				lp->dataSize = m_LastData.dataSize;
				lp->sequenceNumber = m_LastData.sequenceNumber;
				lp->time = m_LastData.time + 240 * m_nRecoveryCount;
				lp->lpData = new BYTE[lp->dataSize];

				CopyMemory(lp->lpData, temp->lpData, lp->dataSize);
				MixingOperation((short*)lp->lpData, 0.25, (short*)m_LastData.lpData, 0.5, (short*)lp->lpData, lp->dataSize/2, 0.5);
				TRACE("\n					Lost Packet Recovery [3-1]");
			}
			else if(m_nRecoveryCount == 2)
			{
				lp = new JittData;
				ZeroMemory(lp, sizeof(JittData));
				lp->dataSize = m_LastData.dataSize;
				lp->sequenceNumber = m_LastData.sequenceNumber;
				lp->time = m_LastData.time + 240 * m_nRecoveryCount;
				lp->lpData = new BYTE[lp->dataSize];

				CopyMemory(lp->lpData, temp->lpData, lp->dataSize);
				MixingOperation((short*)lp->lpData , (float)0.34, (short*)m_LastData.lpData, (float)0.34, (short*)lp->lpData, lp->dataSize/2, 0.5);
				TRACE("\n					Lost Packet Recovery [3-2]");
			}
			else if(m_nRecoveryCount == 3)
			{
				lp = new JittData;
				ZeroMemory(lp, sizeof(JittData));
				lp->dataSize = m_LastData.dataSize;
				lp->sequenceNumber = m_LastData.sequenceNumber;
				lp->time = m_LastData.time + 240 * m_nRecoveryCount;
				lp->lpData = new BYTE[lp->dataSize];

				CopyMemory(lp->lpData, temp->lpData, lp->dataSize);
				MixingOperation((short*)lp->lpData , 0.5, (short*)m_LastData.lpData, 0.25, (short*)lp->lpData, lp->dataSize/2, 0.5);
				TRACE("\n					Lost Packet Recovery [3-3]");
			}
			else
			{
				lp = m_BufferList.RemoveHead();		
				
				m_LastData.dataSize = lp->dataSize;
				m_LastData.sequenceNumber = lp->sequenceNumber;
				m_LastData.time = lp->time;
				CopyMemory(m_LastData.lpData, lp->lpData, lp->dataSize);		
				
				m_nRecoveryCount = 0;
				TRACE("\n					Lost Packet Recovery [3-4]");
			}
		}
		else
		{
			lp = m_BufferList.RemoveHead();	
			m_LastData.dataSize = lp->dataSize;
			m_LastData.sequenceNumber = lp->sequenceNumber;
			m_LastData.time = lp->time;
			CopyMemory(m_LastData.lpData, lp->lpData, lp->dataSize);		
			
			m_nRecoveryCount = 0;			
		}
#endif
	}

	LeaveCriticalSection(&m_critical);
	
	return lp;
}

int CDataQueue::GetDataCount()
{
	return m_BufferList.GetCount();
}

void CDataQueue::EmptyDataQueue()
{	
	JittData*	lp;

	EnterCriticalSection(&m_critical);
	while(!m_BufferList.IsEmpty())
	{	
		lp = NULL;
		lp = m_BufferList.RemoveHead();
		if(lp && lp->lpData)
		{
			delete[] lp->lpData;			
		}
		if(lp)
		{
			delete lp;
		}
	}
	LeaveCriticalSection(&m_critical);
}

void CDataQueue::DeleteDataFromBuffer(int count)
{
	JittData* pJD;
	EnterCriticalSection(&m_critical);
	for(int i=0; i < count; i++)
	{
		pJD = NULL;
		pJD = GetDataFromBuffer();
		if(pJD && pJD->lpData)
		{
			delete[] pJD->lpData ;
			delete pJD;
			pJD = NULL;
		}
		else
		{
			if(pJD)
			{
				delete pJD;
			}
		}
	}
	LeaveCriticalSection(&m_critical);
}

void CDataQueue::MixingOperation(short* src1, float rate1, short* src2, float rate2, short* des, int shortSize, float Rate)
{
	int in;
		
	for(int i=0; i < shortSize ; i++)
	{		
		in = (int)((src1[i] * rate1 + src2[i] * rate2) * Rate);

		if(in < SHRT_MIN )
		{
			des[i] = SHRT_MIN;
		}
		else if(in > SHRT_MAX)
		{
			des[i] = SHRT_MAX;
		}
		else
		{
			des[i] = in;
		}			
	}	
}

unsigned long CDataQueue::GetHighSeqNuber()
{
	if(m_BufferList.GetCount() > 0)
	{
		JittData* temp = m_BufferList.GetTail();
		return temp->sequenceNumber;
	}

	return 0;
}
