#!/bin/bash
#PBS -q qfree
#PBS -N CAE
#PBS -l select=1:ncpus=16:mpiprocs=16,walltime=1:00:00
#PBS -J 1-6
#PBS -A OPEN-7-19

module load OpenMPI

cd $PBS_O_WORKDIR

#mpiexec -bycore -bind-to-core ./test
mpiexec ./evol
