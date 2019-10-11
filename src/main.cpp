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

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size)
{
     char *s = (char *)malloc(size + 1);
     if (s) {
         rand_string(s, size);
     }
     return s;
}

unsigned int get_time33_hash(char *str){
    unsigned int hash = 5381;
    while(*str){
        hash += (hash << 5 ) + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

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

	uint32_t uKey = 0x1234567890;
	
	//para must be const
	int32_t iRet = ShmHashInit(&stShmHashManageNode,&stShmHashHeader,uKey,uLine,szPrimerTable);
	printf("iret:%d\n", iRet);
	
	if (iRet == RET_RIGHT){
		printf("CurrentNodeSize=%d\n",stShmHashManageNode.ptrShmHashHeader->uCurrentSize);
	}


	char szValue[] = "pandaychen";
	char szValueGet[] = "nehcyadnap";

	uint64_t uKeyHash = 12345;

	iRet = ShmHashInert(&stShmHashManageNode, uKeyHash, (uint8_t *)szValue, strlen(szValue));
	printf("iret:%d\n", iRet);

	iRet = ShmHashQuery(&stShmHashManageNode, uKeyHash, (uint8_t *)szValueGet, strlen(szValue));
	printf("iret:%d,current_item:%lu\n", iRet, stShmHashManageNode.ptrShmHashHeader->uCurrentSize);
	
	printf("find value = %s\n", szValueGet);
	
	int i = 0;
	int fail_count=0;
	int total = 10000;
	for ( ; i< total ; i++){
		char *temp = rand_string_alloc(16);		
		if (temp == NULL){
			return -1;
		}
		uint64_t temp_key = get_time33_hash(temp);
		iRet = ShmHashInert(&stShmHashManageNode, temp_key, (uint8_t *)temp, strlen(temp));
		if (iRet!=HASHITEM_INSERT_SUCC){
			fail_count++;
			//printf("insert value=%s,key=%u error\n",temp,temp_key);
		}
		free(temp);
	}

	printf("current cover rate:%f,insert total=%d,failed=%d\n",GetCurrentStorageRate(&stShmHashManageNode),total,fail_count);
	/*
	GeneratePrimerTable(szPrimerTable, uLine,10000);
	int32_t j = 0;
	for (;  j < uLine ; j++){
		printf("%d\n", szPrimerTable[j]);
	}
	*/
	return RET_RIGHT;
}
