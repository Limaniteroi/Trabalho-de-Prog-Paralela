#include <stdio.h>
#include "mpi.h"
#define TAM 1000

int main(int argc, char** argv){
	int meu_ranque, num_procs, etiq = 0, i=0;
    int soma=0, sub=0, mult=0, operacao=1, stop =3;
	int total_num, origem = 0, destino = 1, vet_envia[TAM], vet_recebe[TAM];
	MPI_Status estado;
	double tempo_inicial, tempo_final;
	
	//Inicializa a biblioteca MPI
	MPI_Init(&argc, &argv);
	
	//Começa a marcar o tempo
	tempo_inicial = MPI_Wtime();	
	
	// Diz para cada processo o seu ranque
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
	
	//Diz ao MPi o numero de processos que foi passado como parâmetro
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	//Inicialização do vetor
	for(i=0; i<TAM; i++){
		vet_envia[i] = i+1;
	}
	
	if(meu_ranque == origem){
		
		
		// Processo 0 (Mestre) ira enviar o vetor de numeros para cada processo escravo
		for(i=1; i<num_procs; i++){
			MPI_Send(vet_envia, TAM, MPI_INT, i, etiq, MPI_COMM_WORLD); 
			MPI_Send(operacao, 1, MPI_INT, i, etiq, MPI_COMM_WORLD);	
			operacao++;
		}
		
	}
	
	
	// Processos escravos irão realizar as operações
	else{
		//Processos recebem o vetor que irão utilziar
		MPI_Recv(vet_recebe, TAM, MPI_INT, origem, etiq, MPI_COMM_WORLD, &estado);
		MPI_Recv(&operacao, 1, origem, MPI_INT, origem, etiq, MPI_COMM_WORLD, &estado);
		
		if(operacao == 1){
			
			
			for(i = 0; i<TAM; i++)
				soma += vet_recebe[i];
				
			MPI_Send(&soma, 1, MPI_INT, origem, 1, MPI_COMM_WORLD);		
		}
		
		else if(operacao == 2){
			
			for(i =0; i<TAM; i++)
					sub -= vet_recebe[i];
					
			MPI_Send(&sub, 1, MPI_INT, origem, 2, MPI_COMM_WORLD);
				
		}		
				
		else if(operacao == 3){
			
			for(i=0;i<TAM; i++)
					mult += vet_recebe[i];
	
			MPI_Send(&mult, 1, MPI_INT, origem, 3, MPI_COMM_WORLD);	
				
		
		}
				
	}
		
		 
	
	if(meu_ranque == origem){
		
		
		while(stop!=0){	
			// Verifica se chegou a mensagem no processo de origem
			MPI_Probe(origem, etiq, MPI_COMM_WORLD, &estado);
			
			if(etiq == 1){
				
				MPI_Recv(&soma, 1, MPI_INT, origem, etiq, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				stop--;
			
			}
			
			else if(etiq == 2){
				
				MPI_Recv(&sub, 1, MPI_INT, origem, etiq, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				stop--;
			}	
			
			else if(etiq == 3){
				
				MPI_Recv(&mult, 1, MPI_INT, origem, etiq, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				stop--;
				
			}	
		}
		
		tempo_final = MPI_Wtime();
	
		printf("Soma: %d", soma);
		printf("Subtracao: %d", sub);
		printf("Multiplicacao: %d", mult);
		printf("Tempo de Execucao: %f", tempo_final-tempo_inicial);
			
	}
	
	// Limpa as pendências deixadas pelo MPI
	MPI_Finalize();
}
