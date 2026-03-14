# Page Replacement Algorithms

When physical memory (RAM) is full and a new page needs to be loaded from disk (a page fault occurs), the operating system must choose an existing page to evict. 

This directory contains a simulator to compare fundamental page replacement algorithms based on the number of page faults they generate for a given sequence of memory references.

## Algorithms Implemented

1. **FIFO (First-In, First-Out)**:
   - Evicts the oldest page in memory.
   - Simple to implement (just a queue).
   - Suffers from Belady's Anomaly (giving it more memory can actually *increase* page faults).

2. **LRU (Least Recently Used)**:
   - Evicts the page that has not been accessed for the longest time.
   - An excellent approximation of the exact optimal algorithm.
   - Requires hardware support or significant overhead to track usage timestamps.

3. **Clock (Second Chance Algorithm)**:
   - A realistic approximation of LRU used in actual OSes (like Linux and Windows).
   - Keeps pages in a circular list with a "use bit".
   - If the bit is 1, it's given a second chance, the bit is cleared to 0, and the pointer moves on. If it's 0, the page is evicted.

## Run Instructions

```bash
gcc replacement_sim.c -o replacement_sim
./replacement_sim
```

The simulator prints the sequence of page accesses and tallies the total number of page faults for each algorithm. Compare the performance to see why LRU and Clock are preferred in production systems over basic FIFO.
