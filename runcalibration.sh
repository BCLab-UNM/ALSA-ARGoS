#!/bin/bash

# Runs through the levy exponent $1 times. The i values are for different levy exponents (mu) where 10 = -1.0, 11 = -1.1 ... 30 = -3.0 etc.
# Repeats the experiment for power law, uniform and clustered distributions.
# 50% target detection rate
for j in 1 2 4 8 16 32
do 	 
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/PowerLaw/Mu${i}"
        mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/PowerLaw/
	./runntimes.sh $1 experiments/Searchers${j}/05SensorErrorRate/Bounds10x10/PowerLaw/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/PowerLaw/${i}.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/Clustered/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/Clustered/
	./runntimes.sh $1 experiments/Searchers${j}/05SensorErrorRate/Bounds10x10/Clustered/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/Clustered/${i}.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/Uniform/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/Uniform
	./runntimes.sh $1 experiments/Searchers${j}/05SensorErrorRate/Bounds10x10/Uniform/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/05SensorErrorRate/Bounds10x10/Uniform/${i}.csv
    done
done

# Large Arena, 100% detection rate 
for j in 1 2 4 8 16 32
do 	 
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/PowerLaw/Mu${i}"
        mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/PowerLaw/
	./runntimes.sh $1 experiments/Searchers${j}/0SensorErrorRate/Bounds40x40/PowerLaw/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/PowerLaw/${i}.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/Clustered/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/Clustered/
	./runntimes.sh $1 experiments/Searchers${j}/0SensorErrorRate/Bounds40x40/Clustered/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/Clustered/${i}.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/Uniform/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/Uniform
	./runntimes.sh $1 experiments/Searchers${j}/0SensorErrorRate/Bounds40x40/Uniform/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds40x40/Uniform/${i}.csv
    done
done

# 100% detection rate, 10x10 arena
for j in 1 2 4 8 16 32
do 	 
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/PowerLaw/Mu${i}"
        mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/PowerLaw/
	./runntimes.sh $1 experiments/Searchers${j}/0SensorErrorRate/Bounds10x10/PowerLaw/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/PowerLaw/${i}.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/Clustered/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/Clustered/
	./runntimes.sh $1 experiments/Searchers${j}/0SensorErrorRate/Bounds10x10/Clustered/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/Clustered/${i}.csv
    done
    
    for i in 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
    do
	echo "/Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/Uniform/Mu${i}"
	mkdir -p /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/Uniform
	./runntimes.sh $1 experiments/Searchers${j}/0SensorErrorRate/Bounds10x10/Uniform/Mu${i}.xml /Users/matthew/ownCloud/Data/Robots/ALSA/results/Searchers${j}/0SensorErrorRate/Bounds10x10/Uniform/${i}.csv
    done
done
