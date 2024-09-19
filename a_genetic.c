#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 100  // Limite máximo para o número de vértices
#define MAX_ARESTAS 200   // Limite máximo para o número de arestas
#define TAM_POPULACAO 10  // Tamanho da população
#define NUM_GERACOES 100  // Número de gerações para o algoritmo genético
#define TAXA_MUTACAO 0.05 // Taxa de mutação

int NUM_VERTICES;  // Número de nós (será lido do arquivo)
int NUM_ARESTAS;   // Número de arestas (será lido do arquivo)
int pesos[MAX_VERTICES];  // Lista de pesos dos nós
int arestas[MAX_ARESTAS][2];  // Lista de arestas (conexões entre nós)

// Função para ler o arquivo de população gerado
void ler_arquivo_populacao(const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Ler número de vértices e arestas
    fscanf(arquivo, "%d", &NUM_VERTICES);
    fscanf(arquivo, "%d", &NUM_ARESTAS);

    // Ler pesos dos vértices
    for (int i = 0; i < NUM_VERTICES; i++) {
        fscanf(arquivo, "%d %d", &i, &pesos[i]);
    }

    // Ler arestas
    for (int i = 0; i < NUM_ARESTAS; i++) {
        fscanf(arquivo, "%d %d", &arestas[i][0], &arestas[i][1]);
    }

    fclose(arquivo);
    printf("Arquivo de população lido com sucesso.\n");
}

// Função para gerar uma solução inicial aleatória
void gerar_individuo_aleatorio(int *individuo) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        individuo[i] = rand() % 2;  // 0 ou 1 para indicar se uma câmera será instalada
    }
}

// Função de fitness (avalia o número de câmeras e cobertura das arestas)
int calcular_fitness(int *individuo) {
    int arestas_cobertas = 0;
    int num_cameras = 0;

    // Contar o número de câmeras usadas
    for (int i = 0; i < NUM_VERTICES; i++) {
        if (individuo[i] == 1) {
            num_cameras++;
        }
    }

    // Verificar se cada aresta está coberta por uma câmera
    for (int i = 0; i < NUM_ARESTAS; i++) {
        int v1 = arestas[i][0];
        int v2 = arestas[i][1];
        if (individuo[v1] == 1 || individuo[v2] == 1) {
            arestas_cobertas++;
        }
    }

    // Penalizar soluções que não cobrem todas as arestas
    if (arestas_cobertas == NUM_ARESTAS) {
        return num_cameras;  // Quanto menos câmeras, melhor o fitness
    } else {
        return num_cameras + (NUM_ARESTAS - arestas_cobertas) * 10;  // Penalidade por arestas não cobertas
    }
}

// Função para avaliar a população
void avaliar_populacao(int populacao[TAM_POPULACAO][MAX_VERTICES], int fitness[TAM_POPULACAO]) {
    for (int i = 0; i < TAM_POPULACAO; i++) {
        fitness[i] = calcular_fitness(populacao[i]);
    }
}

// Função de seleção (torneio simples)
int selecionar_individuo(int fitness[TAM_POPULACAO]) {
    int ind1 = rand() % TAM_POPULACAO;
    int ind2 = rand() % TAM_POPULACAO;
    return (fitness[ind1] < fitness[ind2]) ? ind1 : ind2;
}

// Função de cruzamento (crossover de um ponto)
void cruzar_individuos(int pai1[MAX_VERTICES], int pai2[MAX_VERTICES], int filho1[MAX_VERTICES], int filho2[MAX_VERTICES]) {
    int ponto_de_cruzamento = rand() % NUM_VERTICES;

    for (int i = 0; i < ponto_de_cruzamento; i++) {
        filho1[i] = pai1[i];
        filho2[i] = pai2[i];
    }
    for (int i = ponto_de_cruzamento; i < NUM_VERTICES; i++) {
        filho1[i] = pai2[i];
        filho2[i] = pai1[i];
    }
}

// Função de mutação
void mutar_individuo(int individuo[MAX_VERTICES], float taxa_mutacao) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        if ((rand() / (float)RAND_MAX) < taxa_mutacao) {
            individuo[i] = 1 - individuo[i];  // Inverte o gene (0 -> 1 ou 1 -> 0)
        }
    }
}

// Função para gerar a nova geração
void nova_geracao(int populacao[TAM_POPULACAO][MAX_VERTICES], int fitness[TAM_POPULACAO]) {
    int nova_populacao[TAM_POPULACAO][MAX_VERTICES];

    for (int i = 0; i < TAM_POPULACAO; i += 2) {
        int pai1 = selecionar_individuo(fitness);
        int pai2 = selecionar_individuo(fitness);

        // Cruzamento e criação dos filhos
        cruzar_individuos(populacao[pai1], populacao[pai2], nova_populacao[i], nova_populacao[i+1]);

        // Aplicar mutação nos filhos
        mutar_individuo(nova_populacao[i], TAXA_MUTACAO);
        mutar_individuo(nova_populacao[i+1], TAXA_MUTACAO);
    }

    // Substituir a população antiga pela nova
    for (int i = 0; i < TAM_POPULACAO; i++) {
        for (int j = 0; j < NUM_VERTICES; j++) {
            populacao[i][j] = nova_populacao[i][j];
        }
    }
}

int main() {
    srand(time(NULL));  // Semente para geração de números aleatórios

    // Ler o arquivo de população gerado anteriormente
    ler_arquivo_populacao("populacao.txt");

    // Definir a população e fitness
    int populacao[TAM_POPULACAO][MAX_VERTICES];
    int fitness[TAM_POPULACAO];

    // Gerar a população inicial
    for (int i = 0; i < TAM_POPULACAO; i++) {
        gerar_individuo_aleatorio(populacao[i]);
    }

    // Evolução do algoritmo genético
    for (int geracao = 0; geracao < NUM_GERACOES; geracao++) {
        // Avaliar a população
        avaliar_populacao(populacao, fitness);

        // Exibir o melhor fitness da geração atual
        int melhor_fitness = fitness[0];
        for (int i = 1; i < TAM_POPULACAO; i++) {
            if (fitness[i] < melhor_fitness) {
                melhor_fitness = fitness[i];
            }
        }
        printf("Geração %d - Melhor fitness: %d\n", geracao, melhor_fitness);

        // Gerar nova geração
        nova_geracao(populacao, fitness);
    }

    return 0;
}

