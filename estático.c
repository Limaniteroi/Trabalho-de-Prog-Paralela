#include <stdio.h>
#include <omp.h>

int matriz_1[1000][1000];
int matriz_2[1000][1000];
int matriz_resultado[1000][1000];

int main(int argc, char *argv[]) { 

    int i,j,k, soma_produto;
    int tamanho = 1000;
    double tempo_inicial, tempo_final, tempo_decorrido;

    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++){

            matriz_1[i][j] = 1;
            matriz_2[i][j] = 1;
            matriz_resultado[i][j] = 0;
        }
    }

    tempo_inicial = omp_get_wtime();
        
    #pragma omp parallel for private(j, k, soma_produto) schedule (static) num_threads(2)

    for (i = 0; i < tamanho; ++i) {
        for(j = 0; j < tamanho; j++ ){

        soma_produto = 0; 

        for(k = 0; k < tamanho; k++){ 
            soma_produto += matriz_1[i][k] * matriz_2[k][j];
        }

        matriz_resultado[i][j] = soma_produto;

        }
    }

    tempo_final = omp_get_wtime();
    tempo_decorrido = tempo_final - tempo_inicial;

    printf("Tempo de execução: %f segundos\n", tempo_decorrido);
    printf("Resultado esperado (matriz_resultado[0][0]): %d\n", matriz_resultado[0][0]);

    return 0;
}