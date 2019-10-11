/*
	*filename:util.cpp
	*filedesc:general str hash function
	*create date:2015-10-01
	*author:pandaychen
*/


#include "util.h"
#include "comm.h"
#include <time.h>


char *GetDateTimeStrHere(const time_t *mytime)
{
	static char s[50];
	struct tm curr = *localtime(mytime);

	if (curr.tm_year > 50)
		sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
		curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday,
		curr.tm_hour, curr.tm_min, curr.tm_sec);
	else
		sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
		curr.tm_year + 2000, curr.tm_mon + 1, curr.tm_mday,
		curr.tm_hour, curr.tm_min, curr.tm_sec);

	return s;
}

char *GetCurDateTimeStrHere(void)
{
	time_t mytime = time(NULL);

	return GetDateTimeStrHere(&mytime);
}


char *GetShortDateStrHere(const time_t *mytime)
{
	static char s[50];

	struct tm curr = *localtime(mytime);
	if (curr.tm_year > 50)
		sprintf(s, "%04d%02d%02d",
		curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday);
	else
		sprintf(s, "%04d%02d%02d",
		curr.tm_year + 2000, curr.tm_mon + 1, curr.tm_mday);

	return s;
}

char *GetCurShortDateStrHere(void)
{
	time_t mytime = time(NULL);

	return GetShortDateStrHere(&mytime);
}

int32_t LoggingInfo(int32_t t_iType,const char *t_pFilenamePart, const char *t_pLog, int32_t t_iLine, const char *t_pFun, const char *t_pFile){
	//write local log
	char szFilename[128] = { 0 };
	char szLog[2048] = { 0 };

	if (t_iType == ERROR_LEVEL){
		snprintf(szFilename, 128, "./log/error_%s.log", GetCurShortDateStrHere());
		snprintf(szLog, 2048, "[%s][ERROR]%s:%d[%s]:%s\n", GetCurDateTimeStrHere(), t_pFile, t_iLine, t_pFun, t_pLog);
	}
	else if (t_iType == INFO_LEVEL){
		snprintf(szFilename, 128, "./log/info_%s.log", GetCurShortDateStrHere());
		snprintf(szLog, 2048, "[%s][INFO]%s:%d[%s]:%s\n", GetCurDateTimeStrHere(), t_pFile, t_iLine, t_pFun, t_pLog);
	}
	else{
		return RET_WRONG;
	}

	if (strlen(szLog)){
		FILE *pFile = fopen(szFilename, "a+");
		if (pFile){
			fputs(szLog, pFile);
			fflush(pFile);
			fclose(pFile);
		}
	}
	return RET_RIGHT;
}
