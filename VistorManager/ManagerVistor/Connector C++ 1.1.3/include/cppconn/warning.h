/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _SQL_WARNING_H_
#define _SQL_WARNING_H_


#include <stdexcept>
#include <string>
#include <memory>
#include "sqlstring.h"

namespace sql
{

#ifdef _WIN32
#pragma warning (disable : 4290)
//warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#endif

class SQLWarning
{
public:

	SQLWarning(){}

	virtual const sql::SQLString & getMessage() const = 0;

	virtual const sql::SQLString & getSQLState() const = 0;

	virtual int getErrorCode() const = 0;

	virtual const SQLWarning * getNextWarning() const = 0;

	virtual void setNextWarning(const SQLWarning * _next) = 0;

protected:

	virtual ~SQLWarning(){};

	SQLWarning(const SQLWarning& e){};

private:
	const SQLWarning & operator = (const SQLWarning & rhs);

};


} /* namespace sql */

#endif /* _SQL_WARNING_H_ */
