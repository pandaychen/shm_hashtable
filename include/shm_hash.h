/*
	*filename:shm_hash.h
	*filedesc:functions of shmhash
	*create date:2015-10-01
	*author:pandaychen
*/

#ifndef __SHM_HASHFUN_H
#define __SHM_HASHFUN_H

#include "shm_struct.h"
#include "comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */ 

	void *GetPosition(ShmHashManageNode *t_pShmHashManageNode, const uint32_t t_uRow, const uint32_t t_uCol);

	int32_t ShmHashFind(ShmHashManageNode *t_pstShmHashManageNode, const uint64_t t_uHashKey);

	int32_t ShmHashInert(ShmHashManageNode *t_pstShmHashManageNode, uint64_t t_uKey, uint8_t *t_pValue, uint32_t t_uValueSize);

	int32_t ShmHashQuery(ShmHashManageNode *t_pstShmHashManageNode, uint64_t t_uHashKey, uint8_t *pStoreData, uint32_t t_uDataSize);
	
	int32_t ShmHashInit(ShmHashManageNode *t_pShmHashManageNode, ShmHashHeader *t_pShmHashHeader, uint32_t t_uShmKey, uint32_t t_uLines, const uint32_t *t_pPrimerTable);

	double GetCurrentStorageRate(ShmHashManageNode *t_pShmHashManageNode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */ 


#endif
