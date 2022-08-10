// AudioCodec.cpp: implementation of the CAudioCodec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AudioCodec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudioCodec::CAudioCodec()
{
	m_hCODECDLibrary	= NULL;
	m_hCODEC			= NULL;
	m_nFrameSize		= 0;
	m_nPacketSize		= 0;
	m_nCodecFormat		= 0;
}

CAudioCodec::~CAudioCodec()
{

}

int		CAudioCodec::GetAudioFrameSize()
{
	return m_nFrameSize;
}

int		CAudioCodec::GetAudioPacketSize()
{
	return m_nPacketSize;
}
