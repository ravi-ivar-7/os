# Operating Systems Notes & Practice

A workspace for learning OS concepts through code examples and experimentation.

## Structure

```text
OS/
├── process-control/     [Completed] Process management, scheduling, synchronization
├── memory-management/   [Partially Completed] Paging, virtual memory, allocators
├── file-systems/        [Partially Completed] Inodes, allocation, journaling
├── io-systems/          [Partially Completed] Device drivers, interrupts, DMA
└── ...more topics
```

## Quick Start

```bash
# Start with process control
cd process-control
make all
./01-process-basics/fork_basics
```

Read [process-control/README.md](process-control/README.md) for the full guide.

## Topics

### 1. Process Control [Completed]
- Process lifecycle, states, fork/exec
- Context switching mechanics
- Schedulers (FCFS, SJF, RR, Priority, Linux CFS)
- IPC (pipes, shared memory, signals)
- Synchronization (mutex, semaphores, deadlock)

**Time**: 20-30 hours
**[Start here →](process-control/README.md)**

### 2. Memory Management [Partially Completed]
- Virtual memory, paging, TLB
- Page replacement (FIFO, LRU, Clock)
- malloc/free implementation
- Memory-mapped files

### 3. File Systems [Partially Completed]
- Inode structure, directories
- Allocation methods
- ext4, FAT internals
- Journaling, crash recovery

### 4. I/O Systems [Partially Completed]
- Interrupts, DMA, polling
- Device drivers
- Disk scheduling
- Block devices

## Prerequisites

- C programming (pointers, structs)
- Linux command line
- Basic data structures

## Tools

```bash
sudo apt-get install build-essential gdb valgrind linux-tools-generic
```

## Learning Method

1. Read code
2. Compile: `make`
3. Run and observe
4. Modify and experiment
5. Use system tools: `ps`, `top`, `strace`, `/proc`

---

**Begin**: [process-control/README.md](process-control/README.md)
