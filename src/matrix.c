# include <gsl/gsl_matrix.h>
# include "matrix.h"
# include <stdio.h>
# include <stdlib.h>

// FUNÇÃO PARA CRIAR MATRIZ

Matrix* create_matrix (int rows, int cols) {
    Matrix *mat = (Matrix*) malloc(sizeof(Matrix));
    if (mat == NULL) return NULL; // erro na memoria

    mat->rows = rows;
    mat->cols = cols;

    mat->data = (double**) malloc(rows * sizeof(double*));

    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        mat->data[i] = (double*) calloc(cols, sizeof(double));
        if (mat->data[i] == NULL) {
            // For para limpar valores já alocados em caso de falha no codigo

            for (int j = 0; j < i; j++) free(mat->data[j]);
            free(mat->data);
            free(mat);

            return NULL;
        }

    }
    return mat;

}

// FUNÇÃO PARA MOSTRAR A MATRIZ

void print_matrix(gsl_matrix *m) {
    for (size_t i = 0; i < m->size1; i++) {
        for (size_t j = 0; j < m->size2; j++) {
            printf("%f ", gsl_matrix_get(m, i, j));
        }
        printf("\n");
    }
}

// LIMPAR MATRIZ, LIMPA CADA UM DOS ELEMENTOS DE UMA MATRIZ

void free_matrix(Matrix *mat) {
    if (mat == NULL) return;

    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }

    free(mat->data);
    free(mat);
}

// MATRIZ TRANSPOSTA ( FORMULA: MATRIX B[j][i] = A[i][j])

Matrix* transpose_matrix(Matrix *mat) {
    if (mat == NULL) return NULL;

    Matrix *trans = create_matrix(mat->cols, mat->rows); 

    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            trans->data[j][i] = mat->data[i][j];
        }
    }
    return trans;
}

// MULTIPLICAÇÃO DE MATRIZES

Matrix* multiply_matrices(Matrix *A, Matrix *B) {
    if (A->cols != B->rows) {
        
        printf("ERRO: DIMENSÕES INCOMPATÍVEIS PARA A MULTIPLICAÇÃO DE MATRIZES! \n");

        return NULL;
    };

    Matrix *result = create_matrix(A->rows, B->cols);

    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            double sum = 0;
            for (int k = 0; k < A->cols; k++) {
                sum += A->data[i][k] * B->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }

    return result;

}