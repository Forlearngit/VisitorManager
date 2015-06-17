/*
   Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.



















   The lines above are intentionally left blank
*/



#ifndef _SQL_BUILD_CONFIG_H_
#define _SQL_BUILD_CONFIG_H_

#ifndef CPPCONN_PUBLIC_FUNC

#if defined(_WIN32)
 // mysqlcppconn_EXPORTS is added by cmake and defined for dynamic lib build only
  #ifdef mysqlcppconn_EXPORTS
    #define CPPCONN_PUBLIC_FUNC __declspec(dllexport)
  #else
    // this is for static build
    #ifdef CPPCONN_LIB_BUILD
      #define CPPCONN_PUBLIC_FUNC
    #else
      // this is for clients using dynamic lib
      #define CPPCONN_PUBLIC_FUNC __declspec(dllimport)
    #endif
  #endif
#else
  #define CPPCONN_PUBLIC_FUNC
#endif

#endif    //#ifndef CPPCONN_PUBLIC_FUNC

#endif    //#ifndef _SQL_BUILD_CONFIG_H_
