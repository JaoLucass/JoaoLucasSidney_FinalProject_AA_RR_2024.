#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_VERTICES 7  // Número de nós
#define NUM_ARESTAS 12  // Número de arestas (isso representa o número total de conexões únicas)

int arestas[NUM_ARESTAS][2] = {
    {0, 1}, {0, 2}, {0, 3}, {0, 6},
    {1, 6}, {1, 2}, {1, 3},
    {2, 5}, {2, 4},
    {3, 6}, {5, 4},
    {6, 5}
};

// Função para gerar pesos aleatórios para os nós
void gerar_pesos_aleatorios(int pesos[NUM_VERTICES]) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        pesos[i] = rand() % 10 + 1;  // Peso aleatório entre 1 e 10
    }
}

// Função para gerar um arquivo de população com base nos nós e arestas
void gerar_arquivo_populacao(const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Escrever número de nós e arestas no arquivo
    fprintf(arquivo, "%d\n", NUM_VERTICES);
    fprintf(arquivo, "%d\n", 2 * NUM_ARESTAS);  // Para garantir não-direcionamento, dobraremos o número de arestas

    // Gerar pesos aleatórios para os nós
    int pesos[NUM_VERTICES];
    gerar_pesos_aleatorios(pesos);

    // Escrever pesos dos nós no arquivo
    for (int i = 0; i < NUM_VERTICES; i++) {
        fprintf(arquivo, "%d %d\n", i, pesos[i]);
    }

    // Escrever arestas no arquivo (incluindo ambas as direções para garantir grafo não direcionado)
    for (int i = 0; i < NUM_ARESTAS; i++) {
        int v1 = arestas[i][0];
        int v2 = arestas[i][1];
        
        // Adicionar a aresta v1 -> v2
        fprintf(arquivo, "%d %d\n", v1, v2);
        // Adicionar a aresta v2 -> v1 (garantindo simetria)
        fprintf(arquivo, "%d %d\n", v2, v1);
    }

    fclose(arquivo);
    printf("Arquivo de população gerado com sucesso: %s\n", nome_arquivo);
}

int main() {
    srand(time(NULL));  // Semente para gerar números aleatórios

    // Gerar arquivo com a população
    gerar_arquivo_populacao("populacao.txt");

    return 0;
}
