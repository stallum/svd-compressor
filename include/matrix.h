# ifndef MATRIX_H
# define MATRIX_H

# include <stdlib.h>

typedef struct {
    int rows; 
    int cols;
    double **data;
} Matrix;

/**
 * ALOCAÇÃO DINÂMICA: Cria a matriz na memória Heap.
 * Retorna um ponteiro para a struct Matrix.
 */
Matrix* create_matrix(int rows, int cols);

/**
 * MULTIPLICAÇÃO DE MATRIZES
 * Retorna um ponteiro para a struct Matrix.
 */
Matrix* multiply_matrices(Matrix *A, Matrix *B);

/**
 * TRANPOSIÇÃO DE MATRIZES
 * Retorna um ponteiro para a struct Matrix.
 */

 Matrix* transpose_matrix(Matrix *mat);

/**
 * GERENCIAMENTO DE MEMÓRIA: Libera toda a memória alocada.
 * Essencial para não deixar "lixo" na memória (Memory Leaks).
 */
void clean_matrix(Matrix *mat);

/**
 * MANIPULAÇÃO POR PONTEIROS: Define um valor em uma coordenada específica.
 */
void set_value(Matrix *mat, int r, int c, double value);

/**
 * UTILITÁRIO: Apenas para testar se a matriz foi criada corretamente.
 */
void print_matrix(Matrix *mat);

#endif