/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/




#ifndef _SQL_PREPARED_STATEMENT_H_
#define _SQL_PREPARED_STATEMENT_H_

#include <iostream>
#include "statement.h"


namespace sql
{

class Connection;
class ResultSet;
class ResultSetMetaData;
class ParameterMetaData;

class PreparedStatement : public Statement
{
public:
	virtual ~PreparedStatement() {}

	virtual void clearParameters() = 0;

	virtual bool execute(const sql::SQLString& sql) = 0;
	virtual bool execute() = 0;

	virtual ResultSet *executeQuery(const sql::SQLString& sql) = 0;
	virtual ResultSet *executeQuery() = 0;

	virtual int executeUpdate(const sql::SQLString& sql) = 0;
	virtual int executeUpdate() = 0;

	virtual ResultSetMetaData * getMetaData() = 0;

	virtual ParameterMetaData * getParameterMetaData() = 0;

	virtual void setBigInt(unsigned int parameterIndex, const sql::SQLString& value) = 0;

	virtual void setBlob(unsigned int parameterIndex, std::istream * blob) = 0;

	virtual void setBoolean(unsigned int parameterIndex, bool value) = 0;

	virtual void setDateTime(unsigned int parameterIndex, const sql::SQLString& value) = 0;

	virtual void setDouble(unsigned int parameterIndex, double value) = 0;

	virtual void setInt(unsigned int parameterIndex, int32_t value) = 0;

	virtual void setUInt(unsigned int parameterIndex, uint32_t value) = 0;

	virtual void setInt64(unsigned int parameterIndex, int64_t value) = 0;

	virtual void setUInt64(unsigned int parameterIndex, uint64_t value) = 0;

	virtual void setNull(unsigned int parameterIndex, int sqlType) = 0;

	virtual void setString(unsigned int parameterIndex, const sql::SQLString& value) = 0;

	virtual PreparedStatement * setResultSetType(sql::ResultSet::enum_type type) = 0;
};


} /* namespace sql */

#endif /* _SQL_PREPARED_STATEMENT_H_ */
