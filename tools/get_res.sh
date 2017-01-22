#!/bin/sh

mkdir -p _results

for hdir in *; do
    if [ -d $hdir ] && [[[ $hdir != "src" ] || [ $hdir != "_results" ]]]
    then
        cd $hdir
        ../get_res.py --output ../_results/
        cd ..
    fi
done
