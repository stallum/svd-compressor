#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include "image_io.h" // Seus headers do projeto
#include "svd.h"
#include <time.h>


int main() {
    clock_t inicio = clock();
    // 1. Carrega a imagem
    Image* img = read_image();
    if (img == NULL) return 1;

    // 2. Define o nível de compressão (k)
    // Se a imagem tem 500 colunas, k=50 mantém os 50 principais valores singulares
    int k = 20; 
    printf("Iniciando compressao SVD (k=%d)...\n", k);
    
    // Chama a função que você implementou no svd.c
    process_and_compress(img, k);

    // 3. Limpeza total de memória (Regra de Ouro)
    free_svd_image(img);
    printf("Memoria liberada. Fim do programa.\n");
    clock_t fim = clock();

    double tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo de execucao: %f segundos\n", tempo_cpu);
    
    return 0;
}