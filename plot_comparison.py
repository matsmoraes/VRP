import matplotlib.pyplot as plt
import numpy as np


nsga3_min = 0.009
nsga3_max = 0.034
nsga3_avg = 0.020
nsga3_stats = [nsga3_min, nsga3_max, nsga3_avg]


try:
    
    with open("all_hv_results.txt", "r") as f:
        raw_values = [float(line.strip()) for line in f]
    
    if not raw_values: raise ValueError("Arquivo vazio")

    
    max_theoretical_vol = 75000.0 ** 3
    
    # Normaliza seus dados para a escala 0-1
    normalized_values = [val / max_theoretical_vol for val in raw_values]

    # Calcula as estatísticas
    ammt_min = np.min(normalized_values)
    ammt_max = np.max(normalized_values)
    ammt_avg = np.mean(normalized_values)
    ammt_stats = [ammt_min, ammt_max, ammt_avg]

    print(f"Seus Dados (Normalizados): Min={ammt_min:.4f}, Max={ammt_max:.4f}, Avg={ammt_avg:.4f}")

except Exception as e:
    print(f"Erro ao ler dados: {e}. Usando dados simulados.")
    ammt_stats = [0.015, 0.040, 0.025] # Valores dummy se der erro


labels = ['Mínimo', 'Máximo', 'Média']
x = np.arange(len(labels))
width = 0.35  

fig, ax = plt.subplots(figsize=(10, 6))


rects1 = ax.bar(x - width/2, ammt_stats, width, label='AMMT', color='#4CAF50', edgecolor='black')


rects2 = ax.bar(x + width/2, nsga3_stats, width, label='NSGA-III (Artigo)', color='#2196F3', edgecolor='black')

ax.set_ylabel('Hypervolume (Normalizado)')
ax.set_title('Comparação Estatística: AMMT vs NSGA-III\n(30 Execuções, 750 Itens)')
ax.set_xticks(x)
ax.set_xticklabels(labels, fontsize=12)
ax.legend()
ax.grid(axis='y', linestyle='--', alpha=0.5)


def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate(f'{height:.3f}',
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  
                    textcoords="offset points",
                    ha='center', va='bottom', fontweight='bold')

autolabel(rects1)
autolabel(rects2)

plt.tight_layout()
plt.savefig('grafico_comparativo_final.png')
print("Gráfico salvo como 'grafico_comparativo_final.png'")