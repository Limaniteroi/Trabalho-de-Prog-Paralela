#include <stdio.h>
#include "mpi.h"

// Funções MPI podem ser chamadas somente
// após MPI_Init
// &
// antes de MPI_Finalize
int main(int argc, char** argv) {
    int soma = 0, subtracao = 0, mult = 1, TAM = 1000000;
    int total_soma = 0, total_subtracao = 0, total_mult = 1; 
    int vet[TAM];
    int ranque, num_procs;
    int iteracoes_por_processo;
    int lim_inf, lim_sup;
    double tempo_inicial, tempo_final;

    // Realiza operações de preparação necessárias
    // para que a biblioteca MPI seja utilizada.
    MPI_Init(&argc, &argv);

    // Começa a marcar o tempo
    tempo_inicial = MPI_Wtime();

    // Identifica um processo dentro de um comunicador 
    // (primeiro argumento) escrevendo em uma variável 
    // (segundo argumento) seu ranque.
    //
    // OBS1.: um comunicador é um conjunto de processos. Podem se
    // comunicar aqueles processos que estão em um mesmo comunicador.
    //
    // OBS2.: ranque é um inteiro não negativo i que identifica
    // um processo em um comunicador, onde 
    //      i = 0, 1, ..., (N - 1)
    //      N = total de processos em um comunicador. 
    //
    // OBS3.: MPI_COMM_WORLD é um comunicador padrão, definido
    // pela biblioteca MPI e consistindo de todos os processos
    // executando quando o programa começa.
    MPI_Comm_rank(MPI_COMM_WORLD, &ranque);

    // Determina quantos processos estão executando o programa,
    // escrevendo esse número na variável passada como o 
    // segundo parâmetro.
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // TODO: se TAM não for divisível por num_procs, então problema.
    // CORRIGIR DEPOIS.
    iteracoes_por_processo = TAM / num_procs;

    // Cálculo para determinar o intervalo de i no
    // qual cada loop deve iterar.
    lim_inf = ranque * iteracoes_por_processo;
    lim_sup = (ranque + 1) * iteracoes_por_processo;
    
    // Inicialização do vetor.
    for (int i = lim_inf; i < lim_sup; i++) {
        vet[i] = i + 1;
    }

    // Imprime o valor de cada elemento do vetor.
    for (int i = lim_inf; i < lim_sup; i++) {
        printf("vet[%d] = %d\n", i, vet[i]);
    }

    // Soma, subtrai e multiplica.
    for (int i = lim_inf; i < lim_sup; i++) {
        soma = soma + vet[i];
        subtracao = subtracao - vet[i];
        mult = mult * vet[i];
    }

    // Redução para soma.
    MPI_Reduce(
        &soma, // operando: variável de cada processo que será utilizada na operação de redução especificada.
        &total_soma, // resultado: variável onde será armazenado o resultado da operação sobre os operandos.
        1, // quantidade de posições de memória das variáveis acima. Neste caso, 1, por não serem vetores.
        MPI_INT, // Tipo de operando e resultado.
        MPI_SUM, // Operação de redução.
        0, // Ranque do processo raiz.
        MPI_COMM_WORLD // Comunicador.
    );
    
    // Redução para subtração.
    MPI_Reduce(
        &subtracao, 
        &total_subtracao, 
        1,
        MPI_INT,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    // Redução para multiplicação.
    MPI_Reduce(
        &mult, 
        &total_mult,
        1,
        MPI_INT,
        MPI_PROD,
        0,
        MPI_COMM_WORLD
    );

    // Para a marcação do tempo.
    tempo_final = MPI_Wtime();

    // Processo 0, a raiz, imprime os resultados.
    if (ranque == 0) {
        printf("total_soma = %d\n", total_soma);
        printf("total_subtracao = %d\n", total_subtracao);
        printf("total_mult = %d\n", total_mult);
        printf(
            "Foram gastos %3.6f segundos para calcular os resultados acima com uma precisão de %3.3e segundos\n", 
            tempo_final - tempo_inicial,
            MPI_Wtick()
        );
    }

    // Limpa pendências deixadas pelo MPI.
    MPI_Finalize();

    return 0;
}
