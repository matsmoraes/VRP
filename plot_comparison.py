import matplotlib.pyplot as plt
import numpy as np
import os


sizes = [250, 500, 750, 1000]
files = [f"results_{s}_items.txt" for s in sizes]


paper_data = {
    250:  {'min': 0.016, 'max': 0.129, 'avg': 0.076}, 
    500:  {'min': 0.010, 'max': 0.048, 'avg': 0.028},
    750:  {'min': 0.008, 'max': 0.034, 'avg': 0.020},
    1000: {'min': 0.002, 'max': 0.021, 'avg': 0.014}
}


ammt_flat_values = []
paper_flat_values = []
x_labels = []

print("\n--- RESUMO DO BENCHMARK (AMMT vs NSGA-III) ---")

for s, filename in zip(sizes, files):
    p_min = paper_data[s]['min']
    p_max = paper_data[s]['max']
    p_avg = paper_data[s]['avg']
    paper_flat_values.extend([p_min, p_max, p_avg])
    
    
    x_labels.extend([f"{s}\nMin", f"{s}\nMax", f"{s}\nAvg"])

    
    if os.path.exists(filename):
        try:
            with open(filename, "r") as f:
                raw_values = [float(line.strip()) for line in f if line.strip()]
                
            if not raw_values:
                print(f"Aviso: {filename} está vazio. Usando 0.0.")
                ammt_flat_values.extend([0.0, 0.0, 0.0])
            else:
                my_min = np.min(raw_values)
                my_max = np.max(raw_values)
                my_avg = np.mean(raw_values)
                
                ammt_flat_values.extend([my_min, my_max, my_avg])
                
                print(f"[{s} Itens] AMMT -> Min: {my_min:.4f} | Max: {my_max:.4f} | Avg: {my_avg:.4f}")
                print(f"            NSGA3-> Min: {p_min:.4f} | Max: {p_max:.4f} | Avg: {p_avg:.4f}")
                
        except Exception as e:
            print(f"Erro ao ler {filename}: {e}")
            ammt_flat_values.extend([0.0, 0.0, 0.0])
    else:
        print(f"Arquivo {filename} não encontrado.")
        ammt_flat_values.extend([0.0, 0.0, 0.0])


x = np.arange(len(x_labels))
width = 0.35


group_gap = 1.0 
x_positions = []
current_pos = 0
for i in range(len(sizes)):
    for _ in range(3): 
        x_positions.append(current_pos)
        current_pos += 1
    current_pos += group_gap

x_positions = np.array(x_positions)

fig, ax = plt.subplots(figsize=(14, 7))


rects1 = ax.bar(x_positions - width/2, ammt_flat_values, width, label='AMMT', color='#4CAF50', edgecolor='black')


rects2 = ax.bar(x_positions + width/2, paper_flat_values, width, label='NSGA-III (Artigo)', color='#2196F3', edgecolor='black')

ax.set_ylabel('Hypervolume (Normalizado)')
ax.set_title('Benchmark Detalhado: AMMT vs NSGA-III (Min, Max, Média)\n(30 Execuções por tamanho)')
ax.set_xticks(x_positions)
ax.set_xticklabels(x_labels)
ax.legend()
ax.grid(axis='y', linestyle='--', alpha=0.5)

max_y = max(max(ammt_flat_values), max(paper_flat_values))
ax.set_ylim(0, max_y * 1.15)


def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate(f'{height:.3f}',
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),
                    textcoords="offset points",
                    ha='center', va='bottom', fontsize=8, rotation=90, fontweight='bold')

autolabel(rects1)
autolabel(rects2)

plt.tight_layout()
plt.savefig('comparacao_nsga3_completa.png')
print("\nGráfico gerado: 'comparacao_nsga3_completa.png'")