# Custom Malloc Implementation

Dynamic memory allocation is typically handled by the standard C library (`libc`) using functions like `malloc()`, `calloc()`, `realloc()`, and `free()`. But how do these functions actually manage memory dynamically?

This directory contains a simplified version of a dynamic memory allocator to demonstrate the underlying mechanics.

## How It Works

1. **System Call (`sbrk` or `mmap`)**:
   - The OS only gives large chunks of raw memory to the process (usually extending the data segment or "heap").
   - `malloc` is basically a manager slicing up this large chunk into smaller requested sizes for the user program.

2. **The Free List**:
   - The allocator keeps track of available memory using a linked list of free blocks.
   - Every returned block of memory has a hidden "header" right before the returned address holding metadata (size of the block, whether it's free, and a pointer to the next block).

3. **Allocation (malloc)**:
   - When asked for bytes, the allocator scans the free list (e.g., using a First-Fit approach) to find a block large enough.
   - If one is found, it is marked as used. Sometimes large blocks are split.
   - If none is found, it asks the OS for more memory.

4. **Deallocation (free)**:
   - Takes the pointer, figures out where the metadata header is.
   - Marks the block as free and adds it back to the free list.
   - Ideally, adjacent free blocks are coalesced (merged) to prevent fragmentation.

## Run Instructions

```bash
gcc custom_malloc.c -o custom_malloc
./custom_malloc
```

The output will demonstrate allocating several chunks of memory, verifying their addresses, freeing them, and showing how the allocator reuses previously freed blocks for new requests instead of asking the OS for more memory.
