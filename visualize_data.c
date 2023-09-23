#include "visualize_data.h"

uint8_t *decompress_data(uint8_t *data, int j, int n, int mode, int blkref) {
    // Mode:
    // 0 = nearest neighbour
    // blkref = Blocks per reference
    // j = samples per block
    // n = bits per sample

    
}

int main() {
    // 488 pixels per scanline
    int err = SDL_Init(SDL_INIT_VIDEO);
    if(err != 0) {
        printf("Unable to initialize SDL (%s)\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *win = SDL_CreateWindow("Visualize JPSS VIIRS data", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 488, 500, 0);
    if(win == NULL) {
        printf("Unable to create window (%s)\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface *winsurf = SDL_GetWindowSurface(win);
    if(winsurf == NULL) {
        printf("Unable to get window surface (%s)\n", SDL_GetError());
        exit(1);
    }

    int quit = 0;

    SDL_Event event;
    uint8_t *packet_data = malloc(sizeof(uint8_t) * 100000);

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                exit(0);
            }
        }

        if(!feof(stdin)) {
            int read_bytes = 0;
            read_bytes = fread(packet_data, 1, 6, stdin);
            if(read_bytes != 6) {
                printf("Unable to read data\n");
                exit(1);
            }
            int packet_length = get_bits(packet_data, 16, 32);
            read_bytes = fread(packet_data, 1, packet_length + 1, stdin);

            uint8_t *aggr = packet_data + ((32+64+8+8+16+1+4+11+8+8+32+512+16) / 8);
            int aggr_length = get_bits(aggr, 16, 0) - 4;
            
            
        }

        SDL_UpdateWindowSurface(win);
    }
}
