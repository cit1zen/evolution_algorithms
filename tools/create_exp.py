#!/usr/bin/env python3
# Creates experiment with evo. algorithm

import os
import shutil
import logging
import argparse
import itertools

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
logging.basicConfig()

# Source files
src = ["ea.c", "ea.h", "main.c", "ca.c", "ca.h", "params.h", "makefile"]
# Scripts inside experiment variotion dir
support = ["###QSUB.SH", "submit.sh", "get_res.py"]
# Scripts, that do not need to be inside exp. var.
tools = ["run_exp.sh", "make_exp.sh", "stop_exp.sh", "get_res.sh"]

def copy_files(directory, tool_dir="tools/", sup_dir="tools/", src_dir="src/"):
    """
    Copies tools, source code and support files into exp. directory.

    :param directory: Main directory path.
    :type directory: str
    """
    # TODO change paths, so they are set by arg
    # Tools
    for f in [tool_dir + x for x in tools]:
        shutil.copy(f, directory)
    # Support scripts
    for f in [sup_dir + x for x in support]:
        shutil.copy(f, directory)
    # Source files
    os.mkdir(directory + "src/")
    for f in [src_dir + x for x in src]:
        shutil.copy(f, directory + "src/")

def create_symlinks(directory):
    """
    Creates necessary symlink inside experiment directiries

    :param directory: Main directory path.
    :type directory: str
    """
    # Experiment variants
    experiments = [directory + d + "/" for d in os.listdir(directory)
                   if os.path.isdir(directory + d) and d != "src"]
    for exp in experiments:
        # Symlinks to source files
        for f in src:
            os.symlink("../src/" + f, exp + f)
        # Symlink to support scripts
        for f in support:
            os.symlink("../" + f, exp + f)

def main():
    parser = argparse.ArgumentParser(description='Create experiment for ANSELM.')
    parser.add_argument('--start', metavar='N', nargs="+",
                        help='start patterns', required=True)
    parser.add_argument('--target', metavar='N', nargs="+",
                        help='target patterns', required=True)
    parser.add_argument('--params', metavar='N', nargs="+",
                        help='local params of evolution', required=True)
    parser.add_argument('-o', '--output', default="./experiments",
                        help='output dir for experiments')
    args = parser.parse_args()

    # We create couples consisting of start and target pattern
    patterns = itertools.product(args.start, args.target)
    # Create dirs, where experiment will be stored
    # Every couple have its own dir
    for pat in patterns:
        # Path to pattern dir
        pat_dir = "{}/{}-{}/".format(args.output, pat[0].split("/")[-1],
                                     pat[1].split("/")[-1])
        # Check if already exist, if exist remove
        logger.info("Create experiment in {}".format(pat_dir))
        if os.path.exists(pat_dir):
            logger.error("{} already exist, removing".format(pat_dir))
            shutil.rmtree(pat_dir)
        os.mkdir(pat_dir)
        # Copies source files and etc inside dir
        copy_files(pat_dir)
        # Copies patterns inside
        shutil.copy(pat[0], pat_dir + "origin")
        shutil.copy(pat[1], pat_dir + "target")

        # Creation of experiment
        # difference is only in local_params.h
        for exp in args.params:
            logger.debug(" Sub-experiment {}".format(exp))
            exp_dir = pat_dir + exp.split("/")[-1] + "/"
            os.mkdir(exp_dir)
            # Variant local.params.h
            shutil.copy(exp, exp_dir + "local_params.h")

        # Create symlinks inside experiment dirs
        create_symlinks(pat_dir)

if __name__ == "__main__":
    main()