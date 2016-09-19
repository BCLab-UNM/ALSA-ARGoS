#!/bin/bash

# Runs through the levy exponent $1 times. The i values are for different levy exponents (mu) where 10 = -1.0, 11 = -1.1 ... 30 = -3.0 etc.
# Repeats the experiment for power law, uniform and clustered distributions.
for i in 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
do
    ./runntimes.sh $1 experiments/ALSAPL${i}Mu.xml results/ALSAPL${i}Mu.txt
done

for i in 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
do
    ./runntimes.sh $1 experiments/ALSAC${i}Mu.xml results/ALSAC${i}Mu.txt
done

for i in 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
do
    ./runntimes.sh $1 experiments/ALSAU${i}Mu.xml results/ALSAU${i}Mu.txt
done
