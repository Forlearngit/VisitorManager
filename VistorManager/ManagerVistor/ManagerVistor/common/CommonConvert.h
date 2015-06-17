#pragma once
////用于转换字符串的类
#include <string>
using namespace std;
class CCommonConvert
{
public:
	CCommonConvert(void);
	~CCommonConvert(void);
	static int WStringToChar(const std::wstring str,char c_str[],int length);
	static int CharToWString(std::wstring& str,const char c_str[]);
	static void intToWString(int i,std::wstring& str);
	static void WStringToint(int& i,const std::wstring str);
	static BOOL WChar2Char(char* dstBuf, size_t len, const wchar_t* src);
	static int Char2WChar(const char* source,size_t len,wchar_t* dest);
	static string WStringToString(const std::wstring str);
	static wstring StringToWString(const std::string str);
	//获取本机IP
//	CString GetComputerIP();
	//获取程序执行路径
//	CString GetExeFullFilePath();
	////UTF8转std::string
	static std::string UTF8_To_string(const std::string & str);
	////std::string转utf8字符串
	static std::string string_To_UTF8(const std::string & str); 
};
