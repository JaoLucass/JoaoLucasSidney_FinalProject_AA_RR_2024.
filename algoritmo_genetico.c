#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VERTICES 100  // Número máximo de dimensões/vértices
#define TAM_POPULACAO 10  // Tamanho da população
#define NUM_GERACOES 100  // Número de gerações
#define TAXA_MUTACAO 0.05 // Taxa de mutação
#define PI 3.14159265358979323846

// Funções de Benchmark
double sphere_function(double *x, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += x[i] * x[i];
    }
    return sum;
}

double rosenbrock_function(double *x, int n) {
    double sum = 0.0;
    for (int i = 0; i < n - 1; i++) {
        sum += 100.0 * pow((x[i + 1] - x[i] * x[i]), 2) + pow((1 - x[i]), 2);
    }
    return sum;
}

double rastrigin_function(double *x, int n) {
    double sum = 10 * n;
    for (int i = 0; i < n; i++) {
        sum += (x[i] * x[i]) - 10 * cos(2 * PI * x[i]);
    }
    return sum;
}

double ackley_function(double *x, int n) {
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (int i = 0; i < n; i++) {
        sum1 += x[i] * x[i];
        sum2 += cos(2 * PI * x[i]);
    }
    return -20 * exp(-0.2 * sqrt(sum1 / n)) - exp(sum2 / n) + 20 + exp(1);
}

// Função para ler uma população do arquivo
int ler_populacao(FILE *arquivo, double populacao[TAM_POPULACAO][MAX_VERTICES], int n) {
    char buffer[256];

    // Ler o rótulo da população
    if (fgets(buffer, sizeof(buffer), arquivo) == NULL) {
        return 0;  // Fim do arquivo ou erro
    }

    // Ler os indivíduos da população
    for (int i = 0; i < TAM_POPULACAO; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(arquivo, "%lf", &populacao[i][j]) != 1) {
                return 0;  // Erro de leitura
            }
        }
    }

    // Pular linha em branco
    fgets(buffer, sizeof(buffer), arquivo);

    return 1;  // População lida com sucesso
}

// Função de avaliação de fitness com todas as funções de benchmark
void calcular_fitness_todas_funcoes(double *individuo, int n, double *fitness) {
    fitness[0] = sphere_function(individuo, n);
    fitness[1] = rosenbrock_function(individuo, n);
    fitness[2] = rastrigin_function(individuo, n);
    fitness[3] = ackley_function(individuo, n);
}

// Função de seleção por torneio
int selecionar_individuo(double fitness[TAM_POPULACAO][4], int tamanho_populacao) {
    int ind1 = rand() % tamanho_populacao;
    int ind2 = rand() % tamanho_populacao;
    return (fitness[ind1][0] < fitness[ind2][0]) ? ind1 : ind2;
}

// Função de cruzamento de um ponto
void cruzar(double pai1[], double pai2[], double filho1[], double filho2[], int n) {
    int ponto_corte = rand() % n;
    for (int i = 0; i < ponto_corte; i++) {
        filho1[i] = pai1[i];
        filho2[i] = pai2[i];
    }
    for (int i = ponto_corte; i < n; i++) {
        filho1[i] = pai2[i];
        filho2[i] = pai1[i];
    }
}

// Função de mutação
void mutar(double individuo[], int n, double taxa_mutacao) {
    for (int i = 0; i < n; i++) {
        if ((rand() / (double)RAND_MAX) < taxa_mutacao) {
            individuo[i] += (rand() / (double)RAND_MAX) * 2 - 1;  // Pequena variação aleatória
        }
    }
}

int main() {
    srand(time(NULL));  // Semente para geração de números aleatórios

    int n = 5;  // Dimensão do problema (número de variáveis)
    double populacao[TAM_POPULACAO][MAX_VERTICES];
    double nova_populacao[TAM_POPULACAO][MAX_VERTICES];
    double fitness[TAM_POPULACAO][4];  // Armazena o fitness de cada função de benchmark para cada indivíduo

    // Abrir o arquivo de populações
    FILE *arquivo = fopen("populacao.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo populacao.txt!\n");
        return 1;
    }

    // Ler a população inicial
    if (!ler_populacao(arquivo, populacao, n)) {
        printf("Erro ao ler a população inicial!\n");
        return 1;
    }

    // Executar o algoritmo genético
    for (int geracao = 0; geracao < NUM_GERACOES; geracao++) {
        // Avaliar fitness da população atual
        for (int i = 0; i < TAM_POPULACAO; i++) {
            calcular_fitness_todas_funcoes(populacao[i], n, fitness[i]);
        }

        // Criar uma nova população
        for (int i = 0; i < TAM_POPULACAO; i += 2) {
            // Selecionar dois pais
            int pai1 = selecionar_individuo(fitness, TAM_POPULACAO);
            int pai2 = selecionar_individuo(fitness, TAM_POPULACAO);

            // Cruzamento
            cruzar(populacao[pai1], populacao[pai2], nova_populacao[i], nova_populacao[i + 1], n);

            // Mutação
            mutar(nova_populacao[i], n, TAXA_MUTACAO);
            mutar(nova_populacao[i + 1], n, TAXA_MUTACAO);
        }

        // Substituir a população antiga pela nova
        for (int i = 0; i < TAM_POPULACAO; i++) {
            for (int j = 0; j < n; j++) {
                populacao[i][j] = nova_populacao[i][j];
            }
        }
    }

    // Fechar o arquivo de populações
    fclose(arquivo);

    printf("Algoritmo genético executado com sucesso.\n");

    return 0;
}