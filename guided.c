#include <stdio.h>
#include <omp.h>
#include <time.h>

#define TAM 1000
    
int main() 
{

    int j = 0;
    int k = 0;
    int sum = 0;

    int A[TAM][TAM];
    int B[TAM][TAM];
    int C[TAM][TAM];

    double t_inicio = omp_get_wtime();

#pragma omp parallel for private(j)
    for( int i = 0; i < TAM; i++)
    {
    for(int j =0; j<TAM;j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }   

    printf("Matriz C:\n");
#pragma omp parallel for schedule (guided) num_threads(4) private(j, k, sum)   
        for (int i = 0; i < TAM; i++){

            for (k =0; k < TAM; k++){

                for(j = 0; j< TAM; j++){
                    sum += A[i][j] * B[j][k];
                }
                
                C[i][k] = sum;
                sum = 0;
            }
        }

        for ( int i = 0; i < TAM; i++)
        {
            for (int j = 0; j < TAM; j++)
            {
                printf("%d  ", C[i][j]);
            }
            printf("\n");
            
        }
        
        double t_fim = omp_get_wtime();
        printf("Tempo de execucação: \t %f \n", t_fim-t_inicio);
        return 0;
}

