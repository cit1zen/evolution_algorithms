# Author: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT

comp=gcc -O3 -std=c99 -Wall -Wextra -pedantic -lm

# Vytvori vsetky potrebne subory
all: evol

evol: ca.o ea.o evol.o 
	$(comp) $@.o ca.o ea.o -lm -o $@
evol.o: $(source_dir)main.c $(source_dir)params.h
	$(comp) -c $(source_dir)main.c -o $@

ea.o: $(source_dir)ea.c $(source_dir)ea.h $(source_dir)params.h
	$(comp) -c $(source_dir)ea.c -o $@

ca.o: $(source_dir)ca.c $(source_dir)ca.h $(source_dir)params.h
	$(comp) -c $(source_dir)ca.c -o $@

PHONY: clean

clean:
	rm *.o evol