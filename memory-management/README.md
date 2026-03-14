# Memory Management in Operating Systems

This module explores how operating systems manage the physical RAM and provide the illusion of vast, isolated memory spaces to individual processes via Virtual Memory.

## Structure

```text
01-paging-tlb/          Address translation and TLB caching simulation
02-page-replacement/    Demand paging: FIFO, LRU, Clock algorithm comparison
03-custom-malloc/       User-space dynamic memory allocation on top of the OS heap
04-mmap-files/          Memory-mapping mechanics and zero-copy file access
```

## Key Concepts

- **Virtual Memory**: Separating the logical address space from physical RAM layout.
- **Paging**: Dividing memory into fixed-size blocks (pages in virtual memory, frames in physical RAM).
- **Page Faults**: A trap that occurs when a process accesses a page not currently mapped in physical RAM.
- **TLB**: A hardware cache inside the MMU that significantly speeds up page table translations.
- **Allocators**: User-space libraries (like `libc`) that manage the coarse chunks of memory handed out by the OS via `sys_brk` or `sys_mmap`.

## Build Instructions

Each subdirectory contains its own source files. To compile an individual program, navigate to its directory and use `gcc`, for example:

```bash
cd 01-paging-tlb
gcc paging_sim.c -o paging_sim
./paging_sim
```

## Prerequisites

- Understanding of C pointers and basic data structures.
- Familiarity with the Linux command line.
