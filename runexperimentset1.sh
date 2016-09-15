#!/bin/bash
for i in 1 2 4 6 8 10 15 20 25 30
do
    ./runntimes.sh $1 experiments/ALSAPL${i}Searchers.xml results/ALSAPL${i}Searchers.txt
done
