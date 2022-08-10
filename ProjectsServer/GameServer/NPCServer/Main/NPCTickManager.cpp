// NPCTickManager.cpp: implementation of the CNPCTickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCTickManager.h"
#include "NPCIOCP.h"
#include "NPCGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCTickManager::CNPCTickManager(CNPCIOCP *i_pNPCIOCP, DWORD i_nTickInterval/*=NPC_DEFAULT_TICK_INTERVAL*/)
: CTickManager(i_nTickInterval)
{
	m_pNPCIOCP2 = i_pNPCIOCP;
}

CNPCTickManager::~CNPCTickManager()
{

}


void CNPCTickManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
}

void CNPCTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

///////////////////////////////////////////////////////////////////////////////
// 하루에 한번 
void CNPCTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPCTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPCTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPCTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}