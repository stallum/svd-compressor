#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
#include "image.h"

Image loadImage(const char *filename){

    char *ext = strrchr(filename,'.');

    if(!ext){
        printf("Unknown format\n");
        exit(1);
    }

    if(strcmp(ext,".ppm")==0)
        return loadPPM(filename);

    if(strcmp(ext,".jpg")==0 || strcmp(ext,".jpeg")==0)
        return loadJPEG(filename);

    if(strcmp(ext,".png")==0)
        return loadPNG(filename);

    printf("Unsupported format\n");
    exit(1);
}

/* remove extensão do arquivo */
void getBaseName(const char *input,char *base){

    strcpy(base,input);

    char *dot = strrchr(base,'.');

    if(dot)
        *dot = '\0';
}

/* leitura segura de linha */
void readLine(char *buffer,int size){

    fgets(buffer,size,stdin);

    buffer[strcspn(buffer,"\n")] = 0;
}

int main(){

    int option;

    char input[256];
    char output[256];
    char base[256];

    char line[32];

    printf("=================================\n");
    printf("        IMAGE COMPRESSOR\n");
    printf("=================================\n");
    printf("1 - Compress Image\n");
    printf("2 - Decompress Image\n");
    printf("3 - Exit\n");
    printf("Choose option: ");

    fgets(line,sizeof(line),stdin);
    option = atoi(line);

    while (option != 3) {

        if(option == 1){
    
            printf("Input file: ");
            readLine(input, 256);
    
            printf("Output file (press Enter for default): ");
            readLine(output, 256);
    
            if(strlen(output) == 0){
    
                getBaseName(input,base);
                sprintf(output,"compressed_%s.ppm",base);
    
                printf("Default output: %s\n",output);
            }
    
            Image img = loadImage(input);
    
            int freq[256] = {0};
    
            countRGBFrequency(&img,freq);
    
            Node* root = buildHuffmanTree(freq);
    
            HuffCode codes[256] = {0};
    
            buildCodes(root,0,0,codes);
    
            int format;
    
            char *ext = strrchr(input, '.');
    
            if(strcmp(ext,".jpg")==0 || strcmp(ext,".jpeg")==0)
                format = 1;
    
            else if(strcmp(ext,".png")==0)
                format = 2;
    
            else
                format = 3;
    
            compressRGB(&img,output,codes,freq,format);
    
            freeImage(&img);
    
            printf("Compression finished\n");

            fgets(line,sizeof(line),stdin);
            option = atoi(line);

        }
    
        else if(option == 2){
    
            printf("Input file: ");
            readLine(input, 256);
    
            printf("Output file (press Enter for default): ");
            readLine(output, 256);
    
            if(strlen(output) == 0){
    
                getBaseName(input, base);
                sprintf(output, "%s.jpg", base);
    
                printf("Default output: %s\n", output);
            }
    
            decompressRGB(input,output);
    
            printf("Decompression finished\n");

            fgets(line,sizeof(line),stdin);
            option = atoi(line);
        }
    
        else if(option == 3){
    
            printf("Exiting program\n");
        }
    
        else{
    
            printf("Invalid option\n");
        }
    }

    return 0;
}