#include <stdio.h>
#include <stdlib.h>
#include <time.h> // <-- ADICIONADO PARA MEDIR O TEMPO
#include "../include/main.h"

/*
================================================================================
PROTÓTIPOS DE FUNÇÕES EXTERNAS
================================================================================
*/

// De initialization.c
void initPop(Individual *population_ptr); 
void distributeSubpopulation(Individual *population_ptr);

// De fitness.c
void fitness();

// De population.c
void elitism();
void evolvePop();
void updatePop();

// De selection.c
void selection();

// De print.c
void printResults(double time); // <-- MODIFICADO (agora recebe o tempo)
void initReportFile(FILE *f);           
void logGenerationReport(FILE *f, int g);
void saveParetoFrontToFile(FILE *f); 

/*
================================================================================
FUNÇÃO PARA LIBERAR MEMÓRIA
================================================================================
*/
void freeMemory()
{
    printf("Liberando memoria...\n");
    free(population);
    free(parent);
    free(tournamentIndividuals);
    free(nextPop);
    free(newSon);
    free(subpop1);
    free(subpop2);
    free(subpop3); 
}

/*
================================================================================
FUNÇÃO PRINCIPAL (MAIN)
================================================================================
*/
int main() 
{
    clock_t start_time, end_time; // <-- ADICIONADO
    double elapsed_time;           // <-- ADICIONADO
    
    start_time = clock(); // <-- ADICIONADO (Inicia o cronômetro)

    // 1. Abrir arquivo de relatório
    FILE *reportFile = fopen("fitness_report.csv", "w");
    if (reportFile == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo de relatorio.\n");
        return 1;
    }
    initReportFile(reportFile); 

    // Abrir arquivo da Fronteira de Pareto
    FILE *paretoFile = fopen("final_pareto_front.csv", "w");
    if (paretoFile == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo da fronteira.\n");
        return 1;
    }

    // 2. Inicialização
    printf("Iniciando Algoritmo AMMT para MOKP...\n");
    initPop(population); 
    distributeSubpopulation(population); 
    
    printf("Iniciando loop evolucionario (%d geracoes)...\n", ROUNDS);
    
    // 3. Loop Evolucionário
    for (int i = 0; i < ROUNDS; i++) {
        
        fitness(); 
        elitism();
        selection();
        evolvePop();
        updatePop();

        // 4. Salva o relatório
        if (i % 100 == 0 || i == (ROUNDS - 1)) {
            printf("Geracao %d / %d concluida.\n", i + 1, ROUNDS);
            logGenerationReport(reportFile, i + 1); 
        }
    }
    
    end_time = clock(); // <-- ADICIONADO (Para o cronômetro)
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // <-- ADICIONADO (Calcula o tempo)
    
    // 5. Fim
    printf("Evolucao concluida.\n");
    printf("Relatorio de fitness salvo em 'fitness_report.csv'\n");
    fclose(reportFile); 

    // 6. Salva a Fronteira de Pareto Final
    saveParetoFrontToFile(paretoFile);
    fclose(paretoFile);

    // 7. Imprime os resultados finais (no terminal)
    printResults(elapsed_time); // <-- MODIFICADO (passa o tempo)
    
    // 8. Libera a memória
    freeMemory();

    return 0;
}