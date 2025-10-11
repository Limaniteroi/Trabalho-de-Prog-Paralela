#include <stdio.h>
#include "mpi.h"
#define TAM 1000000

int main(int argc, char** argv){
	int meu_ranque, num_procs, etiq = 0, i=0;
    int soma=0, sub=0, operacao=1, stop =3;
    unsigned long long int mult=1;
    int total_num, origem = 0, vet[TAM];
	double tempo_inicial, tempo_final;
	
    //Inicializacao do vetor
	for(i=0; i<TAM; i++){
		vet[i] = i+1;
	}

	//Inicializa a biblioteca MPI
	MPI_Init(&argc, &argv);
	
	//Começa a marcar o tempo
	tempo_inicial = MPI_Wtime();	
	
	// Diz para cada processo o seu ranque
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
	
	//Diz ao MPi o numero de processos que foi passado como parametro
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	if(meu_ranque == origem){
		
		// Processo 0 ira enviaras operacoes para cada processo
		for(i=1; i<num_procs; i++){
            MPI_Send(&operacao, 1, MPI_INT, i, etiq, MPI_COMM_WORLD);	
			operacao++;
		}
    }
		
	
	// Demais processos irao realizar as operacoes
	else{
		
		MPI_Recv(&operacao, 1, MPI_INT, origem, etiq, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		if(operacao == 1){
			
		    for(i = 0; i<TAM; i++)
				soma += vet[i];
				
			MPI_Send(&soma, 1, MPI_INT, origem, 1, MPI_COMM_WORLD);		
		}
		
		else if(operacao == 2){
			
			for(i =0; i<TAM; i++)
					sub -= vet[i];
					
			MPI_Send(&sub, 1, MPI_INT, origem, 2, MPI_COMM_WORLD);
				
		}		
				
		else if(operacao == 3){
			
			for(i=0;i<TAM; i++)
					mult *= vet[i];
	
			MPI_Send(&mult, 1, MPI_INT, origem, 3, MPI_COMM_WORLD);	
						
		}
	}		
		
    if(meu_ranque == origem){

    
		MPI_Recv(&soma, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				
		MPI_Recv(&sub, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
		MPI_Recv(&mult, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
		tempo_final = MPI_Wtime();
	    
		printf("Soma: %d\n", soma);
		printf("Subtracao: %d\n", sub);
		printf("Multiplicacao: %llu\n", mult);
		printf("Tempo de Execucao: %f\n", tempo_final-tempo_inicial);	
  
    }

	
	// Limpa as pendencias deixadas pelo MPI
	MPI_Finalize();
}
