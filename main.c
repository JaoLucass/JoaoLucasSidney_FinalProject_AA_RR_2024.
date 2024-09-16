#define NUM_VERTICES 6
#define NUM_ARESTAS 7

typedef struct {
    int vertice1;
    int vertice2;
} Aresta;

Aresta grafo[NUM_ARESTAS] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0}, {1, 4}
};

int calcular_fitness(int *solucao) {
    int num_cameras = 0;
    int cobertas[NUM_ARESTAS] = {0};

    // Contar número de câmeras instaladas
    for (int i = 0; i < NUM_VERTICES; i++) {
        if (solucao[i] == 1) {
            num_cameras++;
        }
    }

    // Verificar se todas as arestas estão cobertas
    for (int i = 0; i < NUM_ARESTAS; i++) {
        int v1 = grafo[i].vertice1;
        int v2 = grafo[i].vertice2;
        if (solucao[v1] == 1 || solucao[v2] == 1) {
            cobertas[i] = 1;
        }
    }

    // Penalidade por arestas não cobertas
    int penalidade = 0;
    for (int i = 0; i < NUM_ARESTAS; i++) {
        if (cobertas[i] == 0) {
            penalidade += 100;  // Penalidade alta para forçar cobertura
        }
    }

    return num_cameras + penalidade;
}

int main() {
    // Exemplo de solução
    int solucao1[NUM_VERTICES] = {1, 0, 1, 0, 0, 1};  // Câmeras nos vértices 0, 2, 5
    int solucao2[NUM_VERTICES] = {1, 1, 0, 0, 1, 0};  // Câmeras nos vértices 0, 1, 4

    // Calcular e imprimir fitness das soluções
    printf("Fitness da Solução 1: %d\n", calcular_fitness(solucao1));
    printf("Fitness da Solução 2: %d\n", calcular_fitness(solucao2));

    return 0;
}
