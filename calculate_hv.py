import numpy as np
import sys
import os
from pymoo.indicators.hv import HV

OUTPUT_FILE = "all_hv_results.txt"

if len(sys.argv) > 1:
    n_items = int(sys.argv[1])
else:
    n_items = 750 # Fallback

max_theoretical_profit = n_items * 100.0

try:
    if not os.path.exists("final_pareto_front.csv"):
        raise FileNotFoundError("Arquivo final_pareto_front.csv não encontrado.")

    raw_data = np.loadtxt("final_pareto_front.csv", delimiter=",", skiprows=1)

    if raw_data.ndim == 1:
        raw_data = raw_data.reshape(1, -1)

    normalized_data = raw_data / max_theoretical_profit

    data_for_hv = normalized_data * -1
    
    ref_point = np.array([0.0, 0.0, 0.0])
    
    ind = HV(ref_point=ref_point)
    hv_value = ind(data_for_hv)


    with open(OUTPUT_FILE, "a") as f:
        f.write(f"{hv_value}\n")

except Exception as e:
    with open(OUTPUT_FILE, "a") as f:
        f.write(f"0.0\n")