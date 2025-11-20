#!/bin/bash


rm -f all_hv_results.txt

echo "Iniciando experimento de 10 execuções..."

for i in {1..10}
do
   echo "Execucao $i de 10..."
   
   
   ./output/main > /dev/null 
   
   python calculate_hv.py
done

echo "Experimento concluido! Dados salvos em 'all_hv_results.txt'."
echo "Agora rode: python plot_comparison.py"