CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

all: io_sim disk_sim buffer_sim

io_sim: 01-interrupts-dma/io_sim.c
	$(CC) $(CFLAGS) -o 01-interrupts-dma/io_sim 01-interrupts-dma/io_sim.c

disk_sim: 02-disk-scheduling/disk_sim.c
	$(CC) $(CFLAGS) -o 02-disk-scheduling/disk_sim 02-disk-scheduling/disk_sim.c

buffer_sim: 03-drivers-buffering/buffer_sim.c
	$(CC) $(CFLAGS) -o 03-drivers-buffering/buffer_sim 03-drivers-buffering/buffer_sim.c

clean:
	rm -f 01-interrupts-dma/io_sim 02-disk-scheduling/disk_sim 03-drivers-buffering/buffer_sim

help:
	@echo "Available commands:"
	@echo "  make all        - Build all simulations"
	@echo "  make io_sim     - Build Interrupts vs DMA simulation"
	@echo "  make disk_sim   - Build Disk Scheduling simulation"
	@echo "  make buffer_sim - Build Buffering simulation"
	@echo "  make clean      - Remove compiled binaries"
