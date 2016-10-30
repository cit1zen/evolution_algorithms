#!/bin/sh

cd src
	for experiment in *; do
		if [ -d $experiment ]; then
			echo "Terminating" $experiment
			cd $experiment
			./#QDEL.SH
			cd ..
		fi
	done
cd ..
