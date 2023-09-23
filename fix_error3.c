#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
int set_packets = 0;

void fix_packet(uint8_t *packet, int size, uint8_t *output) {
    // Init all bytes to zero
    memset(output, 0, size);

    //Majority vote for each bit in packet
    for (int i = 0; i < size * 8; ++i) {
        int bit_count = 0;
        for (int j = 0; j < 5; ++j) {
            uint8_t bit = get_bits(packets[j], 1, i);
            bit_count += bit;
        }

        // Set or clear bit in output based on vote
        if (bit_count >= 3) {
            // Majority had bit set
            output[i / 8] |= (1 << (i % 8));
        }
    }
}

int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);
    for (int i = 0; i < 5; i++) {
        packets[i] = NULL;
    }

    while (!feof(stdin)) {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 16, 32) + 1;
        data_read += fread(data + 6, 1, data_length, stdin);

        int rand = 0;
        for (rand = random() % 5; packets[rand] != NULL; rand = random() % 5);
        packets[rand] = malloc(data_read);
        memcpy(packets[rand], data, data_read);
        packet_sizes[rand] = data_read;

        set_packets += 1;
        if (set_packets == 5) {
            uint8_t *fixed_packet = malloc(packet_sizes[0]);
            fix_packet(packets[0], packet_sizes[0], fixed_packet);

            int bytes_written = write(fileno(stdout), fixed_packet, packet_sizes[0]);
            if (bytes_written < packet_sizes[0]) {
                fprintf(stderr, "Data lost! %d bytes not written\n", packet_sizes[0] - bytes_written);
            }
            fflush(stdout);

            // Free allocated memory
            for (int i = 0; i < 5; i++) {
                free(packets[i]);
                packets[i] = NULL;
            }
            free(fixed_packet);

            set_packets = 0;
        }
    }
    free(data);
    return 0;
}
