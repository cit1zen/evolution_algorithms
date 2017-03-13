#!/bin/bash
#PBS -q qexp
#PBS -N CAE
#PBS -l select=1:ncpus=16:mpiprocs=16,walltime=1:00:00
#PBS -J 1-6
#PBS -A OPEN-8-36

module load OpenMPI

cd $PBS_O_WORKDIR

mpiexec ./evol ../origin ../target
