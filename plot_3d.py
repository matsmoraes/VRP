import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

try:
    df = pd.read_csv('final_pareto_front.csv')

    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # x = Mochila 1, y = Mochila 2, z = Mochila 3
    img = ax.scatter(df['Obj1_Lucro'], df['Obj2_Lucro'], df['Obj3_Lucro'], 
                     c=df['Obj3_Lucro'], cmap='viridis', s=60, alpha=0.8)

    ax.set_xlabel('Lucro Mochila 1')
    ax.set_ylabel('Lucro Mochila 2')
    ax.set_zlabel('Lucro Mochila 3')
    ax.set_title('Fronteira de Pareto (Maximizar Lucros) - AMMT')
    
    fig.colorbar(img, ax=ax, label='Lucro Mochila 3')

    plt.savefig('fronteira_3d.png')
    print("Gráfico 3D salvo como 'fronteira_3d.png'")
     

except FileNotFoundError:
    print("Erro: O arquivo 'final_pareto_front.csv' não foi encontrado.")
    print("Rode o ./output/main primeiro para gerar os dados.")