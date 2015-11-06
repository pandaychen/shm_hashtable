#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/sem.h>
#include <time.h>
#include <stdio.h>


#include "comm.h"
#include "shm_struct.h"
#include "gene_hashalg.h"
#include "shm_hash.h"


#define SHM_KEY 0x123456


/*test sample*/
int main(int argc ,char **argv){
	
	ShmHashManageNode stShmHashManageNode;
	memset(&stShmHashManageNode, 0, sizeof(stShmHashManageNode));

	ShmHashHeader stShmHashHeader;
	memset(&stShmHashHeader, 0, sizeof(ShmHashHeader));

	uint32_t uLine = 6;

	uint32_t szPrimerTable[] = {
		1511, 1523, 1531, 1543, 1549, 1553,
	};

	uint32_t uKey = 0x12345;
	
	//para must be const
	int32_t iRet = ShmHashInit(&stShmHashManageNode,&stShmHashHeader,uKey,uLine,szPrimerTable);
	printf("iret:%d\n", iRet);

	char szValue[] = "pandaychen";
	char szValueGet[] = "nehcyadnap";

	uint64_t uKeyHash = 12345;

	iRet = ShmHashInert(&stShmHashManageNode, uKeyHash, (uint8_t *)szValue, strlen(szValue));
	printf("iret:%d\n", iRet);

	iRet = ShmHashQuery(&stShmHashManageNode, uKeyHash, (uint8_t *)szValueGet, strlen(szValue));
	printf("iret:%d,current_item:%lu\n", iRet, stShmHashManageNode.stShmHashHeader.uCurrentSize);
	
	printf("%s\n", szValueGet);

	/*
	GeneratePrimerTable(szPrimerTable, uLine,10000);
	int32_t j = 0;
	for (;  j < uLine ; j++){
		printf("%d\n", szPrimerTable[j]);
	}
	*/
	return RET_RIGHT;
}
