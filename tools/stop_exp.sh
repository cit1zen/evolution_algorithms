#!/bin/sh

for experiment in *; do
	if [ -d $experiment ]; then
		if [ "$experiment" != "src" ]; then
			echo "Terminating" $experiment
			cd $experiment
				./#QDEL.SH
			cd ..
		fi
	fi
done
