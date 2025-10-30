#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> // Para DBL_MAX
#include "../include/main.h"

/*
================================================================================
PROTÓTIPOS DE FUNÇÕES EXTERNAS
(Funções que este arquivo chama, mas que estão em outros .c)
================================================================================
*/
// De crossing.c
void crossing(Individual *parent1, Individual *parent2, Individual *son);
// De mutation.c
void mutation(Individual *ind);
// De initialization.c
void distributeSubpopulation(Individual *population_ptr);

/*
================================================================================
1. ELITISMO (Adaptado para MOKP - Maximização)
   Encontra os 'N' melhores indivíduos de uma sub-população
   e os copia para o bloco da próxima geração.
================================================================================
*/
void findElitism(Individual *subPop, Individual *nextPopBlock, int objective_index)
{
    // Array para marcar quais indivíduos já foram escolhidos (0 = não, 1 = sim)
    int used_indices[SUBPOP_SIZE] = {0}; 

    // Loop para escolher os 'N' melhores (N = ELITISM_SIZE_POP)
    for (int e = 0; e < ELITISM_SIZE_POP; e++)
    {
        double best_fitness = -DBL_MAX; // Inicia com o pior fitness (pois queremos MAXIMIZAR o lucro)
        int best_index = -1;

        // 1. Encontra o melhor indivíduo (que ainda não foi pego)
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            // (Verifica se não foi usado) E (Se o fitness dele é MAIOR que o melhor atual)
            if (used_indices[i] == 0 && subPop[i].fitness[objective_index] > best_fitness)
            {
                best_fitness = subPop[i].fitness[objective_index];
                best_index = i;
            }
        }
        
        // 2. Copia o melhor indivíduo encontrado para a próxima população
        if (best_index != -1)
        {
            nextPopBlock[e] = subPop[best_index]; // Cópia da struct inteira
            used_indices[best_index] = 1;         // Marca como usado
        }
    }
}

/*
================================================================================
2. FUNÇÃO PRINCIPAL DE ELITISMO
   Gerencia o elitismo para TODAS as sub-populações.
================================================================================
*/
void elitism()
{
    // Bloco 1: Elitismo da subpop1 (foco no obj 0)
    // Salva os melhores em nextPop[0] até nextPop[ELITISM_SIZE_POP - 1]
    findElitism(subpop1, &nextPop[0], 0);
    
    // Bloco 2: Elitismo da subpop2 (foco no obj 1)
    // Salva os melhores em nextPop[SUBPOP_SIZE] até nextPop[SUBPOP_SIZE + ELITISM_SIZE_POP - 1]
    findElitism(subpop2, &nextPop[SUBPOP_SIZE], 1);
    
    // Bloco 3: Elitismo da subpop3 (foco no obj 2)
    // Salva os melhores em nextPop[SUBPOP_SIZE * 2] até nextPop[SUBPOP_SIZE * 2 + ELITISM_SIZE_POP - 1]
    findElitism(subpop3, &nextPop[SUBPOP_SIZE * 2], 2);
}

/*
================================================================================
3. EVOLUÇÃO DA POPULAÇÃO
   Cria os novos filhos (cruzamento + mutação) para preencher
   o resto da próxima geração.
================================================================================
*/
void evolvePop()
{
    // Loop 3x, uma vez para cada sub-população
    for (int sub_idx = 0; sub_idx < NUM_SUBPOP; sub_idx++)
    {
        // 1. Define os "blocos" de memória para esta sub-população
        int pop_offset = sub_idx * SUBPOP_SIZE;
        Individual *current_parents = &parent[pop_offset];    // Pais desta sub-pop (da selection.c)
        Individual *next_gen_pop = &nextPop[pop_offset]; // Próxima geração desta sub-pop

        // 2. Loop para criar os novos filhos (começa DEPOIS do elitismo)
        for (int i = ELITISM_SIZE_POP; i < SUBPOP_SIZE; i++)
        {
            // 3. Pega dois pais do bloco de pais desta sub-população
            Individual *parent1 = &current_parents[i];
            Individual *parent2 = &current_parents[rand() % SUBPOP_SIZE]; // Pega um aleatório do mesmo bloco

            // 4. Crossover (já chama o 'repair' dentro dele)
            // 'newSon' é um buffer global (definido em initialization.c)
            crossing(parent1, parent2, newSon);
            
            // 5. Mutação (já chama o 'repair' dentro dela)
            mutation(newSon);
            
            // 6. Salva o novo filho (reparado) na próxima geração
            next_gen_pop[i] = *newSon; // Copia os dados do buffer 'newSon'
        }
    }
}

/*
================================================================================
4. ATUALIZAÇÃO DA POPULAÇÃO
   Copia a 'nextPop' (nova geração) de volta para a 'population'
   e redistribui para as sub-populações.
================================================================================
*/
void updatePop()
{
    // 1. Copia a nova geração (nextPop) para a população principal (population)
    for (int i = 0; i < POP_SIZE; i++)
    {
        population[i] = nextPop[i]; // Cópia de struct (copia .id, .itens[], .fitness[])
    }

    // 2. Redistribui a 'population' principal de volta para as 
    // sub-populações (subpop1, subpop2, subpop3)
    // Isso é crucial para que o 'fitness()' e 'elitism()' da PRÓXIMA geração
    // tenham os indivíduos corretos.
    distributeSubpopulation(population);
}

