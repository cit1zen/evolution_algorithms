/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "params.h"
#include "ca.h"
#include "ea.h"


///////////////////////////////////
// Support functions             //
///////////////////////////////////


// Mutates chromosome at num_mut places
static void mutate_chromosome( struct chromosome * chromosome, unsigned num_mut )
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
static void initialize_chromosome( struct chromosome * chromosome )
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
static void copy_chromosome( struct chromosome * origin, struct chromosome *target)
{
	target->fitness=origin->fitness;
	for(unsigned i=0;i<CMR_SIZE*CMR_COUNT;i++)
		target->dna[i]=origin->dna[i];
}

// Printing chromosome
static void print_chromosome( struct chromosome * chromosome )
{
	for(unsigned i=0;i<CMR_SIZE*CMR_COUNT;i++)
		printf("%u", chromosome->dna[i] );
}

// Turnament selection
// return index of winner
static inline unsigned turnament( struct chromosome * chromosomes )
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


#ifdef MB

	int compar(const void chromosome_1*, const void chromosome_2*)
	{
		return ( (struct *chromosome)chromosome_2->fitness - (struct *chromosome)chromosome_1->fitness )
	}

#endif



///////////////////////////////////
// Evolution algorithms          //
///////////////////////////////////


void search()
{
	// Original state of CA
	unsigned origin_pattern[ (int) pow(CA_SIZE,CA_DIMENSIONS) ];
	// We need to load staring pattern
	initialize_ca( origin_pattern , "original_state.txt" );

	// Target state of CA
	unsigned target_pattern[ (int) pow(CA_SIZE,CA_DIMENSIONS) ];
	// TODO
	initialize_ca( target_pattern , "target_state.txt" );

	// 2 populations of chromosomes
	struct chromosome chromosomes_pool_1[POPULATION_SIZE];
	struct chromosome chromosomes_pool_2[POPULATION_SIZE];

	struct chromosome *current_pop = chromosomes_pool_1;
	struct chromosome *next_pop = chromosomes_pool_2;

	// Initialization of chromosomes to random values
	for(unsigned i=0;i<POPULATION_SIZE;i++)
	{
		initialize_chromosome( &chromosomes_pool_1[i] );
	}

	for(unsigned generation=0; generation<GENERATIONS ;generation++)
	{
		// Evaluation
		for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
		{
			// In case of elitism, it is not required to evaluate previusly evaluated chromosome
			if(current_pop[chromosome_index].fitness==0)
				continue;

			// New CA
			unsigned cellular_automaton[ (int) pow(CA_SIZE,CA_DIMENSIONS) ];
			copy_ca( origin_pattern,cellular_automaton);

			// We do non evaluation rounds
			do_cycles( cellular_automaton, NON_EVAL_CYCLES , current_pop[ chromosome_index ].dna );

			// Evaluation rounds
			for(unsigned cycle=NON_EVAL_CYCLES;cycle<CYCLES;cycle++ )
			{
				do_cycles( cellular_automaton, 1 , current_pop[ chromosome_index ].dna );

				unsigned fitness = fitness_function(cellular_automaton,target_pattern);

				if ( fitness > current_pop[chromosome_index].fitness )
					current_pop[chromosome_index].fitness=fitness;

			}

			// If solution was found, we print chromosome and aditional info
			// Yes, dangerous printf is used
			if( current_pop[chromosome_index].fitness>=MAX_FITNESS )
			{
				printf("SOLUTION FOUND: GENERATION %u: ",generation);
				print_chromosome( &current_pop[chromosome_index] );
				printf("\n");
				return;
			}
		}

		/* Simple evolution strategy
		*  Population in next cycle is created by mutating best chromosome
		*  No elitism.
		*/
		#ifdef ES

			// Choosing parent
			// Best chromosome
			unsigned best_chromosome_index=0;
			for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
			{
				if(current_pop[best_chromosome_index].fitness<current_pop[chromosome_index].fitness)
					best_chromosome_index=chromosome_index;
			}


			// Creation of new population
			// TODO
			for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
			{
				copy_chromosome( &current_pop[best_chromosome_index] , &next_pop[chromosome_index] );
				mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
			}

		#endif

		// Simple genetic algorithm with elitism
		#ifdef GA

			// Choosing parent
			unsigned parent_chromosome_index= turnament( current_pop );

			// Elitism
			copy_chromosome( &current_pop[parent_chromosome_index] , &next_pop[0] );

			// Creation of new population
			for(unsigned chromosome_index=1;chromosome_index<POPULATION_SIZE;chromosome_index++)
			{
				copy_chromosome( &current_pop[best_chromosome_index] , &next_pop[chromosome_index] );
				mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
			}

		#endif


		// Simple genetic algorithm without elitism
		#ifdef GA_NO_ELIT

			// Choosing parent
			unsigned parent_chromosome_index= turnament( current_pop );

			// Creation of new population
			for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
			{
				copy_chromosome( &current_pop[best_chromosome_index] , &next_pop[chromosome_index] );
				mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
			}

		#endif



		#ifdef MB

			// Sorting according to fitness
			qsort(current_pop, POPULATION_SIZE, sizeof(struct chromosome), compar);

			// Selecting parent
			// TODO TOASK
			// analogie 1+lambda ES
			// analogie mu+lambda ES 
			unsigned last_best = 0;
			for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)
			{
				if (current_pop[chromosome_index]->fitness==)
					last_best++;
				else
					break;
			}

			// Creation of new population
			for(unsigned chromosome_index=0;chromosome_index<POPULATION_SIZE;chromosome_index++)	
			{
				copy_chromosome( &current_pop[best_chromosome_index] , &next_pop[chromosome_index] );
				mutate_chromosome( &next_pop[chromosome_index], rand()%(MAX_MUTATIONS-MIN_MUTATIONS)+MIN_MUTATIONS );
			}

		#endif

		// Switching populations
		struct chromosome *dummy;
		dummy = current_pop;
		current_pop = next_pop;
		next_pop = dummy;
	}

		

	printf("NO SOLUTIONS FOUND\n");

	return;
}



///////////////////////////////////
// Fitness functions             //
///////////////////////////////////

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