#!/usr/bin/env python2

import re
import os
import json
import random
import argparse

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
    for name in args.input:    
        with open(name, "r") as f:
            # Get experiment results
            # one line one expetiment
            for l in f:
                # HotFix for broken results
                l = re.sub("(?<=[0-9])\|(?=[0-9])", '-', l)

                exp = {}
                for member in l.split("|"):
                    val = member.split(":")
                    if "SUCCESS" in val[0]:
                        SUCC += 1
                    elif "FAIL" in val[0]:
                        FAIL += 1
                        break
                    elif "HEIGHT" in val[0]:
                        exp['ROWS'] = val[1]
                    elif "WIDTH" in val[0]:
                        exp['COLS'] = val[1]
                    elif "CYCLES" in val[0]:
                        exp["CYCLES"] = val[1]
                        AVE_CYC += int(val[1])
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
    print("SUCC: {}".format(SUCC))
    print("FAIL: {}".format(FAIL))
    print("AVERAGE CYCLES: {}".format(AVE_CYC / SUCC))


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