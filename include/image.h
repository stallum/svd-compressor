#ifndef IMAGE_H
#define IMAGE_H

# include <stdio.h>

typedef struct {

    int width;
    int height;
    int channels;
    unsigned char *data;

} Image;

Image loadPPM(const char* filename);
Image loadJPEG(const char* filename);
Image loadPNG(const char* filename);

void savePNG(const char* filename, Image* img);
void saveJPEG(const char* filename, Image* img);


void freeImage(Image *img);



#endif //IMAGE_H