# Algoritmo Genético Multiobjetivo (AMMT) para o Problema da Mochila (MOKP)

Este repositório é uma adaptação do código VRP original de AugustoMacri. O algoritmo foi completamente refatorado para resolver o **Problema da Mochila Multiobjetivo (MOKP)**, com base nos benchmarks dos artigos de Zitzler & Thiele (SPEA) e Wangsom (E-NSGA-III).

## Estrutura do Algoritmo

O algoritmo implementa uma estratégia de sub-populações (AMMT) onde cada sub-população foca em otimizar um único objetivo (lucro de uma mochila específica).

* **Problema:** Problema da Mochila Multiobjetivo (MOKP)
* **Indivíduo:** Vetor binário (`int itens[]`)
* **Objetivos:** Maximizar o lucro total para $N$ mochilas.
* **Restrições:** A capacidade (peso) de cada mochila é tratada usando uma **Função de Reparo** (`repair()`) baseada na razão lucro/peso (greedy repair).

## Como Compilar e Executar

1.  **Compilar:**
    ```bash
    ./compile.sh
    ```
2.  **Executar (com GDB):**
    ```bash
    gdb ./output/main
    (gdb) run
    ```

## Saída

O programa gera três arquivos de resultado:

1.  **`fitness_report.csv`**: Um log do melhor fitness (lucro) de cada objetivo ao longo das gerações.
2.  **`final_pareto_front.csv`**: A Fronteira de Pareto final (soluções não-dominadas), usada para o cálculo do Hypervolume.
3.  **`calculate_hv.py`**: Um script Python para calcular o Hypervolume da fronteira final (requer `pip install pymoo`).
    ```bash
    python calculate_hv.py
    ```
