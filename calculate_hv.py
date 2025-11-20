import numpy as np
import os
from pymoo.indicators.hv import HV

OUTPUT_FILE = "all_hv_results.txt"

try:
    data = np.loadtxt("final_pareto_front.csv", delimiter=",", skiprows=1)

    # Inverte o sinal (Maximização -> Minimização para pymoo)
    data = data * -1 

    ref_point = np.array([0, 0, 0])
    ind = HV(ref_point=ref_point)
    hv_value = ind(data)

    print(f"{hv_value}") 

    with open(OUTPUT_FILE, "a") as f:
        f.write(f"{hv_value}\n")

except Exception as e:
    print(f"0.0") # Em caso de erro (população vazia), assume 0