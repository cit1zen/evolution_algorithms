#!/bin/sh

cd src
	for experiment in *; do
		if [ -d $experiment ]; then
			echo "Making" $experiment
			cd $experiment
			make
			cd ..
		fi
	done
cd ..