#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

// Cada processo escravo vai emitir um array vet_envia com 3 campos para o processo mestre, onde:
//      1. vet_envia[0] := soma_parcial
//      2. vet_envia[1] := sub_parcial
//      3. vet_envia[2] := prod_parcial 
#define TAM_ENVIA 3

// Total de número a serem somados, subtraídos e multiplicados.
// Representa a carga de trabalho que deverá ser dividida entre os processos escravos.
#define TAM 21


int main(int argc, char** argv) {
    // Ranque do processo mestre.
    int raiz = 0; 
    // Vetor com os operandos
    long int vet[TAM];
    // Vetor a partir do qual os escravos irão enviar seus resultados parciais.
    long int vet_envia[TAM_ENVIA];
    // Resultados parciais inicializados com seus respectivos elementos neutros. 
    vet_envia[0] = 0; // soma_parcial 
    vet_envia[1] = 0; // sub_parcial
    vet_envia[2] = 1; // prod_parcial
    // Vetor no qual o mestre receber as somas parciais.
    long int* vet_recebe;
    // Ranque de cada processo.
    int meu_ranque;
    // Total de processos que estão a executar este programa.
    int num_procs;
    // Variáveis para controllar subintervalos onde cada escravo irá trabalhar.
    int iteracoes_por_processo, lim_inf, lim_sup;
    // Para mensurar o tempo de execução
    double tempo_inicial, tempo_final;

    // Realiza operações de preparação necessárias
    // para que a biblioteca MPI seja utilizada.
    MPI_Init(&argc, &argv);

    // Começa a marcar o tempo
    tempo_inicial = MPI_Wtime();

    /* Identifica um processo dentro de um comunicador.
    (primeiro argumento) escrevendo em uma variável 
    (segundo argumento) seu ranque.
     
    OBS1.: um comunicador é um conjunto de processos. Podem se
    comunicar aqueles processos que estão em um mesmo comunicador.
     
    OBS2.: ranque é um inteiro não negativo i que identifica
    um processo em um comunicador, onde 
        i = 0, 1, ..., (N - 1)
        N = total de processos em um comunicador. 
     
    OBS3.: MPI_COMM_WORLD é um comunicador padrão, definido
    pela biblioteca MPI e consistindo de todos os processos
    executando quando o programa começa. 
    */
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);

    // Determina quantos processos estão executando o programa,
    // escrevendo esse número na variável passada como o 
    // segundo parâmetro.
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Mestre aloca o vetor no qual ele irá receber os resultados parciais.
    // [soma_1, sub_1, prod_1, soma_2, sub_2, prod_2, ..., soma_(n-1), sub_(n-1), prod_(n-1)]
    if (meu_ranque == raiz) { 
        vet_recebe = (long int *) malloc(num_procs * TAM_ENVIA * sizeof(long int));
    } else {
        // TODO: se TAM não for divisível por num_procs, então problema.
        // CORRIGIR DEPOIS.
        //
        // Aqui estamos dividindo o total de iterações para cada
        // um dos processos escravos. Como 0 é mestre, precisamos
        // subtrair 1 de num_procs.
        iteracoes_por_processo = TAM / (num_procs - 1);

        //  Cálculo para determinar o intervalo de i no
        // qual cada loop deve iterar.
        //
        //  Nos 3 loops abaixo, os processos escravos 
        // fazem, cada um, uma parte da soma, subtração
        // e multiplicação total.
        lim_inf = (meu_ranque - 1) * iteracoes_por_processo;
        lim_sup = (meu_ranque) * iteracoes_por_processo;


        // Inicialização do vetor.
        printf("Sou o processo %d e estou inicializando meus itens.\n", meu_ranque);
        for (int i = lim_inf; i < lim_sup; i++) {
            vet[i] = i + 1;
        }

        // Imprime o valor de cada elemento do vetor.
        for (int i = lim_inf; i < lim_sup; i++) {
            printf("vet[%d] = %ld\n", i, vet[i]);
        }

        // Realiza as operações parciais, passando por todos os itens de vet 
        // que estão no subintervalo do escravo.
        for (int i = lim_inf; i < lim_sup; i++) {
            vet_envia[0] += vet[i];
            vet_envia[1] -= vet[i];
            vet_envia[2] *= vet[i];
        }
    }

    // Mestre coleta trabalho dos escravos.
    MPI_Gather(
        vet_envia, // vet_envia
        TAM_ENVIA, // cont_envia
        MPI_LONG, // tipo_envia
        vet_recebe, // vet_recebe
        TAM_ENVIA, // cont_recebe
        MPI_LONG, // tipo_recebe
        raiz, // raiz
        MPI_COMM_WORLD // comunicador padrão
    );
    
    // Para a marcação do tempo.
    tempo_final = MPI_Wtime();

    // Processo mestre (raiz) faz o pós-processamento:
    // i.e., soma as "somas parciais"
    if (meu_ranque == 0) {
        for (int i = TAM_ENVIA; i < num_procs * TAM_ENVIA; i+=TAM_ENVIA) {
            vet_recebe[0] += vet_recebe[i];
            vet_recebe[1] += vet_recebe[i+1];
            vet_recebe[2] *= vet_recebe[i+2];
        }
        printf("Sou o processo %d e o resultado final é:\n", meu_ranque);
        printf("\tSoma: %ld\n", vet_recebe[0]);
        printf("\tSubtração: %ld\n", vet_recebe[1]);
        printf("\tProduto: %ld\n", vet_recebe[2]);
    }

    // Limpa pendências deixadas pelo MPI.
    MPI_Finalize();

    return 0;
}
