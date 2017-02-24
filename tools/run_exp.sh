#!/bin/sh

for experiment in *; do
	if [ -d $experiment ]; then
		cd $experiment
		echo "Running" $experiment
		for set in *; do
			if [ -d $set ]; then
				echo "  "$set
				cd $set
					./###QSUB.SH
				cd ..
			fi
		done
		cd ..
	fi
done