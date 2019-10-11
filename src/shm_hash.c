/*
	E*filename:shm_hash.h
	*filedesc:shm hash function
	*create date:2015-10-01
	*author:pandaychen
*/

#include "shm_hash.h"
#include "comm.h"
#include "shm_struct.h"

extern uint32_t g_uInitSign;

//get the current usage
double GetCurrentStorageRate(ShmHashManageNode *t_pShmHashManageNode){
	if (NULL == t_pShmHashManageNode){
		return RET_WRONG;
	}
	uint64_t uCur = t_pShmHashManageNode->ptrShmHashHeader->uCurrentSize;
	uint64_t uTotal = t_pShmHashManageNode->uMaxTableSize;
	
	return (double)(uCur) / (double)(uTotal);
}



//get pos (row,col)
void *GetPosition(ShmHashManageNode *t_pShmHashManageNode, const uint32_t t_uRow, const uint32_t t_uCol){

	//row -- lines num
	//col -- hash%lines[row]

	//get pos from the begin
	uint64_t uPos = t_pShmHashManageNode->uPrimerTotalTable[t_uRow] + t_uCol; 
	if (uPos >= t_pShmHashManageNode->uMaxTableSize + sizeof(ShmHashHeader)){
		return NULL;
	}
	return (void *)(uint64_t)(t_pShmHashManageNode->pMemInfo) + sizeof(ShmHashHeader)+ uPos * sizeof(ShmHashNode);
}


int32_t ShmHashFind(ShmHashManageNode *t_pstShmHashManageNode, const uint64_t t_uHashKey){
	//uint64_t uHash = 0;
	
	ShmHashNode *pstShmHashNode = NULL;
	int32_t iRow = 0;
	int32_t iCol = 0;
	uint32_t uLines = t_pstShmHashManageNode->uShmHashLines;
	for (iRow = 0; iRow < t_pstShmHashManageNode->uShmHashLines; iRow++){
		//find keys in every single row
		iCol = (t_uHashKey + uLines*SHM_KEY_FACTOR) % t_pstShmHashManageNode->szOriPrimerTable[iRow];
		pstShmHashNode = (ShmHashNode *)GetPosition(t_pstShmHashManageNode, iRow, iCol);
		if (NULL == pstShmHashNode){
			break;
		}
		else{
			if (pstShmHashNode->uKey == t_uHashKey){
				//rencently
				t_pstShmHashManageNode->pLastFoundNode = pstShmHashNode;
				return HASHITEM_FOUND;
			}
		}
	}
	return HASHITEM_NOTFOUND;
}


int32_t ShmHashInert(ShmHashManageNode *t_pstShmHashManageNode,uint64_t t_uKey,uint8_t *t_pValue,uint32_t t_uValueSize){
	
	int32_t iRet = ShmHashFind(t_pstShmHashManageNode, t_uKey);
	if (iRet == HASHITEM_FOUND){
		return HASHITEM_INSERT_KEY_EXISTS;
	}

	uint64_t uRow = 0;
	uint64_t uCol = 0;
	uint32_t uLines = t_pstShmHashManageNode->uShmHashLines;
	ShmHashNode *pstShmHashNode = NULL;


	for (uRow = 0; uRow < uLines; uRow++){
		uCol = (t_uKey + uLines *SHM_KEY_FACTOR) % t_pstShmHashManageNode->szOriPrimerTable[uRow];
		pstShmHashNode = (ShmHashNode *)GetPosition(t_pstShmHashManageNode, uRow, uCol);
		if (pstShmHashNode == NULL){
			return HASHITEM_INSERT_OUTOFMEM;
		}
		else
		{
			if (pstShmHashNode->uKey == 0){
				//not use yet
				//add key-_-
				pstShmHashNode->uKey = t_uKey;
				pstShmHashNode->uKeySize = sizeof(uint64_t);	//get rid of '\0'
				pstShmHashNode->uValueSize = t_uValueSize>SHM_DATA_MAX_SIZE-1?SHM_DATA_MAX_SIZE-1:t_uValueSize;
				memcpy(pstShmHashNode->stStore, t_pValue, t_uValueSize > SHM_DATA_MAX_SIZE - 1 ? (SHM_DATA_MAX_SIZE - 1): t_uValueSize);
				t_pstShmHashManageNode->ptrShmHashHeader->uCurrentSize++;
				return HASHITEM_INSERT_SUCC;
			}else{
				//printf("node is used.\n");
			}
		}
	}
	//all the appropriate position filled
	return HASHITEM_INSET_ERROR;
}

int32_t ShmHashQuery(ShmHashManageNode *t_pstShmHashManageNode, uint64_t t_uHashKey,uint8_t *pStoreData,uint32_t t_uDataSize){
	//
	int32_t iRet = ShmHashFind(t_pstShmHashManageNode, t_uHashKey);
	if (iRet != HASHITEM_FOUND){
		return HASHITEM_NOTFOUND;
	}

	//pStoreData
	//t_uDataSize
	ShmHashNode *pActualBuff=(ShmHashNode *)t_pstShmHashManageNode->pLastFoundNode;
	uint32_t uDataActSize = pActualBuff->uValueSize;
	memcpy(pStoreData, pActualBuff->stStore, t_uDataSize > uDataActSize ? uDataActSize : t_uDataSize);
	return HASHITEM_FOUND;
}


int32_t ShmHashInit(ShmHashManageNode *t_pShmHashManageNode, ShmHashHeader *t_pShmHashHeader, uint32_t t_uShmKey, uint32_t t_uLines,const uint32_t *t_pPrimerTable){
	if (NULL == t_pShmHashManageNode || NULL == t_pPrimerTable){
		return RET_WRONG;
	}

	if (t_uLines > PRIMER_TABLE_LEN){
		t_uLines = PRIMER_TABLE_LEN;
	}

	int32_t iAttachSign = 0;
	uint64_t uMaxSize = 0;
	int32_t i = 0;
	void *pMem = NULL;
	int32_t iOriPrimerTableSize = 0;

	//PRIMER_TABLE: 		2   3   5   7   11  13  17  ( bucket size, using primes )  
	//uPrimerTotalTable:    0   2   5   10  17  28  41  ( line address begin position )  
	for (i = 0; i < t_uLines; i++){
		//0 use
		uMaxSize += t_pPrimerTable[i];
		t_pShmHashManageNode->szOriPrimerTable[i] = t_pPrimerTable[i];
		iOriPrimerTableSize++;
		if (i == 0){
			t_pShmHashManageNode->uPrimerTotalTable[i] = 0;	//0 nouse
		}
		else
		{
			t_pShmHashManageNode->uPrimerTotalTable[i] = t_pShmHashManageNode->uPrimerTotalTable[i - 1] + t_pPrimerTable[i - 1];
		}
	}

	//uint32_t uShareMemTotalSize = sizeof(ShmHashNode)* uMaxSize + sizeof(ShmHashManageNode);
	uint64_t uShareMemTotalSize = sizeof(ShmHashNode)* uMaxSize + sizeof(ShmHashHeader);

	int32_t iShmId = shmget(t_uShmKey, uShareMemTotalSize, 0666);


	if(iShmId <0){
		printf("shmget error,try to create a new one:%s\n",strerror(errno));
		//try again
		//iShmId = shmget(t_uShmKey, uShareMemTotalSize, IPC_CREAT|IPC_EXCL|0666);
		iShmId = shmget(t_uShmKey, uShareMemTotalSize, IPC_CREAT |IPC_EXCL| 0666);
		if (iShmId < 0){
			//Invalid argument may be shm size too small
			if (iShmId == ENOSPC){
				printf("shm require size overflow!\n");
			}
			printf("shmget error again:%s %lu\n", strerror(errno),uShareMemTotalSize);
			return RET_WRONG;
		}

		pMem = shmat(iShmId, NULL, 0);
		
		if ((int64_t)pMem == -1) {
			printf("shmat error:%s\n", strerror(errno) );
			return RET_WRONG;
		}

		//linear
		memset(pMem, 0, uShareMemTotalSize);

		printf("init succ\n");
	}
	else{
		//exists,pointer to shm
		iAttachSign = 1;
		printf("shm already exists\n");
		pMem = shmat(iShmId, NULL, 0);
		/*
		if ( int64_t(pMem) == -1){	//wrong
			printf("shmat error:%s\n", strerror(errno) );
			return RET_WRONG;
		}
		*/

		if ((int64_t)pMem == -1) {
			printf("shmat error:%s\n", strerror(errno));
			return RET_WRONG;
		}
	}

	if (iAttachSign == 0){
		//initialize the shm buffer
		ShmHashHeader *pstShmHashHeader;
		//tie to the header AND init value 
		pstShmHashHeader = (ShmHashHeader *)(uint64_t)pMem;
		pstShmHashHeader->uCurrentSize = 0;
		pstShmHashHeader->uStatus = HASH_STATUS_NORMAL;
		pstShmHashHeader->uReserver1 = pstShmHashHeader->uReserver2 = 0;
	
		t_pShmHashManageNode->ptrShmHashHeader = pstShmHashHeader;	//fix header write bugs!!
		t_pShmHashManageNode->pMemInfo = (void *)pMem;
		t_pShmHashManageNode->uMaxTableSize = uMaxSize;
		t_pShmHashManageNode->uMemSize = uShareMemTotalSize;
		t_pShmHashManageNode->uShmKey = t_uShmKey;
		t_pShmHashManageNode->pLastFoundNode = NULL;
		t_pShmHashManageNode->uShmHashLines = t_uLines;
		t_pShmHashManageNode->uOriPrimerTableSize = iOriPrimerTableSize;
	}
	else{
		//attach succ
        	ShmHashHeader *pstShmHashHeader= NULL;
		pstShmHashHeader = (ShmHashHeader *)(uint64_t)pMem;
		printf("attach-current size=%d\n",pstShmHashHeader->uCurrentSize);

		t_pShmHashManageNode->ptrShmHashHeader  = pstShmHashHeader;	//fix shm write bugs!
		t_pShmHashManageNode->pMemInfo = (void *)pMem;
		t_pShmHashManageNode->uMaxTableSize = uMaxSize;
		t_pShmHashManageNode->uMemSize = uShareMemTotalSize;
		t_pShmHashManageNode->uShmKey = t_uShmKey;
		t_pShmHashManageNode->pLastFoundNode = NULL;
		t_pShmHashManageNode->uShmHashLines = t_uLines;
		t_pShmHashManageNode->uOriPrimerTableSize = iOriPrimerTableSize;
	}

	//g_uInitSign = 1;

	return RET_RIGHT;
}


