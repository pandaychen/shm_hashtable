/*
	*filename:comm.h
	*filedesc:common define
	*create date:2015-10-01
	*author:pandaychen
*/

#ifndef __COMMON_H
#define __COMMON_H

#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <error.h>
#include <stdlib.h>
#include "util.h"
#include <time.h>

#define LOGNAME "logger"

enum COMMON_VAR
{
	RET_WRONG = 0,
	RET_RIGHT = 1,
};

enum LOG_TYPE{
	INFO_LEVEL = 1,
	ERROR_LEVEL = 2,
};

#endif
