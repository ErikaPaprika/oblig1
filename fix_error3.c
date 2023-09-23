#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAX_PACKETS 1000 
#define MAX_PACKET_SIZE 100000 

typedef struct {
    uint8_t data[MAX_PACKET_SIZE];
    int size;
    unsigned int sequence;
} Packet;

void sort_packets(Packet packets[], int n) {
    int i, j;
    for(i = 0; i < n-1; i++) {
        for(j = 0; j < n-i-1; j++) {
            if(packets[j].sequence > packets[j+1].sequence) {
                Packet temp = packets[j];
                packets[j] = packets[j+1];
                packets[j+1] = temp;
            }
        }
    }
}

int main() {
    Packet packets[MAX_PACKETS];
    int packet_count = 0;

    while(!feof(stdin) && packet_count < MAX_PACKETS) {
        int data_read = fread(packets[packet_count].data, 1, 6, stdin);
        if(data_read < 6) break; // Incomplete header, stop reading

        // Read sequence from header
        packets[packet_count].sequence = get_bits(packets[packet_count].data, 14, 18);

        // Read packet length
        int data_length = get_bits(packets[packet_count].data, 16, 32);
        
        // Read the rest of packet
        data_read += fread(packets[packet_count].data + 6, 1, data_length + 1, stdin);
        if(data_read != data_length + 6 + 1) break; // Incomplete packet, stop reading

        packets[packet_count].size = data_read;
        packet_count++;
    }

    // Sort packets
    sort_packets(packets, packet_count);

    // Write sorted packets back
    for(int i = 0; i < packet_count; i++) {
        fwrite(packets[i].data, 1, packets[i].size, stdout);
    }

    return 0;
}
