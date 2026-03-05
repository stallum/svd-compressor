#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "image_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

Image* convert_stb_to_gsl(unsigned char *stb_data, int w, int h, int c) {
    Image *img = (Image*) malloc(sizeof(Image));
    if (!img) return NULL;

    img->width = w;
    img->height = h;
    img->channels = c;
    img->channels_matrices = (gsl_matrix**) malloc(c * sizeof(gsl_matrix*));

    if (!img->channels_matrices) {
        free(img);
        return NULL;
    }

    for (int k = 0; k < c; k++) {
        img->channels_matrices[k] = gsl_matrix_alloc(h, w);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int stb_index = (i * w + j) * c + k;
                gsl_matrix_set(img->channels_matrices[k], i, j, (double)stb_data[stb_index]);
            }
        }
    }
    return img;
}

Image* read_image () {

    int width, height, channels;
    const char* filename = "./Example/luna.jpg"; // Supports JPEG, PNG, BMP, TGA, PSD, GIF, HDR, PIC, PNM

    // Load the image
    unsigned char* image_data = stbi_load(filename, &width, &height, &channels, 0);

    
    
    if (image_data == NULL) {
        printf("Error: Could not load image %s\n", filename);
        // You can use stbi_failure_reason() for a brief explanation of the failure
        printf("Reason: %s\n", stbi_failure_reason());
        return NULL;
    }
    Image* img = convert_stb_to_gsl(image_data, width, height, channels);
    stbi_image_free(image_data);
    
    // Image successfully loaded:
    printf("Image loaded:\n");
    printf("  Width: %d\n", img->width);
    printf("  Height: %d\n", img->height);
    printf("  Channels: %d\n", img->channels);
    for (int k = 0; k < img->channels; k++) {
        printf("\nChannel %d:\n", k);
        print_matrix(img->channels_matrices[k]);
    }

    return img;

}

void free_svd_image(Image *img) {
    if (img) {
        for (int k = 0; k < img->channels; k++) {
            gsl_matrix_free(img->channels_matrices[k]);
        }
        free(img->channels_matrices);
        free(img);
    }
}

