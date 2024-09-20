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

// Função para gerar um indivíduo aleatório para funções de benchmark
void gerar_individuo_benchmark(double *individuo, int n, double min, double max) {
    for (int i = 0; i < n; i++) {
        individuo[i] = min + ((double)rand() / RAND_MAX) * (max - min);  // Gera números entre [min, max]
    }
}

// Função de avaliação de fitness usando diferentes funções de benchmark
double calcular_fitness_benchmark(double *individuo, int n, int tipo_funcao) {
    switch (tipo_funcao) {
        case 1:
            return sphere_function(individuo, n);
        case 2:
            return rosenbrock_function(individuo, n);
        case 3:
            return rastrigin_function(individuo, n);
        case 4:
            return ackley_function(individuo, n);
        default:
            return sphere_function(individuo, n);  // Função padrão
    }
}

// Função de crossover (um ponto)
void cruzar_individuos(double *pai1, double *pai2, double *filho1, double *filho2, int n) {
    int ponto_de_cruzamento = rand() % n;

    for (int i = 0; i < ponto_de_cruzamento; i++) {
        filho1[i] = pai1[i];
        filho2[i] = pai2[i];
    }
    for (int i = ponto_de_cruzamento; i < n; i++) {
        filho1[i] = pai2[i];
        filho2[i] = pai1[i];
    }
}

// Função de mutação
void mutar_individuo(double *individuo, int n, double taxa_mutacao, double min, double max) {
    for (int i = 0; i < n; i++) {
        if ((rand() / (double)RAND_MAX) < taxa_mutacao) {
            individuo[i] = min + ((double)rand() / RAND_MAX) * (max - min);  // Mutar com valor aleatório no intervalo [min, max]
        }
    }
}

// Função de seleção (torneio simples)
int selecionar_individuo(double *fitness, int tamanho_populacao) {
    int ind1 = rand() % tamanho_populacao;
    int ind2 = rand() % tamanho_populacao;
    return (fitness[ind1] < fitness[ind2]) ? ind1 : ind2;
}

int main() {
    srand(time(NULL));  // Semente para gerar números aleatórios

    int n = 5;  // Dimensão do problema (número de vértices ou variáveis)
    double min = -5.12;  // Limite inferior do intervalo
    double max = 5.12;   // Limite superior do intervalo
    int tipo_funcao;     // Função de benchmark escolhida pelo usuário

    // Perguntar ao usuário qual função de benchmark usar
    printf("Escolha a função de benchmark:\n");
    printf("1. Sphere Function\n");
    printf("2. Rosenbrock Function\n");
    printf("3. Rastrigin Function\n");
    printf("4. Ackley Function\n");
    printf("Digite o número da função escolhida: ");
    scanf("%d", &tipo_funcao);

    if (tipo_funcao < 1 || tipo_funcao > 4) {
        printf("Escolha inválida! Usando Sphere Function por padrão.\n");
        tipo_funcao = 1;  // Se a escolha for inválida, usar Sphere como padrão
    }

    double populacao[TAM_POPULACAO][n];
    double nova_populacao[TAM_POPULACAO][n];
    double fitness[TAM_POPULACAO];

    // Abrir o arquivo CSV para salvar os dados de fitness
    FILE *arquivo_dados = fopen("fitness_data.csv", "w");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo fitness_data.csv!\n");
        return 1;
    }

    // Escrever o cabeçalho do CSV
    fprintf(arquivo_dados, "Geracao,Melhor_Fitness\n");

    // Gerar a população inicial
    for (int i = 0; i < TAM_POPULACAO; i++) {
        gerar_individuo_benchmark(populacao[i], n, min, max);
    }

    // Evolução do algoritmo genético
    for (int geracao = 0; geracao < NUM_GERACOES; geracao++) {
        // Avaliar a população
        for (int i = 0; i < TAM_POPULACAO; i++) {
            fitness[i] = calcular_fitness_benchmark(populacao[i], n, tipo_funcao);
        }

        // Encontrar o melhor fitness
        double melhor_fitness = fitness[0];
        for (int i = 1; i < TAM_POPULACAO; i++) {
            if (fitness[i] < melhor_fitness) {
                melhor_fitness = fitness[i];
            }
        }

        // Exibir o melhor fitness da geração atual
        printf("Geração %d - Melhor fitness: %.5f\n", geracao, melhor_fitness);

        // Salvar o melhor fitness no arquivo CSV
        fprintf(arquivo_dados, "%d,%.5f\n", geracao, melhor_fitness);

        // Cruzamento e Mutação
        for (int i = 0; i < TAM_POPULACAO; i += 2) {
            int pai1 = selecionar_individuo(fitness, TAM_POPULACAO);
            int pai2 = selecionar_individuo(fitness, TAM_POPULACAO);

            // Cruzamento
            cruzar_individuos(populacao[pai1], populacao[pai2], nova_populacao[i], nova_populacao[i+1], n);

            // Mutação
            mutar_individuo(nova_populacao[i], n, TAXA_MUTACAO, min, max);
            mutar_individuo(nova_populacao[i+1], n, TAXA_MUTACAO, min, max);
        }

        // Substituir a população antiga pela nova
        for (int i = 0; i < TAM_POPULACAO; i++) {
            for (int j = 0; j < n; j++) {
                populacao[i][j] = nova_populacao[i][j];
            }
        }
    }

    // Fechar o arquivo CSV
    fclose(arquivo_dados);

    printf("Dados de fitness salvos no arquivo fitness_data.csv.\n");

    return 0;
}