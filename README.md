# Trabalhos de Programação Paralela

Este repositório documenta dois projetos de programação paralela desenvolvidos com o objetivo de analisar e comparar diferentes estratégias de paralelização para tarefas computacionais distintas.

O trabalho foi desenvolvido por **Marianna, Rodrigo e João Vitor**.

## **Análise de Paralelismo com MPI**

Este repositório contém as implementações de um programa em C para realizar operações matemáticas (soma, subtração e multiplicação) em um vetor, utilizando a biblioteca MPI (Message Passing Interface) para processamento paralelo. 

O objetivo do projeto é comparar o desempenho de diferentes estratégias de paralelização para a mesma tarefa computacional.

-----

### **Descrição do Problema**

O código original, presente no arquivo `sequencial.c`, realiza as seguintes operações em um vetor de 1000 elementos:

  * Inicialização do vetor com valores de `x + 1`.
  * Cálculo da soma de todos os elementos.
  * Cálculo da subtração (progressiva) de todos os elementos.
  * Cálculo da multiplicação de todos os elementos.

O projeto de paralelização modifica o programa sequencial para distribuir o trabalho entre múltiplos processos, utilizando diferentes modelos de comunicação.

-----

### **Versões Paralelas**

Três versões do programa foram implementadas, cada uma seguindo um modelo de paralelismo específico:

#### **a) Versão com 4 Processos Dedicados**

Nesta versão, a execução é limitada a quatro processos. Cada um dos três primeiros processos é dedicado a uma única função: um para a soma, outro para a subtração e um terceiro para a multiplicação. O quarto processo atua como o coordenador, informando aos demais qual tarefa devem executar e, ao final, imprimindo os resultados.

#### **b) Versão com Mestre/Escravo e Comunicação Coletiva**

Esta implementação utiliza um modelo mestre/escravo com um número variável de processos. A divisão das tarefas é feita de forma balanceada, e a comunicação é realizada através de operações MPI_Gather para coletar os resultados de forma eficiente.

#### **c) Versão com Modelo Pipeline**

Nesta versão, os processos são organizados em uma cadeia (pipeline). Cada processo realiza uma parte do cálculo e passa o resultado parcial para o próximo processo, até que o último processo da cadeia tenha o resultado final.

-----

### **Requisitos e Compilação**

Para compilar e executar o código, você precisará de um compilador C e de uma biblioteca MPI, como o OpenMPI ou o MPICH.

1.  **Instale o MPI:**

    ```bash
    # Exemplo para sistemas baseados em Debian/Ubuntu
    sudo apt-get install libopenmpi-dev
    ```

2.  **Compile o código:**
    Use o compilador `mpicc` para compilar o arquivo C.

    ```bash
    mpicc -o <nome_do_executavel> <nome_do_arquivo>.c 
    ```

    *Exemplo: `mpicc -o versao_a versao_a.c `*

-----

### **Como Executar**

Use o comando `mpirun`, especificando o número de processos (`-np`) para cada versão.

```bash
# Executar a Versão a) com 4 processos
mpirun -np 4 paralelo_a

# Executar a Versão b) com 8 processos (exemplo)
mpirun -np 8 paralelo_b

# Executar a Versão c) com 4 processos
mpirun -np 4 paralelo_c

# Compilar todos
Use o arquivo compile_all.sh

```

### **Resultados Obtidos**

Os valores foram obtidos através da média de 10 execuções de cada uma das versões.
Verifique em `tempos.py`

Para todos os programas (1000):
Soma = 500500 ; 
Subtração = -500500 ; 
Multiplicação = 0

-----

## **Multiplicação de Matrizes com OpenMP**

### **Descrição do Problema**
O segundo projeto foca na paralelização da multiplicação de matrizes (Matrix Multiplication) utilizando a API OpenMP para programação em memória compartilhada. O objetivo é desenvolver um programa que multiplique duas matrizes quadradas de dimensões 1000 x 1000 cujos elementos são todos iguais a 1. A análise deve explorar o impacto do número de threads e dos diferentes mecanismos de escalonamento (schedule) no desempenho.

-----

### **Versões Paralelas (OpenMP)**
A paralelização é baseada na divisão do trabalho entre $P$ threads utilizando a diretiva $\text{OpenMP}$. 
A análise exige a execução da mesma rotina com diferentes parâmetros:

| Fator de Análise  |   Valores Explorados |
| :--- | :--- |
| Número de Threads  | 2, 4, 8, e 16 |
| Método de Escalonamento | (static) Estático, (dynamic) Dinâmico e (guided) Guiado |

Três versões do programa sequencial foram implementadas, cada uma utilizando uma política diferente de escalonamento de laços com OpenMP para paralelizar a multiplicação de duas matrizes 1000×1000.

##### Versão com Escalonamento Estático 
Nesta versão, as matrizes são inicializadas com valores 1 e a multiplicação é paralelizada com a diretiva #pragma omp parallel for schedule(static) num_threads(16).​
O laço externo sobre as linhas da matriz resultado é dividido em blocos fixos e atribuídos antecipadamente às 16 threads, que calculam as entradas da matriz resultado e, ao final, o tempo total de execução é medido e salvo em um arquivo de texto para análise de desempenho.​

##### Versão com Escalonamento Dinâmico 
Aqui as matrizes também são preenchidas com 1 e a multiplicação é feita em paralelo com #pragma omp parallel for schedule(dynamic, 100) num_threads(16).​
O laço externo é particionado em blocos de 100 iterações, e esses blocos são atribuídos dinamicamente às threads à medida que terminam seu trabalho, permitindo que threads ociosas assumam novos blocos até completar todas as linhas, com o tempo de execução sendo calculado e registrado em um arquivo de texto.​

##### Versão com Escalonamento Guided 
Nesta versão, as matrizes A e B são inicializadas com 1, e a multiplicação é paralelizada com #pragma omp parallel for schedule(guided) num_threads(16).​
As iterações do laço externo começam em blocos maiores e vão diminuindo de tamanho automaticamente, conforme o guided scheduling, enquanto as threads calculam as entradas da matriz C e, ao final, o programa mede o tempo de execução e o armazena no arquivo de texto.

-----

### **Requisitos e Compilação (OpenMP)**
Para compilar e executar o código, é necessário um compilador C que suporte OpenMp (como GCC ou Clang).

### Instale o Compilador:
```bash
# Exemplo para GCC
sudo apt-get install gcc
```

### Execute:
Defina o número de threads (P) usando a variável de ambiente OMP_NUM_THREADS antes de executar.

```bash
# Executar com 4 threads
export OMP_NUM_THREADS=4
```

### Execute o código:

Use o flag -fopenmp para habilitar a compilação paralela.
```bash
gcc -o <nome_do_executavel> <nome_do_arquivo>.c -fopenmp

# Executar a Versão de Escalonamento Estático
gcc -o teste estático.c -fopenmp

# Executar a Versão de Escalonamento Dinâmico
gcc -o teste dinamico.c -fopenmp

# Executar a Versão de Escalonamento Guided
gcc -o teste guided.c -fopenmp
```
