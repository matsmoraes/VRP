#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/main.h" // Inclui o main.h que modificamos

/*
================================================================================
1. DEFINIÇÃO DAS VARIÁVEIS GLOBAIS
   Estas são as variáveis declaradas como 'extern' no main.h
================================================================================
*/

// --- Matrizes da Instância do MOKP ---
double profits[NUM_OBJETIVOS][NUM_ITENS];
double weights[NUM_OBJETIVOS][NUM_ITENS];
double capacities[NUM_OBJETIVOS];

// --- Populações do Algoritmo ---
Individual *population;
Individual *parent;
Individual *tournamentIndividuals;
Individual *nextPop;
Individual *newSon;

// --- Sub-populações (Sua lógica AMMT) ---
// Você precisará adaptar a lógica 'distributeSubpopulation'
Individual *subpop1;
Individual *subpop2;
Individual *subpop3;

// --- Auxiliares do Algoritmo ---
int *tournamentFitness;
int *populationFitness;
int cont;

// Declaração de funções que serão usadas neste arquivo
void generateInstance();
int isViable(Individual *ind);
void repair(Individual *ind);

/*
================================================================================
2. FUNÇÃO PARA GERAR A INSTÂNCIA DO PROBLEMA
   Baseado nas regras do artigo ZT1999.
================================================================================
*/
void generateInstance()
{
    printf("Gerando nova instancia MOKP: %d Objetivos, %d Itens...\n", NUM_OBJETIVOS, NUM_ITENS);

    for (int i = 0; i < NUM_OBJETIVOS; i++)
    {
        double total_weight_for_this_knapsack = 0.0;
        
        for (int j = 0; j < NUM_ITENS; j++)
        {
            // Gera lucros e pesos aleatórios no intervalo [10, 100]
            profits[i][j] = (rand() % (PROFIT_MAX - PROFIT_MIN + 1)) + PROFIT_MIN;
            weights[i][j] = (rand() % (WEIGHT_MAX - WEIGHT_MIN + 1)) + WEIGHT_MIN;
            
            total_weight_for_this_knapsack += weights[i][j];
        }

        // Capacidade é 50% do peso total de todos os itens para esta mochila
        capacities[i] = 0.5 * total_weight_for_this_knapsack;
    }
    printf("Instancia gerada.\n");
}

/*
================================================================================
3. FUNÇÃO DE REPARO (A PARTE MAIS IMPORTANTE)
   Conserta um indivíduo que excede a capacidade (inviável).
   Baseado no método "greedy repair" do artigo ZT1999.
================================================================================
*/

// Estrutura auxiliar para ordenar os itens pela sua "qualidade"
typedef struct {
    int index;    // índice original do item (de 0 a NUM_ITENS-1)
    double ratio; // Razão lucro/peso (q_j no artigo)
} ItemRatio;

// Função de comparação para o qsort (ordenar do MENOR para o MAIOR ratio)
int compareItemRatios(const void *a, const void *b)
{
    ItemRatio *itemA = (ItemRatio *)a;
    ItemRatio *itemB = (ItemRatio *)b;
    if (itemA->ratio < itemB->ratio) return -1;
    if (itemA->ratio > itemB->ratio) return 1;
    return 0;
}

// Verifica se um indivíduo ultrapassa a capacidade da mochila
int isViable(Individual *ind)
{
    for (int i = 0; i < NUM_OBJETIVOS; i++)
    {
        double current_weight = 0.0;
        for (int j = 0; j < NUM_ITENS; j++)
        {
            if (ind->itens[j] == 1)
            {
                current_weight += weights[i][j];
            }
        }
        
        // Se exceder a capacidade de QUALQUER mochila, é inviável
        if (current_weight > capacities[i])
        {
            return 0; // 0 = Falso (não é viável)
        }
    }
    return 1; // 1 = Verdadeiro (é viável)
}

void repair(Individual *ind)
{
    // Se o indivíduo já for viável, não faça nada.
    if (isViable(ind))
    {
        return;
    }

    // Se for inviável, precisamos remover itens.
    // Primeiro, crie uma lista de todos os itens que este indivíduo PEGOU (itens[j] == 1)
    
    // Use malloc para alocação dinâmica se NUM_ITENS for muito grande
    ItemRatio *items_to_check = (ItemRatio *)malloc(NUM_ITENS * sizeof(ItemRatio));
    if (items_to_check == NULL) {
        printf("Erro de alocacao de memoria em repair()!\n");
        exit(1);
    }
    
    int items_count = 0;

    for (int j = 0; j < NUM_ITENS; j++)
    {
        if (ind->itens[j] == 1)
        {
            // Calcule a razão lucro/peso (q_j) deste item
            // O artigo ZT1999 define q_j = max(p_ij / w_ij) para todas as mochilas i
            double max_ratio = 0.0;
            for (int i = 0; i < NUM_OBJETIVOS; i++)
            {
                // Evita divisão por zero se o peso for 0 (embora nosso mínimo seja 10)
                if (weights[i][j] > 0) 
                {
                    double ratio = profits[i][j] / weights[i][j];
                    if (ratio > max_ratio)
                    {
                        max_ratio = ratio;
                    }
                }
            }
            
            items_to_check[items_count].index = j;
            items_to_check[items_count].ratio = max_ratio;
            items_count++;
        }
    }

    // Ordene os itens que o indivíduo pegou, do PIOR (menor ratio) para o MELHOR (maior ratio)
    qsort(items_to_check, items_count, sizeof(ItemRatio), compareItemRatios);

    // Agora, remova os itens (começando pelo pior) até o indivíduo se tornar viável
    for (int k = 0; k < items_count; k++)
    {
        // Pega o índice do item com o pior ratio
        int item_index_to_remove = items_to_check[k].index;
        
        // Remove o item (seta para 0)
        ind->itens[item_index_to_remove] = 0;

        // Verifica se o indivíduo agora é viável
        if (isViable(ind))
        {
            break; // O indivíduo foi reparado, saia do loop
        }
    }
    
    // Libera a memória alocada
    free(items_to_check);
}


/*
================================================================================
4. INICIALIZAÇÃO DA POPULAÇÃO
   Cria a população inicial de indivíduos aleatórios e VIÁVEIS.
================================================================================
*/
void initPop(Individual *population_ptr) // Renomeado para evitar conflito com a var global
{
    (void)population_ptr;

    // Inicializa a semente do gerador aleatório
    srand(time(NULL));

    // 1. Gera os dados do problema (lucros, pesos, capacidades)
    generateInstance();

    // 2. Aloca memória para a população
    population = (Individual *)malloc(POP_SIZE * sizeof(Individual));
    parent = (Individual *)malloc(POP_SIZE * sizeof(Individual));
    tournamentIndividuals = (Individual *)malloc(QUANTITYSELECTEDTOURNAMENT * sizeof(Individual));
    nextPop = (Individual *)malloc(POP_SIZE * sizeof(Individual));
    newSon = (Individual *)malloc(sizeof(Individual));
    
    // Alocar memória para sub-populações 
    subpop1 = (Individual *)malloc(SUBPOP_SIZE * sizeof(Individual));
    subpop2 = (Individual *)malloc(SUBPOP_SIZE * sizeof(Individual));
    subpop3 = (Individual *)malloc(SUBPOP_SIZE * sizeof(Individual));

    // 3. Cria cada indivíduo da população
    printf("Inicializando populacao de %d individuos...\n", POP_SIZE);
    for (int k = 0; k < POP_SIZE; k++)
    {
        population[k].id = k;
        
        // Cria um vetor binário aleatório
        for (int j = 0; j < NUM_ITENS; j++)
        {
            population[k].itens[j] = rand() % 2; // 50% de chance de ser 0 ou 1
        }

        // 4. REPARA o indivíduo para garantir que ele seja viável
        repair(&population[k]);
        
        // O fitness será calculado pela função 'fitness' separadamente
        for(int obj = 0; obj < NUM_OBJETIVOS; obj++) {
            population[k].fitness[obj] = 0.0;
        }
    }
    printf("Populacao inicial criada e reparada.\n");
}


void distributeSubpopulation(Individual *population_ptr)
{
    (void)population_ptr;
    printf("Adaptando distributeSubpopulation...\n");

    for (int i = 0; i < POP_SIZE; i++)
    {
        int index = i / SUBPOP_SIZE; // Será 0, 1, ou 2
        int index2 = i % SUBPOP_SIZE;
        
        // Assumindo 3 sub-populações para 3 objetivos (AGORA CORRETO)
        switch (index)
        {
            case 0: // Subpop Foco Objetivo 1 (fitness[0])
                subpop1[index2].id = population[i].id;
                for(int j=0; j<NUM_ITENS; j++) {
                    subpop1[index2].itens[j] = population[i].itens[j];
                }
                break;

            case 1: // Subpop Foco Objetivo 2 (fitness[1])
                subpop2[index2].id = population[i].id;
                 for(int j=0; j<NUM_ITENS; j++) {
                    subpop2[index2].itens[j] = population[i].itens[j];
                }
                break;
            
            case 2: // Subpop Foco Objetivo 3 (fitness[2]) 
                subpop3[index2].id = population[i].id;
                 for(int j=0; j<NUM_ITENS; j++) {
                    subpop3[index2].itens[j] = population[i].itens[j];
                }
                break;
        }
    }
}