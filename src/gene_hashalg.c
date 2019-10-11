/*
	*filename:gene_hashalg.C
	*filedesc:general str hash function
	*create date:2015-10-01
	*author:pandaychen
*/


#include "gene_hashalg.h"

#define TIME33_HASH_MIXED_SHIFT() uHash = ((uHash<<5)+uHash) + *t_pstr++ 

uint64_t HashFunTime33Ori(const char *t_pstr,const uint32_t t_uStrLen){
	uint64_t uHash = 5381;

	if (!t_pstr){
		return 0;
	}

	for (; *t_pstr; t_pstr++){
		uHash = uHash * 33 + *t_pstr;
	}

	return uHash;
}

uint64_t HashFunTime33V1(const char *t_pstr, uint32_t t_uStrLen){

	uint64_t uHash = 5381;
	
	for (; t_uStrLen >= 8; t_uStrLen -= 8){
		TIME33_HASH_MIXED_SHIFT(); // 1  
		TIME33_HASH_MIXED_SHIFT(); // 2   
		TIME33_HASH_MIXED_SHIFT(); // 3   
		TIME33_HASH_MIXED_SHIFT(); // 4   
		TIME33_HASH_MIXED_SHIFT(); // 5   
		TIME33_HASH_MIXED_SHIFT(); // 6   
		TIME33_HASH_MIXED_SHIFT(); // 7   
		TIME33_HASH_MIXED_SHIFT(); // 8   
	}
	switch (t_uStrLen)
	{
	case 7: TIME33_HASH_MIXED_SHIFT();
	case 6: TIME33_HASH_MIXED_SHIFT();
	case 5: TIME33_HASH_MIXED_SHIFT();
	case 4: TIME33_HASH_MIXED_SHIFT();
	case 3: TIME33_HASH_MIXED_SHIFT();
	case 2: TIME33_HASH_MIXED_SHIFT();
	case 1: TIME33_HASH_MIXED_SHIFT(); break;
	case 0: break;
	}

	return uHash;
}


//a high performance and low collision hash
//fork from http://sites.google.com/site/murmurhash/avalanche
//it's much better than FNV
uint64_t HashFunMurmurv1(const void * t_pkey, int32_t t_ilen, uint32_t t_uSeed){

	/*t_uSeed --- a primer like 0xEE6B27EB*/
	//uint64_t uHash;

	const uint32_t m = 0x5bd1e995;
	const int32_t  r = 24;

	uint32_t h1 = t_uSeed ^ t_ilen;
	uint32_t h2 = 0;

	const uint32_t * data = (const uint32_t *)t_pkey;

	while (t_ilen >= 8)
	{
		uint32_t k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		t_ilen -= 4;

		uint32_t k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		t_ilen -= 4;
	}

	if (t_ilen >= 4)
	{
		uint32_t k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		t_ilen -= 4;
	}

	switch (t_ilen)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	uint64_t uHash = h1;

	uHash = (uHash << 32) | h2;

	return uHash;
}


void GeneratePrimerTable(uint32_t *t_pPrimerTable, uint32_t t_uLines,uint32_t t_uMaxNum)
{
	//���� 6n+1 6n-1 ��������ԭ��
	//the max num must greater than six
	if (t_uMaxNum< 5)
		return;

	unsigned long t, m, n, p;
	unsigned long x;
	int i, j, a, b, k;
	x = t_uMaxNum;

	int z = 0;

	x = x / 6;  //x>=5


	for (t = x; t >= 0, z < t_uLines; t--)
	{
		i = 1; j = 1; k = t % 10;
		m = 6 * t;
		/**i,j��ֵ ���Ƿ������֤�ı�־Ҳ�Ƕ�Ӧ��6t-1��6t+1�����Ա�־**/
		if (((k - 4) == 0) || ((k - 9) == 0) || ((m + 1) % 3 == 0))j = 0;
		/*�˴��Ǽ���֤6*t-1,6*t+1 �ǲ������������������������**/
		if (((k - 6) == 0) || ((m - 1) % 3 == 0))i = 0;
		/***��ͨ�������ж�ȥ��ĩβ��5������3��������***/
		for (p = 1; p * 6 <= sqrt(m + 1) + 2; p++)
		{
			n = p * 6;
			/**��6*p-1��6*p+1����α�������Գ�*****/
			k = p % 10;
			a = 1; b = 1;
			/**ͬ���˴�a,b��ֵҲ�������жϳ����Ƿ�Ϊ������߳�������������**/
			if (((k - 4) == 0) || ((k - 9) == 0))a = 0;
			if (((k - 6) == 0))b = 0;
			if (i){
				/*���i����Ͷ�m-1����ν6*t-1������֤����Ȼ��Ҫ������n+1,n-1,���Դ���*/
				if (a){ if ((m - 1) % (n + 1) == 0)i = 0; }       /***һ����������˵����������������Ϊ�㼴��i ��ֵΪ��***/
				if (b){ if ((m - 1) % (n - 1) == 0)i = 0; }
			}

			if (j){
				/**���j����Ͷ�m+1����ν6*t+1������֤����Ȼ��Ҫ������n+1,n-1,���Դ���*/
				if (a){ if ((m + 1) % (n + 1) == 0)j = 0; }        /***һ����������˵����������������Ϊ�㼴��j ��ֵΪ��***/
				if (b){ if ((m + 1) % (n - 1) == 0)j = 0; }
			}
			if ((i + j) == 0)break;
			/**����Ѿ�֪��6*t-1,6*t+1�������������Ǿͽ����Գ�ѭ��***/
		}
		if (j){ t_pPrimerTable[z] = m + 1; z++; if (z >= t_uLines)break; }
		if (i){ t_pPrimerTable[z] = m - 1; z++; if (z >= t_uLines)break; }
	}

}


/*other classic hash fuctions*/

