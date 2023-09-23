#include "common.h"


uint32_t get_bits(uint8_t *c, int num_bits, int start_bit) {
    uint8_t mask = 0x80;
    uint8_t *point = c;
    uint32_t result = 0;
    for(int i = 0; i < start_bit / 8; i++) {
        point++;
    }
    for(int i = 0; i < start_bit % 8; i++) {
        mask = mask >> 1;
    }

    int remaining_bits = num_bits;
    while(remaining_bits) {
        result = result << 1;
        if(mask & *point) {
            result += 1;
        }
        mask = mask >> 1;
        if(!mask) {
            point += 1;
            mask = 0x80;
        }
        remaining_bits -= 1;
    }
    return result;
}

void print_bits(uint8_t *c, int num_bits, int start_bit) {
    uint8_t mask = 0x80;
    uint8_t *point = c;
    for(int i = 0; i < start_bit / 8; i++) {
        point++;
    }
    for(int i = 0; i < start_bit % 8; i++) {
        mask = mask >> 1;
    }

    int remaining_bits = num_bits;
    while(remaining_bits) {
        if(mask & *point) {
            fprintf(stderr, "1");
        } else {
            fprintf(stderr, "0");
        }
        mask = mask >> 1;
        if(!mask) {
            point += 1;
            mask = 0x80;
        }
        remaining_bits -= 1;
    }
}

