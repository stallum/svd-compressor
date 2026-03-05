#ifndef SVD_H
#define SVD_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_io.h"

/**
 * STRUCT: SVD
 * Guarda o resultado da compressão. Em vez de uma matriz gigante,
 * guardamos 3 matrizes menores que, multiplicadas, reconstroem a imagem.
 */
typedef struct {
    int k;               // Quantidade de valores singulares mantidos
    Matrix *U_reduced;   // Matriz U cortada
    Matrix *S_diagonal;  // Vetor/Matriz de valores singulares
    Matrix *Vt_reduced;  // Matriz V transposta cortada
} SVD;

/**
 * FUNÇÃO DE COMPRESSÃO:
 * Recebe a imagem original e o nível de compressão 'k'.
 * Retorna a struct com as matrizes decompostas.
 */
void process_and_compress(Image *img, int r);

/**
 * FUNÇÃO DE RECONSTRUÇÃO:
 * Pega as matrizes menores e multiplica elas para voltar a ser uma imagem.
 */
Matrix* reconstruct_from_svd(SVD *compressed);

/**
 * LIMPEZA:
 * Libera a memória de todas as matrizes dentro da struct SVD.
 */
void free_compressed_svd(SVD *compressed);

#endif