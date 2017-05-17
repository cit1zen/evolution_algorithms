/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: EA header
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

// Evolution strategy
// Best chromosome is chosen as parent of next generation
void ES_ELIT_search(char *, char *);

// Genetic algorithm with turnament selection and without elitism
void GA_search(char *, char *);

// Genetic algorithm with turnament selection and elitism
void GA_ELIT_search(char *, char *);

// ESP version of evolution strategy
void ESP_search(char *,char *);

// ESP version of evolution strategy with elitism
void ESP_ELIT_search(char *,char *);

/*
* Fitness fuctions
*/
unsigned patt_fitness(unsigned *,unsigned *);
unsigned patt_stable(unsigned *, unsigned *, unsigned *, unsigned);

#endif