/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _SQL_PARAMETER_METADATA_H_
#define _SQL_PARAMETER_METADATA_H_

#include <cppconn/sqlstring.h>


namespace sql
{

class ParameterMetaData
{
public:
	enum
	{
		parameterModeIn,
		parameterModeInOut,
		parameterModeOut,
		parameterModeUnknown
	};
	enum
	{
		parameterNoNulls,
		parameterNullable,
		parameterNullableUnknown
	};

	virtual sql::SQLString getParameterClassName(unsigned int param) = 0;

	virtual int getParameterCount() = 0;

	virtual int getParameterMode(unsigned int param) = 0;

	virtual int getParameterType(unsigned int param) = 0;

	virtual sql::SQLString getParameterTypeName(unsigned int param) = 0;

	virtual int getPrecision(unsigned int param) = 0;

	virtual int getScale(unsigned int param) = 0;

	virtual int isNullable(unsigned int param) = 0;

	virtual bool isSigned(unsigned int param) = 0;

protected:
	virtual ~ParameterMetaData() {}
};


} /* namespace sql */

#endif /* _SQL_PARAMETER_METADATA_H_ */
