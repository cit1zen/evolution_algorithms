
/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: TODO
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#include "params.h"
#include "local_params.h"
#include "ca.h"

/*
* Gets cell state
* unsigned ... coordinates of cell
* x axis, y axis, z axis, ...
*/
#if CA_DIMENSIONS == 2
unsigned get_cell( unsigned * CA, unsigned width, unsigned height)
{
	if ((width < CA_SIZE) && (height < CA_SIZE))
	{
		return CA[(height*CA_SIZE)+width];
	}
	else
	{
		return 0;
	}
}
#else
unsigned get_cell( unsigned * CA, ... )
{
	va_list arguments;
	unsigned position = 0;

	va_start ( arguments, CA );

	for ( int x = 0; x < CA_DIMENSIONS ; x++ )        
	{
		// va_arg( arguments, unsigned ) 				: position at axis
		// (( x * CA_SIZE ):( x * CA_SIZE )?1)          : axis multiplier
		unsigned axis_position = va_arg( arguments, unsigned );
		// If axis position is inside CA
		if ( axis_position < CA_SIZE )
			position += axis_position * (( x * CA_SIZE )?( x * CA_SIZE ):1);
		// If outside of CA
		else
		{
			va_end ( arguments );
			return 0;
		}
	}
	va_end ( arguments );
	return CA[position];
}
#endif

/*
* Sets cell state
* unsigned new_state
* unsigned ... coordinates of cell
* x axis, y axis, z axis, ...
*/
#if CA_DIMENSIONS == 2
void set_cell( unsigned * CA, unsigned new_state, unsigned width, unsigned height)
{
	if ((width < CA_SIZE) && (height < CA_SIZE))
	{
		CA[(height*CA_SIZE)+width] = new_state;
	}
}
#else
void set_cell( unsigned * CA, unsigned new_state, ... )
{
	va_list arguments;
	unsigned position = 0;

	va_start ( arguments, new_state );

	for ( int x = 0; x < CA_DIMENSIONS ; x++ )        
	{
		// va_arg( arguments, unsigned ) 				: position at axis
		// (( x * CA_SIZE ):( x * CA_SIZE )?1)          : axis multiplier
		unsigned axis_position = va_arg( arguments, unsigned );
		// If axis position is inside CA
		if ( axis_position < CA_SIZE )
			position += axis_position * (( x * CA_SIZE )?( x * CA_SIZE ):1);
		// If outside of CA
		else
			break;
		
	}
	va_end ( arguments );
	CA[position]=new_state;
}
#endif


/*
* Do num_cycles without evaluating fitness
* unsigned num_cycles : number of cycles :
* pointer to CA structure
*/
unsigned do_cycles( unsigned * current, unsigned * next, unsigned num_cycles , unsigned * rules)
{
	for(unsigned cycle=0; cycle < num_cycles ; cycle++)
	{

		#ifdef CMR

			#if CA_DIMENSIONS == 2

				for(unsigned height=0;height<CA_SIZE;height++)
				{
					for(unsigned width=0;width<CA_SIZE;width++)
					{
						// Cross
						#ifdef VON_NEUMANN

							// Position in cmr rules
							// cmr_index points at start of cmr rule
							for( int cmr_index = 0 ; cmr_index < (CMR_COUNT*CMR_SIZE); cmr_index+=CMR_SIZE )
							{
								// Decision block, every if must be true
								// North
								if(RESOLVE(rules[cmr_index+1],rules[cmr_index],get_cell( current, width, height-1 )))
								{
								// West
								if(RESOLVE(rules[cmr_index+3],rules[cmr_index+2],get_cell( current, width-1, height )))
								{
								// Center
								if(RESOLVE(rules[cmr_index+5],rules[cmr_index+4],get_cell( current, width, height )))
								{
								// East
								if(RESOLVE(rules[cmr_index+7],rules[cmr_index+6],get_cell( current, width+1, height )))
								{
								// South
								if(RESOLVE(rules[cmr_index+9],rules[cmr_index+8],get_cell( current, width, height+1 )))
								// If everything fits => new state
								{
									set_cell( next, rules[cmr_index+10] , width, height );
									break;									
								}}}}}

							}

						// Square
						#elif MOORE

							// Position in cmr rules
							// cmr_index points at start of cmr rule
							for( int cmr_index = 0 ; cmr_index < (CMR_COUNT*CMR_SIZE); cmr_index+=CMR_SIZE )
							{
								// Decision block, every if must be true

								// Northwest
								if(RESOLVE(rules[cmr_index+1],rules[cmr_index],get_cell( current, width-1, height-1 )))
								// North
								if(RESOLVE(rules[cmr_index+3],rules[cmr_index+2],get_cell( current, width, height-1 )))
								// Northeast
								if(RESOLVE(rules[cmr_index+5],rules[cmr_index+4],get_cell( current, width+1, height-1 )))
								// West
								if(RESOLVE(rules[cmr_index+7],rules[cmr_index+6],get_cell( current, width-1, height )))
								// Center
								if(RESOLVE(rules[cmr_index+9],rules[cmr_index+8],get_cell( current, width, height )))
								// East
								if(RESOLVE(rules[cmr_index+11],rules[cmr_index+10],get_cell( current, width+1, height )))
								// Southwest
								if(RESOLVE(rules[cmr_index+13],rules[cmr_index+12],get_cell( current, width-1, height+1 )))
								// South
								if(RESOLVE(rules[cmr_index+15],rules[cmr_index+14],get_cell( current, width, height+1 )))
								// Southeast
								if(RESOLVE(rules[cmr_index+17],rules[cmr_index+16],get_cell( current, width+1, height+1 )))
								{
									set_cell( next, rules[cmr_index+18] , width, height );
									break;	
								}				
							}

						#endif
					}
				}

			#endif

		#endif

	}

	return 0;
}

/*
* Prints CA
* Only capable of printing 1D and 2D automaton
*/
void print_ca( unsigned * CA )
{
	#if CA_DIMENSIONS == 1

		for(int i=0;i<CA_SIZE;i++)
		{
			printf("%d",get_cell(CA,i));
		}
		printf("\n");

	#elif CA_DIMENSIONS == 2

		for(int height=0;height<CA_SIZE;height++)
		{
			for(int width=0;width<CA_SIZE;width++)
			{
				printf("%d ",get_cell(CA,width,height));
			}
			printf("\n");
		}

	#endif 
}

/*
* Initialize CA to specified pattern
* char * filename : path to pattern
*/
void initialize_ca( unsigned * CA , char * filename )
{
	FILE *file;
	file = fopen(filename, "r");

	unsigned state = 0;

	for(int i=0;i< pow(CA_SIZE,CA_DIMENSIONS) && fscanf(file,"%u", &state);i++)
	{
		CA[i]=state;
	}

	fclose(file);
}


/*
* Copies pattern to target automaton
*/
void copy_ca( unsigned * origin, unsigned * target)
{
	for(int i=0;i< pow(CA_SIZE,CA_DIMENSIONS) ;i++)
	{
		target[i]=origin[i];
	}
}