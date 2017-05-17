# Author: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT

comp=gcc -O3 -std=c99 -Wall -Wextra -pedantic -lm

all: evol

evol: ca.o ea.o evol.o 
	$(comp) $@.o ca.o ea.o -lm -o $@
evol.o: main.c params.h local_params.h
	$(comp) -c main.c -o $@

ea.o: ea.c ea.h params.h local_params.h
	$(comp) -c ea.c -o $@

ca.o: ca.c ca.h params.h local_params.h
	$(comp) -c ca.c -o $@

PHONY: clean
clean:
	rm *.o evol
