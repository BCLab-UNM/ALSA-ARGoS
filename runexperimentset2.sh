#!/bin/bash
for i in 0 2 4 6 8 10 15 20 25 30
do
    ./runntimes.sh $1 experiments/ALSAPLError${i}.xml results/ALSAPLError${i}.txt
done
