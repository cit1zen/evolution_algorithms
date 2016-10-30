#!/bin/sh

cd src
	for experiment in *; do
		if [ -d $experiment ]; then
			echo "Running" $experiment
			cd $experiment
			./###QSUB.SH
			cd ..
		fi
	done
cd ..
