#pragma once
#include <time.h>
#include <string>
#ifdef   WIN32     
#define   LOCALTIME_R(t)     _localtime64((t))     
#else     
#define   LOCALTIME_R(t)     localtime_r((t),   (struct   tm   *)&tmres)     
#endif  
using namespace std;
class CDealTime
{
public:
	CDealTime(void);
	~CDealTime(void);
	CDealTime(__time64_t time);
	CDealTime(UINT year,UINT month,UINT day,UINT hour,UINT min,UINT sec);
	
	bool operator==( CDealTime time ) const ;
	bool operator!=( CDealTime time ) const ;
	bool operator<( CDealTime time ) const ;
	bool operator>( CDealTime time ) const ;
	bool operator<=( CDealTime time ) const ;
	bool operator>=( CDealTime time ) const ;
	
	CDealTime& operator+=(CDealTime time);
	CDealTime& operator-=(CDealTime time);
	CDealTime operator+(CDealTime time);
	CDealTime operator-(CDealTime time);
	std::wstring Format(LPCTSTR format);
public:
	static CDealTime GetCurrentTime();
 	UINT GetYear();
 	UINT GetMonth();
 	UINT GetDay();
 	UINT GetHour();
 	UINT GetMin();
 	UINT GetSec();
	UINT GetWDay();//ÐÇÆÚ¼¸
private:
	__time64_t m_time;
};
