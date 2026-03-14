# Operating Systems Notes & Practice

A workspace for learning OS concepts through code examples and experimentation.

## Structure

```text
OS/
├── process-control/
├── memory-management/
└── file-systems/
```

## Quick Start

```bash
# Start with process control
cd process-control
make all
./01-process-basics/fork_basics
```

Read [process-control/README.md](process-control/README.md) for more details.

## Topics

### Process Control
- Process lifecycle, states, fork/exec
- Context switching mechanics
- Schedulers (FCFS, SJF, RR, Priority, Linux CFS)
- IPC (pipes, shared memory, signals)
- Synchronization (mutex, semaphores, deadlock)

[Documentation →](process-control/README.md)


### Memory Management
[Documentation →](memory-management/README.md)
- Virtual memory, paging, TLB
- Page replacement (FIFO, LRU, Clock)
- malloc/free implementation
- Memory-mapped files

### File Systems
- Inode structure, directories
- Allocation methods
- ext4, FAT internals
- Journaling, crash recovery

[Documentation →](file-systems/README.md)

## Prerequisites

- C programming (pointers, structs)
- Linux command line
- Basic data structures

## Tools

```bash
sudo apt-get install build-essential gdb valgrind linux-tools-generic
```

---
