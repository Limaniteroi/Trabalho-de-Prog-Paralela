#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int TAM = 1000;
    int vet[TAM];
    int soma = 0, subtracao = 0, mult = 1;
    double tempo_inicial, tempo_final;
    
    // MPI_Init: Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    tempo_inicial= MPI_Wtime(); // Inicializa o temporizador MPI
    
    // MPI_Comm_rank: Obtém o identificador (rank) do processo atual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // MPI_Comm_size: Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Verifica se há pelo menos 4 processos (produtor + 3 operações)
    if (size < 4) {
        if (rank == 0) {
            printf("Este programa precisa de pelo menos 4 processos!\n");
            printf("Execute com: mpirun -np 4 ./programa\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    // ==================== PROCESSO 0: PRODUTOR ====================
    if (rank == 0) {
        printf("=== PROCESSO %d: PRODUTOR ===\n", rank);
        
        // Inicializa o vetor
        for (int x = 0; x < TAM; x++) {
            vet[x] = x + 1;
        }
        
        // Exibe o vetor
        printf("Vetor inicializado:\n");
        for (int x = 0; x < TAM; x++) {
            printf("vet[%d] = %d\n", x, vet[x]);
        }
        printf("\n");
        
        // MPI_Send: Envia o vetor para o próximo processo (rank 1)
        // Parâmetros: buffer, tamanho, tipo, destino, tag, comunicador
        MPI_Send(vet, TAM, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Processo %d: Vetor enviado para processo 1\n\n", rank);
    }
    
    // ==================== PROCESSO 1: SOMA ====================
    else if (rank == 1) {
        printf("=== PROCESSO %d: CALCULA SOMA ===\n", rank);
        
        // MPI_Recv: Recebe o vetor do processo anterior (rank 0)
        // Parâmetros: buffer, tamanho, tipo, origem, tag, comunicador, status
        MPI_Recv(vet, TAM, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d: Vetor recebido do processo 0\n", rank);
        
        // Realiza a operação de SOMA
        for (int x = 0; x < TAM; x++) {
            soma = soma + vet[x];
        }
        printf("Processo %d: Soma calculada = %d\n\n", rank, soma);
        
        // Envia o vetor para o próximo processo (rank 2)
        MPI_Send(vet, TAM, MPI_INT, 2, 0, MPI_COMM_WORLD);
        printf("Processo %d: Vetor enviado para processo 2\n", rank);
        
        // Envia o resultado da soma para o processo final (rank 3)
        MPI_Send(&soma, 1, MPI_INT, 3, 1, MPI_COMM_WORLD);
        printf("Processo %d: Resultado da soma enviado para processo 3\n\n", rank);
    }
    
    // ==================== PROCESSO 2: SUBTRAÇÃO ====================
    else if (rank == 2) {
        printf("=== PROCESSO %d: CALCULA SUBTRAÇÃO ===\n", rank);
        
        // Recebe o vetor do processo 1
        MPI_Recv(vet, TAM, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d: Vetor recebido do processo 1\n", rank);
        
        // Realiza a operação de SUBTRAÇÃO
        for (int x = 0; x < TAM; x++) {
            subtracao = subtracao - vet[x];
        }
        printf("Processo %d: Subtração calculada = %d\n\n", rank, subtracao);
        
        // Envia o vetor para o próximo processo (rank 3)
        MPI_Send(vet, TAM, MPI_INT, 3, 0, MPI_COMM_WORLD);
        printf("Processo %d: Vetor enviado para processo 3\n", rank);
        
        // Envia o resultado da subtração para o processo final
        MPI_Send(&subtracao, 1, MPI_INT, 3, 2, MPI_COMM_WORLD);
        printf("Processo %d: Resultado da subtração enviado para processo 3\n\n", rank);
    }
    
    // ==================== PROCESSO 3: MULTIPLICAÇÃO E RESULTADO FINAL ====================
    else if (rank == 3) {
        printf("=== PROCESSO %d: CALCULA MULTIPLICAÇÃO E EXIBE RESULTADOS ===\n", rank);
        
        // Recebe o vetor do processo 2
        MPI_Recv(vet, TAM, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d: Vetor recebido do processo 2\n", rank);
        
        // Realiza a operação de MULTIPLICAÇÃO
        for (int x = 0; x < TAM; x++) {
            mult = mult * vet[x];
        }
        printf("Processo %d: Multiplicação calculada = %d\n\n", rank, mult);
        
        // Recebe os resultados dos processos anteriores
        MPI_Recv(&soma, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d: Resultado da soma recebido do processo 1\n", rank);
        
        MPI_Recv(&subtracao, 1, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d: Resultado da subtração recebido do processo 2\n\n", rank);
        
        // Exibe todos os resultados finais
        tempo_final = MPI_Wtime(); // Finaliza o temporizador MPI
        printf("Soma          = %d\n", soma);
        printf("Subtração     = %d\n", subtracao);
        printf("Multiplicação = %d\n", mult);
        printf("Foram gastos %3.6f segundos para calcular os resultados acima.\n",
            tempo_final - tempo_inicial);
    }
    
    // MPI_Finalize: Finaliza o ambiente MPI
    MPI_Finalize();
    
    return 0;
}
