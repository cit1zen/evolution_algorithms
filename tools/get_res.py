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
    # TODO input
    parser = argparse.ArgumentParser(description='Get results of experiment.')
    parser.add_argument('--output', default="./", help="Output directory")
    args = parser.parse_args()

    SUCC = 0
    FAIL = 0

    print("Results of {}".format(os.getcwd().split("/")[-1]))
    for name in [f for f in os.listdir('.') if re.match("^CAE.*", f)]:
        with open(name, "r") as f:
            for l in f:
                # Get experiment results
                experiment = {}
                for member in l.split("|"):
                    val = member.split(":")
                    if "SUCCESS" in val[0]:
                        SUCC += 1
                    elif "FAIL" in val[0]:
                        FAIL += 1
                        break
                    elif "RULES" in val[0]:
                        experiment[val[0]] = val[1].split(" ")
                    else:
                        experiment[val[0]] = val[1]
                # Writes founded rules into file
                if "RULES" in experiment:
                    with open(args.output + str(random.randint(100000, 999999)) + ".json","w") as w:
                        json.dump(experiment, w, ensure_ascii=False)
    print("SUCC: {}".format(SUCC))
    print("FAIL: {}".format(FAIL))

if __name__ == "__main__":
    main()