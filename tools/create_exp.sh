#!/bin/sh
# Author: Adam Ormandy <xorman00@stud.fit.vutbr.cz>

# Paths
source_dir=src/
local_par_dir=src/experiment_params/


# Simple args
if [ $# -eq 3 ]; then
	experiment_name=$1
    target_pattern=$2
    origin_pattern=$3
else
    echo "Input not valid"
    exit 1
fi


# Create experiment dir
experiment_dir="experiments/"$experiment_name
mkdir $experiment_dir
echo "Create experiment in " $experiment_dir


# Copy run_exp.sh
cp "tools/run_exp.sh" $experiment_dir

# Copy make_exp.sh
cp "tools/make_exp.sh" $experiment_dir

# Copy stop_exp.sh
cp "tools/stop_exp.sh" $experiment_dir

# Copy submit.sh
cp "tools/submit.sh" $experiment_dir

# Copy ###QSUB.SH
cp "tools/###QSUB.SH" $experiment_dir

# Copy target and origin pattern
cp $target_pattern $experiment_dir"/target"
cp $origin_pattern $experiment_dir"/origin"

# Copy source code
mkdir $experiment_dir"/src/"
for source_file in "$source_dir"*
do
    if [[ -f $source_file ]]
    then
        cp $source_file $experiment_dir"/src/"
    fi
done


# Create experiments
for entry in "$local_par_dir"*
do
    echo "Experiment " $(echo "$entry" | grep -oE -e "[^/]+" | grep -oE -e ".+\.h" | grep -oE -e "^[^.]+")

    mkdir $experiment_dir"/src/"$(echo "$entry" | grep -oE -e "[^/]+" | grep -oE -e ".+\.h" | grep -oE -e "^[^.]+")

    sub_dir=$experiment_dir"/src/"$(echo "$entry" | grep -oE -e "[^/]+" | grep -oE -e ".+\.h" | grep -oE -e "^[^.]+")"/"

    # Copy of experiment local_param.h
    cp $entry $sub_dir"local_params.h"
done

# Create symlink in experiments
cd $experiment_dir"/src/"
    for experiment in *
    do
        if [[ -d $experiment ]]
        then
            cd $experiment

                # Symlink to origin and target pattern
                ln -s "../../target"
                ln -s "../../origin"

                # Symlinks to source files
                for source in ../*
                do
                    if [[ -f $source ]]
                    then
                        ln -s $source
                    fi
                done

                # Symlink submit.sh
                ln -s "../../submit.sh"

                # Symlink to ###QSUB.SH
                ln -s "../../###QSUB.SH"
            cd ..
        fi
    done
cd "../../../"
    
    



    
    


