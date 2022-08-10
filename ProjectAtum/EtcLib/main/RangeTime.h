///
///		ADD BY JUN SNAG YOUB
///		이미지의 깜박임을 처리하기 위해 장성한
///		Remain Time Clas
///		Range Time Class
///

#ifndef RANGE_TIME_H_
#define RANGE_TIME_H_


//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////////
///	\struct		REMAIN_TIME
/// \brief		Tick 호출시마다 시간이 0보다 크면 0보다 작을 때까지
///				fElapsedTime를 빼준다
/////////////////////////////////////////////////////////////////////////////////

class CRemainTime
{
protected:
	float m_fRemainTime;
	float m_fStartTime;
public:	
	CRemainTime()
	{
		m_fStartTime = m_fRemainTime = 1.0f;
	}

	CRemainTime( float fRemainTime )
	{
		Set( fRemainTime );
	}

	void Tick( float fElapsedTime )
	{
		if( m_fRemainTime < 0 )
			return;
		m_fRemainTime -= fElapsedTime;
	}
	void Set( float fRemainTime )
	{
		if( fRemainTime <= 0.0f )
			fRemainTime = 0.0f;
		
		m_fStartTime = m_fRemainTime = fRemainTime;
	}
	float Get( )
	{
		return m_fRemainTime;
	}
	float GetRemainRate()
	{
		return ( m_fRemainTime/m_fStartTime );
	}
	BOOL IsRemain()
	{
		if( m_fRemainTime > 0 )
			return TRUE;

		return FALSE;
	}
	
};

class CRemainLoopTime : public CRemainTime
{
private:
	int m_nLoopCount;
public:
	CRemainLoopTime()
	{
		Set( 1.0f, 1 );
	}
	CRemainLoopTime(float fRemainTime, int nLoopCount = 1)
	{
		Set( fRemainTime, nLoopCount );
	}

	void Set( float fRemainTime, int nLoopCount = 1)
	{
		CRemainTime::Set( fRemainTime );
		m_nLoopCount = nLoopCount ;
	}
	void Tick( float fElapsedTime )
	{
		if( m_fRemainTime > 0 )
			m_fRemainTime -= fElapsedTime;
		else
		{
			if( m_nLoopCount > 0 )
				m_nLoopCount--;
		}
	}
	BOOL IsNextTime()
	{
		if( m_fRemainTime > 0 )
			return FALSE;
		else
		{
			if( m_nLoopCount > 0 )
				return TRUE;
			else
				return FALSE;
		}

	}
	BOOL IsRemainLoop()
	{
		if( m_nLoopCount > 0 )
			return TRUE;
		else
			return FALSE;
	}
	void StartNext()
	{
		float fRemainTime = m_fStartTime + m_fRemainTime;
		Set( fRemainTime, m_nLoopCount );
	}
};

/////////////////////////////////////////////////////////////////////////////////
///	\class		CRangeTime
/// \brief		Tick 호출시마다 m_fMovePosition은 FixStart부터 FixEnd 의 범위까지 
///				계속 움직인다.
/////////////////////////////////////////////////////////////////////////////////

class CRangeTime
{
private:
	float m_fFixStart;
	float m_fFixEnd;
	float m_fFixMid;
	float m_fTimer;
	
public:
	CRangeTime()
	{
		Set( 0.0f, 1.0f, 0.6f);
	}
	CRangeTime( float fFixStart, float fFixEnd, float fMidRate )
	{
		Set( fFixStart,fFixEnd,fMidRate);
	}
	void Set( float fFixStart, float fFixEnd, float fMidRate )
	{
		m_fFixStart=fFixStart;
		m_fFixEnd=fFixEnd;
		SetMidPosition( fMidRate);
		m_fTimer = m_fFixStart;
	}
	void SetMidPosition( float fMidRate )
	{
		m_fFixMid = ( m_fFixEnd - m_fFixStart ) * fMidRate;
	}
	float Get()
	{
		return m_fTimer;
	}
	float GetPositionRate()
	{
		if( IsOverMiddle() )
		{
			return (m_fTimer-m_fFixMid)/(m_fFixEnd - m_fFixMid);
		}
		else
		{
			return (m_fFixMid - m_fTimer)/(m_fFixMid - m_fFixStart);
		}
	}
	float GetPositionRateInTotal()
	{
		return ( m_fTimer - m_fFixStart)/( m_fFixEnd - m_fFixStart );
	}
	BOOL IsOverMiddle()
	{
		return ( m_fTimer > m_fFixMid ) ? TRUE : FALSE;
	}
	BOOL IsUnderMiddle()
	{
		return ( m_fTimer <= m_fFixMid ) ? TRUE : FALSE;
	}
	BOOL IsOverEnd()
	{
		return ( m_fTimer > m_fFixEnd ) ? TRUE : FALSE;
	}

	void Start()
	{
		m_fTimer = m_fFixStart;
	}
	void Stop()
	{
		m_fTimer = m_fFixEnd + 0.001f;
	}
	void Tick( float fElapedTime )
	{
		if( m_fTimer > m_fFixEnd )
			return;
		m_fTimer += fElapedTime;
	}
};


#endif