#ifndef IMAGE_H
#define IMAGE_H

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



#endif