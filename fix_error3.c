#include "common.h"
#include "common.c"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
int set_packets = 0;

// void fix_error() {
int main(FILE *input)
{
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);
    
    while(!feof(input)) {
        get_bits(data, 14, 18);
    for (int i = 0; i < 5; i++) 
    {
        packets[i] = NULL;
    }

    }
    while (!feof(input))// finne ut hvordan du kan lese data
    {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 14, 18);
        data_read += fread(data + 6, 1, data_length, stdin);
        set_packets += 1;


        if (set_packets == 5)
        {
            for (int i = 0; i < 5; i++) { 
            for (int j = 0; j < 5; j++) {
                if (packet_sizes[j] > packets[j + 1]) {
                    // Bytt plass på pakkene og størrelsesinformasjonen
                    uint8_t *packets= packets[j];
                    int tmp = packets[j];
                    packets[j] = packets[j + 1];
                    packets[j + 1] = tmp;

                    int packets = packets[j];
                    packets[j] = packets[j + 1];
                    packets[j + 1] = packets;

                    int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                    fflush(stdout);
                    packets[i] = NULL;
                
                set_packets = 0;
                }
            }
        }
            
                // sorteringsfunksjon
            
            
                //  bubblesort
        }
    }
}

// int main()
// {
//     fix_error(stdin);
// }