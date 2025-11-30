#include <stdio.h>
#include <omp.h>
#include <time.h>

int A[1000][1000];
int B[1000][1000];
int C[1000][1000];
    
int main() 
{

    int i, j, k;
    int sum = 0;

    for( int i = 0; i < 1000; i++)
    {
        for(int j =0; j<1000;j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }   

    
    double t_inicio = omp_get_wtime();

    #pragma omp parallel for schedule (guided) num_threads(16) private(j, k, sum)   
    for (int i = 0; i < 1000; i++)
    {

        for (k =0; k < 1000; k++)
        {

                sum = 0;

            for(j = 0; j< 1000; j++)
            {
                    sum += A[i][k] * B[k][j];
            }
                
                C[i][j] = sum;
        }
    }
        
    double t_fim = omp_get_wtime();
    printf("Tempo de execucação: %f segundos\n", t_fim-t_inicio);
    //printf("%d\n", C[0][0]);

    FILE *f = fopen("tempo_guided_16.txt", "a");
    if (f) {
        fprintf(f, "%f\n", t_fim - t_inicio);
        fclose(f);
    }

    return 0;
}

