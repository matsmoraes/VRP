import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pymoo.indicators.hv import HV


try:
    # Carrega os dados do seu arquivo
    df = pd.read_csv('final_pareto_front.csv')
    raw_data = df.values

    max_theoretical_profit = 75000.0 
    
    # Divide seus dados pelo máximo para ficar entre 0 e 1
    normalized_data = raw_data / max_theoretical_profit

    # Inverte o sinal para o pymoo (Maximização -> Minimização)
    data_for_calc = -1 * normalized_data

    # Calcula o HV normalizado
    ref_point = np.array([0, 0, 0]) 
    ind = HV(ref_point=ref_point)
    
    hv_ammt_normalized = ind(data_for_calc)
    
    print(f"HV Absoluto (estimado): {ind(raw_data * -1):.2e}")
    print(f"HV Normalizado (para o gráfico): {hv_ammt_normalized:.4f}")

except Exception as e:
    print(f"Erro ao ler/calcular: {e}")
    hv_ammt_normalized = 0.0


hv_nsga3_paper = 0.020 

algoritmos = ['AMMT (Seu Trabalho)', 'NSGA-III (Artigo)']
valores_hv = [hv_ammt_normalized, hv_nsga3_paper]
cores = ['#4CAF50', '#2196F3'] # Verde vs Azul

plt.figure(figsize=(10, 6))
bars = plt.bar(algoritmos, valores_hv, color=cores, width=0.5)

for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 0.0005, f'{yval:.4f}', 
             va='bottom', ha='center', fontsize=14, fontweight='bold')

plt.ylabel('Hypervolume (Normalizado 0-1)', fontsize=12)
plt.title('Comparação de Qualidade Normalizada\n(MOKP 750 Itens, 3 Objetivos)', fontsize=14)
plt.ylim(0, max(valores_hv) * 1.2) 
plt.grid(axis='y', linestyle='--', alpha=0.5)

plt.savefig('comparacao_final.png')
print("\nGráfico salvo como 'comparacao_final.png'")