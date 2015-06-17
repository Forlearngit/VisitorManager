#pragma once
#include <string>
using namespace std;
class CWriteLogError
{
public:
	CWriteLogError(void);
	~CWriteLogError(void);
	void WriteErrLog(const std::wstring strSockLog);
private:
	std::wstring m_strLogFilePath;
	void WriteLogWithTime(const std::wstring& strSockLog);
	BOOL AppendWriteFile(const std::wstring& strText,const std::wstring& strFileName);
};
