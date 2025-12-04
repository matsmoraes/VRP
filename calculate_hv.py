import numpy as np
import sys
import os
from pymoo.indicators.hv import HV


if len(sys.argv) > 1:
    n_items = int(sys.argv[1])
else:
    n_items = 750

max_theoretical_profit = n_items * 100.0

try:
    if not os.path.exists("final_pareto_front.csv"):
        # Se não existir, imprime 0.0 e encerra
        print("0.0")
        sys.exit(0)

    raw_data = np.loadtxt("final_pareto_front.csv", delimiter=",", skiprows=1)

    if raw_data.ndim == 1:
        raw_data = raw_data.reshape(1, -1)

    normalized_data = raw_data / max_theoretical_profit

    data_for_hv = normalized_data * -1
    
    ref_point = np.array([0.0, 0.0, 0.0])
    
    ind = HV(ref_point=ref_point)
    hv_value = ind(data_for_hv)

    print(f"{hv_value}")

except Exception as e:
    print("0.0")