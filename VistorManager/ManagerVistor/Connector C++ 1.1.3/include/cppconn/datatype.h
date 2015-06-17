/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _SQL_DATATYPE_H_
#define _SQL_DATATYPE_H_

namespace sql
{

class DataType
{
	DataType();
public:
	enum {
		UNKNOWN = 0,
		BIT,
		TINYINT,
		SMALLINT,
		MEDIUMINT,
		INTEGER,
		BIGINT,
		REAL,
		DOUBLE,
		DECIMAL,
		NUMERIC,
		CHAR,
		BINARY,
		VARCHAR,
		VARBINARY,
		LONGVARCHAR,
		LONGVARBINARY,
		TIMESTAMP,
		DATE,
		TIME,
		YEAR,
		GEOMETRY,
		ENUM,
		SET,
		SQLNULL
	};
};

} /* namespace */

#endif /* _SQL_DATATYPE_H_ */
