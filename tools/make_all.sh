#!/bin/sh

./tools/create_exp.py --start sets/sym_xy_axis/seed --target sets/sym_xy_axis/target/* --params sets/sym_xy_axis/50*

./tools/create_exp.py --start sets/test/seed --target sets/test/target --params sets/test/50*