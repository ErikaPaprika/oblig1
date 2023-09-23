#include "common.h"
#include <stdlib.h>

void read_packets(FILE *input) {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);
    
    while(!feof(input)) {
        
        // Read header
        printf("Read %lu bytes for header (should be 6) - ", fread(data, 1, 6, input));
        printf(" APID [%d]", get_bits(data, 11, 5));
        printf(" Sequence counter [%d]", get_bits(data, 14, 18));
        printf(" Data length [%d]", get_bits(data, 16, 32));
        int data_length = get_bits(data, 16, 32);
        printf(" - Read %lu bytes for data (should be %d)\n", fread(data, 1, data_length + 1, input), data_length + 1);
        
    }
    

}

int main() 
{
    read_packets(stdin);
}
