# JoaoLucasSidney_FinalProject_AA_RR_2024.
# Projeto de Algoritmo Genético para Cobertura de Vértices

Este projeto visa resolver o problema de Cobertura de Vértices em grafos utilizando um algoritmo genético implementado em C. A visualização dos resultados e a análise de desempenho são realizadas em Python.

## Estrutura do Projeto

- **algoritmo_genetico.c**: Implementa o algoritmo genético para resolver o problema de cobertura de vértices.
- **gerador_populacao.c**: Gera populações iniciais aleatórias para o algoritmo genético.
- **resultados.py**: Código Python responsável pela geração de gráficos dos resultados obtidos pelo algoritmo genético.

## Pré-requisitos

Para compilar e executar os códigos C e gerar gráficos em Python, certifique-se de ter os seguintes pacotes instalados:

- **Compilador GCC**: Para compilar os arquivos `.c`.
- **Python 3.x**: Para executar o script de geração de gráficos.
- Bibliotecas Python necessárias (instaláveis via `pip`):
  ```bash
  pip install matplotlib numpy
Compilação e Execução
1. Compilando o Código em C
No terminal, navegue até o diretório onde os arquivos .c estão localizados e execute os seguintes comandos:

./gerador_populacao > populacao_inicial.txt
./algoritmo_genetico < populacao_inicial.txt > resultados.txt

populacao_inicial.txt: Contém os dados da população inicial gerados pelo arquivo gerador_populacao.c.
resultados.txt: Saída gerada pela execução do algoritmo genético.

3. Gerando Gráficos em Python
Com os resultados gerados pelo algoritmo genético, utilize o script Python para gerar gráficos e visualizar o desempenho:

python resultados.py resultados.txt

O script resultados.py irá ler o arquivo resultados.txt e gerar gráficos de acordo com os dados de desempenho do algoritmo.

Observações
Certifique-se de que os arquivos de entrada e saída (como populacao_inicial.txt e resultados.txt) estejam no mesmo diretório do código Python para evitar problemas de caminho de arquivo.
Modifique o código conforme necessário para ajustar o tamanho da população, número de iterações, e outras variáveis específicas do algoritmo genético.
