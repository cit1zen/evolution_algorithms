#!/usr/bin/env python2
# Creates experiment with evo. algorithm

import re
import os
import shutil
import logging
import argparse
import ConfigParser


logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
logging.basicConfig()

# Source files
src = ["ea.c", "ea.h", "main.c", "ca.c", "ca.h", "params.h", "makefile"]
# Scripts inside experiment variotion dir
support = ["###QSUB.SH"]
# Scripts, that do not need to be inside exp. var.
# tools = ["run_exp.sh", "make_exp.sh", "stop_exp.sh", "get_res.sh"]
tools = []

def copy_files(directory, tool_dir="tools/", sup_dir="tools/", src_dir="src/"):
    """
    Copies tools, source code and support files into exp. directory.

    Args:
        directory - Main directory path.
    """
    # Support scripts
    for f in [tool_dir + x for x in tools]:
        shutil.copy(f, directory)
    # Support scripts
    for f in [sup_dir + x for x in support]:
        shutil.copy(f, directory)
    # Source files
    os.mkdir(directory + "src/")
    for f in [src_dir + x for x in src]:
        shutil.copy(f, directory + "src/")


def create_symlinks(dir):
    """
    Creates necessary symlinks in experiment directory."

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
    ncpus = config.get('submit', 'npcus')
    mpiprocs = config.get('submit', 'mpiprocs')
    walltime = config.get('submit', 'walltime')
    processors = config.get('submit', 'processors')
    project = config.get('submit', 'project')
    queue = config.get('submit', 'queue')
    submit = ("#!/bin/bash\n"
              "#PBS -q {}\n"
              "#PBS -N CAE\n"
              "#PBS -l select=1:ncpus={}:mpiprocs={},walltime={}\n"
              "#PBS -J 1-{}\n"
              "#PBS -A {}\n"
              "\n"
              "module load OpenMPI\n"
              "\n"
              "cd $PBS_O_WORKDIR\n"
              "\n"
              "mpiexec ./evol ../origin ../target\n"
              ).format(queue, ncpus, mpiprocs, walltime,
                       processors, project)
    with open(target_dir + 'submit.sh', 'w') as f:
        f.write(submit)


def create_params(config, section, target):
    """
    Add experiment specific params to targer file
    (params.h|local_params.h).

    Args:
        config - configparser object.
        section - experiment section
        target - target file.
    """
    params = {}
    for param in [x for x in config.options(section) if 'param_' in x]:
        # KEY - param name
        # VALUE - param value
        # PARAM_WIDTH_PARAM 12
        params[param[param.find('_') + 1:]] = config.get(section, param)
    
    with open(target, 'a') as f:
        f.write('\n')
        for key in params:
            f.write("#define "+ key.upper() + " " + params[key] + '\n')

def main():
    parser = argparse.ArgumentParser(
        description='Create experiment for ANSELM.')
    parser.add_argument('-c', '--config', required=True,
                        help='configuration of experiments')
    parser.add_argument('-o', '--output', default="./experiments",
                        help='output dir for experiments')
    args = parser.parse_args()

    config = ConfigParser.ConfigParser()
    config.read(args.config)

    # Create dirs, where experiment will be stored
    # Every couple have its own dir
    for exp in [x for x in config.sections() if re.match("exp_.+", x)]:
        # Path to pattern dir
        exp_dir = "{}/{}/".format(args.output, exp[exp.find('_') + 1:])
        # Check if already exist, if exist remove
        logger.info("Create experiment in {}".format(exp_dir))
        if os.path.exists(exp_dir):
            logger.error("{} already exist, removing".format(exp_dir))
            shutil.rmtree(exp_dir)
        os.mkdir(exp_dir)
        # Copies source files and etc inside dir
        copy_files(exp_dir)
        # Copies patterns inside
        shutil.copy(config.get(exp, 'origin'), exp_dir + "origin")
        shutil.copy(config.get(exp, 'target'), exp_dir + "target")

        # create submit.sh
        create_submit(config, exp_dir)

        # Creation of sub-experiments
        # difference is only in local_params.h
        for sub_exp in re.split(',', config.get('default', 'params')):
            logger.debug(" Sub-experiment {}".format(sub_exp))
            sub_exp_dir = exp_dir + sub_exp.split("/")[-1] + "/"
            os.mkdir(sub_exp_dir)
            # Variant local.params.h
            shutil.copy(sub_exp, sub_exp_dir + "local_params.h")
            # Add experiment specific params into local_params.h
            create_params(config, exp, sub_exp_dir + "local_params.h")
            # Create symlinks inside experiment dir
            create_symlinks(sub_exp_dir)


if __name__ == "__main__":
    main()
