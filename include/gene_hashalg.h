/*
	*filename:gene_hashalg.h
	*filedesc:general str hash function
	*create date:2015-10-01
	*author:pandaychen
*/

#ifndef __GENE_HASH_ALGORITHM
#define __GENE_HASH_ALGORITHM

#include "comm.h"
#include <stdint.h>
#include <math.h>


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */ 


void GeneratePrimerTable(uint32_t *t_pPrimerTable, uint32_t t_uSize, uint32_t t_uMaxNum);

uint64_t HashFunMurmurv1(const void * t_pkey, int32_t t_ilen, uint32_t t_uSeed);

uint64_t HashFunTime33Ori(const char *t_pstr, const uint32_t t_uStrLen);

uint64_t HashFunTime33V1(const char *t_pstr, uint32_t t_uStrLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */ 

#endif
