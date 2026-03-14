# Disk Allocation Methods

How a file system actually lays out a file's data on disk matters enormously for performance and reliability. Three classic strategies exist:

## 1. Contiguous Allocation

All blocks of a file are stored one after another on disk.

- **Pros**: Extremely fast sequential reads (single seek). Simple to implement.
- **Cons**: External fragmentation. Difficult to grow a file without moving it. Must know file size upfront.
- **Used by**: CD-ROM file systems, some older OS designs.

```
Block: [10][11][12][13][14]  <- file A
Block: [20][21]              <- file B (gap between - fragmentation)
```

## 2. Linked Allocation

Each block contains a pointer to the next block, forming a chain.

- **Pros**: No external fragmentation. Files can grow easily.
- **Cons**: Sequential access is slow — must follow the chain. Random access requires traversing from the start. Pointer corruption can corrupt the entire file. Pointers waste space.
- **Used by**: FAT file system (a variant - uses a separate table for the pointers).

```
Block 5 -> Block 12 -> Block 3 -> Block 18 -> NULL
```

## 3. Indexed Allocation (Used by Unix/ext4)

Each file has an index block (or inode) that holds all the pointers to its data blocks.

- **Pros**: Fast direct access to any block. No external fragmentation.
- **Cons**: Index block itself takes up space. For small files, the overhead is disproportionate.
- **Used by**: ext2/3/4, APFS, most modern Unix file systems.

## FAT (File Allocation Table) - A Variant

FAT is a linked-allocation scheme but stores the "next block" pointers in a separate table (the FAT) at the start of the volume rather than inside the data blocks. This makes it faster than naive linked allocation.

```
FAT Table:
Index: [0][1][2][3][4][5][6][7][8]
Value: [-][5][8][7][E][3][E][E][2]

File A starts at block 1: 1->5->3->7 = EOF (E)
File B starts at block 2: 2->8->2... 
```

## Run Instructions

```bash
gcc allocation_sim.c -o allocation_sim
./allocation_sim
```
