import pandas as pd
import matplotlib.pyplot as plt

# Carrega o arquivo CSV, com a primeira linha para os tempos absolutos
df = pd.read_csv('resultados.csv', header=None, skiprows=1, names=['Tempo Absoluto', 'Posição', 'Velocidade', 'Aceleração', 'Jerk'])
tempos_absolutos = pd.read_csv('resultados.csv', header=None).iloc[0].values  # Lê a primeira linha como tempos absolutos

# Define a figura e os eixos dos subplots
fig, axs = plt.subplots(4, 1, figsize=(10, 15), sharex=True)
fig.suptitle("Gráficos de Posição, Velocidade, Aceleração e Jerk em função do Tempo Absoluto")

# Função para plotar dados e adicionar linhas verticais tracejadas com rótulos
def plot_with_dashed_lines(ax, x, y, label, color):
    ax.plot(x, y, color=color, label=label)
    ax.set_ylabel(label)
    for i, tempo in enumerate(tempos_absolutos):
        ax.axvline(x=tempo, color='gray', linestyle='--', linewidth=0.7)
        ax.text(tempo, ax.get_ylim()[0] - (ax.get_ylim()[1] - ax.get_ylim()[0]) * 0.05, f"T{i+1}", 
                ha='center', va='top', color='gray', fontsize=9)
    ax.grid(True)

plot_with_dashed_lines(axs[0], df['Tempo Absoluto'], df['Posição'], "Posição", 'b')
plot_with_dashed_lines(axs[1], df['Tempo Absoluto'], df['Velocidade'], "Velocidade", 'g')
plot_with_dashed_lines(axs[2], df['Tempo Absoluto'], df['Aceleração'], "Aceleração", 'r')
plot_with_dashed_lines(axs[3], df['Tempo Absoluto'], df['Jerk'], "Jerk", 'purple')

axs[3].tick_params(labelbottom=False)

# Ajusta o layout e mostra o gráfico
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
