#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_VERTICES 6  // Número de vértices no grafo
#define TAM_POPULACAO 10  // Tamanho da população (número de indivíduos)
#define NUM_ARESTAS 7  // Número de arestas (ruas) no grafo

// Grafo representado por uma lista de arestas
// Cada aresta conecta dois vértices
int arestas[NUM_ARESTAS][2] = {
    {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {4, 5}, {2, 5}
};

// Função para gerar aleatoriamente 0 ou 1
int gerar_aleatorio_binario() {
    return rand() % 2;  // Retorna 0 ou 1
}

// Função para gerar um indivíduo aleatório
void gerar_individuo_aleatorio(int *individuo, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        individuo[i] = gerar_aleatorio_binario();  // 0 ou 1 para cada vértice
    }
}

// Função para gerar uma população de indivíduos aleatórios
void gerar_populacao_aleatoria(int populacao[TAM_POPULACAO][NUM_VERTICES]) {
    for (int i = 0; i < TAM_POPULACAO; i++) {
        gerar_individuo_aleatorio(populacao[i], NUM_VERTICES);  // Gera cada indivíduo
    }
}

// Função de fitness para um indivíduo
int calcular_fitness(int *individuo) {
    int arestas_cobertas = 0;
    int num_cameras = 0;

    // Contar quantas câmeras foram usadas
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

    // Penalizar indivíduos que não cobrem todas as arestas
    if (arestas_cobertas == NUM_ARESTAS) {
        return num_cameras;  // Quanto menor o número de câmeras, melhor o fitness
    } else {
        return num_cameras + (NUM_ARESTAS - arestas_cobertas) * 10;  // Penalidade para cada aresta não coberta
    }
}

// Função para avaliar o fitness de toda a população
void avaliar_populacao(int populacao[TAM_POPULACAO][NUM_VERTICES], int fitness[TAM_POPULACAO]) {
    for (int i = 0; i < TAM_POPULACAO; i++) {
        fitness[i] = calcular_fitness(populacao[i]);  // Avaliar fitness do indivíduo
    }
}

// Função de seleção por torneio simples
int selecionar_individuo(int fitness[TAM_POPULACAO]) {
    int ind1 = rand() % TAM_POPULACAO;
    int ind2 = rand() % TAM_POPULACAO;

    // Retorna o índice do indivíduo com melhor fitness
    return (fitness[ind1] < fitness[ind2]) ? ind1 : ind2;
}

// Função de cruzamento de um ponto
void cruzar_individuos(int pai1[NUM_VERTICES], int pai2[NUM_VERTICES], int filho1[NUM_VERTICES], int filho2[NUM_VERTICES]) {
    int ponto_de_cruzamento = rand() % NUM_VERTICES;  // Escolhe ponto de cruzamento

    for (int i = 0; i < ponto_de_cruzamento; i++) {
        filho1[i] = pai1[i];
        filho2[i] = pai2[i];
    }
    for (int i = ponto_de_cruzamento; i < NUM_VERTICES; i++) {
        filho1[i] = pai2[i];
        filho2[i] = pai1[i];
    }
}

// Função para aplicar mutação em um indivíduo
void mutar_individuo(int individuo[NUM_VERTICES], float taxa_mutacao) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        if ((rand() / (float)RAND_MAX) < taxa_mutacao) {
            individuo[i] = 1 - individuo[i];  // Inverte o gene (0 -> 1 ou 1 -> 0)
        }
    }
}

// Função para criar a próxima geração
void nova_geracao(int populacao[TAM_POPULACAO][NUM_VERTICES], int fitness[TAM_POPULACAO], float taxa_mutacao) {
    int nova_populacao[TAM_POPULACAO][NUM_VERTICES];

    // Geração de novos indivíduos por cruzamento
    for (int i = 0; i < TAM_POPULACAO; i += 2) {
        int pai1 = selecionar_individuo(fitness);
        int pai2 = selecionar_individuo(fitness);

        // Gerar dois filhos por cruzamento
        cruzar_individuos(populacao[pai1], populacao[pai2], nova_populacao[i], nova_populacao[i+1]);

        // Aplicar mutação
        mutar_individuo(nova_populacao[i], taxa_mutacao);
        mutar_individuo(nova_populacao[i+1], taxa_mutacao);
    }

    // Copiar nova geração para a população original
    for (int i = 0; i < TAM_POPULACAO; i++) {
        for (int j = 0; j < NUM_VERTICES; j++) {
            populacao[i][j] = nova_populacao[i][j];
        }
    }
}

int main() {
    srand(time(NULL));  // Semente para geração de números aleatórios

    int populacao[TAM_POPULACAO][NUM_VERTICES];
    int fitness[TAM_POPULACAO];
    int num_geracoes = 100;  // Número de gerações
    float taxa_mutacao = 0.05;  // Taxa de mutação

    // Gerar população inicial
    gerar_populacao_aleatoria(populacao);

    // Evolução por várias gerações
    for (int geracao = 0; geracao < num_geracoes; geracao++) {
        // Avaliar fitness da população
        avaliar_populacao(populacao, fitness);

        // Criar nova geração
        nova_geracao(populacao, fitness, taxa_mutacao);

        // Melhor fitness da geração atual
        int melhor_fitness = fitness[0];
        for (int i = 1; i < TAM_POPULACAO; i++) {
            if (fitness[i] < melhor_fitness) {
                melhor_fitness = fitness[i];
            }
        }

        printf("Geração %d - Melhor fitness: %d\n", geracao, melhor_fitness);
    }

    return 0;
}
