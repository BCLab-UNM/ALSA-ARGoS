#!/bin/bash

# Runs through the levy exponent $1 times. The i values are for different levy exponents (mu) where 10 = -1.0, 11 = -1.1 ... 30 = -3.0 etc.
# Repeats the experiment for power law, uniform and clustered distributions.
for j in 1 2 4 8 16 32
do 	 
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	./runntimes.sh $1 experiments/Calibration${j}/ALSAPL${i}Mu${j}Searchers.xml results/Calibration${j}/05SensorError/ALSAPL${i}Mu${j}Searchers075SensorError.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	./runntimes.sh $1 experiments/Calibration${j}/ALSAC${i}Mu${j}Searchers.xml results/Calibration${j}/05SensorError/ALSAC${i}Mu${j}Searchers075SensorError.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	./runntimes.sh $1 experiments/Calibration${j}/ALSAU${i}Mu${j}Searchers.xml results/Calibration${j}/05SensorError/ALSAU${i}Mu${j}Searchers075SensorError.csv
    done
done
