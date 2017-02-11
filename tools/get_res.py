#!/usr/bin/env python3

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
    parser.add_argument('input', nargs="+", required=True, help="experiment results")
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

    print("Results of {}".format(os.getcwd().split("/")[-1]))
    # Process experiments
    for name in args.input:    
        with open(name, "r") as f:
            # Get experiment results
            # one line one expetiment
            for l in f:
                experiment = {}
                for member in l.split("|"):
                    val = member.split(":")
                    if "SUCCESS" in val[0]:
                        SUCC += 1
                    elif "FAIL" in val[0]:
                        FAIL += 1
                        break
                    elif "CYCLES" in val[0]:
                        experiment[val[0]] = val[1]
                        AVE_CYC += val[1]
                    elif "RULES" in val[0]:
                        # List compr. because of blank rules
                        experiment[val[0]] = [x for x in val[1].split(" ") if x]
                    else:
                        experiment[val[0]] = val[1]
                        
                # Create json file with rules, if experiment was success
                if "RULES" in experiment:
                    # Add target and origin pattern
                    if origin:
                        experiment["origin"] = origin
                    if target:
                        experiment["target"] = target
                    # Save into the file
                    with open(args.output + str(random.randint(100000, 999999)) + ".json","w") as w:
                        json.dump(experiment, w, ensure_ascii=False)
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
        Dict. containing lattice.
    """
    lat = {}
    lat[rows] = 0
    lat[cols] = 0
    lat[values] = []
    with open(filename, "r") as f:
        for line in f:
            lat[rows] +=1
            for cell in line:
                if lat[rows] == 1:
                    lat[cols] += 1
                lat[values].append(cell)

if __name__ == "__main__":
    main()