#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

uint8_t *packet_buffers[5];
int packet_sizes[5];

int majority_vote(int bit_counts[]) {
    return bit_counts[1] > bit_counts[0] ? 1 : 0;
}

void fix_packet(int packet_size) {
    uint8_t *fixed_packet = malloc(packet_size);
    memset(fixed_packet, 0, packet_size);

    for (int byte_index = 0; byte_index < packet_size; ++byte_index) {
        int bit_counts[2] = {0, 0};

        for (int bit_index = 0; bit_index < 8; ++bit_index) {
            for (int i = 0; i < 5; ++i) {
                int bit = get_bits(packet_buffers[i] + byte_index, 1, bit_index);
                bit_counts[bit]++;
            }
            int majority_bit = majority_vote(bit_counts);
            fixed_packet[byte_index] |= (majority_bit << bit_index);
        }
    }

    int bytes_written = write(fileno(stdout), fixed_packet, packet_size);
    if (bytes_written < packet_size) {
        fprintf(stderr, "Data lost! %d bytes not written\n", packet_size - bytes_written);
    }
    fflush(stdout);

    free(fixed_packet);
}

int main() {
    int set_packets = 0;
    uint8_t header_buffer[6];

    while (!feof(stdin)) {
        int header_read = fread(header_buffer, 1, 6, stdin);
        if (header_read < 6) {
            fprintf(stderr, "Incomplete header read\n");
            continue;
        }
        
        int data_length = get_bits(header_buffer, 16, 32) + 1;
        int packet_size = 6 + data_length;
        uint8_t *data_buffer = malloc(packet_size);

        memcpy(data_buffer, header_buffer, 6);

        int data_read = fread(data_buffer + 6, 1, data_length, stdin);
        if (data_read < data_length) {
            fprintf(stderr, "Incomplete data read\n");
            free(data_buffer);
            continue;
        }

        packet_buffers[set_packets] = data_buffer;
        packet_sizes[set_packets] = packet_size;

        set_packets++;
        if (set_packets == 5) {
            fix_packet(packet_size);

            for (int i = 0; i < 5; ++i) {
                free(packet_buffers[i]);
                packet_buffers[i] = NULL;
            }

            set_packets = 0;
        }
    }

    return 0;
}
