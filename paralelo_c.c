#include <stdio.h>
#include <stdlib.h> // Para malloc
#include <math.h>
#include "mpi.h"
#define TAM 1000

int somatorio(int vet_pedaco[], int tam_pedaco) {
    int soma = 0;
    for(int x = 0; x < tam_pedaco; x++){
        soma += vet_pedaco[x];
    }
    return soma;
}

int multiplicacao(int vet_pedaco[], int tam_pedaco) {
    int mult = 1; 
    for(int x = 0; x < tam_pedaco; x++){
        mult *= vet_pedaco[x];
    }
    return mult;
}

int subtracao(int vet_pedaco[], int tam_pedaco) {
    int sub = 0;
    for(int x = 0; x < tam_pedaco; x++){
        sub -= vet_pedaco[x];
    }
    return sub;
}

int main (int argc, char *argv[]) {
    int meu_ranque, num_procs;
    double tempo_inicio, tempo_fim;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // 1. Definição do tamanho do pedaço
    int quantidade_por_processo = TAM / num_procs;

    // O restante deve ser tratado separadamente ou com uma abordagem mais robusta
    // como padding (simplificado aqui para tamanhos exatos).
    if (TAM % num_procs != 0) {
        if (meu_ranque == 0) {
            printf("Atenção: TAM (%d) não é divisível por num_procs (%d). Por favor entre com um número divisível.\n", TAM, num_procs);
        }
        MPI_Finalize();
        return 1;
    }

    int *vet = NULL;
    int *vet_local = (int *)malloc(quantidade_por_processo * sizeof(int));

    if (meu_ranque == 0) {
        // 2. Inicialização do vetor (apenas pelo Mestre)
        vet = (int *)malloc(TAM * sizeof(int));
        for(int i = 0; i < TAM; i++){
            vet[i] = i + 1; // Vetor de 1 a 1000
        }
        tempo_inicio = MPI_Wtime();
    }

    // 3. Distribuição dos dados (Scatter)
    MPI_Scatter(vet, quantidade_por_processo, MPI_INT, 
                vet_local, quantidade_por_processo, MPI_INT, 
                0, MPI_COMM_WORLD);

    // 4. Cálculo Local
    int soma_local = somatorio(vet_local, quantidade_por_processo);
    int mult_local = multiplicacao(vet_local, quantidade_por_processo);
    int sub_local = subtracao(vet_local, quantidade_por_processo);

    // 5. Agregação dos Resultados (Reduce)
    int soma_global, mult_global, sub_global;

    // A. Reduce para SOMA
    MPI_Reduce(&soma_local, &soma_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // B. Reduce para MULTIPLICAÇÃO
    MPI_Reduce(&mult_local, &mult_global, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

    // C. Reduce para SUBTRAÇÃO (usando MPI_SUM para somar os negativos)
    // Subtração total = vet[0] - vet[1] - vet[2]... 
    // Se somarmos os 'sub_local' que já vieram negativos, obtemos o resultado total (excluindo a primeira operação).
    MPI_Reduce(&sub_local, &sub_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if (meu_ranque == 0) {
        tempo_fim = MPI_Wtime();
        
        // A subtração agregada (sub_global) é: -vet[1] - vet[2] - ... - vet[999].
        // O valor correto é: vet[0] + sub_global.
        // Como o vetor começa em 1, vet[0] = 1.
        int sub_total = 1 + sub_global; 

        printf("\n--- Resultados ---\n");
        printf("Soma Total = %d\n", soma_global);
        printf("Subtracao Total (1 - 2 - 3 ... - 1000) = %d\n", sub_total);
        printf("Multiplicacao Total = %d\n", mult_global);
        printf("Tempo gasto = %3.6f segundos.\n", tempo_fim - tempo_inicio);
    }
    
    free(vet_local);
    if (meu_ranque == 0) {
        free(vet);
    }
    
    MPI_Finalize();
    return 0;
}