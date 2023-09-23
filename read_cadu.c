#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common.h"
#include "read_cadu.h"

#define CADU_SIZE 1024
#define CADU_DATA_SIZE 884
// VIIRS operational image data
#define APID 808

const char* CADUFILE = "data/cadu_1";

void print_vcdu(uint8_t *c) {
    uint8_t *data = c + (96/8);
    unsigned int fhp = get_bits(c, 11, 85);
    while(fhp != 0x7ff) {
        uint8_t *packet = data + fhp;
        // Get packet length, including header (7 bytes)
        unsigned int packet_length = get_bits(packet, 16, 32) + 7;

        if (get_bits(packet, 11, 5) == APID && packet_length < 2000) {
            int bytes_written = write(fileno(stdout), packet, packet_length);
            if(bytes_written < packet_length) {
                fprintf(stderr, "Data lost! %d bytes not written\n", packet_length - bytes_written);
            }
            fflush(stdout);
        }

        if(packet_length + fhp < CADU_DATA_SIZE) {
            // We have more packets within the CADU
            fhp += packet_length;
        } else {
            // We are done with the CADU
            break;
        }
        if(fhp >= CADU_DATA_SIZE) {
            break;
        }
    }
}

void prn(uint8_t *c, int offset) {
    uint8_t *point = c;
    point += offset;
    if(offset >= 4) {
        uint8_t rand_target = pr[(offset-4) % 255];
        *point ^= rand_target;
    }
}

void readFrame(const char* fileName) {
    FILE *f = fopen(fileName, "r");

    if(f == NULL) {
        fprintf(stderr, "Couldn't open file!\n");
        exit(1);
    }

    while(!feof(f)) {
        uint8_t *current = malloc(CADU_SIZE);
        int read_items = fread(current, CADU_SIZE, 1, f);
        if(read_items == 0) {
            fprintf(stderr, "Incomplete CADU read\n");
        }
        for(int i = 0; i < CADU_SIZE; i++) {
            prn(current, i);
        }
        print_vcdu(current);
        free(current);
    }

}


int main() {
    readFrame(CADUFILE);
    return 0;
}
