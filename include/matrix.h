# ifndef MATRIX_H
# define MATRIX_H

# include <stdlib.h>

typedef struct
{
    int rows; 
    int columns;
    double **data;
} Matrix;


Matrix* create_matrix(int rows, int columns);

#endif
