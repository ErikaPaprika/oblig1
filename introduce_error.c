#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
int set_packets = 0;

int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);
    for(int i = 0; i < 5; i++) {
        packets[i] = NULL;
    }
    while(!feof(stdin)) {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 16, 32) + 1;
        data_read += fread(data + 6, 1, data_length, stdin);

        int rand = 0;
        for(rand = random() % 5; packets[rand] != NULL; rand = random() % 5);
        packets[rand] = malloc(data_read);
        memcpy(packets[rand], data, data_read);
        packet_sizes[rand] = data_read;
        set_packets += 1;
        if(set_packets == 5) {
            for(int i = 0; i < 5; i++) {
                int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                fflush(stdout);
                packets[i] = NULL;
            }
            set_packets = 0;
        }        
    }
}
