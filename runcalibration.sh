#!/bin/bash

# Runs through the levy exponent $1 times. The i values are for different levy exponents (mu) where 10 = -1.0, 11 = -1.1 ... 30 = -3.0 etc.
# Repeats the experiment for power law, uniform and clustered distributions.
for j in 1 2 4 8 16 32
do 	 
    for i in 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	./runntimes.sh $1 experiments/ALSAPL${i}Mu${j}Searchers.xml results/ALSAPL${i}Mu${j}Searchers.txt &
    done
    
    for i in 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	./runntimes.sh $1 experiments/ALSAC${i}Mu${j}Searchers.xml results/ALSAC${i}Mu${j}Searchers.txt &
    done
    
    for i in 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	./runntimes.sh $1 experiments/ALSAU${i}Mu${j}Searchers.xml results/ALSAU${i}Mu${j}Searchers.txt &
    done
done
