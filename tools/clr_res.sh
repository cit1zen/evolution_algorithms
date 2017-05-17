#!/bin/sh
# Remove results

rm -r _results

cd experiments

for experiment in *; do
	if [ -d $experiment ]; then
		cd $experiment
		echo "Clearing" $experiment
		for set in *; do
			if [ -d $set ]; then
				echo "  "$set
				cd $set
					rm *.o *.e
				cd ..
			fi
		done
		cd ..
	fi
done

cd ..