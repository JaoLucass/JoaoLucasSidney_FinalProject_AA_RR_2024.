import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Função para normalizar os dados
def normalize_data(fitness_values):
    min_val = np.min(fitness_values)
    max_val = np.max(fitness_values)
    return (fitness_values - min_val) / (max_val - min_val)

# Leitura do CSV
def read_benchmark_data(file_path):
    """
    Lê os dados de fitness dos benchmarks do arquivo CSV.
    """
    return pd.read_csv(file_path)

# Plot comparando os benchmarks com normalização
def plot_comparacao_normalizada(df):
    """
    Gera um gráfico de comparação entre benchmarks com valores normalizados.
    """
    plt.figure(figsize=(10, 6))
    
    for func in df.columns[1:]:
        normalized_values = normalize_data(df[func])
        plt.plot(df['Populacao'], normalized_values, label=func)
    
    plt.title("Comparação de Desempenho Entre Benchmarks (Normalizado)")
    plt.xlabel("População")
    plt.ylabel("Fitness (Normalizado)")
    plt.legend()
    plt.grid(True)
    
    # Salvando o gráfico em PNG
    plt.savefig('comparacao_normalizada.png', dpi=300)
    plt.show()

# Plot com escala logarítmica
def plot_comparacao_log(df):
    """
    Gera um gráfico de comparação com escala logarítmica para melhor visualização dos valores.
    """
    plt.figure(figsize=(10, 6))
    
    for func in df.columns[1:]:
        plt.plot(df['Populacao'], df[func], label=func)
    
    plt.yscale('log')
    plt.title("Comparação de Desempenho Entre Benchmarks (Escala Logarítmica)")
    plt.xlabel("População")
    plt.ylabel("Fitness (Escala Log)")
    plt.legend()
    plt.grid(True)
    
    # Salvando o gráfico em PNG
    plt.savefig('comparacao_logaritmica.png', dpi=300)
    plt.show()

# Subplots para cada função de benchmark
def plot_subplots(df):
    """
    Cria subplots individuais para cada função de benchmark para facilitar a visualização.
    """
    fig, axs = plt.subplots(len(df.columns)-1, figsize=(10, 15), sharex=True)
    
    for i, func in enumerate(df.columns[1:]):
        axs[i].plot(df['Populacao'], df[func], label=func)
        axs[i].set_title(f"Desempenho da Função {func}")
        axs[i].set_ylabel("Fitness")
        axs[i].legend()
        axs[i].grid(True)
    
    plt.xlabel("População")
    plt.tight_layout()
    
    # Salvando o gráfico em PNG
    plt.savefig('subplots_benchmarks.png', dpi=300)
    plt.show()

# Boxplot para análise estatística das funções
def plot_boxplot(df):
    """
    Gera um boxplot para análise estatística das funções de benchmark.
    """
    plt.figure(figsize=(10, 6))
    
    # Utiliza melt para transformar o dataframe em formato longo
    df_melted = pd.melt(df, id_vars=["Populacao"], var_name="Função", value_name="Fitness")
    sns.boxplot(x="Função", y="Fitness", data=df_melted)
    
    plt.title("Distribuição de Fitness por Função")
    plt.xlabel("Função")
    plt.ylabel("Fitness")
    plt.grid(True)
    
    # Salvando o gráfico em PNG
    plt.savefig('boxplot_benchmarks.png', dpi=300)
    plt.show()

# Função principal para executar as análises
def main():
    # Substitua o caminho abaixo pelo caminho correto para o arquivo CSV
    file_path = "fitness_data.csv"
    
    # Lendo os dados do arquivo CSV
    df = read_benchmark_data(file_path)
    
    print("Gerando gráficos de comparação com normalização...")
    plot_comparacao_normalizada(df)
    
    print("Gerando gráficos com escala logarítmica...")
    plot_comparacao_log(df)
    
    print("Gerando subplots para cada função de benchmark...")
    plot_subplots(df)
    
    print("Gerando boxplot para análise estatística das funções de benchmark...")
    plot_boxplot(df)

if __name__ == "__main__":
    main()
