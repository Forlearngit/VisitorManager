/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _SQL_RESULTSET_METADATA_H_
#define _SQL_RESULTSET_METADATA_H_

#include "sqlstring.h"
#include "datatype.h"

namespace sql
{

class ResultSetMetaData
{
public:
	enum
	{
		columnNoNulls,
		columnNullable,
		columnNullableUnknown
	};

	virtual SQLString getCatalogName(unsigned int column) = 0;

	virtual unsigned int getColumnCount() = 0;

	virtual unsigned int getColumnDisplaySize(unsigned int column) = 0;

	virtual SQLString getColumnLabel(unsigned int column) = 0;

	virtual SQLString getColumnName(unsigned int column) = 0;

	virtual int getColumnType(unsigned int column) = 0;

	virtual SQLString getColumnTypeName(unsigned int column) = 0;

	virtual unsigned int getPrecision(unsigned int column) = 0;

	virtual unsigned int getScale(unsigned int column) = 0;

	virtual SQLString getSchemaName(unsigned int column) = 0;

	virtual SQLString getTableName(unsigned int column) = 0;

	virtual bool isAutoIncrement(unsigned int column) = 0;

	virtual bool isCaseSensitive(unsigned int column) = 0;

	virtual bool isCurrency(unsigned int column) = 0;

	virtual bool isDefinitelyWritable(unsigned int column) = 0;

	virtual int isNullable(unsigned int column) = 0;

	virtual bool isNumeric(unsigned int column) = 0;

	virtual bool isReadOnly(unsigned int column) = 0;

	virtual bool isSearchable(unsigned int column) = 0;

	virtual bool isSigned(unsigned int column) = 0;

	virtual bool isWritable(unsigned int column) = 0;

	virtual bool isZerofill(unsigned int column) = 0;

protected:
	virtual ~ResultSetMetaData() {}
};


} /* namespace sql */

#endif /* _SQL_RESULTSET_METADATA_H_ */
