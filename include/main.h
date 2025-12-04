#ifndef MAIN_H
#define MAIN_H

#include <math.h>

// --- PARÂMETROS DO PROBLEMA DA MOCHILA (MOKP) ---
// Baseado nos artigos ZT1999 e OLA2019
#define NUM_OBJETIVOS 3 // Número de mochilas (ex: 2, 3, ou 4)
#define NUM_ITENS 1000

// Parâmetros para geração de instâncias (ZT1999)
#define PROFIT_MIN 10
#define PROFIT_MAX 100
#define WEIGHT_MIN 10
#define WEIGHT_MAX 100

// --- PARÂMETROS DO AMMT ---
#define POP_SIZE 93
#define ELITISMRATE 0.05
#define MUTATIONRATE (1.0 / (double)NUM_ITENS) // Taxa de mutação (ex: 1/NUM_ITENS)
#define ROUNDS 300
#define SELECTION 1    // 1- Roulette. 2- Tournament
#define CROSSINGTYPE 1 // 1- One Crossing point. 2 - Two Crossing points

// Lógica de Sub-população 
#define NUM_SUBPOP 3 // This represents the number of subpopulations
#define SUBPOP_SIZE (POP_SIZE / NUM_SUBPOP)
#define ELITISM_SIZE_POP ((int)ceil(SUBPOP_SIZE * ELITISMRATE))

#define BEFORE_COMPARATION_RATE 0.05
#define GENERATIONS_BEFORE_COMPARATION ((int)ceil(ROUNDS * BEFORE_COMPARATION_RATE))

// Cromossomo ---
typedef struct
{
    int id;
    int itens[NUM_ITENS];         // VETOR BINÁRIO (cromossomo)
    double fitness[NUM_OBJETIVOS]; // Guarda o fitness (lucro) de cada objetivo (mochila)
} Individual;

// --- DADOS DA INSTÂNCIA (O PROBLEMA) ---
// Matrizes para guardar os lucros, pesos e capacidades
extern double profits[NUM_OBJETIVOS][NUM_ITENS];
extern double weights[NUM_OBJETIVOS][NUM_ITENS];
extern double capacities[NUM_OBJETIVOS];

// --- VARIÁVEIS GLOBAIS DO ALGORITMO ---
extern Individual *population;
extern Individual *parent;
extern Individual *tournamentIndividuals;
extern Individual *nextPop;
extern Individual *newSon;

// Lógica de Sub-população 
extern Individual *subpop1;
extern Individual *subpop2;
extern Individual *subpop3;

// Selection
#define QUANTITYSELECTEDTOURNAMENT 2 // Quantity of selected individuals
extern int *tournamentFitness;

// fitness
extern int *populationFitness;

// Global variables
extern int cont;

#endif