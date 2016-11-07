/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#include "params.h"
#include "local_params.h"
#include "ca.h"
#include "ea.h"	


// Random numbers gen. initialization
unsigned rand_init()
{
	unsigned int seed;
	FILE* fp = fopen("/dev/urandom","rb");
	if (fp == NULL) fp = fopen("/dev/random","rb");
	if (fp != NULL) {
		//printf("Init from /dev/{u}random\n");
		int dummy = fread(&seed, sizeof(seed), 1, fp);
		fclose(fp);
	} else {
		seed = (unsigned int)((unsigned) time(NULL) + getpid());
	}
	srand(seed);
	return seed;
}

// TODO add some arguments maybe
int main(int argc, char * argv[])
{
	// RNG
	unsigned int seed=rand_init();
	srand(seed);

	if( argc == 2 )
	{
		#ifdef ES
			ES_search(argv[0], argv[1]);
		#endif
		#ifdef GA
			GA_ELIT_search(argv[0], argv[1]);
		#endif
		#ifdef GA_NO_ELIT
			GA_NO_ELIT_search(argv[0], argv[1]);
		#endif
		#ifdef MB
			MB_search(argv[0], argv[1]);
		#endif
	}
	else
	{
		perror("Bad arguments");
	}
	return 0;
}