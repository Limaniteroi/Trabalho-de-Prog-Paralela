#include <stdio.h>
#include <time.h>

int main () {
    int x, soma=0, subtracao=0, mult=1, TAM = 1000;
    int vet[TAM];
    clock_t tempo_inicial, tempo_final, pulsos_decorridos;
    double tempo_em_segundos;

    tempo_inicial = clock();

    // Inicializa os elementos do vetor.
    // Cada elemento i, é inicializado com o valor i+1.
    for (x=0;x<TAM;x++) {
        vet[x] = x + 1;
    }

    // Imprime o valor de cada elemento do vetor.
    for (x=0;x<TAM;x++) {
        printf("vet[%d] = %d\n", x, vet[x]);
    }

    // Soma todos os elementos do vetor
    // &
    // Subtrai todos os elementos do vetor
    // &
    // Multiplica todos os elementos do vetor
    for (x=0;x<TAM;x++) {
        soma = soma + vet[x];
        subtracao = subtracao - vet[x];
        mult = mult * vet[x];
    }

    tempo_final = clock();
    pulsos_decorridos = tempo_final - tempo_inicial;
    tempo_em_segundos = (double)pulsos_decorridos / CLOCKS_PER_SEC;

    printf("Soma = %d\n", soma);
    printf("Subtracao = %d\n", subtracao);
    printf("Multiplicacao = %d\n", mult);
    printf("Foram gastos %3.17lf segundos.\n", tempo_em_segundos);
}
