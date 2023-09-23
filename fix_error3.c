#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
unsigned int sequences[5];
int set_packets = 0;

void fix_error(uint8_t *data[], int n) {
    int i, j;
    for(i = 0; i < n - 1; i++) {
        for(j = 0; j < n - i - 1; j++) {
            unsigned int seq1 = get_bits(data[j], 14, 18);
            unsigned int seq2 = get_bits(data[j + 1], 14, 18);
            if (seq1 > seq2) {
                uint8_t *temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;

                int temp_size = packet_sizes[j];
                packet_sizes[j] = packet_sizes[j + 1];
                packet_sizes[j + 1] = temp_size;
            }
        }
    }
}

int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);

    while(!feof(stdin)) {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 16, 32) + 1;
        data_read += fread(data + 6, 1, data_length, stdin);

        packets[set_packets] = malloc(data_read);
        memcpy(packets[set_packets], data, data_read);
        packet_sizes[set_packets] = data_read;

        set_packets++;

        if(set_packets == 5) {
            fix_error(packets, set_packets);

            for(int i = 0; i < set_packets; i++) {
                int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                if(bytes_written < packet_sizes[i]) {
                    fprintf(stderr, "Data lost! %d bytes not written\n", packet_sizes[i] - bytes_written);
                }

                free(packets[i]);
                packets[i] = NULL;
            }

            set_packets = 0;
        }
    }

    free(data);

    return 0;
}

