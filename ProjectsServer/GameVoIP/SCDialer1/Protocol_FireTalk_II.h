#ifndef _PROTOCOL_FIRETALK_II_H
#define _PROTOCOL_FIRETALK_II_H
// #include "Protocol_FireTalk_II.h"

typedef enum
{
	_PUBLIC_TYPE	= 0,	// Public IP
	_NAT_CONE		= 1,
//	_NAT_CONE_A		= 1,	// Private IP : 패킷이 나갈때 목적주소가 틀려도 소스주소는 동일
//	_NAT_CONE_B		= 2,	// Private IP : 패킷이 나갈때 목적주소가 틀리면 소스주소도 틀리게 나간다
	_NAT_SYMETRIC	= 3,	// Private IP : 패킷이 들어갈때 소스주소가 클리면 들어갈수 없다.
	_NOTUSE_UDP		= 4,	// UDP 패킷 전송이 되지 않는다.
	_NULL_TYPE		= 9
}NetworkType;

enum NATErrorInfos 
{
	// 컨트롤의 에러 정보
	ERR_NOTHING					= 0,	// 에러 없음
	ERR_GENERAL_POINTER			= 1,	// 컨트롤의 에러 정보
	ERR_CREATE_CHANNEL			= 2,	// 컨트롤의 에러 정보
	ERR_START_CHANNEL			= 3,	// 컨트롤의 에러 정보
	ERR_END_CHANNEL				= 4,	// 컨트롤의 에러 정보

	// 서버의 에러 정보
//	INVALID_PRODUCT				= 10,	// 프로젝트 ID가 틀리다. 
	INVALID_PRODUCT_VERSION		= 11,	// 프로젝트 version이 틀리다
//	INVALID_ID					= 12,	// 
	NOTOPENPORTROUTECREATE		= 13,	// RTP 라우트를위한 UDP 포트를 오픈할수 없다.	

	// 클라이언트의 에러 정보
	ERR_CREATE_OPENPORT			= 30,	// RTP 채널에서 UDP 포트를 오픈 할수 없다.
	ERR_NO_RESPONSE_UDP			= 31,	// FireTalk로 UDP 패킷을 보냈는데 응답이 없다. FireTalk가 죽어있거나 UDP 포트를 확인해야함.
	ERR_No_RESPOnSE_TCP			= 32,	// FireTalk로 TCP 접속을 시도했으나 실패함. FireTalk가 죽어있거나 포트를 확인해야함.	

	// NAT 분석에서의 에러 정보
	ERR_NO_RESPONSE_SERVER		= 41,	// 서버로 NAT 분석을 위해 UDP 패킷을 보냈는데 응답이 없다. 서버가 죽어있거나 UDP 포트를 확인해야함.
	ERR_No_RESPOnSE_FIRETALK	= 42	// FireTalk II로 NAT 분석을 위해 UDP 패킷을 보냈는데 응답이 없다. FireTalk II가 죽어있거나 UDP 포트를 확인해야함.


};


enum UDPMsgType
{
	_ANALYZE_NAT_A			= 1,
	_ANALYZE_NAT_A_M		= 2,
	_ANALYZE_NAT_A_ACK		= 3,
	_ANALYZE_NAT_B			= 4, 
	_ANALYZE_NAT_B_ACK		= 5, 
	_SEEK_PUBLIC_INFO		= 6,
	_SEEK_PUBLIC_INFO_ACK	= 7,
	_ACTIVE_CONFIRM			= 8
};
typedef struct
{
	UDPMsgType type;
	union
	{
		struct NAT_A_MSG
		{				
			char	strRouteIP[16];
			UINT	uRoutePort;
		} natAmsg;			
		struct NAT_AM_MSG
		{				
			char	strRouteIP[16];
			UINT	uRoutePort;
		} natAMmsg;
		struct NAT_AACK_MSG
		{				
			char	strRouteIP[16];
			UINT	uRoutePort;
		} natAackmsg;
		struct NAT_B_ACK_MSG 
		{
			char	strRouteIP[16];
			UINT	uRoutePort;
		} natBackmsg;
		struct SEEKACK_MSG 
		{
			char	strRouteIP[16];
			UINT	uRoutePort;
		} seekackmsg;
	} Data;
}UDPMSG;


#endif