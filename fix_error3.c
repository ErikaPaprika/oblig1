#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
int set_packets = 0;

// Function do a bit flip wosah awsome 
void flip_bit(uint8_t *byte, int bit_position) {
    *byte ^= (1 << bit_position);
}

int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);

    for (int i = 0; i < 5; i++) {
        packets[i] = NULL;
    }

    while (!feof(stdin)) {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 14, 18);
        data_read += fread(data + 6, 1, data_length, stdin);
        set_packets += 1;

        // Simulate error correction by flipping the same bits back
        for (int i = 0; i < data_length; i++) {
            for (int bit_position = 0; bit_position < 8; bit_position++) {
                if (random() % 2) { // Randomly decide whether to flip the bit
                    flip_bit(data + 6 + i, bit_position);
                }
            }
        }

        if (set_packets == 5) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 4; j++) {
                    if (packet_sizes[j] > packet_sizes[j + 1]) {
                        // Swap packets and packet sizes
                        uint8_t *temp_packet = packets[j];
                        int temp_size = packet_sizes[j];

                        packets[j] = packets[j + 1];
                        packet_sizes[j] = packet_sizes[j + 1];

                        packets[j + 1] = temp_packet;
                        packet_sizes[j + 1] = temp_size;
                    }
                }

                int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                fflush(stdout);
                packets[i] = NULL;
            }
            set_packets = 0;
        }
    }

    free(data);
    return 0;
}
