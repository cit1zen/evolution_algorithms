#!/bin/sh

mkdir -p _results

for hdir in *; do
    if [ -d $hdir ] && [[ $hdir != "src" ]]
    then
        cd $hdir
        ../get_res.py $hdir/* --output ../_results/ --origin origin --target target
        cd ..
    fi
done
