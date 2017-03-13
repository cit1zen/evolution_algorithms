#!/bin/sh

cd experiments

for experiment in *; do
	if [ -d $experiment ]; then
		cd $experiment
		echo "Making" $experiment
		for set in *; do
			if [ -d $set ] && [ $set != 'src' ]; then
				cd $set
					make
				cd ..
			fi
		done
		cd ..
	fi
done
cd ..