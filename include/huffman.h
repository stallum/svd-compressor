#ifndef HUFFMAN_H
#define HUFFMAN_H
# include "image.h"

typedef struct Node {
    unsigned char data;
    unsigned freq;
    struct Node *left, *right;
} Node;

typedef struct {
    unsigned int code;
    int length;
} HuffCode;

void countFrequency (const char *filename, int freq[256]);

Node* buildHuffmanTree(int freq[256]);

void buildCodes(Node* root, unsigned int code, int length, HuffCode table[256]);

void compressFile(const char* input,
                  const char* output,
                  HuffCode codes[256],
                  int freq[256]);

void countRGBFrequency(Image* img, int freq[256]);

void compressRGB(Image* img,
                 const char* output,
                 HuffCode codes[256],
                 int freq[256],
                 int format);

void decompressRGB(const char* input, const char* output);

void freeTree(Node* root);


#endif // HUFFMAN_H