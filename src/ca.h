/*
* Author: xorman00  <xorman00@stud.fit.vutbr.cz>
* Description: Cellular automaton - header
*/

#ifndef CA_H
#define CA_H

/*
* Gets cell state
* unsigned ... coordinates of cell
*/
unsigned get_cell( unsigned * CA, unsigned width, unsigned height);

/*
* Sets cell state
* unsigned new_state
* unsigned ... coordinates of cell
*/
void set_cell( unsigned * CA, unsigned new_state, unsigned width, unsigned height);

/*
* Do one generation/cycle of CA
* pointer to CA structure
* pointer CMR rules
*/
void do_cycle( unsigned * current, unsigned * next, unsigned * rules);

/*
* Prints CA
* Only capable of printing 1D and 2D automaton
*/
void print_ca( unsigned * CA );

/*
* Initialize CA to specified pattern
* char * filename: path to pattern
*/
void initialize_ca( unsigned * CA , char * filename );

/*
* Copies origin to target automaton
*/
void copy_ca( unsigned * origin, unsigned * target);

/*
* Resolves chromosome coding
* CMR coding:
* 0  cell_state >= condition_state
* 1  cell_state <= condition_state
* 2  cell_state == condition_state
* 3  cell_state != condition_state
* Structure XAXAXAXAX where X is state and A is cmr condition
*/
#define RESOLVE(condition_code,condition_state,cell_state)						\
	( (condition_code == 0) ? (cell_state >= condition_state ? true : false ) : \
	( (condition_code == 1) ? (cell_state <= condition_state ? true : false ) : \
	( (condition_code == 2) ? (cell_state == condition_state ? true : false ) : \
	( (condition_code == 3) ? (cell_state != condition_state ? true : false ) : false ) ) ) )

#endif