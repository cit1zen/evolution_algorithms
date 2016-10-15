# Author: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT

comp=gcc -O3 -std=c99 -Wall -Wextra -pedantic -lm

# Vytvori vsetky potrebne subory
all: main

# Mainfile
main: ca.o ea.o main.o 
	$(comp) main.o ca.o ea.o -lm -o $@
main.o: main.c params.h
	$(comp) -c main.c -o $@


# Evolution algoriths and fitness functions
ea.o: ea.c ea.h params.h
	$(comp) -c ea.c -o $@

# Cellular automaton
ca.o: ca.c ca.h params.h
	$(comp) -c ca.c -o $@

PHONY: clean clean_o

clean:
	rm *.o main

clean_o:
	rm *.o