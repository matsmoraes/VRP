import numpy as np
from pymoo.indicators.hv import HV

try:
    # Carrega os dados do arquivo CSV
    data = np.loadtxt("final_pareto_front.csv", delimiter=",", skiprows=1)

    # --- CORREÇÃO CRÍTICA ---
    # A pymoo assume minimização. Como estamos maximizando lucro,
    # invertemos o sinal dos dados para calcular o volume corretamente.
    data = data * -1 
    # ------------------------

    # Define o Ponto de Referência
    # Como invertemos os dados para negativos, o 0 continua sendo um bom
    ref_point = np.array([0, 0, 0])

    # Inicializa o calculador de Hypervolume
    ind = HV(ref_point=ref_point)

    # Calcula o Hypervolume
    hv_value = ind(data)

    print("\n--- Calculo do Hypervolume ---")
    print(f"Ponto de Referencia: {ref_point}")
    print(f"Solucoes da Fronteira: {len(data)}")
    print(f"Hypervolume (HV): {hv_value:.4e}") 
    print("----------------------------------")

except FileNotFoundError:
    print("Erro: O arquivo 'final_pareto_front.csv' nao foi encontrado.")
    print("Por favor, rode o programa em C primeiro.")
except Exception as e:
    print(f"Ocorreu um erro: {e}")