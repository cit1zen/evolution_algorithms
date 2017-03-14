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
		fread(&seed, sizeof(seed), 1, fp);
		fclose(fp);
	} else {
		seed = (unsigned int)((unsigned) time(NULL) + getpid());
	}
	srand(seed);
	return seed;
}

// Prints parameters
void print_params(unsigned seed)
{
	// Normal info
	printf("|SEED:%u|STATES:%u|WIDTH:%u|HEIGHT:%u",seed,STATES_COUNT,WIDTH_PARAM,HEIGHT_PARAM);
	printf("|CYCLES:%u",CYCLES);
	printf("|GEN:%u",GENERATIONS);
	// Hood
	#ifdef VON_NEUMANN
		printf("|HOOD:VON_NEUMANN");
	#endif
	#ifdef MOORE
		printf("|HOOD:MOORE");
	#endif
	// Fitness function
	#ifdef PATTERN_FITNESS
		printf("|FITNESS:PATTERN_FITNESS");
	#endif
	// Evo. algorithm
	#ifdef ES
		printf("|EVO_ALGO:ES");
	#endif
	#ifdef GA
		printf("|EVO_ALGO:GA");
	#endif
	#ifdef GA_NO_ELIT
		printf("|EVO_ALGO:GA_NO_ELIT");
	#endif
	#ifdef MB
		printf("|EVO_ALGO:MB");
	#endif	
	printf("\n");
}

// TODO add some arguments maybe
int main(int argc, char * argv[])
{
	// RNG
	unsigned int seed=rand_init();

	if( argc == 3 )
	{
		#ifdef ES
			ES_search(argv[1], argv[2]);
		#endif
		#ifdef GA
			GA_ELIT_search(argv[1], argv[2]);
		#endif
		#ifdef GA_NO_ELIT
			GA_NO_ELIT_search(argv[1], argv[2]);
		#endif
		#ifdef MB
			MB_search(argv[1], argv[2]);
		#endif
		print_params(seed);
	}
	else
	{
		perror("Bad arguments");
	}
	return 0;
}