CC=gcc
CFLAGS=-Wall -Wextra -g

all: inodes allocation internals journaling

inodes: 01-inodes-dirs/inode_sim.c
	$(CC) $(CFLAGS) -o 01-inodes-dirs/inode_sim 01-inodes-dirs/inode_sim.c

allocation: 02-allocation-methods/allocation_sim.c
	$(CC) $(CFLAGS) -o 02-allocation-methods/allocation_sim 02-allocation-methods/allocation_sim.c

internals: 03-fs-internals/fs_internals.c
	$(CC) $(CFLAGS) -o 03-fs-internals/fs_internals 03-fs-internals/fs_internals.c

journaling: 04-journaling/journaling_sim.c
	$(CC) $(CFLAGS) -o 04-journaling/journaling_sim 04-journaling/journaling_sim.c

clean:
	rm -f 01-inodes-dirs/inode_sim
	rm -f 02-allocation-methods/allocation_sim
	rm -f 03-fs-internals/fs_internals
	rm -f 04-journaling/journaling_sim
	rm -f 04-journaling/journal.log
	rm -f 04-journaling/filesystem.dat
