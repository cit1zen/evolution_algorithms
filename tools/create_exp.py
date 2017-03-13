#!/usr/bin/env python3
# Creates experiment with evo. algorithm

import re
import os
import shutil
import logging
import argparse
import configparser


logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
logging.basicConfig()

# Source files
src = ["ea.c", "ea.h", "main.c", "ca.c", "ca.h", "params.h", "makefile"]
# Scripts inside experiment variotion dir
support = ["###QSUB.SH", "get_res.py"]
# Scripts, that do not need to be inside exp. var.
tools = ["run_exp.sh", "make_exp.sh", "stop_exp.sh", "get_res.sh"]


def copy_files(directory, tool_dir="tools/", sup_dir="tools/", src_dir="src/"):
    """
    Copies tools, source code and support files into exp. directory.

    Args:
        directory - Main directory path.
    """
    # TODO change paths, so they are set by arg
    # Tools
    # for f in [tool_dir + x for x in tools]:
    #     shutil.copy(f, directory)
    # Support scripts
    for f in [sup_dir + x for x in support]:
        shutil.copy(f, directory)


def create_symlinks(dir):
    """
    Creates necessary symlinks in experiment directory.

    Args:
        dir - Target direcory.
    """
    # Symlinks to source files
    for f in src:
        os.symlink("../src/" + f, dir + f)
    # Symlink to support scripts
    for f in support:
        os.symlink("../" + f, dir + f)
    os.symlink("../submit.sh", dir + 'submit.sh')


def create_submit(config, target_dir):
    """
    Create submit file using config.

    Args:
        config - Path to configuration file.
        target_dir - Target directory.
    """
    ncpus = config['submit']['npcus']
    mpiprocs = config['submit']['mpiprocs']
    walltime = config['submit']['walltime']
    processors = config['submit']['processors']
    project = config['submit']['project']
    queue = config['submit']['queue']
    submit = ("""#!/bin/bash
              #PBS -q {}
              #PBS -N CAE
              #PBS -l select=1:ncpus={}:mpiprocs={},walltime={}
              #PBS -J 1-{}
              #PBS -A {}

              module load OpenMPI

              cd $PBS_O_WORKDIR

              mpiexec ./evol ../origin ../target
              """).format(queue, ncpus, mpiprocs, walltime,
                          processors, project)
    with open(target_dir + 'submit.sh', 'w') as f:
        f.write(submit)


def main():
    parser = argparse.ArgumentParser(
        description='Create experiment for ANSELM.')
    parser.add_argument('-c', '--config', required=True,
                        help='configuration of experiments')
    parser.add_argument('-o', '--output', default="./experiments",
                        help='output dir for experiments')
    args = parser.parse_args()

    config = configparser.ConfigParser()
    config.read(args.config)

    # Create dirs, where experiment will be stored
    # Every couple have its own dir
    for pat in [x for x in config.sections() if re.match("exp_.+", x)]:
        # Path to pattern dir
        pat_dir = "{}/{}/".format(args.output, pat[pat.find('_') + 1:])
        # Check if already exist, if exist remove
        logger.info("Create experiment in {}".format(pat_dir))
        if os.path.exists(pat_dir):
            logger.error("{} already exist, removing".format(pat_dir))
            shutil.rmtree(pat_dir)
        os.mkdir(pat_dir)
        # Copies source files and etc inside dir
        copy_files(pat_dir)
        # Copies patterns inside
        shutil.copy(config[pat]['origin'], pat_dir + "origin")
        shutil.copy(config[pat]['target'], pat_dir + "target")

        # create submit.sh
        create_submit(config, pat_dir)

        # Creation of experiment
        # difference is only in local_params.h
        for exp in re.split(',', config['default']['params']):
            logger.debug(" Sub-experiment {}".format(exp))
            exp_dir = pat_dir + exp.split("/")[-1] + "/"
            os.mkdir(exp_dir)
            # Variant local.params.h
            shutil.copy(exp, exp_dir + "local_params.h")
            # Create symlinks inside experiment dir
            create_symlinks(exp_dir)


if __name__ == "__main__":
    main()
