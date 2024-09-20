#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VERTICES 100  // Número máximo de dimensões/vértices
#define TAM_POPULACAO 10  // Tamanho da população
#define NUM_GERACOES 100  // Número de gerações
#define TAXA_MUTACAO 0.05 // Taxa de mutação
#define NUM_POPULACOES 5  // Número de populações no arquivo
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

int main() {
    srand(time(NULL));  // Semente para geração de números aleatórios

    int n = 5;  // Dimensão do problema (número de variáveis)
    double populacao[TAM_POPULACAO][MAX_VERTICES];
    double fitness[TAM_POPULACAO][4];  // Armazena o fitness de cada função de benchmark para cada indivíduo

    // Abrir o arquivo de populações
    FILE *arquivo = fopen("populacao.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo populacao.txt!\n");
        return 1;
    }

    // Abrir arquivo CSV para salvar os dados de fitness
    FILE *arquivo_dados = fopen("fitness_data.csv", "w");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo fitness_data.csv!\n");
        return 1;
    }
    
    // Escrever cabeçalho do CSV
    fprintf(arquivo_dados, "Populacao,Individuo,Sphere,Rosenbrock,Rastrigin,Ackley\n");

    int num_populacao = 1;
    while (ler_populacao(arquivo, populacao, n)) {
        // Avaliar a população lida usando todas as funções de benchmark
        for (int i = 0; i < TAM_POPULACAO; i++) {
            calcular_fitness_todas_funcoes(populacao[i], n, fitness[i]);

            // Salvar os dados no CSV
            fprintf(arquivo_dados, "%d,%d,%.5f,%.5f,%.5f,%.5f\n", 
                    num_populacao, i + 1, fitness[i][0], fitness[i][1], fitness[i][2], fitness[i][3]);
        }
        num_populacao++;
    }

    fclose(arquivo);
    fclose(arquivo_dados);

    printf("Dados de fitness salvos no arquivo fitness_data.csv.\n");

    return 0;
}