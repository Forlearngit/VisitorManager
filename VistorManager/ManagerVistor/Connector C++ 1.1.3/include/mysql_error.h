/*
   Copyright (c) 2013, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _MYSQL_ERROR_H_
#define _MYSQL_ERROR_H_

namespace sql
{
namespace mysql
{
	/* Driver specific errors */
	enum DRIVER_ERROR {
		/* Underlying client library(cl) can't deal with expired password.
		   Raised when password actually expires */
		deCL_CANT_HANDLE_EXP_PWD= 820
	};
} /* namespace mysql */
} /* namespace sql */

#endif /* _MYSQL_ERROR_H_ */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
