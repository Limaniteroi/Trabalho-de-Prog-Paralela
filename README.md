# Trabalho de Programação Paralela

### **Análise de Paralelismo com MPI**

Este repositório contém as implementações de um programa em C para realizar operações matemáticas (soma, subtração e multiplicação) em um vetor, utilizando a biblioteca MPI (Message Passing Interface) para processamento paralelo. O trabalho foi desenvolvido por Marianna, Rodrigo e João Vitor.

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

# Executar todos
Use o arquivo compile_all.sh

```

### **Resultados Obtidos**

A tabela abaixo resume os resultados de desempenho para cada uma das versões paralelas, comparadas à execução sequencial.

| Versão | Tempo de Execução | Speedup | Eficiência |
| :--- | :--- | :--- | :--- |
| **Sequencial** | 1,17E-03 | - | - |
| **Versão a) (4 processos)** | 5,53E-05 | 18,08 | 451,88% |
| **Versão b) (4 processos)** | 6,10E-04 | 1,93 | 48,18% |
| **Versão c) (4 processos)** | 1,87E-03 | 0,63 | 15,72% |
-----
Os valores foram obtidos através da média de 10 execuções de cada uma das versões.
Verifique em `tempos.py`

### **Autores**

  * Marianna de Brito Lima
  * Rodrigo Ramos Vinagre Cavalcanti de Carvalho
  * João Vítor Luciano Gonçalves
