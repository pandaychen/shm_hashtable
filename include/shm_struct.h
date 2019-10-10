/*
	*filename:shm_struct.h
	*filedesc:
	*create date:2015-10-01
	*author:pandaychen
*/

#ifndef __SHM_HASHSTRUCT_H
#define __SHM_HASHSTRUCT_H

#include <stdint.h>
#include "util.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

uint32_t g_uInitSign;

enum COMMON{
	SHM_DATA_MAX_SIZE = 2000,
	SHM_HASH_MAX_LINES = 32,
	SHM_DUMPER_PATH_SIZE = 128,
	PRIMER_TABLE_LEN = 64,
	SHM_KEY_FACTOR = 5381,
};

enum HASH_OPER_STATUS{
	HASHITEM_NOTFOUND = 1,
	HASHITEM_FOUND = 2,

	HASHITEM_INSERT_SUCC = 3,

	HASHITEM_INSERT_KEY_EXISTS = 4,
	HASHITEM_INSERT_OUTOFMEM = 5,
	HASHITEM_INSET_ERROR,
};

enum SHM_HASH_STATUS{
	HASH_STATUS_NORMAL = 0,
};

const int BIG_PRIMER_TABLE[] = {
	1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693,
	1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879,
	1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999
};


struct __shm_hash_node
{
	uint64_t uKey;	//hash key
	//ValueType stValue;
	uint32_t uKeySize;	//+'\0'
	uint32_t uValueSize;	
	uint8_t stStore[SHM_DATA_MAX_SIZE];	//key --- '\0' --- value
};

typedef struct __shm_hash_node ShmHashNode;

//the pointer of the shm begin
struct __shm_hash_header{
	uint64_t uCurrentSize;
	uint64_t uStatus;
	uint64_t uReserver1;
	uint64_t uReserver2;
};

typedef struct __shm_hash_header ShmHashHeader;

//这是一个内存结构!
struct __shm_hash_manage_node {
	//ShmHashHeader stShmHashHeader;	//FIX WRITE SHM BUGS!
	ShmHashHeader *ptrShmHashHeader; 
	void *pMemInfo;	//pointer to actual mem
	uint64_t uCurSize;
	uint32_t uShmHashLines;
	uint64_t uMemSize;	//size of the share mem
	uint64_t uPrimerTotalTable[SHM_HASH_MAX_LINES];
	uint64_t uMaxTableSize;	//size of table
	void *pLastFoundNode;
	uint32_t uShmKey;
	char stDumperPath[SHM_DUMPER_PATH_SIZE];
	uint32_t szOriPrimerTable[SHM_HASH_MAX_LINES*2];
	uint32_t uOriPrimerTableSize;
};

typedef struct  __shm_hash_manage_node ShmHashManageNode;

#endif
