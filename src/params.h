/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

// Debug mode
// #define DEBUG

/*
* ==================================================================
* CA settings
* ==================================================================
*/

// Size of cellular automaton
/*
#define WIDTH_PARAM 12
#define HEIGHT_PARAM 12
*/

/*
* =======================================================================================
* EA settings ( fitness fucntion and evolution algorithm is specified in settings below )
* =======================================================================================
*/

// Size of population, num of chromosomes
#define POPULATION_SIZE 8
// Number of STATES_COUNT
// #define STATES_COUNT 5
// Number of non evaluating cycles
// #define NON_EVAL_CYCLES 4
// Number of all cycles
// #define CYCLES 10
// Number of generations
#define GENERATIONS 2000000
// Number of mutations
#define MAX_MUTATIONS 3
#define MIN_MUTATIONS 1

/* Tournament is used in :
 * GA
 * GA_NO_ELIT
*/
#define TURNAMENT_ROUNDS 3


/*
* ==================================================================
* Chromosome encoding
* ==================================================================
*/ 

// Conditionally matching rules
#define CMR
// Size of CMR chromosome
// Defined in local_params.h
// #define CMR_COUNT 20

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
		// #define CMR_SIZE (pow(3,CA_DIMENSIONS)*2+1)
		#define CMR_SIZE 19
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
		// #define CMR_SIZE (CA_DIMENSIONS*4+3)
		#define CMR_SIZE 11
	#endif
#endif


/*
* ==================================================================
* Fitness functions
* ==================================================================
*/ 


// Fitness functions
// #define PATTERN_FITNESS
// Maximal fitness
// Defined in local_params.h
// #define MAX_FITNESS (pow(CA_SIZE,CA_DIMENSIONS))
// #define MAX_FITNESS HEIGHT_PARAM*WIDTH_PARAM

// Fitness function for stable patterns or 
#define PATTERN_STABLE
#define TEST_FRAME 2
// // #define MAX_FITNESS (2*pow(CA_SIZE,CA_DIMENSIONS))
#define MAX_FITNESS 2*HEIGHT_PARAM*WIDTH_PARAM

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
// #define GA


// Evolution algorithm with turnament selection and without elitism
//#define GA_NO_ELIT

// TODO name for this algorithm
// #define MB
