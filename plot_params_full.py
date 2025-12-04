import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import sys

try:
    df = pd.read_csv("param_study_results.csv")
except FileNotFoundError:
    print("Erro: Arquivo 'param_study_results.csv' não encontrado.")
    sys.exit(1)


elitism_used = df['Elitism'].iloc[0]


sns.set_theme(style="whitegrid")


g = sns.catplot(
    data=df, 
    x="Mutation", 
    y="AvgHV", 
    hue="Selection", 
    col="Size",
    kind="bar",
    height=5, 
    aspect=0.8,
    palette="viridis",
    edgecolor="black",
    sharey=False 
)


g.fig.suptitle(f"Benchmark de Parâmetros AMMT (Elitismo: {elitism_used})", y=1.05, fontsize=16)
g.set_axis_labels("Taxa de Mutação", "Hypervolume Médio")
g.set_titles("Itens: {col_name}")


for ax in g.axes.flat:
    for container in ax.containers:
        ax.bar_label(container, fmt='%.3f', padding=3, fontsize=9, fontweight='bold')

plt.tight_layout()
plt.savefig("estudo_parametros_completo.png")
print(f"\nGráfico gerado: 'estudo_parametros_completo.png' (Elitismo: {elitism_used})")
