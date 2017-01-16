#!/bin/sh

for experiment in *; do
	if [ -d $experiment ]; then
		if [ "$experiment" != "src" ]; then
			echo "Running" $experiment
			cd $experiment
				./###QSUB.SH
			cd ..
		fi
	fi
done