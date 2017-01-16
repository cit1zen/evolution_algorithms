#!/bin/sh

for experiment in *; do
	if [ -d $experiment ]; then
		if [ "$experiment" != "src" ]; then
			echo "Making" $experiment
			cd $experiment
				make
			cd ..
		fi
	fi
done