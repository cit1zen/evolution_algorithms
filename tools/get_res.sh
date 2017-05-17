#!/bin/sh
# Get results and statistics

mkdir -p ./_results

cd 'experiments'
	for experiment in *; do
		if [ -d $experiment ]; then
			mkdir -p ../_results/$experiment
			cd $experiment
			echo "Processing" $experiment
			for set in *; do
				if [ -d $set ] && [ $set != 'src' ]; then
					mkdir -p ../../_results/$experiment/$set
					cd $set
						../../../tools/get_res.py * --output ../../../_results/$experiment/$set/ --origin ../origin --target ../target
					cd ..
				fi
			done
			echo " "
			cd ..
		fi
	done
cd ..
