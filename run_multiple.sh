#!/bin/bash

rm -f all_hv_results.txt

TOTAL_RUNS=30

echo "Iniciando experimento de $TOTAL_RUNS execuções (simulando 30 instâncias aleatórias)..."

for i in $(seq 1 $TOTAL_RUNS)
do
   echo "Execucao $i de $TOTAL_RUNS..."
   
   ./output/main > /dev/null 
   
   python calculate_hv.py
done

echo "Experimento concluido! Dados salvos em 'all_hv_results.txt'."
echo "Agora rode: python plot_comparison.py"