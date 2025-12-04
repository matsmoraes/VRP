#!/bin/bash

SIZES=(250 500 750 1000)
HEADER_FILE="include/main.h"
TEMP_RESULTS="all_hv_results.txt"

for size in "${SIZES[@]}"
do

    sed -i "s/^#define NUM_ITENS.*/#define NUM_ITENS $size/" $HEADER_FILE
    
    
    ./compile.sh > /dev/null 2>&1
    
    
    rm -f $TEMP_RESULTS
    
    
    for i in {1..30}
    do
        
        ./output/main > /dev/null 
        
        
        python calculate_hv.py $size
    done
    
    
    mv $TEMP_RESULTS "results_${size}_items.txt"
done