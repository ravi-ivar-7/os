CC=gcc
CFLAGS=-Wall -Wextra -g

all: paging replacement malloc mmap

paging: 01-paging-tlb/paging_sim.c
	$(CC) $(CFLAGS) -o 01-paging-tlb/paging_sim 01-paging-tlb/paging_sim.c

replacement: 02-page-replacement/replacement_sim.c
	$(CC) $(CFLAGS) -o 02-page-replacement/replacement_sim 02-page-replacement/replacement_sim.c

malloc: 03-custom-malloc/custom_malloc.c
	$(CC) $(CFLAGS) -o 03-custom-malloc/custom_malloc 03-custom-malloc/custom_malloc.c

mmap: 04-mmap-files/mmap_demo.c
	$(CC) $(CFLAGS) -o 04-mmap-files/mmap_demo 04-mmap-files/mmap_demo.c

clean:
	rm -f 01-paging-tlb/paging_sim
	rm -f 02-page-replacement/replacement_sim
	rm -f 03-custom-malloc/custom_malloc
	rm -f 04-mmap-files/mmap_demo
	rm -f test.txt
