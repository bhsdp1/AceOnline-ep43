// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

#include <Windows.h>
#include <Shlwapi.h>

/*
"예외시호출할함수"를 SetUnhandledExceptionFilter함수로 등록 후 "예외시호출할함수"안에서
Exception::CDarkCrash::OnPlayMiniDump( pException ) 를 호출 해 주면 된다.
그리고, 유저 이름은 게임에 접속하고 틈이 날 때 한번 해주자.

LONG WINAPI 예외시호출할함수(_EXCEPTION_POINTERS *pException)
{
	Exception::CDarkCrash::OnPlayMiniDump( pException );

	char* strBuf[1024] = {0, };
	Exception::CDarkCrash::OutInfo( strBuf );
	return 0;
}

void main()
{
	::SetUnhandledExceptionFilter( 예외시호출할함수 )
	SetUserName("hsson");
}
*/

namespace Exception
{
	class CDarkCrash
	{
        public: 
		static bool SetUserName( char* _pUserName );
		static bool OnPlayMiniDump( _EXCEPTION_POINTERS *pException );
		static bool OutInfo( char* _val );
     };
}

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송