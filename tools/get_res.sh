#!/bin/sh

mkdir -p _results

cp -n default.cas _results
cp -n pyca_capt.py _results

for hdir in *; do
    if [ -d $hdir ] && [[ $hdir != _* ]]
    then
        echo "Results in" $hdir
        cd $hdir
        ./sumres.py
        for ldir in *; do
            if [ -d $ldir ]; then
                cd $ldir
                ./2dca_evol results.cmr
                for ftab in *; do
                    if [[ $ftab == *.tab ]]
                    then
                        cp -n $ftab "../../_results/$hdir$ldir$ftab"
                    fi
                done
                cd ..
            fi
        done
        cd ..
    fi
done
