#include "stdafx.h"
#include "DoFile.h"

CDoFile::CDoFile(void)
{
	m_hFile = INVALID_HANDLE_VALUE;
}

CDoFile::~CDoFile(void)
{
//	MyCloseFile();
}

int CDoFile::MyCreateDirectory(const std::wstring& path)
{
	int nerr = -1;
	std::wstring _path = path;
	
	int length = _path.length();
	std::wstring lastStr = _path.substr(length-1,length);
	if(_tcscmp(lastStr.c_str(),_T("\\")) ==0 )
		_path=path.substr(0,length-1);
	if(MyIsFileExit(_path))//文件夹/目录已经存在
		return 0;
	std::wstring parentPath=MyParentPath(_path);
	if(!parentPath.empty())//父目录
	{
		nerr = MyCreateDirectory(parentPath.c_str());//递归创建
		if(0 != nerr)
		{
			return nerr;
		}
	}
	if(CreateDirectory(_path.c_str(),NULL))
		nerr=0;
	return nerr;
}

BOOL CDoFile::MyIsFileExit(const std::wstring& pathname)
{
	int length=pathname.length();

	if(length<=0)
		return FALSE;

	std::wstring _pathname=pathname;

	if(_tcscmp(_pathname.substr(length-1,length).c_str(),_T("\\")) == 0)
		_pathname = _pathname.substr(0,length-1);

	WIN32_FIND_DATA find_data = {0};
	HANDLE handle = FindFirstFile(_pathname.c_str(),&find_data);
	if(handle != INVALID_HANDLE_VALUE)
	{
		if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	FindClose(handle);
}

std::wstring CDoFile::MyParentPath(const std::wstring& path)
{
	if(path.length() < 2) return _T("");

	std::wstring minPath = path;
	int length = minPath.length();
	if(_tcscmp(minPath.substr(length-1,length).c_str(),_T("\\")) == 0)
		minPath = minPath.substr(0,length-1);
// 	int pos = minPath.ReverseFind('\\');
// 	if( pos != -1 )
// 		minPath = minPath.Left( pos );
	std::wstring::size_type pos = minPath.find_last_of('\\');
	if(pos!=minPath.npos)
		minPath = minPath.substr(0,pos);
	return minPath;
}

BOOL CDoFile::MyDeleteDirectory(const std::wstring& path)
{
//	CFileFind tempFind;//搜索变量
//	char tempFileFind[MAX_PATH]={0};//格式化变量字符
	std::wstring DirName=path;
	if(!DirName.empty() && _tcscmp(DirName.substr(DirName.length()-1,DirName.length()).c_str(),_T("\\")) == 0)
		DirName=DirName.substr(0,DirName.length()-1);

	std::wstring tempDirName = DirName; //+ _T("\\*.*");
	tempDirName.append(_T("\\*.*"));
//	BOOL IsFind=tempFind.FindFile(w_tempFileFind);
	WIN32_FIND_DATA find_data = {0};
	HANDLE fileHandle = FindFirstFile(tempDirName.c_str(),&find_data);
	if(fileHandle!=INVALID_HANDLE_VALUE)
	{
		WIN32_FIND_DATA find_data_next={0};
		//此函数用于遍历目录或文件时，判断当前目录下是否有下一个目录或文件
		while(FindNextFile(fileHandle,&find_data_next))
		{
//			CString FoundFileName=tempFind.GetFileName();//找到的目录的名字
			std::wstring FoundFileName = find_data_next.cFileName;
			if(wcscmp(FoundFileName.c_str(),_T(".."))==0 || wcscmp(FoundFileName.c_str(),_T("."))==0)
				continue;
			if(find_data_next.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )//如果是目录
			{
				std::wstring AddFoundFileName=DirName;
				AddFoundFileName.append(_T("\\"));
				AddFoundFileName += FoundFileName;
				MyDeleteDirectory(AddFoundFileName);//递归调用,当不是目录或者没找到exit
			}
			else
			{
				//是文件直接删除
				std::wstring AddFounFilePath=DirName;
				AddFounFilePath.append(_T("\\"));
				AddFounFilePath+=FoundFileName;
				DeleteFile(AddFounFilePath.c_str());//删除目录下文件
			}
		}
	}
	FindClose(fileHandle);
	//删除目录
	if(!RemoveDirectory(DirName.c_str()))
	{
		return FALSE;
	}
	return TRUE;
}

std::wstring CDoFile::MyGetExeFullFilePath()
{
	TCHAR lpPath[MAX_PATH]=TEXT("");
	DWORD dwResult=GetModuleFileName(NULL,lpPath,MAX_PATH);
	if(dwResult==0)return _T("");
	for(int i=dwResult;i>0;i--)
	{
		if(lpPath[i]=='\\'){lpPath[i]=0;break;}
	}
	m_strPath=lpPath;
	return m_strPath;
}


BOOL CDoFile::MyOpenFile(std::wstring fileName,UINT nOpenFlags)
{
	ASSERT((nOpenFlags & typeText) == 0);
	ASSERT(m_hFile == INVALID_HANDLE_VALUE);
	nOpenFlags &= ~(UINT)typeBinary;
	m_hFile = INVALID_HANDLE_VALUE;
	m_strFileName.empty();
	if(!fileName.empty())
		m_strFileName = fileName;
	else
		return FALSE;
	ASSERT(shareCompat == 0);
	ASSERT((modeRead|modeWrite|modeReadWrite) == 3);
	//访问文件的形式
	DWORD dwAccess = 0;
	switch (nOpenFlags & 3)
	{
	case modeRead:
		dwAccess = GENERIC_READ;
		break;
	case modeWrite:
		dwAccess = GENERIC_WRITE;
		break;
	case modeReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	default:
		ASSERT(FALSE);
	}
	//文件共享
	DWORD dwShareMode = 0;
	switch (nOpenFlags & 0x70)    // map compatibility mode to exclusive
	{
	default:
		ASSERT(FALSE);  // invalid share mode?
	case shareCompat:
	case shareExclusive:
		dwShareMode = 0;
		break;
	case shareDenyWrite:
		dwShareMode = FILE_SHARE_READ;
		break;
	case shareDenyRead:
		dwShareMode = FILE_SHARE_WRITE;
		break;
	case shareDenyNone:
		dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
		break;
	}
	//文件 SECURITY_ATTRIBUTES
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

	// map creation flags
	DWORD dwCreateFlag;
	if (nOpenFlags & modeCreate)
	{
		if (nOpenFlags & modeNoTruncate)
			dwCreateFlag = OPEN_ALWAYS;
		else
			dwCreateFlag = CREATE_ALWAYS;
	}
	else
		dwCreateFlag = OPEN_EXISTING;

	ASSERT((nOpenFlags&(osRandomAccess|osSequentialScan)) != (osRandomAccess|
		osSequentialScan) );

	DWORD dwFlags = FILE_ATTRIBUTE_NORMAL;
	if (nOpenFlags & osNoBuffer)
		dwFlags |= FILE_FLAG_NO_BUFFERING;
	if (nOpenFlags & osWriteThrough)
		dwFlags |= FILE_FLAG_WRITE_THROUGH;
	if (nOpenFlags & osRandomAccess)
		dwFlags |= FILE_FLAG_RANDOM_ACCESS;
	if (nOpenFlags & osSequentialScan)
		dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;
	//创建
	HANDLE hFile = ::CreateFile(fileName.c_str(), dwAccess, dwShareMode, &sa,
		dwCreateFlag, dwFlags, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{	
		return FALSE;
	}
	m_hFile = hFile;
	return TRUE;
}

BOOL CDoFile::MyCloseFile()
{
	BOOL flag = FALSE;
	if(m_hFile!=INVALID_HANDLE_VALUE)
		if(S_OK == CloseHandle(m_hFile))
			flag = TRUE;
	m_hFile = INVALID_HANDLE_VALUE;
	m_strFileName = _T("");
	return flag;
}

UINT CDoFile::MyReadFile(void* lpBuf,UINT nCount)
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);

	if (nCount == 0)
		return 0;
	ASSERT(lpBuf!=NULL);
	DWORD dwRead;
	if(!::ReadFile(m_hFile,lpBuf,(DWORD)nCount,&dwRead,NULL))
		return 0;
	return (UINT)dwRead;
}

BOOL CDoFile::MyWriteFile(const void* lpBuf,UINT nCount)
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
	ASSERT(nCount != 0);
	ASSERT(lpBuf != NULL);
	DWORD dwWrited;
	return WriteFile(m_hFile,lpBuf,nCount,&dwWrited,NULL);
	//注意没有采用异常机制，如果WriteFile出现错误，请用GetLastError获取错误码
}

ULONGLONG CDoFile::MySeekFile(LONGLONG lOff, UINT nFrom)
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
	ASSERT(nFrom == begin || nFrom == end || nFrom == current);
	ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);
	
	LARGE_INTEGER dlibMove;
	dlibMove.QuadPart = lOff;
	dlibMove.LowPart = ::SetFilePointer(m_hFile, dlibMove.LowPart, &dlibMove.HighPart,
		(DWORD)nFrom);
	if (dlibMove.LowPart  == INVALID_SET_FILE_POINTER)
		if (::GetLastError() != NO_ERROR)
			return ::GetLastError();
	return dlibMove.QuadPart;
}

ULONGLONG CDoFile::MyFileGetPosition()const
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
	LARGE_INTEGER liPos;
	liPos.QuadPart = 0;
	liPos.LowPart = ::SetFilePointer(m_hFile, liPos.LowPart, &liPos.HighPart , FILE_CURRENT);
	if (liPos.LowPart == INVALID_SET_FILE_POINTER)
		if (::GetLastError() != NO_ERROR)
			return ::GetLastError();

	return liPos.QuadPart;
}

BOOL CDoFile::MyFileFlush()
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	if (!::FlushFileBuffers(m_hFile))
		return FALSE;
	return TRUE;
}

ULONGLONG CDoFile::MyFileGetLength()const
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
	ULARGE_INTEGER liSize;
	liSize.LowPart = ::GetFileSize(m_hFile, &liSize.HighPart);
	if (liSize.LowPart == INVALID_FILE_SIZE)
		if (::GetLastError() != NO_ERROR)
			return ::GetLastError();
	return liSize.QuadPart;
}

BOOL CDoFile::MyFileSetLength(ULONGLONG newLength)
{
	ASSERT(m_hFile != INVALID_HANDLE_VALUE);
	MySeekFile(newLength,(UINT)begin);//先设置文件指针
	if(!::SetEndOfFile(m_hFile))//Sets the physical file size for the specified file to the current position of the file pointer.
		return FALSE;
	return TRUE;
}

void CDoFile::MyFileSeekToBegin()
{
	MySeekFile(0,begin);
}

void CDoFile::MyFileSeekToEnd()
{
	MySeekFile(0,end);
}