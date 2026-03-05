#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include "matrix.h"

/* * Estrutura para armazenar a imagem decomposta em canais GSL.
 * Atende aos requisitos de Struct e Ponteiros.
 */
typedef struct {
    int width;
    int height;
    int channels;
    gsl_matrix **channels_matrices; 
} Image;

/**
 * Converte os dados brutos da stb_image para a estrutura de matrizes GSL.
 * @param stb_data Ponteiro para os pixels carregados pela stb_image.
 * @param w Largura da imagem.
 * @param h Altura da imagem.
 * @param c Número de canais (ex: 3 para RGB).
 * @return Ponteiro para a struct Image alocada dinamicamente.
 */
Image* convert_stb_to_gsl(unsigned char *stb_data, int w, int h, int c);

/**
 * Realiza a decomposição SVD e reconstrói a imagem com base no fator de compressão.
 * @param img Ponteiro para a struct Image.
 * @param k_percent Porcentagem de valores singulares a manter (0.0 a 1.0).
 */
void compress_image_svd(Image *img, double k_percent);

/**
 * Libera toda a memória alocada dinamicamente para a imagem.
 */
void free_svd_image(Image *img);

/**
 * Lê a imagem a partir do caminho destinado.
 */
Image* read_image ();

/**
 * Salva a imagem comprimida
 */

// Image* save_image(Image* i);



#endif // IMAGE_IO_H