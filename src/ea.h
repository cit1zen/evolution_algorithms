/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

#ifndef EA
#define EA


// CMR chromosome
struct chromosome
{
	// Fitness of this particular chromosome
	unsigned fitness;
	// Rules for cullular automata coded in CMR
	unsigned dna[CMR_SIZE*CMR_COUNT];
};


// Evolution strategy
// Best chromosome is chosen as parent of next generation
void ES_search(char *, char *);

// Genetic algorithm with turnament selection and elitism
void GA_ELIT_search(char *, char *);

// Genetic algorithm with turnament selection and without elitism
void GA_NO_ELIT_search(char *, char *);

// TODO name for this algorithm
void MB_search(char *,char *);

/*
* Fitness fuction
* return fitness
*/
unsigned fitness_function(unsigned *,unsigned *);

#endif