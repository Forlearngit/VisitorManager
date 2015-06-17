#include "StdAfx.h"
#include "DealTime.h"

CDealTime::CDealTime(void)
{

}

CDealTime::~CDealTime(void)
{
}

CDealTime::CDealTime(__time64_t time)
{
	m_time = time;
}

CDealTime::CDealTime(UINT year,UINT month,UINT day,UINT hour,UINT min,UINT sec)
{
	struct tm tmNow;
	tmNow.tm_year = year - 1900;
	tmNow.tm_mon = month;
	tmNow.tm_mday = day;
	tmNow.tm_hour = hour;
	tmNow.tm_min = min;
	tmNow.tm_sec = sec;
	m_time = _mktime64(&tmNow); 
}

CDealTime CDealTime::GetCurrentTime()
{
	struct tm *tmNow;
	__time64_t longTime;
	_time64(&longTime);
	tmNow = LOCALTIME_R(&longTime);
	return CDealTime(longTime);
}


UINT CDealTime::GetYear() 
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_year + 1900;
}

UINT CDealTime::GetMonth() 
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_mon;
}

UINT CDealTime::GetDay()
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_mday;
}

UINT CDealTime::GetHour()
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_hour;
}

UINT CDealTime::GetMin() 
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_min;
}

UINT CDealTime::GetSec() 
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_sec;
}

UINT CDealTime::GetWDay()
{
	struct tm *tmNow;
	_time64(&m_time);
	tmNow = LOCALTIME_R(&m_time);
	return tmNow->tm_wday;
}

bool CDealTime::operator==(CDealTime time) const
{
	return m_time == time.m_time;
}

bool CDealTime::operator!=(CDealTime time) const
{
	return m_time != time.m_time;
}

bool CDealTime::operator<(CDealTime time) const
{
	return m_time < time.m_time;
}

bool CDealTime::operator <=(CDealTime time) const
{
	return m_time <= time.m_time;
}

bool CDealTime::operator >(CDealTime time) const
{
	return m_time > time.m_time;
}

bool CDealTime::operator >=(CDealTime time)const
{
	return m_time >= time.m_time;
}

CDealTime CDealTime::operator +(CDealTime time)
{
	return CDealTime(m_time + time.m_time);
}

CDealTime CDealTime::operator -(CDealTime time)
{
	return CDealTime(m_time - time.m_time);
}

CDealTime& CDealTime::operator +=(CDealTime time)
{
	m_time += time.m_time;
	return *this;
}

CDealTime& CDealTime::operator -=(CDealTime time)
{
	m_time -= time.m_time;
	return *this;
}

std::wstring CDealTime::Format(LPCTSTR format)
{
	std::wstring strResult;
	std::wstring tempFormat(format);
	std::wstring::size_type posY = tempFormat.find(_T("%Y"));
	if(posY != tempFormat.npos)
	{
		UINT year = GetYear();
		wchar_t wtYear[5]={0};
		swprintf_s(wtYear,_T("%d"),year);
//		tempFormat.replace(posY,posY+2,wtYear,4);
		strResult += wtYear;
	}
	wstring::size_type posM = tempFormat.find(_T("%M"));
	if(posM != tempFormat.npos)
	{
		UINT month = GetMonth();
		wchar_t wtMonth[3]={0};
		swprintf_s(wtMonth,_T("%d"),month);
		wstring subStr = tempFormat.substr(posY+2,posM-posY-2);
		strResult += subStr;
		strResult += wtMonth;
//		tempFormat.replace(posM,posM+2,wtMonth,2);
	}
	wstring::size_type posD = tempFormat.find(_T("%D"));
	if(posD != tempFormat.npos)
	{
		UINT day = GetDay();
		wchar_t wtDay[3]={0};
		swprintf_s(wtDay,_T("%d"),day);
//		tempFormat.replace(posD,posD+2,wtDay,2);
		wstring subStr = tempFormat.substr(posM+2,posD-posM-2);
		strResult += subStr;
		strResult += wtDay;
	}
	wstring::size_type posH = tempFormat.find(_T("%H"));
	if(posH != tempFormat.npos)
	{
		UINT hour = GetHour();
		wchar_t wtHour[3]={0};
		swprintf_s(wtHour,_T("%d"),hour);
//		tempFormat.replace(posH,posH+2,wtHour,2);
		wstring subStr = tempFormat.substr(posD+2,posH-posD-2);
		strResult += subStr;
		strResult += wtHour;
	}
	wstring::size_type posMin = tempFormat.find(_T("%m"));
	if(posMin != tempFormat.npos)
	{
		UINT Min = GetMin();
		wchar_t wtMin[3]={0};
		swprintf_s(wtMin,_T("%d"),Min);
		wstring subStr = tempFormat.substr(posH+2,posMin-posH-2);
		strResult += subStr;
		strResult += wtMin;
//		tempFormat.replace(posMin,posMin+2,wtMin,2);
	}
	wstring::size_type posS = tempFormat.find(_T("%S"));
	if(posS != tempFormat.npos)
	{
		UINT Sec = GetSec();
		wchar_t wtSec[3]={0};
		swprintf_s(wtSec,_T("%d"),Sec);
//		tempFormat.replace(posS,posS+2,wtSec,2);
		wstring subStr = tempFormat.substr(posMin+2,posS-posMin-2);
		strResult += subStr;
		strResult += wtSec;
	}
	return strResult;
}