import matplotlib.pyplot as plt
import numpy as np
import os

sizes = [250, 500, 750, 1000]
files = [f"results_{s}_items.txt" for s in sizes]

# --- DADOS APROXIMADOS DO ARTIGO (E-NSGA-III - Barras Vermelhas Fig. 3) ---
paper_means = {
    250: 0.076, 
    500: 0.028, 
    750: 0.020, 
    1000: 0.014 
}

my_means = []
paper_values = []

print("\n--- RESUMO DO BENCHMARK ---")

for s, filename in zip(sizes, files):
    if os.path.exists(filename):
        try:
            with open(filename, "r") as f:
                raw_values = [float(line.strip()) for line in f if line.strip()]
                
            if not raw_values:
                print(f"Aviso: {filename} está vazio. Usando 0.0.")
                my_means.append(0.0)
            else:
                avg_hv = np.mean(raw_values) 
                my_means.append(avg_hv)
                
                print(f"Tamanho {s}: Seu HV Médio = {avg_hv:.4f} | Artigo = {paper_means[s]:.4f}")
                
        except Exception as e:
            print(f"Erro ao ler {filename}: {e}")
            my_means.append(0.0)
    else:
        print(f"Arquivo {filename} não encontrado. Rodou o benchmark completo?")
        my_means.append(0.0)
    
    paper_values.append(paper_means[s])


x = np.arange(len(sizes))
width = 0.35

fig, ax = plt.subplots(figsize=(12, 6))

rects1 = ax.bar(x - width/2, my_means, width, label='Seu Código (AMMT)', color='#4CAF50', edgecolor='black')
rects2 = ax.bar(x + width/2, paper_values, width, label='Artigo (E-NSGA-III)', color='#F44336', edgecolor='black')

ax.set_ylabel('Hypervolume Médio (Normalizado)')
ax.set_xlabel('Número de Itens (Tamanho do Problema)')
ax.set_title('Benchmark Completo: AMMT vs Artigo (30 Execuções por tamanho)')
ax.set_xticks(x)
ax.set_xticklabels(sizes)
ax.legend()
ax.grid(axis='y', linestyle='--', alpha=0.5)

def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate(f'{height:.3f}',
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),
                    textcoords="offset points",
                    ha='center', va='bottom', fontsize=10, fontweight='bold')

autolabel(rects1)
autolabel(rects2)

plt.tight_layout()
plt.savefig('benchmark_completo.png')
print("\nGráfico salvo como 'benchmark_completo.png'")