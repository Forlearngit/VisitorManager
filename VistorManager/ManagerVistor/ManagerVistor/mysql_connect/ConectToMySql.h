#pragma once

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string>
using namespace std;

class CConectToMySql
{
public:
	CConectToMySql(void);
	~CConectToMySql(void);
private:
	sql::Driver* m_pSqlDriver;
	//	std::auto_ptr<sql::Connection> m_sqlCon;
	sql::Connection* m_pSqlCon;
	//	std::auto_ptr<sql::Statement> m_sqlStatement;
	sql::Statement* m_pSqlState;
	//	std::auto_ptr<sql::ResultSet> m_sqlRes;
public:
	BOOL ConnectToDB(const std::string& IP,USHORT port,const std::string& UserName,
		const std::string& passWord);//链接数据库
public:
	BOOL Execute(const std::string& sql);//执行语句不返回数据集
	std::auto_ptr<sql::ResultSet> ExecuteQuery(const std::string& sql);//执行sql语句返回数据集
	//注意判断执行语句出现异常智能指针为空的情形
public:
	BOOL IsConnect();
};

