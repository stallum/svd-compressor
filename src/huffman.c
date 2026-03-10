#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "bit_io.h"
#include "image.h"

typedef struct
{
    int size;
    Node *array[256];
} MinHeap;

Node *newNode(unsigned char data, unsigned freq)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->left = n->right = NULL;
    n->data = data;
    n->freq = freq;
    return n;
}

void swap(Node **a, Node **b)
{
    Node *t = *a;
    *a = *b;
    *b = t;
}

void heapify(MinHeap *h, int i)
{

    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < h->size && h->array[l]->freq < h->array[smallest]->freq)
        smallest = l;

    if (r < h->size && h->array[r]->freq < h->array[smallest]->freq)
        smallest = r;

    if (smallest != i)
    {
        swap(&h->array[i], &h->array[smallest]);
        heapify(h, smallest);
    }
}

Node *extractMin(MinHeap *h)
{

    Node *n = h->array[0];
    h->array[0] = h->array[--h->size];

    heapify(h, 0);

    return n;
}

void insertHeap(MinHeap *h, Node *n)
{

    int i = h->size++;

    while (i && n->freq < h->array[(i - 1) / 2]->freq)
    {

        h->array[i] = h->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    h->array[i] = n;
}

Node *buildHuffmanTree(int freq[256])
{

    MinHeap heap;
    heap.size = 0;

    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            heap.array[heap.size++] = newNode(i, freq[i]);

    for (int i = heap.size / 2 - 1; i >= 0; i--)
        heapify(&heap, i);

    while (heap.size > 1)
    {

        Node *left = extractMin(&heap);
        Node *right = extractMin(&heap);

        Node *parent = newNode(0, left->freq + right->freq);

        parent->left = left;
        parent->right = right;

        insertHeap(&heap, parent);
    }

    return extractMin(&heap);
}

void buildCodes(Node *root, unsigned int code, int length, HuffCode table[256])
{

    if (!root->left && !root->right)
    {

        table[root->data].code = code;
        table[root->data].length = length;

        return;
    }

    if (root->left)
        buildCodes(root->left, code << 1, length + 1, table);

    if (root->right)
        buildCodes(root->right, (code << 1) | 1, length + 1, table);
}

void countFrequency(const char *filename, int freq[256])
{

    FILE *f = fopen(filename, "rb");

    unsigned char b;

    while (fread(&b, 1, 1, f))
        freq[b]++;

    fclose(f);
}

void writeHeader(FILE *out, int freq[256], int size)
{

    fwrite("HUF1", 1, 4, out);

    fwrite(&size, sizeof(int), 1, out);

    for (int i = 0; i < 256; i++)
        fwrite(&freq[i], sizeof(int), 1, out);
}

void compressFile(const char *input,
                  const char *output,
                  HuffCode codes[256],
                  int freq[256])
{

    FILE *in = fopen(input, "rb");
    FILE *out = fopen(output, "wb");

    fseek(in, 0, SEEK_END);
    int size = ftell(in);
    rewind(in);

    writeHeader(out, freq, size);

    BitWriter bw;
    init_BitWriter(&bw, out);

    unsigned char c;

    while (fread(&c, 1, 1, in))
    {

        HuffCode hc = codes[c];

        write_code(&bw, hc.code, hc.length);
    }

    flush_BitWriter(&bw);

    fclose(in);
    fclose(out);
}

void countRGBFrequency(Image *img, int freq[256])
{

    int size = img->width * img->height * img->channels;

    for (int i = 0; i < size; i++)
    {
        freq[img->data[i]]++;
    }
}

void writeHeaderRGB(FILE *out, Image *img, int freq[256], int format)
{

    fwrite("HUF2", 1, 4, out);

    fwrite(&format, sizeof(int), 1, out);

    fwrite(&img->width, sizeof(int), 1, out);
    fwrite(&img->height, sizeof(int), 1, out);
    fwrite(&img->channels, sizeof(int), 1, out);

    for (int i = 0; i < 256; i++)
        fwrite(&freq[i], sizeof(int), 1, out);
}

void compressRGB(Image *img,
                 const char *output,
                 HuffCode codes[256],
                 int freq[256],
                 int format)
{

    FILE *out = fopen(output, "wb");

    writeHeaderRGB(out, img, freq, format);

    BitWriter bw;

    init_BitWriter(&bw, out);

    int size = img->width * img->height * img->channels;

    for (int i = 0; i < size; i++)
    {

        unsigned char value = img->data[i];

        HuffCode hc = codes[value];

        write_code(&bw, hc.code, hc.length);
    }

    flush_BitWriter(&bw);

    fclose(out);
}

void freeTree(Node *root)
{

    if (!root)
        return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

void decompressRGB(const char *input, const char *output)
{

    FILE *in = fopen(input, "rb");

    if (!in)
    {
        printf("Cannot open compressed file\n");
        exit(1);
    }

    char magic[5] = {0};
    fread(magic, 1, 4, in);

    if (strcmp(magic, "HUF2") != 0)
    {
        printf("Invalid file format\n");
        exit(1);
    }

    int format;
    fread(&format, sizeof(int), 1, in);

    Image img;

    fread(&img.width, sizeof(int), 1, in);
    fread(&img.height, sizeof(int), 1, in);
    fread(&img.channels, sizeof(int), 1, in);

    int freq[256];

    for (int i = 0; i < 256; i++)
        fread(&freq[i], sizeof(int), 1, in);

    Node *root = buildHuffmanTree(freq);

    int size = img.width * img.height * img.channels;

    img.data = malloc(size);

    BitReader br;
    init_BitReader(&br, in);

    for (int i = 0; i < size; i++)
    {

        Node *node = root;

        while (node->left || node->right)
        {

            int bit = read_bit(&br);

            if (bit == 0)
                node = node->left;
            else
                node = node->right;
        }

        img.data[i] = node->data;
    }

    fclose(in);

    if (format == 1)
        saveJPEG(output, &img);

    else if (format == 2)
        savePNG(output, &img);

    else
        printf("Unknown output format\n");

    freeImage(&img);
}