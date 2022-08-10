#include "StdAfx.h"
#include "GLogDataEventParticipationRate.h"

CGLogDataEventParticipationRate::CGLogDataEventParticipationRate(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataEventParticipationRate::~CGLogDataEventParticipationRate(void)
{
}

void CGLogDataEventParticipationRate::ResetData()
{
	MEMSET_ZERO(m_arrEventParticipationRateData,sizeof(CEventParticipationRateData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataEventParticipationRate::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog EventParticipationRate Select Fin!!! ==============\r\n",TRUE);

	//Insert & Delete�� ���྿ ó��.
	for(int Cnt = 0 ; Cnt < SelectCnt; Cnt++)
	{
		Err = this->GLogDBInsert(Cnt);
		if(ERR_NO_ERROR != Err
			&& ERR_DB_COM_DUPLICATE_INSERT != Err)
		{
			return this->ErrorProcessing(Err);
		}

		Err = this->GLogDBDelete(Cnt);
		if(ERR_NO_ERROR != Err)
		{
			return this->ErrorProcessing(Err);
		}
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog EventParticipationRate Update Fin!!! ==============\r\n",TRUE);
	this->ResetData();

	return ERR_NO_ERROR;
}

Err_t CGLogDataEventParticipationRate::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_EVENT_PARTICIPATION_RATE_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataEventParticipationRate",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CEventParticipationRateData	tmpEventParticipationRateData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpEventParticipationRateData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		LSTRNCPY_OLEDB(tmpEventParticipationRateData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpEventParticipationRateData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpEventParticipationRateData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		tmpEventParticipationRateData.StartDate = m_pSourceRecordSet->Fields->GetItem("StartDate")->GetValue();
		tmpEventParticipationRateData.EndDate = m_pSourceRecordSet->Fields->GetItem("EndDate")->GetValue();
		tmpEventParticipationRateData.ParticipationCount = m_pSourceRecordSet->Fields->GetItem("ParticipationCount")->GetValue();
		tmpEventParticipationRateData.ParticipationRate = m_pSourceRecordSet->Fields->GetItem("ParticipationRate")->GetValue();
		LSTRNCPY_OLEDB(tmpEventParticipationRateData.Description,m_pSourceRecordSet->Fields->GetItem("Description")->GetValue());
		memcpy(&m_arrEventParticipationRateData[nCnt],&tmpEventParticipationRateData,sizeof(CEventParticipationRateData));
		MEMSET_ZERO(&tmpEventParticipationRateData,sizeof(CEventParticipationRateData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataEventParticipationRate::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_EVENT_PARTICIPATION_RATE_INSERT;

		// ���ν����� �� ���ڰ� ����
		this->DBInsertParametersAppend(i_nCnt);

		// ���ν��� ����
		m_pDestCommand->Execute(NULL,NULL,adCmdStoredProc);
		
		// ���ڰ� �ʱ�ȭ
		this->DBInsertParametersDelete();
	}
	catch(_com_error& e)
	{
		// ���ڰ� �ʱ�ȭ
		this->DBInsertParametersDelete();
		return ErrorLog(L"CGLogDataEventParticipationRate",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataEventParticipationRate::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_EVENT_PARTICIPATION_RATE_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrEventParticipationRateData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataEventParticipationRate",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataEventParticipationRate::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrEventParticipationRateData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrEventParticipationRateData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrEventParticipationRateData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("StartDate"),adDate,adParamInput,8,_variant_t(m_arrEventParticipationRateData[i_nCnt].StartDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("EndDate"),adDate,adParamInput,8,_variant_t(m_arrEventParticipationRateData[i_nCnt].EndDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ParticipationCount"),adInteger,adParamInput,0,_variant_t(m_arrEventParticipationRateData[i_nCnt].ParticipationCount)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ParticipationRate"),adSmallInt,adParamInput,0,_variant_t(m_arrEventParticipationRateData[i_nCnt].ParticipationRate)));
	VARIANT vDescription;
	vDescription.vt = VT_BSTR;
	vDescription.bstrVal = _bstr_t(m_arrEventParticipationRateData[i_nCnt].Description); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Description"),adWChar,adParamInput,SIZE_MAX_DESCRIPTION+1,vDescription));
}

void CGLogDataEventParticipationRate::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("StartDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("EndDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ParticipationCount"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ParticipationRate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Description"));
}