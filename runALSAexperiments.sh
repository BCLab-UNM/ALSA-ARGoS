#!/bin/bash
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers8Mu17.xml results/ALSAPL30Min40x40Searchers8Mu17.csv
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers16Mu17.xml results/ALSAPL30Min40x40Searchers16Mu17.csv
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers32Mu17.xml results/ALSAPL30Min40x40Searchers32Mu17.csv
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers64Mu17.xml results/ALSAPL30Min40x40Searchers64Mu17.csv

./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers8Mu24.xml results/ALSAPL30Min40x40Searchers8Mu24.csv
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers16Mu24.xml results/ALSAPL30Min40x40Searchers16Mu24.csv
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers32Mu24.xml results/ALSAPL30Min40x40Searchers32Mu24.csv
./runntimes.sh $1 experiments/ALSAPL30Min40x40Searchers64Mu24.xml results/ALSAPL30Min40x40Searchers64Mu24.csv


