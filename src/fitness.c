#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/main.h" // Inclui o main.h que modificamos

// Protótipo da função (declaração)
void calculateFitness(Individual *ind);


void calculateFitness(Individual *ind)
{
    // Zera o fitness atual
    for (int i = 0; i < NUM_OBJETIVOS; i++)
    {
        ind->fitness[i] = 0.0;
    }

    // Loop por cada mochila (objetivo)
    for (int i = 0; i < NUM_OBJETIVOS; i++)
    {
        double total_profit = 0.0;
        
        // Loop por cada item
        for (int j = 0; j < NUM_ITENS; j++)
        {
            // Se o indivíduo "pegou" este item
            if (ind->itens[j] == 1)
            {
                // some o lucro deste item (para esta mochila específica)
                total_profit += profits[i][j];
            }
        }
        
        // Salva o lucro total da mochila 'i' no vetor de fitness do indivíduo
        ind->fitness[i] = total_profit;
    }
}


void fitness()
{
    // 1. Calcula o fitness (todos os N objetivos) para a POPULAÇÃO PRINCIPAL
    for (int k = 0; k < POP_SIZE; k++)
    {
        calculateFitness(&population[k]);
    }

    // 2. AVALIA AS SUB-POPULAÇÕES (para sua lógica AMMT)

    // Avalia subpop1 (focada no objetivo 0)
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        calculateFitness(&subpop1[k]);
    }
    
    // Avalia subpop2 (focada no objetivo 1)
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        calculateFitness(&subpop2[k]);
    }
    
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        calculateFitness(&subpop3[k]);
    }
}
