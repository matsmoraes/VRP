#!/bin/bash


HEADER_FILE="include/main.h"
TEMP_RESULTS="temp_run_data.txt"
OUTPUT_CSV="param_study_results.csv"


echo "Size,Selection,Mutation,Elitism,MinHV,MaxHV,AvgHV" > $OUTPUT_CSV


SIZES=(250 500 750 1000)
SELECTIONS=(1 2)  # 1=Roleta, 2=Torneio
MUT_LABELS=("Dynamic" "0.01" "0.05")
MUT_VALUES=("(1.0 \/ (double)NUM_ITENS)" "0.01" "0.05")

echo "========================================================"
echo " INICIANDO ESTUDO DE PARÂMETROS (FULL GRID)"
echo "========================================================"

for size in "${SIZES[@]}"
do
    sed -i "s/^#define NUM_ITENS.*/#define NUM_ITENS $size/" $HEADER_FILE
    
    for sel in "${SELECTIONS[@]}"
    do
        if [ "$sel" -eq 1 ]; then SEL_NAME="Roulette"; else SEL_NAME="Tournament"; fi

        for i in "${!MUT_LABELS[@]}"
        do
            mut_label="${MUT_LABELS[$i]}"
            mut_value="${MUT_VALUES[$i]}"

            sed -i "s/^#define SELECTION.*/#define SELECTION $sel/" $HEADER_FILE
            sed -i "s/^#define MUTATIONRATE.*/#define MUTATIONRATE $mut_value/" $HEADER_FILE

            
            elitism_val=$(grep "^#define ELITISMRATE" $HEADER_FILE | awk '{print $3}')

            echo ">> Config: Size=$size | Sel=$SEL_NAME | Mut=$mut_label | Elit=$elitism_val"

           
            ./compile.sh > /dev/null 2>&1
            
            
            rm -f $TEMP_RESULTS
            for run in {1..30}
            do
                ./output/main > /dev/null
                python calculate_hv.py $size >> $TEMP_RESULTS
            done

            stats=$(python3 -c "import numpy as np; d=np.loadtxt('$TEMP_RESULTS'); d=np.atleast_1d(d); print(f'{np.min(d):.5f},{np.max(d):.5f},{np.mean(d):.5f}')")
            
            echo "$size,$SEL_NAME,$mut_label,$elitism_val,$stats" >> $OUTPUT_CSV
        done
    done
done

echo ""
echo "Estudo finalizado! Dados salvos em $OUTPUT_CSV"
rm -f $TEMP_RESULTS