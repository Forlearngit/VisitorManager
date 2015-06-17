/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _SQL_STATEMENT_H_
#define _SQL_STATEMENT_H_

#include "config.h"
#include "resultset.h"

#include <string>

namespace sql
{

class ResultSet;
class Connection;
class SQLWarning;


class Statement
{
public:
	virtual ~Statement() {};

	virtual Connection * getConnection() = 0;

	virtual void cancel() = 0;

	virtual void clearWarnings() = 0;

	virtual void close() = 0;

	virtual bool execute(const sql::SQLString& sql) = 0;

	virtual ResultSet * executeQuery(const sql::SQLString& sql) = 0;

	virtual int executeUpdate(const sql::SQLString& sql) = 0;

	virtual size_t getFetchSize() = 0;

	virtual unsigned int getMaxFieldSize() = 0;

	virtual uint64_t getMaxRows() = 0;

	virtual bool getMoreResults() = 0;

	virtual unsigned int getQueryTimeout() = 0;

	virtual ResultSet * getResultSet() = 0;

	virtual sql::ResultSet::enum_type getResultSetType() = 0;

	virtual uint64_t getUpdateCount() = 0;

	virtual const SQLWarning * getWarnings() = 0;

	virtual void setCursorName(const sql::SQLString & name) = 0;

	virtual void setEscapeProcessing(bool enable) = 0;

	virtual void setFetchSize(size_t rows) = 0;

	virtual void setMaxFieldSize(unsigned int max) = 0;

	virtual void setMaxRows(unsigned int max) = 0;

	virtual void setQueryTimeout(unsigned int seconds) = 0;

	virtual Statement * setResultSetType(sql::ResultSet::enum_type type) = 0;
};

} /* namespace sql */

#endif /* _SQL_STATEMENT_H_ */
