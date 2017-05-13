#!/usr/bin/env python2

import re
import os
import math
import json
import random
import argparse

import numpy

def main():
    """
    Prints out experiment statistics and creates json files
    with founded rules.
    """
    parser = argparse.ArgumentParser(description='Get results of experiment.')
    parser.add_argument('input', nargs="+", help="experiment results")
    parser.add_argument('--output', default="./", help="output directory")
    parser.add_argument('--origin', help="origin pattern")
    parser.add_argument('--target', help="target pattern")
    args = parser.parse_args()

    SUCC = 0
    FAIL = 0
    AVE_CYC = 0
    AVE_GEN = 0

    # Get origin pattern, seed
    origin = {}
    if args.origin:
        origin = load_lattice(args.origin)

    # Get target pattern
    target = {}
    if args.target:
        target = load_lattice(args.target)

    print("Results of %s" % os.getcwd().split("/")[-1])
    # Process experiments
    generations = []
    for name in args.input:
        # Not result file
        if 'CAE' not in name:
            continue
        # List containing how many generations
        # were needed to find each solution
        with open(name, "r") as f:
            # Get experiment results
            # one line one expetiment
            for l in f:
                # HotFix for broken results
                # l = re.sub("(?<=[0-9])\|(?=[0-9])", '-', l)

                exp = {}
                for member in l.split("|"):
                    val = member.split(":")
                    if "SUCCESS" in val[0]:
                        SUCC += 1
                    if "FAIL" in val[0]:
                        FAIL += 1
                        break
                    elif "HEIGHT" in val[0]:
                        exp['ROWS'] = val[1]
                    elif "WIDTH" in val[0]:
                        exp['COLS'] = val[1]
                    elif "GENERATION" in val[0]:
                        generations.append(int(val[1]))
                    elif "RULES" in val[0]:
                        # List compr. because of blank rules
                        val[0] = re.sub("\n", '', val[0])
                        exp[val[0]] = [[y for y in x.split('-')]
                                       for x in val[1].split(" ") if x]
                    else:
                        try:
                            exp[val[0]] = val[1]
                        except IndexError:
                            pass
                        
                # Create json file with rules, if experiment was success
                if "RULES" in exp:
                    # Add target and origin pattern
                    if origin:
                        exp["origin"] = origin
                    if target:
                        exp["target"] = target
                    # Save into the file
                    with open(args.output + exp['SEED'] + ".json","w") as w:
                        json.dump(exp, w,
                                  separators=(',', ':'),
                                  ensure_ascii=False,
                                  indent=4)
    # Print out stats
    print("SUCC: {}".format(len(generations)))
    print("MAX: {}".format(max(generations) if generations else '-'))
    print("MIN: {}".format(min(generations) if generations else '-'))
    print("AVE: {}".format(numpy.mean(generations)) if generations else '-')
    print("STD: {}".format(numpy.std(generations)) if generations else '-')


def load_lattice(filename):
    """
    Loads lattice from file

    Args:
        filename - File containing lattice.

    Returns:
        Lattice(List).
    """
    lat = []
    with open(filename, "r") as f:
        for line in f:
            lat.append([])
            for cell in line.split(' '):
                lat[-1].append(cell)
    return lat

if __name__ == "__main__":
    main()