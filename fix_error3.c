#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
int set_packets = 0;

int compare(const void *a, const void *b) {
    uint8_t *packet_a = *(uint8_t **)a;
    uint8_t *packet_b = *(uint8_t **)b;
    int sequence_a = get_bits(packet_a, 14, 18);
    int sequence_b = get_bits(packet_b, 14, 18);
    return sequence_a - sequence_b;
}

int main() {
    while(!feof(stdin)) {
        for (set_packets = 0; set_packets < 5; ++set_packets) {
            uint8_t header[6];
            int header_read = fread(header, 1, 6, stdin);
            if (header_read == 0) {
                fprintf(stderr, "Incomplete header read.\n");
                return 1;
            }
            
            int data_length = get_bits(header, 16, 32) + 1;
            packets[set_packets] = malloc(6 + data_length);
            memcpy(packets[set_packets], header, 6);
            
            int data_read = fread(packets[set_packets] + 6, 1, data_length, stdin);
            if(data_read != data_length) {
                fprintf(stderr, "Incomplete data read.\n");
                return 1;
            }
            
            packet_sizes[set_packets] = 6 + data_length;
        }

        // Sort the packets based on their sequence number
        qsort(packets, 5, sizeof(uint8_t *), compare);

        // Write the sorted packets to stdout
        for (int i = 0; i < 5; ++i) {
            int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
            if (bytes_written != packet_sizes[i]) {
                fprintf(stderr, "Data lost! %d bytes not written\n", packet_sizes[i] - bytes_written);
            }
            fflush(stdout);
            free(packets[i]);
        }
    }
    return 0;
}
