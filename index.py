import pandas as pd
import matplotlib.pyplot as plt

arquivo = pd.read_csv('resultados.csv', header=None, skiprows=1, names=['Tempo Absoluto', 'Posição', 'Velocidade', 'Aceleração', 'Jerk'])
tempos_absolutos = pd.read_csv('resultados.csv', header=None).iloc[0].values  # Lê a primeira linha como tempos absolutos

fig, axs = plt.subplots(4, 1, figsize=(10, 15), sharex=True)
fig.suptitle("Gráficos de Posição, Velocidade, Aceleração e Jerk em função do Tempo Absoluto")

def plot_with_dashed_lines(ax, x, y, label, color):
    ax.plot(x, y, color=color, label=label)
    ax.set_ylabel(label)
    ax.grid(True)

plot_with_dashed_lines(axs[0], arquivo['Tempo Absoluto'], arquivo['Posição'], "Posição", 'b')
plot_with_dashed_lines(axs[1], arquivo['Tempo Absoluto'], arquivo['Velocidade'], "Velocidade", 'g')
plot_with_dashed_lines(axs[2], arquivo['Tempo Absoluto'], arquivo['Aceleração'], "Aceleração", 'r')
plot_with_dashed_lines(axs[3], arquivo['Tempo Absoluto'], arquivo['Jerk'], "Jerk", 'purple')

axs[3].tick_params(labelbottom=True)

# Ajusta o layout e mostra o gráfico
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
