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
#define TAM 1000


int main(int argc, char** argv) {
    // Ranque do processo mestre.
    int raiz = 0; 
    // Vetor com os operandos
    int vet[TAM];
    // Vetor a partir do qual os escravos irão enviar seus resultados parciais.
    int vet_envia[TAM_ENVIA];
    // Resultados parciais inicializados com seus respectivos elementos neutros. 
    vet_envia[0] = 0; // soma_parcial 
    vet_envia[1] = 0; // sub_parcial
    vet_envia[2] = 1; // prod_parcial
    // Vetor no qual o mestre receberá os resultados parciais.
    int* vet_recebe;
    // Ranque de cada processo.
    int meu_ranque;
    // Total de processos que estão a executar este programa.
    int num_procs;
    // Variáveis para controlar subintervalos onde cada escravo irá trabalhar.
    int iteracoes_por_processo, lim_inf, lim_sup;
    // Para medir o tempo de execução.
    double tempo_inicial, tempo_final;

    // Realiza operações de preparação necessárias
    // para que a biblioteca MPI seja utilizada.
    MPI_Init(&argc, &argv);

    // Começa a marcar o tempo.
    tempo_inicial = MPI_Wtime();

    // Identifica um processo dentro de um comunicador.
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);

    // Determina quantos processos estão executando o programa.
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Mestre aloca o vetor no qual ele irá receber os resultados parciais.
    // [soma_1, sub_1, prod_1, soma_2, sub_2, prod_2, ..., soma_(n-1), sub_(n-1), prod_(n-1)]
    if (meu_ranque == raiz) { 
        vet_recebe = (int *) malloc(num_procs * TAM_ENVIA * sizeof(int));
    } else {
        // OBS.: se TAM não for divisível por num_procs => problema.
        //
        // Aqui estamos dividindo o total de iterações para cada
        // um dos processos escravos. Como 0 é mestre, precisamos
        // subtrair 1 de num_procs.
        iteracoes_por_processo = TAM / (num_procs - 1);

        // limites inferior e superior dos subintervalos nos quais
        // cada processo escravo deve iterar.
        //
        // [lim_inf, lim_sup)
        lim_inf = (meu_ranque - 1) * iteracoes_por_processo;
        lim_sup = meu_ranque != (num_procs - 1) 
            ? (meu_ranque) * iteracoes_por_processo 
            : TAM; // O escravo de maior ranque itera até o final,
            // de modo que o programa funciona mesmo quando TAM, não é
            // divisível pelo número de escravos.

        // Inicialização do vetor.
        for (int i = lim_inf; i < lim_sup; i++) {
            vet[i] = i + 1;
        }

        // Imprime o valor de cada elemento do vetor.
        for (int i = lim_inf; i < lim_sup; i++) {
            printf("vet[%d] = %d\n", i, vet[i]);
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
        MPI_INT, // tipo_envia
        vet_recebe, // vet_recebe
        TAM_ENVIA, // cont_recebe
        MPI_INT, // tipo_recebe
        raiz, // raiz
        MPI_COMM_WORLD // comunicador padrão
    );
    
    // Processo mestre (raiz) faz o pós-processamento:
    // i.e., soma as "somas parciais", 
    //       soma as "subtrações parciais"
    //       multiplica os "produtos parciais"
    if (meu_ranque == 0) {
        for (int i = 0; i < num_procs * TAM_ENVIA; i+=TAM_ENVIA) {
            vet_recebe[0] += vet_recebe[i];
            vet_recebe[1] += vet_recebe[i+1];
            vet_recebe[2] *= vet_recebe[i+2];
        }
        
        tempo_final = MPI_Wtime();

        printf("Soma: %d\n", vet_recebe[0]);
        printf("Subtração: %d\n", vet_recebe[1]);
        printf("Produto: %d\n", vet_recebe[2]);
        printf(
            "Foram gastos %3.6f segundos\n",
            tempo_final - tempo_inicial
        );
        
    }

    // Limpa pendências deixadas pelo MPI.
    MPI_Finalize();

    return 0;
}
