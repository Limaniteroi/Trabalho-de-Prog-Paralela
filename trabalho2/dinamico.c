#include <stdio.h>
#include <time.h>
#include <omp.h>

int matriz_1[1000][1000];
int matriz_2[1000][1000];
int matriz_resultado[1000][1000];

int main(void){

    int i, j, k;
    double tempo_inicial, tempo_final, tempo_decorrido;

    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++){

            matriz_1[i][j] = 1;
            matriz_2[i][j] = 1;
        }
    }

    tempo_inicial = omp_get_wtime();

    #pragma omp parallel for private(i, j, k) schedule (dynamic, 100) num_threads(16)
    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++){

            int soma_produto = 0; 

            for(k = 0; k < 1000; k++){ 
                soma_produto += matriz_1[i][k] * matriz_2[k][j];
            }
            
            matriz_resultado[i][j] = soma_produto;
            
        }
    }

    tempo_final = omp_get_wtime();
    tempo_decorrido = tempo_final - tempo_inicial;

    printf("Tempo de execução: %f segundos\n", tempo_decorrido);
    //printf("%d\n", matriz_resultado[0][0]);

    FILE *f = fopen("tempo_dinamico_16.txt", "a");
    if (f) {
        fprintf(f, "%f\n", tempo_decorrido);
        fclose(f);
    }

    return 0;
}