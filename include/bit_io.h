#ifndef BIT_IO_H
#define BIT_IO_H

# include <stdio.h>

typedef struct {

    FILE* file;
    unsigned char buffer;
    int bit_count;

} BitReader;


typedef struct {
    
    FILE* file;
    unsigned char buffer;
    int bit_count;
    
} BitWriter;

void init_BitReader(BitReader* br, FILE* f);
int read_bit(BitReader* br);

void init_BitWriter(BitWriter* bw, FILE* f);
void write_bit(BitWriter* bw, int bit);

void write_code(BitWriter* bw, unsigned int code, int length);

void flush_BitWriter(BitWriter* bw);

# endif // BIT_IO_H