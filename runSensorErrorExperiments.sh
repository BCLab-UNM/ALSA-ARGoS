#!/bin/bash

# Runs through the levy exponent $1 times. The i values are for different levy exponents (mu) where 10 = -1.0, 11 = -1.1 ... 30 = -3.0 etc.
# Repeats the experiment for power law, uniform and clustered distributions.
# 50% target detection rate

for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
do
    for j in 0 0125 025 0375 05 0625 075 0875 1
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/PowerLaw/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/PowerLaw/
	./runntimes.sh $1 experiments/Searchers8/${j}SensorErrorRate/Bounds40x40/PowerLaw/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/PowerLaw/${i}.csv

	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/Uniform/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/Uniform/
	./runntimes.sh $1 experiments/Searchers8/${j}SensorErrorRate/Bounds40x40/Uniform/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/Uniform/${i}.csv
	
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/Clustered/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/Clustered/
	./runntimes.sh $1 experiments/Searchers8/${j}SensorErrorRate/Bounds40x40/Clustered/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers8/${j}SensorErrorRate/Bounds40x40/Clustered/${i}.csv
    done
done

