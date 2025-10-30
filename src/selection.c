#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "../include/main.h"

/*
================================================================================
IMPORTANTE: MAXIMIZAÇÃO vs MINIMIZAÇÃO

O Problema da Mochila é de MAXIMIZAÇÃO (queremos MAIS lucro).
As funções abaixo foram adaptadas para MAXIMIZAR o fitness (lucro).
================================================================================
*/

/*
================================================================================
1. SELEÇÃO POR ROLETA (Adaptada para MOKP - Maximização)
   Seleciona pais de 'subPop' e os salva em 'parentPop'.
   'objective_index' diz qual mochila (0, 1, 2...) esta seleção deve focar.
================================================================================
*/
void rouletteSelection(Individual *subPop, Individual *parentPop, int objective_index)
{
    double total_fitness = 0.0;
    
    // 1. Calcula o fitness total da sub-população (para este objetivo)
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        total_fitness += subPop[i].fitness[objective_index];
    }

    // 2. Realiza a seleção N vezes (N = SUBPOP_SIZE)
    for (int n = 0; n < SUBPOP_SIZE; n++)
    {
        // "Gira a roleta" (sorteia um número entre 0 e total_fitness)
        double random_point = (double)rand() / (double)RAND_MAX * total_fitness;
        
        double current_sum = 0.0;
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            current_sum += subPop[i].fitness[objective_index];
            
            // Se o ponto sorteado parar neste indivíduo...
            if (current_sum >= random_point)
            {
                // ...copia ele para a população de pais
                parentPop[n] = subPop[i];
                break; 
            }
        }
    }
}

/*
================================================================================
2. SELEÇÃO POR TORNEIO (Adaptada para MOKP - Maximização)
   Seleciona o melhor indivíduo de um grupo aleatório de 'k' indivíduos.
   'objective_index' diz qual mochila (0, 1, 2...) esta seleção deve focar.
================================================================================
*/
void tournamentSelection(Individual *subPop, Individual *parentPop, int objective_index)
{
    // Realiza a seleção N vezes (N = SUBPOP_SIZE)
    for (int n = 0; n < SUBPOP_SIZE; n++)
    {
        Individual *best_competitor = NULL;
        double best_fitness = -DBL_MAX; // Começa com o menor fitness possível (pois queremos maximizar)

        // 1. Sorteia 'k' competidores (k = QUANTITYSELECTEDTOURNAMENT)
        for (int k = 0; k < QUANTITYSELECTEDTOURNAMENT; k++)
        {
            // Pega um índice aleatório da sub-população
            int random_index = rand() % SUBPOP_SIZE;
            Individual *competitor = &subPop[random_index];

            // 2. Compara com o melhor encontrado ATÉ AGORA
            // (Note a lógica invertida: > em vez de <, pois estamos MAXIMIZANDO)
            if (competitor->fitness[objective_index] > best_fitness)
            {
                best_fitness = competitor->fitness[objective_index];
                best_competitor = competitor;
            }
        }
        
        // 3. O vencedor do torneio é copiado para a população de pais
        parentPop[n] = *best_competitor;
    }
}


/*
================================================================================
3. FUNÇÃO PRINCIPAL DE SELEÇÃO (AGORA COMPLETA)
   Esta é a função que seu loop principal de evolução (em main.c ou population.c)
   deve chamar. Ela gerencia a seleção de TODAS as sub-populações.
================================================================================
*/
void selection()
{
    // O array 'parent' (definido em initialization.c) tem tamanho POP_SIZE.
    // Vamos preenchê-lo em 3 partes (blocos de SUBPOP_SIZE).
    
    // Bloco 1: Pais da subpop1 (focada no objetivo 0)
    // Os pais selecionados serão salvos em parent[0] até parent[SUBPOP_SIZE - 1]
    if (SELECTION == 1) {
        rouletteSelection(subpop1, &parent[0], 0);
    } else {
        tournamentSelection(subpop1, &parent[0], 0);
    }
    
    // Bloco 2: Pais da subpop2 (focada no objetivo 1)
    // Os pais selecionados serão salvos em parent[SUBPOP_SIZE] até parent[(SUBPOP_SIZE * 2) - 1]
    if (SELECTION == 1) {
        rouletteSelection(subpop2, &parent[SUBPOP_SIZE], 1);
    } else {
        tournamentSelection(subpop2, &parent[SUBPOP_SIZE], 1);
    }
    
    // Bloco 3: Pais da subpop3 (focada no objetivo 2)
    // Os pais selecionados serão salvos em parent[(SUBPOP_SIZE * 2)] até parent[POP_SIZE - 1]
    if (SELECTION == 1) {
        rouletteSelection(subpop3, &parent[SUBPOP_SIZE * 2], 2);
    } else {
        tournamentSelection(subpop3, &parent[SUBPOP_SIZE * 2], 2);
    }
}