import numpy as np
from pymoo.indicators.hv import HV

try:
    # Define o Ponto de Referência
    # O Hypervolume precisa de um "pior ponto" como referência.
    ref_point = np.array([0, 0, 0])

    # Carrega os dados do arquivo CSV
    # skiprows=1 pula o cabeçalho ("Obj1_Lucro", etc.)
    data = np.loadtxt("final_pareto_front.csv", delimiter=",", skiprows=1)

    # Inicializa o calculador de Hypervolume
    ind = HV(ref_point=ref_point)

    # Calcula o Hypervolume
    hv_value = ind(data)

    print("\n--- Calculo do Hypervolume ---")
    print(f"Ponto de Referencia: {ref_point}")
    print(f"Solucoes da Fronteira: {len(data)}")
    print(f"Hypervolume (HV): {hv_value}")
    print("----------------------------------")

except FileNotFoundError:
    print("Erro: O arquivo 'final_pareto_front.csv' nao foi encontrado.")
    print("Por favor, rode o programa em C primeiro.")
except Exception as e:
    print(f"Ocorreu um erro: {e}")