/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "params.h"
#include "local_params.h"
#include "ca.h"
#include "ea.h"


///////////////////////////////////
// Support functions and macros  //
///////////////////////////////////


// Mutates chromosome at num_mut places
void mutate_chromosome( struct chromosome * chromosome, unsigned num_mut )
{
	for(unsigned mutation=0;mutation<num_mut;mutation++)
	{
		unsigned mutated_cmr = rand()%CMR_COUNT;
		unsigned mutated_position = rand()%CMR_SIZE;

		// If on even position => state
		if (mutated_position%2==0)
			chromosome->dna[(mutated_cmr*CMR_SIZE)+mutated_position]=rand()%STATES_COUNT;
		// If on odd position => contition
		else
			chromosome->dna[(mutated_cmr*CMR_SIZE)+mutated_position]=rand()%4;
	}

	if( num_mut == 0 )
		chromosome->fitness=0;
}

// Initialization of chromosome
void initialize_chromosome( struct chromosome * chromosome )
{
	chromosome->fitness=0;

	#ifdef CMR
		
		for(unsigned cmr=0;cmr<CMR_COUNT;cmr++)
		{
			for(unsigned i=0;i<CMR_SIZE;i++)
			{
				// If on even position => state
				if (i%2==0)
				{
					chromosome->dna[(cmr*CMR_SIZE)+i]=rand()%STATES_COUNT;
				}
				// If on odd position => contition
				else
				{
					chromosome->dna[(cmr*CMR_SIZE)+i]=rand()%4;
				}
			}
		}
	#endif
}

// Copies origin chromosome to target chromosome
void copy_chromosome( struct chromosome * origin, struct chromosome *target)
{
	target->fitness=origin->fitness;
	for(unsigned i=0;i<CMR_SIZE*CMR_COUNT;i++)
		target->dna[i]=origin->dna[i];
}

// Printing chromosome
void print_chromosome( struct chromosome * chromosome )
{
	for(unsigned i=0;i<CMR_SIZE*CMR_COUNT;i++)
		printf("%u", chromosome->dna[i] );
}

// Turnament selection
// return index of winner
inline unsigned turnament( struct chromosome * chromosomes )
{
	unsigned best_index = rand()%POPULATION_SIZE;
	// Because first one is randomly selected
	for( unsigned turnament_round = 1; turnament_round < TURNAMENT_ROUNDS; turnament_round++ )
	{
		unsigned opponent_index = rand()%POPULATION_SIZE;
		if( chromosomes[best_index].fitness < chromosomes[opponent_index].fitness )
			best_index=opponent_index;
	}
	return best_index;
}


// TODO
int compar(const void * chromosome_1, const void * chromosome_2)
{
	if ( ((struct chromosome *)chromosome_2)->fitness > ((struct chromosome *)chromosome_1)->fitness )
		return 0;
	else
		return 1;
}

// Creates CA and loads state of CA from file
#define init_and_load_CA(name_of_ca, source_file) 				\
	unsigned name_of_ca[ (int) pow(CA_SIZE,CA_DIMENSIONS) ];	\
	initialize_ca( name_of_ca , source_file );


// Frame function used to create every evol. algo. search function
void evol_frame(char * original_state_file, char * target_state_file, void (*next_gen_function)(struct chromosome * current_pop, struct chromosome * next_pop ))
{
	/* 2 populations of chromosomes */ 
	struct chromosome chromosomes_pool_1[POPULATION_SIZE];
	struct chromosome chromosomes_pool_2[POPULATION_SIZE];

	/* Pointers are used for switching between generations */	
	struct chromosome *current_pop = chromosomes_pool_1;
	struct chromosome *next_pop = chromosomes_pool_2;

	/* Initializes current pop to random chromosomes */
	for(unsigned i=0;i<POPULATION_SIZE;i++)
	{					
		initialize_chromosome( &current_pop[i] );
	}

	/* Initialize origin state CA */
	init_and_load_CA(origin_pattern, original_state_file) 

	/* Initialize CA with wanted pattern */
	init_and_load_CA(target_pattern, target_state_file)

	/* Evolution */
	for(unsigned generation=0; generation<GENERATIONS ;generation++)
	{
		/* Evaluation of population*/
		for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
		{
			/* In case of elitism */
			/* So already evaluated chromosomes are not evaluated again */ 
			if(current_pop[chromosome_index].fitness==0)
				continue;

			/* New CA used for evaluation of this chromosome */
			unsigned cellular_automaton[ (int) pow(CA_SIZE,CA_DIMENSIONS) ];
			copy_ca( origin_pattern,cellular_automaton);

			/* Do non evaluation runs */
			do_cycles( cellular_automaton, NON_EVAL_CYCLES , current_pop[ chromosome_index ].dna );

			/* Evaluation runs */
			/* Testing agains fitness function */
			for(unsigned cycle=NON_EVAL_CYCLES;cycle<CYCLES;cycle++ )
			{
				do_cycles( cellular_automaton, 1, current_pop[ chromosome_index ].dna);

				unsigned fitness = fitness_function(cellular_automaton,target_pattern);

				if ( fitness > current_pop[chromosome_index].fitness )
					current_pop[chromosome_index].fitness=fitness;
			}

			/* If solution was found, we print chromosome and aditional info */
			if( current_pop[chromosome_index].fitness>=MAX_FITNESS )
			{
				printf("SOLUTION FOUND: GENERATION %u: ",generation);
				print_chromosome( &current_pop[chromosome_index] );
				printf("\n");
				return;
			}
		}

		next_gen_function(current_pop,next_pop);

		/* Switch populations/generations */
		struct chromosome *dummy;
		dummy = current_pop;
		current_pop = next_pop;
		next_pop = dummy;

	}
	printf("NO SOLUTIONS FOUND\n");
	return;
}


/* Evolution strategy */

void ES_next_gen(struct chromosome * current_pop, struct chromosome * next_pop )
{
	// Find best chromosome
	unsigned best_chromosome_index=0;
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		if(current_pop[best_chromosome_index].fitness<current_pop[chromosome_index].fitness)
			best_chromosome_index=chromosome_index;
	}

	// Use best chromosome as parrent for next generation
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		copy_chromosome( &current_pop[best_chromosome_index] , &next_pop[chromosome_index] );
		mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
	}
}

void ES_search(char * original_state_file, char * target_state_file)
{
	evol_frame(original_state_file, target_state_file, ES_next_gen);
}


/* Genetic algorithm with elitism */

void GA_ELIT_next_gen(struct chromosome * current_pop, struct chromosome * next_pop)
{
	// Choose new parent in turnament
	unsigned parent_chromosome_index = turnament(current_pop);

	// Elitism
	copy_chromosome( &current_pop[parent_chromosome_index] , &next_pop[0]);

	// Create new pop
	for(unsigned chromosome_index=1; chromosome_index<POPULATION_SIZE; chromosome_index++)
	{
		copy_chromosome(&current_pop[parent_chromosome_index], &next_pop[chromosome_index]);
		mutate_chromosome(&next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS) + MIN_MUTATIONS);
	}
}

void GA_ELIT_search(char * original_state_file, char * target_state_file )
{
	evol_frame(original_state_file,target_state_file,GA_ELIT_next_gen);
}


/* Genetic algorithm without elitism */

void GA_NO_ELIT_next_gen(struct chromosome * current_pop, struct chromosome * next_pop )
{
	// Choose new parent in turnament
	unsigned parent_chromosome_index= turnament( current_pop );

	// Create new pop
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		copy_chromosome( &current_pop[parent_chromosome_index] , &next_pop[chromosome_index] );
		mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
	}
}

void GA_NO_ELIT_search(char * original_state_file, char * target_state_file )
{
	evol_frame(original_state_file,target_state_file, GA_NO_ELIT_next_gen);
}


/* MB algorithm */

void MB_next_gen(struct chromosome * current_pop, struct chromosome * next_pop )
{
	// Sorting according to fitness
	// TODO if working properly
	qsort(current_pop, POPULATION_SIZE, sizeof(struct chromosome), compar);

	// Selecting parent
	unsigned last_best = 0;
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		if (current_pop[chromosome_index].fitness==current_pop[last_best].fitness)
			last_best++;
		else
			break;
	}
	unsigned parent_chromosome_index = rand()%last_best;

	// Create new pop
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)	
	{
		copy_chromosome( &current_pop[parent_chromosome_index] , &next_pop[chromosome_index] );
		mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
	}
}

void MB_search(char * original_state_file, char * target_state_file )
{
	evol_frame(original_state_file,target_state_file,MB_next_gen);
}


/* PATTERN_FITNESS fitness function */

#ifdef PATTERN_FITNESS
/*
* Matches current state of CA to pattern and return number of matching cells	
*/
unsigned fitness_function(unsigned * cellular_automaton, unsigned * pattern)
{
	unsigned match=0;
	for(int i=0;i< pow(CA_SIZE,CA_DIMENSIONS) ;i++)
	{
		if(cellular_automaton[i]==pattern[i])
			match++;
	}
	return match;
}

#endif