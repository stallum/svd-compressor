# include <stdlib.h>
# include "bit_io.h"

void init_BitWriter(BitWriter* bw, FILE* f) {
    bw->file = f;
    bw->buffer = 0;
    bw->bit_count = 0;
}

void write_bit(BitWriter* bw, int bit) {
    bw->buffer <<= 1;

    bw->buffer |= bit;
    bw->bit_count++;

    if (bw->bit_count == 8) {
        fwrite(&bw->buffer,1,1,bw->file);

        bw->buffer=0;
        bw->bit_count=0;
    }
}

void write_code(BitWriter* bw, unsigned int code, int length) {

    for (int i = length - 1; i >= 0; i--) {
        write_bit(bw, (code >> i) & 1);
    }
}

void flush_BitWriter(BitWriter* bw) {
    while (bw->bit_count > 0) {
        write_bit(bw, 0);
    }
}

void init_BitReader(BitReader* br, FILE* f) {
    br->file = f;
    br->buffer = 0;
    br->bit_count = 0;
}

int read_bit(BitReader* br) {
    if (br->bit_count == 0) {
        if (fread(&br->buffer, 1, 1, br->file) != 1) return -1;
        br->bit_count = 8;
    }

    int bit = (br->buffer >> 7) & 1;

    br->buffer <<= 1;
    br->bit_count--;

    return bit;

}