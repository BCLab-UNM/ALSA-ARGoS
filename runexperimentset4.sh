#!/bin/bash
for i in 10 12 14 16 18 20
do
    ./runntimes.sh $1 experiments/ALSAU60minArea${i}x${i}.xml results/ALSAU60minArea${i}x${i}.txt
done
