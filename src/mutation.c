#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/main.h"


void repair(Individual *ind);

/*
================================================================================
1. MUTAÇÃO BIT-FLIP (Para MOKP)
   Percorre o vetor de 'itens' e inverte (flipa) bits com base
   na taxa de mutação.
================================================================================
*/
void bitFlipMutation(Individual *ind)
{
    for (int j = 0; j < NUM_ITENS; j++)
    {
        // Gera um número aleatório entre 0.0 e 1.0
        double prob = (double)rand() / (double)RAND_MAX;

        // Se o número for menor que a taxa de mutação, inverte o bit
        if (prob < MUTATIONRATE)
        {
            // Lógica para inverter o bit (0 vira 1, 1 vira 0)
            ind->itens[j] = 1 - ind->itens[j];
        }
    }
}

/*
================================================================================
2. FUNÇÃO PRINCIPAL DE MUTAÇÃO
   Esta função é chamada pelo loop principal de evolução.
   Ela aplica a mutação e, em seguida, REPARA o indivíduo.
================================================================================
*/
void mutation(Individual *ind)
{
    // 1. Aplica a mutação Bit-Flip
    bitFlipMutation(ind);

    // 2. REPARA o indivíduo (CRÍTICO!)
    // A mutação pode ter tornado o indivíduo inviável (ex: 0 -> 1)
    // A função repair() garante que ele respeite a capacidade.
    repair(ind);
}
