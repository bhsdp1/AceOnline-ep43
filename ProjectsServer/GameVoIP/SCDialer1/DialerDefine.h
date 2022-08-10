#ifndef __DIALERDEFINE__H__
#define __DIALERDEFINE__H__
// coded by palbang //////////////////
// 환경 변수
// 상태창 시작점, 폭, 길이
#define STATUS_X	10
#define STATUS_Y	45
#define STATUS_W	140
// 전체 텍스트 높이
#define TEXT_H		17
// 시간 표시창 시작점
#define TIME_X		70
#define TIME_Y		8
// 안테나 시작점, 폭, 높이
#define	ANTENA_X	2
#define	ANTENA_Y	1
#define ANTENA_W	36
#define ANTENA_H	22
// 전화 번호 입력 시작점, 폭
#define	PHONE_X		8
#define	PHONE_Y		29
#define PHONE_W		144  //??
// 숫자, 문자 하나 길이
#define NUMBER_W	12
// 쿠폰 정보 좌표
//#define COUPON_X	96
//#define COUPON_Y	52
//#define COUPON_W	35
// 폰북버튼 위치
//#define BOOK_X		8
//#define BOOK_Y		52
///////end of added code////////////////////////

#define PRODUCTID	0
#define MAXDATALENGTH 80

enum MessageType 
{
	_ERROR = 0x10, 
	_LOGIN, 
	_OK, 
	_DIAL, 
	_ALERT, //20
	_STARTRTP, 
	_CONNECTED, 
	_HANGUP, 
	_BUTTON, 
	_COMPLETE, //25
	_ALIVE, 
	_REGISTRATION, 
	_RETRIEVE, 
	_REGOK, 
	_ROUTERTP, //30
	_ROUTEOK, 
	_REQPROXY, //32
	_RESPROXY, 
	_NOPROXY, 
	_PHONEBOOK, 
	_PREREGISTRATION/*PCN;PWD 구조가 너무 길때 PWD 먼저보냄*/, 
	_SIGNUP, 
	_SEARCHUSER = 0x26, //38
	_MYSTATUS,
	_FORWARDING, //40
	_VIDEOMSG,
	_NOTICE,
	_TEXT,
	_REREGISTRATION,  // 강제 로그아웃 후에 다시 로그인
	_GETRATE, //45
	_MONEY,
	_UPDATE,
	_NONFASTSTART,
	_ASK_ACTIVE_CONFIRM, //by cje 021112
	_RETRY_ACTIVE_CONFIRM, //50
	_ACTIVE_CONFIRM_OK,
	_PASSIVE_START,
	_VIDEOPAUSE,
	_VOICECODEC = 54,
	_VIDEOCODEC = 55,
	_REQMAKECONFERENCE,
	_RESMAKECONFERENCE, 
	_REQJOINCONFERENCE,
	_RESJOINCONFERENCE,
	_REQINVITECONFERENCE = 60,
	_RESINVITECONFERENCE,
	_STATUSCONFERENCE,
	_LEAVECONFERENCE, 
	_CLOSECONFERENCE
};
enum ProgressType {_SIGNUPING = 0, _MODIFYING, _PHONEBOOKING, _BILLING, /*_FORWARDING, */_LOGINING, _DIALING, _SEARCHUSERING};
//enum {INVALID_PRODUCT = 1, INVALID_ID, INVALID_NUMBER, NETWORK_ERROR, NO_FREE_CHANNEL, OVER_TIME, NO_ANSWER, BUSY, REFUSE, INVALID_PWD};
enum ErrorInfos {
	INVALID_PRODUCT = 1, 
	INVALID_ID, // SCDialer1 --> Invalid PCN
	INVALID_NUMBER,  // WebCallient + SCDialer1 --> Invalid Number
	NETWORK_ERROR, // WebCallient + SCDialer1 --> Server error
	NO_FREE_CHANNEL,
	OVER_TIME, // WebCallient + SCDialer1 --> Server error
	NOBANDWIDTH, // WebCallient --> Invalid Number ?
	GATEKEEPERRESOURCE, 
	UNREACHABLEDESTINATION, 
	DESTINATIONREJECTION = 10, 
	INVALIDREVISION, 
	NOPERMISION, 
	UNREACHABLEGATEKEEPER,
	GATEWAYRESOURCE, 
	BADFORMATADDRESS, // WebCallient + SCDialer1 --> Invalid Number
	ADAPTIVEBUSY, 
	INCONF = 17, // WebCallient + SCDialer1 --> Busy.. Try again
	UNDEFINEREASON, 
	ROUTECALLTOGATEKEEPER, 
	CALLFORWARDED = 20, 
	ROUTECALLTOMC,
	FACILITYCALLDEFLECTION, 
	SECURITYDENIED, // PC2PC 에서는 일시 사용중지
	CALLEDPARTYNOTREGISTERED, 
	CALLERNOTREGISTERED, 
	NEWCONNECTIONNEEDED, 
	NOH245, 
	CONFERENCELISTCHOICE, 
	STARTH245,
	NO_ANSWER = 30, // SCDialer1 에서 상대방이 접속되어 있지 않을때 --> No Answer
	REFUSE = 31, // SCDialer1 에서 상대방이 접속되어 있지만 전화받기를 거부할 때 --> Refuse
	INVALID_PWD = 32, // SCDialer1 에서 password 가 틀렸을 때.. --> Invalid PWD
	DUPLICATEPCN,
	DBERROR,
	NOTLOGIN,
	LOGINSOMEWHERE, // 다른 곳에서 로그인을 해왔다는 Error msg
	INSUFFICIENTBALANCE,
	DBPROTOCOLERROR,
	CONFERENCEDISABLE, // 컨퍼런스 기능이 활성화 되어 있지 않음
	CONFERENCENOPERMISSIONTOMAKE = 40, // 컨퍼런스를 만들 권한이 없음
	CONFERENCENOPERMISSIONTOJOIN, // 컨퍼런스에 참가할 권한이 없음
	CONFERENCENOBANDWIDTHTOJOIN = 42, // 컨퍼런스에 참가할 공간이 없음
	CONFERENCENOCONFERENCEID, // 요청한 컨퍼런스 id 가 없음
	CONFERENCENOSERVERINFO, // 컨퍼런스에 참가할 FireTalk server 가 없음
	CONFERENCEINVITENOUSERID, // 초대한 사용자가 없음
	CONFERENCEINVITENOPERMISSION, // 초대한 사용자가 Conference permission 이 없음
	SERIAL_DEVICE_USEING	= 50,
	SERIAL_INVALID_PORT		= 51,
	SERIAL_INVALID_DEVICE	= 52,
	NOTSUPPROTED_VOICECODEC = 53,
    NOTSUPPROTED_VIDEOCODEC,
    NOTSUPPORTED_VIDOSIZE
};
typedef struct {
	MessageType Type;
	union {
		struct SIGNUP_MSG {
			unsigned long step; // 0 : pcn;pwd, 1 : e-mail;name, 2 : pwd modify, 3 : e-mail modify
			char UserInfo[MAXDATALENGTH];
		} signupmsg; // 
		struct SERCHUSER_MSG {
			unsigned long type;
			char UserInfo[MAXDATALENGTH];
		} searchusermsg; // signup 시 duplicate 검사에도 이용
		struct REGISTRATION_MSG {
			unsigned long version;
			char UserPhoneNumber[MAXDATALENGTH];
		} registrationmsg;
		struct UPDATE_MSG {
			unsigned long version;
			char Url[MAXDATALENGTH];
		} updatemsg;
		struct LOGIN_MSG {
			unsigned long Product;
			char UserID[MAXDATALENGTH];
		} loginmsg;
		struct DIAL_MSG {
			unsigned long RtpPort;
			char PhoneNumber[MAXDATALENGTH];  // Caller IP Addr in SCDialer1 
		} dialmsg;
		struct STARTRTP_MSG {  
			unsigned long RtpPort;
			char RtpAddr[MAXDATALENGTH];  
		} startrtpmsg;  
		struct ROUTE_MSG {
			unsigned long ProxyServerPort;
			char ProxyServerIP[MAXDATALENGTH];
		} routemsg;
		struct PHONEBOOK_MSG {
			unsigned long count;
			char PhoneBook[MAXDATALENGTH];
		} phonebookmsg;
		struct MYSTATUS_MSG {
			unsigned long status;
			char MyStatus[MAXDATALENGTH];
		} mystatusmsg;
		struct FORWARDING_MSG {
			unsigned long type;
			char Forwarding[MAXDATALENGTH];
		} forwardingmsg;
		struct VIDEO_MSG {
			unsigned long VideoPort;
			char VideoIP[MAXDATALENGTH];
		} videomsg;
		struct NOTICE_MSG {
			unsigned long Type;
			char Message[MAXDATALENGTH];
		} noticemsg;
		struct TEXT_MSG {
			unsigned long length;
			char Message[MAXDATALENGTH];
		} textmsg;
		struct NONFASTSTART_MSG {
			unsigned long RtpPort;
			char RTPAddr[MAXDATALENGTH];
		} nonfaststartmsg;		
		struct VIDEOPAUSE_MSG {
			unsigned long Flag;
			char Pause[MAXDATALENGTH];
		} videopausemsg;
		struct MONEY_MSG {
			unsigned long Interval;
			char Money[MAXDATALENGTH];
		} moneymsg;
		struct REQMAKECONFERENCE_MSG {
			unsigned AttendantCount;
			char MakeConferenceInfo[MAXDATALENGTH];
		} reqmakeconferencemsg;
		struct RESMAKECONFERENCE_MSG {
			unsigned FireTalkPort;
			char FireTalkIP[MAXDATALENGTH];
		} resmakeconferencemsg;
		struct REQJOINCONFERENCE_MSG {
			unsigned JoinData;
			char JoinRoomID[MAXDATALENGTH];
		} reqjoinconferencemsg;
		struct RESJOINCONFERENCE_MSG {
			unsigned Permission;
			char JoinConferenceInfo[MAXDATALENGTH];
		} resjoinconferencemsg;
		struct REQINVITECONFERENCE_MSG {
			unsigned ReqInviteData;
			char InviteUserID[MAXDATALENGTH];
		} reqinviteconferencemsg;
		struct RESINVITECONFERENCE_MSG {
			unsigned Accept;
			char InviteUserID[MAXDATALENGTH];
		} resinviteconferencemsg;
		struct STATUSCONFERENCE_MSG {
			unsigned Status;
			char StatusInfo[MAXDATALENGTH];
		} statusconferencemsg;
		struct LEAVECONFERENCE_MSG {
			unsigned LeaveConferenceData;
			char LeaveConferenceInfo[MAXDATALENGTH];
		} leaveconferencemsg;
		struct CLOSECONFERENCE_MSG {
			unsigned CloseConferenceData;
			char CloseConferenceInfo[MAXDATALENGTH];
		} closeconferencemsg;
		struct VOICECODEC_MSG{
			unsigned long voicecodec;
			char voicecodecinfo[MAXDATALENGTH];
		} voicecodecmsg;
        struct VIDEOCODEC_MSG{
			unsigned long videocodec;
			char videocodecinfo[MAXDATALENGTH];
        } videocodecmsg;
		short ErrorInfo; /*{INVALID_PRODUCT = 1, INVALID_ID, INVALID_NUMBER, NETWORK_ERROR, NO_FREE_CHANNEL, OVER_TIME}*/
		char Button[MAXDATALENGTH]; // phone button 'type : string' 1,2,3,4,5...,*,#
	} Data;
} MESSAGE, *PMESSAGE;

typedef struct {
	BOOL bSet;
	unsigned int MediaPort;
} MEDIAPORTINFO;

enum clientSate 
{ 
	_READYSTATE		= 0,
	_DIALBLOCK		= 1,
	_ALERTINGSTATE	= 2,
	_DIALINGSTATE	= 3,
	_ERRORSTATE		= 4
};

enum AuthType { _PCN = 0, _ID, _EMAIL, _NAME, _UNKNOWN };
//enum CallNetWorkType { _INTERNAL =0 , _EXTERNAL}; //by cje 011206
enum CallNetworkType {
			_INTERNAL = 0x00,	
			_OPENINTERNET ,
			_FULLCONE,
			_RESTRICTED,
			_UDPBLOCK,
			_NOACCESS,
			_NOINFO= 0x06
};
enum RTPTRANSFERTYPE {
			_NORMAL = 0x00,
			_ACTIVE,
			_PASSIVE = 0x02
};
#ifdef _SCDialer1
enum CallMode { SCDialer1, PC2PHONE };
#endif // _SCDialer1

#define UM_MSGSOCKET		WM_USER+100
#define UM_SOKET_ERROR		WM_USER+101
#define UM_WCASOCKET		WM_USER+102

#endif
