# Memory-Mapped Files

The `mmap` (memory map) system call maps files or devices into memory. It is a method of utilizing virtual memory to lazily load portions of a file into physical memory, bypassing traditional `read` and `write` calls.

## How It Works

Normally, reading a file requires:
1. The kernel reading the file from the disk into a buffer in kernel space.
2. The kernel copying the data from the buffer into the user-space process.

With `mmap`:
1. The OS sets up virtual memory pages that point directly to blocks of the file on the disk.
2. The file is accessed as if it were an array in RAM.
3. When a page is touched that isn't in physical RAM yet, a page fault occurs, and the OS loads it directly from the disk into the user-space mapping.
4. Changes written to the memory array are automatically synced back to the disk.

## Why Use `mmap`?

- **Zero-copy operations**: Data doesn't need to be copied between kernel and user space.
- **Inter-Process Communication (IPC)**: Multiple processes mapping the same file can see each other's changes instantly in memory.
- **Lazy loading**: Accessing a huge file doesn't require loading it fully into RAM.

## Run Instructions

```bash
# Create a dummy file to map
echo "MAPPING_TEST: THIS IS THE ORIGINAL FILE CONTENT." > test.txt

gcc mmap_demo.c -o mmap_demo
./mmap_demo

cat test.txt
```

Observe the program reading the string, making changes via pointer arithmetic (as if it was just memory), and the actual text file on disk (`test.txt`) being modified as a result.
