#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/main.h"

// Protótipo da função de reparo (definida em initialization.c)
void repair(Individual *ind);

/*
================================================================================
1. CROSSOVER DE PONTO ÚNICO (Para MOKP)
   Troca os vetores binários 'itens[]' de dois pais.
================================================================================
*/
void onePointCrossing(Individual *parent1, Individual *parent2, Individual *son)
{
    // Escolhe um ponto de corte aleatório (entre 1 e NUM_ITENS-2)
    int cut_point = 1 + (rand() % (NUM_ITENS - 2));

    // Filho pega a primeira parte do Pai 1
    for (int i = 0; i < cut_point; i++)
    {
        son->itens[i] = parent1->itens[i];
    }

    // Filho pega a segunda parte do Pai 2
    for (int i = cut_point; i < NUM_ITENS; i++)
    {
        son->itens[i] = parent2->itens[i];
    }

    // IMPORTANTE: O novo filho pode ser inviável (exceder a capacidade).
    // Devemos repará-lo ANTES de ele ir para a próxima população.
    repair(son);
}

/*
================================================================================
2. CROSSOVER DE DOIS PONTOS (Para MOKP)
   Troca o segmento do meio de dois pais.
================================================================================
*/
void twoPointCrossing(Individual *parent1, Individual *parent2, Individual *son)
{
    // Escolhe dois pontos de corte aleatórios
    int cut_point1 = 1 + (rand() % (NUM_ITENS - 3));
    int cut_point2 = cut_point1 + 1 + (rand() % (NUM_ITENS - cut_point1 - 2));

    // Filho pega a Parte 1 do Pai 1
    for (int i = 0; i < cut_point1; i++)
    {
        son->itens[i] = parent1->itens[i];
    }

    // Filho pega a Parte 2 (meio) do Pai 2
    for (int i = cut_point1; i < cut_point2; i++)
    {
        son->itens[i] = parent2->itens[i];
    }

    // Filho pega a Parte 3 do Pai 1
    for (int i = cut_point2; i < NUM_ITENS; i++)
    {
        son->itens[i] = parent1->itens[i];
    }

    // REPARA o novo filho para garantir viabilidade
    repair(son);
}

/*
================================================================================
3. FUNÇÃO PRINCIPAL DE CROSSOVER
   Esta função é chamada pelo loop principal. Ela decide qual
   tipo de crossover usar.
================================================================================
*/
void crossing(Individual *parent1, Individual *parent2, Individual *son)
{
    // A variável 'CROSSINGTYPE' está definida em main.h
    if (CROSSINGTYPE == 1)
    {
        onePointCrossing(parent1, parent2, son);
    }
    else if (CROSSINGTYPE == 2)
    {
        twoPointCrossing(parent1, parent2, son);
    }
    else
    {
        // Padrão: crossover de ponto único
        onePointCrossing(parent1, parent2, son);
    }

    // O 'repair(son)' já foi chamado dentro das funções onePoint/twoPoint
}
