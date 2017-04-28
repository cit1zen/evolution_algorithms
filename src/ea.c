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

#ifdef DEBUG
// Control sum
unsigned EA_CONTROL_SUM = 0;
#endif

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

	// If chromosome was mutated, we set fitness to 0
	if( num_mut != 0 )
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
	for(unsigned cmr=0; cmr<CMR_COUNT;cmr++)
	{
		printf("%u", chromosome->dna[(cmr*CMR_SIZE)]);
		for(unsigned i=1;i<CMR_SIZE;i++)
		{
			printf("-%u", chromosome->dna[(cmr*CMR_SIZE)+i]);
		}
		printf(" ");
	}
}

// Turnament selection
// return index of winner
unsigned turnament( struct chromosome * chromosomes )
{
	#ifdef DEBUG
	EA_CONTROL_SUM = 0;
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		printf("[%u] %u ", chromosome_index, chromosomes[chromosome_index].fitness);
		EA_CONTROL_SUM += chromosomes[chromosome_index].fitness;
	}
	printf("\n");
	#endif

	unsigned best_index = rand()%POPULATION_SIZE;
	// Because first one is randomly selected
	for( unsigned turnament_round = 0; turnament_round < TURNAMENT_ROUNDS; turnament_round++ )
	{
		unsigned opponent_index = rand()%POPULATION_SIZE;
		if( chromosomes[best_index].fitness < chromosomes[opponent_index].fitness )
			best_index=opponent_index;
	}
	return best_index;
}

// Compares fitness of two chromosomes
int compar(const void * chromosome_1, const void * chromosome_2)
{
	return ( ((struct chromosome *)chromosome_2)->fitness - ((struct chromosome *)chromosome_1)->fitness );
}

// Creates CA and loads state of CA from file
#define init_and_load_CA(name_of_ca, source_file) 				\
	unsigned name_of_ca[ (int) HEIGHT_PARAM*WIDTH_PARAM ];	\
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

	// Size of automaton
	const unsigned size_of_automaton = HEIGHT_PARAM*WIDTH_PARAM;

	/* Evolution */
	for(unsigned generation=0; generation<GENERATIONS ;generation++)
	{
		/* Evaluation of population*/
		for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
		{
			/* In case of elitism */
			/* So already evaluated chromosomes are not evaluated again */ 
			if(current_pop[chromosome_index].fitness!=0)
				continue;

			/* New CA used for evaluation of this chromosome */
			unsigned cellular_automaton_1[ size_of_automaton ];
			unsigned cellular_automaton_2[ size_of_automaton ];

			unsigned * current = cellular_automaton_1;
			unsigned * next = cellular_automaton_2;

			copy_ca(origin_pattern, current);
			copy_ca(origin_pattern, next);

			/* Do non evaluation runs */
			do_cycles(current, next, NON_EVAL_CYCLES , current_pop[ chromosome_index ].dna);

			#ifdef PATTERN_STABLE
			unsigned history[CYCLES] = {0};
			#endif

			/* Evaluation runs */
			/* Testing agains fitness function */
			unsigned fitness = 0;
			for(unsigned cycle=NON_EVAL_CYCLES;cycle<CYCLES;cycle++ )
			{
				do_cycles( current, next, 1, current_pop[ chromosome_index ].dna);

				#ifdef PATTERN_FITNESS
				fitness = patt_fitness(next, target_pattern);
				// Best fitness is fitness of whole chromosome
				if(fitness > current_pop[chromosome_index].fitness)
				{
					current_pop[chromosome_index].fitness = fitness;
				}
				#endif

				#ifdef PATTERN_STABLE
				fitness = patt_stable(next, target_pattern, history, cycle);
				// Best fitness is fitness of whole chromosome
				if(fitness > current_pop[chromosome_index].fitness)
				{
					current_pop[chromosome_index].fitness = fitness;
				}
				#endif

				/* If solution was found, we print chromosome and aditional info */
				if( current_pop[chromosome_index].fitness>=MAX_FITNESS )
				{
					printf("SUCCESS|GENERATION:%u|LEN:%u|RULES:",generation,cycle);
					print_chromosome( &current_pop[chromosome_index] );
					return;
				}
				/* Switch of lattices */
				unsigned * temp = current;
				current = next;
				next = temp;
			}
		}

		next_gen_function(current_pop,next_pop);

		/* Switch populations/generations */
		struct chromosome *dummy;
		dummy = current_pop;
		current_pop = next_pop;
		next_pop = dummy;

	}
	printf("FAIL");
	return;
}


/* Evolution strategy */

void ES_next_gen(struct chromosome * current_pop, struct chromosome * next_pop )
{
	// Find best chromosome
	unsigned best_chromosome_index=0;
	#ifdef DEBUG
	printf("ES\n");
	#endif
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		#ifdef DEBUG
		printf("[%u] %u ", chromosome_index, current_pop[chromosome_index].fitness);
		#endif
		if(current_pop[best_chromosome_index].fitness<current_pop[chromosome_index].fitness)
			best_chromosome_index=chromosome_index;
	}
	#ifdef DEBUG
	fflush(stdout);
	printf("\n");
	#endif

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
	#ifdef DEBUG
	printf("GA_ELIT\n");
	#endif

	// Choose new parent in turnament
	unsigned parent_chromosome_index = turnament(current_pop);

	#ifdef DEBUG
	unsigned ANOTHER_SUM = 0;
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		printf("[%u] %u ", chromosome_index, current_pop[chromosome_index].fitness);
		ANOTHER_SUM += current_pop[chromosome_index].fitness;
	}
	fflush(stdout);
	printf("\n");
	if (EA_CONTROL_SUM != ANOTHER_SUM)
	{
		printf("SUM ERROR\n");
		exit(1);
	}
	#endif

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
	#ifdef DEBUG
	printf("GA_NO_ELIT\n");
	#endif
	// Choose new parent in turnament
	unsigned parent_chromosome_index = turnament( current_pop );

	#ifdef DEBUG
	unsigned ANOTHER_SUM = 0;
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		printf("[%u] %u ", chromosome_index, current_pop[chromosome_index].fitness);
		ANOTHER_SUM += current_pop[chromosome_index].fitness;
	}
	fflush(stdout);
	printf("\n");
	if (EA_CONTROL_SUM != ANOTHER_SUM)
	{
		printf("SUM ERROR\n");
		exit(1);
	}
	#endif

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
	qsort(current_pop, POPULATION_SIZE, sizeof(struct chromosome), compar);

	// Selecting parent
	unsigned last_best = 0;
	#ifdef DEBUG
	printf("MB\n");
	#endif
	for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
	{
		if (current_pop[chromosome_index].fitness==current_pop[0].fitness)
		{
			#ifdef DEBUG
				printf("%i ", current_pop[chromosome_index].fitness);
			#endif
			last_best++;
		}
		else
			break;
	}
	#ifdef DEBUG
	fflush(stdout);
	printf("\n");
	#endif
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


/* Fitness functions */

/*
* Matches current state of CA to pattern and return number of matching cells	
*/
unsigned patt_fitness(unsigned * cellular_automaton, unsigned * pattern)
{
	unsigned match=0;
	for(int i=0;i<HEIGHT_PARAM*WIDTH_PARAM;i++)
	{
		if(cellular_automaton[i]==pattern[i])
			match++;
	}
	return match;
}

/*
* Matches best two fitness scores, so only stable rules can get 100 fitness
* Or if we want to find oscilators
*/
unsigned patt_stable(unsigned * cellular_automaton, unsigned * pattern, unsigned * history, unsigned cycle)
{
	unsigned match=0;
	for(int i=0;i < HEIGHT_PARAM*WIDTH_PARAM ;i++)
	{
		if(cellular_automaton[i]==pattern[i])
			match++;
	}
	history[cycle] = match;
	unsigned second = 0;
	unsigned first = 0;
	if (cycle >= TEST_FRAME)
	{
		if (history[cycle] < history[cycle-1])
		{
			second = history[cycle];
			first = history[cycle-1];
		}
		else
		{
			second = history[cycle-1];
			first = history[cycle];
		}

		for (unsigned off = 2; off < TEST_FRAME; off++) 
		{
			if(first <= history[cycle-off])
			{
				second = first;
				first = history[cycle-off];
			}
		}
	}
	return first + second;
}
