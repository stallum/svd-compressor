# include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <png.h>
#include "image.h"


// FUNÇÃO PARA CARREGAR IMAGENS .JPEG
Image loadJPEG(const char* filename) {
    FILE *file = fopen(filename, "rb");
    
    if (!file) {
        Image img = {0};
        return img;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int channels = cinfo.output_components;
    
    int size = width * height * channels;

    unsigned char *data = (unsigned char *)malloc(size);

    while(cinfo.output_scanline < (unsigned int)height){

        unsigned char *row =
            data + cinfo.output_scanline * width * channels;

        jpeg_read_scanlines(&cinfo,&row,1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(file);

    Image img;

    img.width = width;
    img.height = height;
    img.channels = channels;
    img.data = data;

    return img;
    
}

// FUNÇÃO PARA CARREGAR IMAGENS .PNG
Image loadPNG(const char* filename) {
    FILE *file = fopen(filename, "rb");
    
    if (!file) {
        Image img = {0};
        return img;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    
    png_init_io(png, file);
    png_read_info(png,info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    int channels = png_get_channels(png, info);
    
    int size = width * height * channels;

    unsigned char *data = (unsigned char *)malloc(size);

    png_bytep rows[height];
    
    for(int y=0;y<height;y++)
        rows[y] = data + y * width * channels;

    png_read_image(png,rows);

    fclose(file);

    png_destroy_read_struct(&png, &info, NULL);
    
    Image img;

    img.width = width;
    img.height = height;
    img.channels = channels;
    img.data = data;

    return img;

}


// FUNÇÃO PARA CARREGAR ARQUIVOS PPM
Image loadPPM(const char* filename) {

    FILE* f = fopen(filename,"rb");

    if(!f){
        printf("Cannot open image\n");
        exit(1);
    }

    char format[3];

    int width,height,max;

    fscanf(f,"%2s",format);

    if(format[0] != 'P' || format[1] != '6'){
        printf("Not a PPM P6 image\n");
        exit(1);
    }

    fscanf(f,"%d %d",&width,&height);
    fscanf(f,"%d",&max);

    fgetc(f);

    int size = width * height * 3;

    unsigned char* data = malloc(size);

    fread(data,1,size,f);

    fclose(f);

    Image img;

    img.width = width;
    img.height = height;
    img.channels = 3;
    img.data = data;

    return img;
}

// FUNÇÃO PARA SALVAR IMAGENS JPEG
void saveJPEG(const char* filename, Image* img){

    FILE* file = fopen(filename,"wb");

    if(!file){
        printf("Cannot create JPEG file\n");
        exit(1);
    }
struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo,file);

    cinfo.image_width = img->width;
    cinfo.image_height = img->height;
    cinfo.input_components = img->channels;

    if(img->channels == 3)
        cinfo.in_color_space = JCS_RGB;
    else
        cinfo.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo,90,TRUE);

    jpeg_start_compress(&cinfo,TRUE);

    while(cinfo.next_scanline < cinfo.image_height){

        unsigned char* row =
            img->data + cinfo.next_scanline * img->width * img->channels;

        jpeg_write_scanlines(&cinfo,&row,1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(file);
}


// FUNÇÃO PARA SALVAR PNG
void savePNG(const char* filename, Image* img){

    FILE* fp = fopen(filename,"wb");

    if(!fp){
        printf("Cannot create PNG file\n");
        exit(1);
    }

    png_structp png =
        png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

    png_infop info =
        png_create_info_struct(png);

    png_init_io(png,fp);

    int color;

    if(img->channels == 3)
        color = PNG_COLOR_TYPE_RGB;
    else
        color = PNG_COLOR_TYPE_GRAY;

    png_set_IHDR(
        png,
        info,
        img->width,
        img->height,
        8,
        color,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png,info);

    png_bytep rows[img->height];

    for(int y=0;y<img->height;y++)
        rows[y] = img->data + y * img->width * img->channels;

    png_write_image(png,rows);

    png_write_end(png,NULL);

    fclose(fp);
}

// FREE IMAGE
void freeImage(Image* img){

    free(img->data);
}