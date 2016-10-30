#!/bin/bash

for ident in `qstat -u xorman00 | grep xorman00 | awk -F ' ' '{ print $1 }'`
do
        ident="${ident/'[]'/[1-6]}"
        qstat -a $ident | grep xorman00
done
