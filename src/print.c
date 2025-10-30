#include <stdio.h>
#include <stdlib.h>
#include <float.h> // Para DBL_MAX
#include <math.h>  // Para sqrt() e pow()
#include "../include/main.h"

// Protótipos das funções de relatório (que já existiam)
void initReportFile(FILE *reportFile);
void logGenerationReport(FILE *reportFile, int generation);
void saveParetoFrontToFile(FILE *paretoFile);
int isDominated(Individual *ind1, Individual *ind2);

/*
================================================================================
(BLOCO NOVO) 1. FUNÇÕES AUXILIARES DE ESTATÍSTICA
================================================================================
*/

// Calcula a Média de fitness de uma sub-população para um objetivo
double calculateMean(Individual *subPop, int obj_index) {
    double sum = 0.0;
    for (int i = 0; i < SUBPOP_SIZE; i++) {
        sum += subPop[i].fitness[obj_index];
    }
    return sum / (double)SUBPOP_SIZE;
}

// Calcula o Desvio Padrão
double calculateStdDev(Individual *subPop, int obj_index, double mean) {
    double sum_sq_diff = 0.0;
    for (int i = 0; i < SUBPOP_SIZE; i++) {
        sum_sq_diff += pow(subPop[i].fitness[obj_index] - mean, 2);
    }
    return sqrt(sum_sq_diff / (double)SUBPOP_SIZE);
}

// Encontra o Melhor Fitness (MAXIMIZAÇÃO de lucro)
double findBestFitness(Individual *subPop, int obj_index) {
    double best = -DBL_MAX; // Inicia com o menor valor possível
    for (int i = 0; i < SUBPOP_SIZE; i++) {
        if (subPop[i].fitness[obj_index] > best) {
            best = subPop[i].fitness[obj_index];
        }
    }
    return best;
}

/*
================================================================================
(BLOCO NOVO) 2. FUNÇÃO PRINCIPAL DE IMPRESSÃO (Substitui a antiga)
   Imprime o relatório final formatado como o seu VRP.
================================================================================
*/
void printResults(double total_time) 
{
    // 1. Imprime os parâmetros da execução
    printf("\n--- Resumo dos Parametros ---\n");
    printf("Population Size: %d\n", POP_SIZE);
    printf("Number of Objectives (Mochilas): %d\n", NUM_OBJETIVOS);
    printf("Number of Items: %d\n", NUM_ITENS);
    printf("Selection Type: %s\n", SELECTION == 1 ? "Roulette" : "Tournament");
    printf("Crossover Points: %d points\n", CROSSINGTYPE);
    printf("Mutation Rate: %f\n", MUTATIONRATE);
    printf("Elitism Rate: %f\n", ELITISMRATE);
    printf("Rounds: %d\n", ROUNDS);
    printf("Time: %f\n", total_time);

    // 2. Prepara os dados para o loop
    // (Criamos arrays para acessar as sub-pops e seus nomes)
    Individual* sub_pops[] = {subpop1, subpop2, subpop3};
    char sub_pop_names[][20] = {"Objetivo 1", "Objetivo 2", "Objetivo 3"};

    // 3. Loop para imprimir as estatísticas de cada sub-população
    for (int i = 0; i < NUM_OBJETIVOS; i++) {
        
        printf("--------------------%s------------------\n", sub_pop_names[i]);
        
        Individual* current_subpop = sub_pops[i];
        
        // Calcula as estatísticas
        // (Nota: o 'primeiro fitness' é apenas o fitness do indivíduo no índice 0)
        double first_fitness = current_subpop[0].fitness[i];
        double best_fitness = findBestFitness(current_subpop, i);
        double mean_fitness = calculateMean(current_subpop, i);
        double std_dev = calculateStdDev(current_subpop, i, mean_fitness);

        // Imprime as estatísticas
        printf("A primeiro fitness da subPop %s eh: %f\n", sub_pop_names[i], first_fitness);
        printf("A melhor fitness da subPop %s eh: %f\n", sub_pop_names[i], best_fitness);
        printf("A media dos fitness da subPop %s eh: %f\n", sub_pop_names[i], mean_fitness);
        printf("O desvio Padrao da subPop %s eh: %f\n", sub_pop_names[i], std_dev);

        // Imprime a lista de fitness da sub-população
        // (SUBPOP_SIZE = 93 / 3 = 31)
        for (int j = 0; j < SUBPOP_SIZE; j++) {
            printf("%.2f", current_subpop[j].fitness[i]);
            if (j < SUBPOP_SIZE - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    printf("--------------------------------------------------\n");
}


/*
================================================================================
3. Funções de Relatório CSV (As mesmas de antes)
================================================================================
*/
void initReportFile(FILE *reportFile)
{
    if (reportFile == NULL) {
        printf("Erro ao abrir o arquivo de relatorio!\n");
        return;
    }
    fprintf(reportFile, "Geracao,MaxLucroObj1,MaxLucroObj2,MaxLucroObj3\n");
}

void logGenerationReport(FILE *reportFile, int generation)
{
    if (reportFile == NULL) {
        return;
    }

    double best_obj1 = -DBL_MAX;
    double best_obj2 = -DBL_MAX;
    double best_obj3 = -DBL_MAX;

    for (int k = 0; k < POP_SIZE; k++)
    {
        if (population[k].fitness[0] > best_obj1) {
            best_obj1 = population[k].fitness[0];
        }
        if (population[k].fitness[1] > best_obj2) {
            best_obj2 = population[k].fitness[1];
        }
        if (population[k].fitness[2] > best_obj3) {
            best_obj3 = population[k].fitness[2];
        }
    }

    fprintf(reportFile, "%d,%.2f,%.2f,%.2f\n",
            generation,
            best_obj1,
            best_obj2,
            best_obj3);
}

/*
================================================================================
4. Funções da Fronteira de Pareto (As mesmas de antes)
================================================================================
*/
int isDominated(Individual *ind1, Individual *ind2)
{
    int i;
    int ind2_is_better_in_at_least_one = 0; 

    for (i = 0; i < NUM_OBJETIVOS; i++)
    {
        if (ind1->fitness[i] > ind2->fitness[i])
        {
            return 0; // Não é dominado
        }
        if (ind2->fitness[i] > ind1->fitness[i])
        {
            ind2_is_better_in_at_least_one = 1;
        }
    }
    return ind2_is_better_in_at_least_one;
}

void saveParetoFrontToFile(FILE *paretoFile)
{
    if (paretoFile == NULL) {
        printf("Erro ao abrir o arquivo da Fronteira de Pareto!\n");
        return;
    }
    fprintf(paretoFile, "Obj1_Lucro,Obj2_Lucro,Obj3_Lucro\n");
    int solutions_saved = 0;
    
    for (int i = 0; i < POP_SIZE; i++)
    {
        int i_is_dominated = 0;
        for (int j = 0; j < POP_SIZE; j++)
        {
            if (i == j) continue;
            if (isDominated(&population[i], &population[j]))
            {
                i_is_dominated = 1;
                break; 
            }
        }
        if (!i_is_dominated)
        {
            fprintf(paretoFile, "%.2f,%.2f,%.2f\n",
                    population[i].fitness[0],
                    population[i].fitness[1],
                    population[i].fitness[2]);
            solutions_saved++;
        }
    }
    printf("Fronteira de Pareto final com %d solucoes salva em 'final_pareto_front.csv'\n", solutions_saved);
}