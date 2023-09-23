#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"

void fix_error(uint8_t *packet) {
    // Introduce_error bytter bits randomly?
    int data_length = get_bits(packet, 16, 32) + 1;
    // Backflip lol. Neida, men prøver flip tilbake

    for (int i = 0; i < data_length; i++) {
        packet[i] = packet[i]; // Ingen endring? backflip
    }
}

int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);

    while (!feof(stdin)) {

        printf("Read %lu headerbytes (Skal væer 6) - ", fread(data, 1, 6, stdin));
        printf(" APID/header dest [%d]", get_bits(data, 11, 5));
        printf(" Sequence counter [%d]", get_bits(data, 14, 18));
        printf(" Data length [%d]", get_bits(data, 16, 32));
        int data_length = get_bits(data, 16, 32);
        printf(" - Read %lu databytes (Skal være %d)\n", fread(data, 1, data_length + 1, stdin), data_length + 1);

        // Fix error
        fix_error(data);

        // Output 
        fwrite(data, 1, data_length + 7, stdout);
        fflush(stdout);
    }

    free(data);
    return 0;
}
