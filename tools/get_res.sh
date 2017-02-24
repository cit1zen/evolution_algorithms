#!/bin/sh

mkdir -p ../_results

for experiment in *; do
	if [ -d $experiment ]; then
		cd $experiment
		echo "Processing" $experiment
		for set in *; do
			if [ -d $set ]; then
				cd $set
					../../../tools/get_res.py * --output ../../../_results/ --origin origin --target target
				cd ..
			fi
		done
		cd ..
	fi
done
