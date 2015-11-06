/*
	*filename:util.h
	*filedesc:
	*create date:2015-10-01
	*author:pandaychen
*/

#ifndef __PANDAY_UTIL_H
#define __PANDAY_UTIL_H

#include <stdint.h>
#include <stdio.h>
#include "comm.h"

int32_t LoggingInfo(int32_t t_iType,const char *t_pFilenamePart,const char *t_pLog,int32_t t_iLine,const char *t_pFun,const char *t_pFile);

#define LOG_INFO(str,filename) {LoggingInfo(LOG_INFO_LEVEL,filename,str,__LINE__,__FUNCTION__,__FILE__);}

//#define LOG_INFO(str,filename) {printf("%s %s\n",str,filename);}

#define LOG_ERROR(str,filename) {LoggingInfo(LOG_ERROR_LEVEL,filename,str,__LINE__,__FUNCTION__,__FILE__);}

#endif // !__PANDAY_UTIL_H
