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


/*
* Seach algorithm
* Most of the time some sort of Genetic algorithm
*/
void search();


/*
* Fitness fuction
* return fitness
*/
unsigned fitness_function(unsigned*,unsigned*);

#endif