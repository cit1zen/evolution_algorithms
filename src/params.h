/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

/*
* ==================================================================
* CA settings
* ==================================================================
*/


// Size of cellular automaton
#define CA_SIZE 12
// Number of dimensions
#define CA_DIMENSIONS 2

/*
* =======================================================================================
* EA settings ( fitness fucntion and evolution algorithm is specified in settings below )
* =======================================================================================
*/

// Size of population, num of chromosomes
#define POPULATION_SIZE 8
// Maximal fitness
#define MAX_FITNESS (pow(CA_SIZE,CA_DIMENSIONS))
// Number of STATES_COUNT
#define STATES_COUNT 2
// Number of non evaluating cycles
#define NON_EVAL_CYCLES 4
// Number of all cycles
#define CYCLES 20
// Number of generations
#define GENERATIONS 10000
// Number of mutations
#define MAX_MUTATIONS 3
#define MIN_MUTATIONS 1

/* Tournament is used in :
 * GA
 * GA_NO_ELIT
*/
#define TURNAMENT_ROUNDS 4


/*
* ==================================================================
* Chromosome encoding
* ==================================================================
*/ 

// Conditionally matching rules
#define CMR
// Size of CMR chromosome
#define CMR_COUNT 20
/* CMR coding
* 0  cell_state >= condition_state
* 1  cell_state <= condition_state
* 2  cell_state == condition_state
* 3  cell_state != condition_state
* Structure XAXAXAXAX where X is state and A is cmr condition
*/


/*
* ==================================================================
* Neighbourhood
* ==================================================================
*/ 

/*
* 0 0 0 0 0
* 0 X X X 0
* 0 X C X 0
* 0 X X X 0
* 0 0 0 0 0
*/
//#define MOORE
#ifdef MOORE
	#ifdef CMR
		#define CMR_SIZE (pow(3,CA_DIMENSIONS)*2+1)
	#endif
#endif
/*
* 0 0 0 0 0
* 0 0 X 0 0
* 0 X C X 0
* 0 0 X 0 0
* 0 0 0 0 0
*/
#define VON_NEUMANN
#ifdef VON_NEUMANN
	#ifdef CMR
		#define CMR_SIZE (CA_DIMENSIONS*4+3)
	#endif
#endif


/*
* ==================================================================
* Fitness functions
* ==================================================================
*/ 


// Fitness functions
#define PATTERN_FITNESS


/*
* ==================================================================
* EU algoriths with their settings
* ==================================================================
*/ 


/* Simple evolution strategy
*  Population in next cycle is created by mutating best chromosome
*  No elitism.
*/
//#define ES

// Evolution algorithm with turnament selection and elitism
//#define GA


// Evolution algorithm with turnament selection and without elitism
//#define GA_NO_ELIT

// TODO name for this algorithm
#define MB
