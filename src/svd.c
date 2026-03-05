#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_io.h"

gsl_matrix* gsl_matrix_transpose_new(const gsl_matrix *m) {
    gsl_matrix *tr = gsl_matrix_alloc(m->size2, m->size1);
    if (!tr) return NULL;
    for (size_t i = 0; i < m->size1; i++) {
        for (size_t j = 0; j < m->size2; j++) {
            gsl_matrix_set(tr, j, i, gsl_matrix_get(m, i, j));
        }
    }
    return tr;
}

gsl_matrix* reconstruct_channel(gsl_matrix *U, gsl_vector *S, gsl_matrix *V, int r) {
    if (!U || !S || !V) return NULL;
    
    int h = U->size1;
    int w = V->size1; 

    gsl_matrix *res = gsl_matrix_alloc(h, w);
    if (!res) return NULL;

    gsl_matrix_const_view Ur = gsl_matrix_const_submatrix(U, 0, 0, h, r);
    gsl_matrix_const_view Vr = gsl_matrix_const_submatrix(V, 0, 0, w, r);
    gsl_matrix *Vr_scaled = gsl_matrix_alloc(w, r);
    gsl_matrix_memcpy(Vr_scaled, &Vr.matrix);
    
    for (int j = 0; j < r; j++) {
        double sj = gsl_vector_get(S, j);
        gsl_vector_view col = gsl_matrix_column(Vr_scaled, j);
        gsl_vector_scale(&col.vector, sj);
    }

    gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, &Ur.matrix, Vr_scaled, 0.0, res);

    gsl_matrix_free(Vr_scaled);

    return res;
}

void process_and_compress(Image *img, int r) {
    if (!img || !img->channels_matrices) return;

    int h = img->height;
    int w = img->width;
    int is_landscape = (h < w);


    #pragma omp parallel for
    for (int k = 0; k < img->channels; k++) {
        gsl_matrix *A_work = NULL;
        
        // Regra da GSL: M >= N. Se landscape, trabalhamos com a transposta.
        if (is_landscape) {
            A_work = gsl_matrix_transpose_new(img->channels_matrices[k]);
        } else {
            A_work = gsl_matrix_alloc(h, w);
            gsl_matrix_memcpy(A_work, img->channels_matrices[k]);
        }

        // int M = A_work->size1; // Nova altura (pode ser w se for landscape)
        int N = A_work->size2; // Nova largura (pode ser h se for landscape)
        int r_adj = (r > N) ? N : r;

        gsl_matrix *V = gsl_matrix_alloc(N, N);
        gsl_vector *S = gsl_vector_alloc(N);
        gsl_vector *work = gsl_vector_alloc(N);

        if (!A_work || !V || !S || !work) {
            fprintf(stderr, "Falha na alocação canal %d\n", k);
            continue;
        }

        // Executa SVD (A_work vira U)
        gsl_linalg_SV_decomp(A_work, V, S, work);

        // Reconstrói a partir dos componentes
        gsl_matrix *reconstructed = reconstruct_channel(A_work, S, V, r_adj);
        printf("reconstruido canal %d\n", k);
        if (reconstructed) {
            if (is_landscape) {
                // Se era landscape, a reconstrução está em (w x h), transpomos de volta
                gsl_matrix *final = gsl_matrix_transpose_new(reconstructed);
                gsl_matrix_memcpy(img->channels_matrices[k], final);
                gsl_matrix_free(final);
            } else {
                gsl_matrix_memcpy(img->channels_matrices[k], reconstructed);
            }
            gsl_matrix_free(reconstructed);
        }

        gsl_matrix_free(A_work);
        gsl_matrix_free(V);
        gsl_vector_free(S);
        gsl_vector_free(work);
    }
}