#!/bin/bash
for i in 20 40 60 80 100 150 200 250
do
    ./runntimes.sh $1 experiments/ALSAU60minTargets${i}.xml results/ALSAU60minTargets${i}.txt
done
