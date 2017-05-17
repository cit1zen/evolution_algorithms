#!/bin/sh
# Stop all experiments

cd experiments

for experiment in *; do
	if [ -d $experiment ]; then
		cd $experiment
		echo "Terminating" $experiment
		for set in *; do
			if [ -d $set ] && [ $set != 'src' ]; then
				cd $set
					./#QDEL.SH
				cd ..
			fi
		done
		cd ..
	fi
done

cd ..
