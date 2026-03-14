# I/O Systems

## Structure

```text
01-interrupts-dma/      Polling, Interrupts, and Direct Memory Access (DMA)
02-disk-scheduling/     Algorithms for minimizing disk head seeks (FCFS, SSTF, SCAN)
03-drivers-buffering/   Device drivers and the impact of single vs double buffering
```

## Key Concepts

- **Polling**: CPU loops constantly querying if a device is ready (wastes CPU cycles).
- **Interrupts**: Hardware signals CPU asynchronously when data is ready. CPU does other work in the meantime.
- **DMA**: A dedicated hardware controller transfers massive data blocks directly between the device and RAM. CPU is only interrupted once per block.
- **Disk Scheduling**: Optimizing the physical mechanical movement of the hard drive arm.
- **Buffering**: Storing data temporarily in RAM to match the speed difference between fast CPU and slow devices.

## Build Instructions

```bash
make all    # Build all programs
make clean  # Remove all binaries
```

Or build individually:

```bash
cd 01-interrupts-dma && gcc io_sim.c -o io_sim -pthread
cd 02-disk-scheduling && gcc disk_sim.c -o disk_sim
cd 03-drivers-buffering && gcc buffer_sim.c -o buffer_sim -pthread
```

## Observing Real I/O

```bash
# View system interrupts mapped to hardware
cat /proc/interrupts

# View currently used DMA channels
cat /proc/dma

# View block devices (disks)
lsblk

# Monitor real-time I/O activity per process
sudo iotop
```
