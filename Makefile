CC=gcc
CFLAGS=-g -O3

all: read_cadu read_packet introduce_error fix_error2

read_cadu: read_cadu.c common.h common.c
	$(CC) $(CFLAGS) read_cadu.c common.c -o read_cadu

read_packet: read_packet.c common.h common.c
	$(CC) $(CFLAGS) read_packet.c common.c -o read_packet

introduce_error: introduce_error.c common.h common.c
	$(CC) $(CFLAGS) introduce_error.c common.c -o introduce_error

fix_error: fix_error2.c common.h common.c
	$(CC) $(CFLAGS) fix_error2.c common.c -o fix_error2


clean:
	rm -f read_cadu read_packet introduce_error fix_error2
