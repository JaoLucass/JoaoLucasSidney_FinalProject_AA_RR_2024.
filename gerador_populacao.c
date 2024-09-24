#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_POPULACOES 1000  // Número de populações a serem geradas
#define MIN_TAMANHO 10       // Tamanho mínimo da população e número de vértices
#define MAX_TAMANHO 200      // Reduzido para 200 para evitar problemas de memória
#define MIN_VERTICES 10      // Número mínimo de vértices (dimensões)
#define MAX_VERTICES 200     // Reduzido para 200 para evitar problemas de memória

// Função para gerar um indivíduo aleatório
void gerar_individuo(double *individuo, int n, double min, double max) {
    for (int i = 0; i < n; i++) {
        individuo[i] = min + ((double)rand() / RAND_MAX) * (max - min);  // Gera números entre [min, max]
    }
}

// Função para gerar uma população
void gerar_populacao(double **populacao, int tamanho_populacao, int n, double min, double max) {
    for (int i = 0; i < tamanho_populacao; i++) {
        gerar_individuo(populacao[i], n, min, max);
    }
}

// Função para salvar uma população no arquivo
void salvar_populacao(FILE *arquivo, double **populacao, int tamanho_populacao, int n, int num_populacao) {
    fprintf(arquivo, "População %d - Tamanho: %d, Vértices: %d\n", num_populacao, tamanho_populacao, n);
    for (int i = 0; i < tamanho_populacao; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(arquivo, "%.5f ", populacao[i][j]);
        }
        fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "\n");
}

// Função para verificar se a combinação de tamanho de população e vértices já foi usada
int combinacao_usada(int tamanho_populacao, int n, int combinacoes[MAX_POPULACOES][2], int num_combinacoes) {
    for (int i = 0; i < num_combinacoes; i++) {
        if (combinacoes[i][0] == tamanho_populacao && combinacoes[i][1] == n) {
            return 1;  // Combinação já usada
        }
    }
    return 0;  // Combinação nova
}

int main() {
    srand(time(NULL));  // Semente para geração de números aleatórios

    double min_val = -5.12, max_val = 5.12;  // Limites para os valores dos vértices
    int combinacoes[MAX_POPULACOES][2];      // Armazena combinações já usadas (tamanho da população e vértices)
    int num_combinacoes = 0;                 // Contador de combinações únicas

    // Abrir arquivo para salvar populações
    FILE *arquivo = fopen("populacao.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo populacao.txt!\n");
        return 1;
    }

    for (int p = 0; p < MAX_POPULACOES; p++) {
        int tamanho_populacao, num_vertices;

        // Gerar combinações únicas de tamanho da população e número de vértices
        do {
            tamanho_populacao = MIN_TAMANHO + rand() % (MAX_TAMANHO - MIN_TAMANHO + 1);  // Tamanho da população aleatório entre 10 e 200
            num_vertices = MIN_VERTICES + rand() % (MAX_VERTICES - MIN_VERTICES + 1);    // Número de vértices aleatório entre 10 e 200
        } while (combinacao_usada(tamanho_populacao, num_vertices, combinacoes, num_combinacoes));

        // Armazenar a combinação única
        combinacoes[num_combinacoes][0] = tamanho_populacao;
        combinacoes[num_combinacoes][1] = num_vertices;
        num_combinacoes++;

        // Alocar memória para a população
        double **populacao = (double **)malloc(tamanho_populacao * sizeof(double *));
        if (populacao == NULL) {
            printf("Erro na alocação de memória para a população.\n");
            return 1;
        }

        for (int i = 0; i < tamanho_populacao; i++) {
            populacao[i] = (double *)malloc(num_vertices * sizeof(double));
            if (populacao[i] == NULL) {
                printf("Erro na alocação de memória para o indivíduo %d da população %d.\n", i, p+1);
                return 1;
            }
        }

        // Gerar a população e salvar no arquivo
        gerar_populacao(populacao, tamanho_populacao, num_vertices, min_val, max_val);
        salvar_populacao(arquivo, populacao, tamanho_populacao, num_vertices, p + 1);

        // Liberar a memória da população
        for (int i = 0; i < tamanho_populacao; i++) {
            free(populacao[i]);
        }
        free(populacao);
    }

    // Fechar o arquivo
    fclose(arquivo);

    printf("Populações geradas e salvas no arquivo populacao.txt.\n");

    return 0;
}

