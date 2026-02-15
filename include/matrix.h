# ifndef MATRIX_H
# define MATRIX_H

# include <stdlib.h>

typedef struct
{
    int rows; 
    int columns;
    double **data;
} Matrix;

/**
 * ALOCAÇÃO DINÂMICA: Cria a matriz na memória Heap.
 * Retorna um ponteiro para a struct Matrix.
 */
Matrix* create_matrix(int rows, int cols);

/**
 * GERENCIAMENTO DE MEMÓRIA: Libera toda a memória alocada.
 * Essencial para não deixar "lixo" na memória (Memory Leaks).
 */
void free_matrix(Matrix *mat);

/**
 * MANIPULAÇÃO POR PONTEIROS: Define um valor em uma coordenada específica.
 */
void set_value(Matrix *mat, int r, int c, double value);

/**
 * UTILITÁRIO: Apenas para testar se a matriz foi criada corretamente.
 */
void print_matrix_dims(Matrix *mat);

#endif
