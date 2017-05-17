
/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: Cellular automaton - code
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
unsigned get_cell( unsigned * CA, unsigned width, unsigned height)
{
	if ((width < WIDTH_PARAM) && (height < HEIGHT_PARAM))
	{
		return CA[(height*WIDTH_PARAM)+width];
	}
	else
	{	
		return 0;
	}
}

/*
* Sets cell state
* unsigned new_state
* unsigned ... coordinates of cell
* x axis, y axis, z axis, ...
*/
void set_cell( unsigned * CA, unsigned new_state, unsigned width, unsigned height)
{
	if ((width < WIDTH_PARAM) && (height < HEIGHT_PARAM))
	{
		CA[(height*WIDTH_PARAM)+width] = new_state;
	}
}

/*
* Do one generation/cycle of CA
* pointer to CA structure
* pointer CMR rules
*/
void do_cycle( unsigned * current, unsigned * next, unsigned * rules)
{
	for(unsigned height=0;height<HEIGHT_PARAM;height++)
	{
		for(unsigned width=0;width<WIDTH_PARAM;width++)
		{
			// Cross
			#ifdef VON_NEUMANN

				// Position in cmr rules
				// cmr_index points at start of cmr rule
				unsigned changed = 0;
				for( int cmr_index = 0 ; cmr_index < (CMR_COUNT*CMR_SIZE); cmr_index+=CMR_SIZE )
				{
					// Skip rules that cause blinking of lattice
					#ifdef SKIP_BLINK
						// Decision block, every if must be true
						// North
						if(RESOLVE(rules[cmr_index+1],rules[cmr_index], 0))
						{
						// West
						if(RESOLVE(rules[cmr_index+3],rules[cmr_index+2], 0))
						{
						// Center
						if(RESOLVE(rules[cmr_index+5],rules[cmr_index+4], 0))
						{
						// East
						if(RESOLVE(rules[cmr_index+7],rules[cmr_index+6], 0))
						{
						// South
						if(RESOLVE(rules[cmr_index+9],rules[cmr_index+8], 0))
						// If everything fits => new state
						{
							continue;
						}}}}}
					#endif
					
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
						set_cell(next, rules[cmr_index+10], width, height);
						changed = 1;
						break;								
					}}}}}
				}
				if(!changed)
				{
					set_cell(next, get_cell( current, width, height ), width, height);
				}

			// Square
			#elif MOORE

				// Position in cmr rules
				// cmr_index points at start of cmr rule
				unsigned changed = 0;
				for( int cmr_index = 0 ; cmr_index < (CMR_COUNT*CMR_SIZE); cmr_index+=CMR_SIZE )
				{
					// Skip rules that cause blinking of lattice
					#ifdef SKIP_BLINK
						// Northwest
						if(RESOLVE(rules[cmr_index+1],rules[cmr_index], 0))
						// North
						if(RESOLVE(rules[cmr_index+3],rules[cmr_index+2], 0))
						// Northeast
						if(RESOLVE(rules[cmr_index+5],rules[cmr_index+4], 0))
						// West
						if(RESOLVE(rules[cmr_index+7],rules[cmr_index+6], 0))
						// Center
						if(RESOLVE(rules[cmr_index+9],rules[cmr_index+8], 0))
						// East
						if(RESOLVE(rules[cmr_index+11],rules[cmr_index+10], 0))
						// Southwest
						if(RESOLVE(rules[cmr_index+13],rules[cmr_index+12], 0))
						// South
						if(RESOLVE(rules[cmr_index+15],rules[cmr_index+14], 0))
						// Southeast
						if(RESOLVE(rules[cmr_index+17],rules[cmr_index+16], 0))
						{
							continue;
						}
					#endif

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
						changed = 1;
						break;	
					}				
				}
				if(!changed)
				{
					set_cell(next, get_cell( current, width, height ), width, height);
				}

			#endif
		}
	}
}

/*
* Prints CA
* Only capable of printing 1D and 2D automaton
*/
void print_ca( unsigned * CA )
{
	for(int height=0;height<HEIGHT_PARAM;height++)
	{
		for(int width=0;width<WIDTH_PARAM;width++)
		{
			printf("%d ",get_cell(CA,width,height));
		}
		printf("\n");
	}
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

	for(int i=0;i< HEIGHT_PARAM*WIDTH_PARAM && fscanf(file,"%u", &state);i++)
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
	for(int i=0;i< HEIGHT_PARAM*WIDTH_PARAM ;i++)
	{
		target[i]=origin[i];
	}
}