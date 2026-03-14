# File Systems

## Structure

```text
01-inodes-dirs/         Inode structure, directory traversal simulation
02-allocation-methods/  Contiguous, FAT (linked), and indexed allocation comparison
03-fs-internals/        ext4 and FAT32 on-disk layout simulation
04-journaling/          Write-ahead logging and crash recovery demonstration
```

## Key Concepts

- **Inode**: The core metadata structure. Files and directories are inodes pointing to data blocks.
- **Directory**: A file whose data is a list of (name → inode number) mappings.
- **Allocation**: How the filesystem decides which disk blocks to assign to a file.
- **Journaling**: A write-ahead log that guarantees filesystem consistency after a crash.

## Build Instructions

```bash
make all    # Build all programs
make clean  # Remove all binaries
```

Or build individually:

```bash
cd 01-inodes-dirs && gcc inode_sim.c -o inode_sim
cd 02-allocation-methods && gcc allocation_sim.c -o allocation_sim
cd 03-fs-internals && gcc fs_internals.c -o fs_internals
cd 04-journaling && gcc journaling_sim.c -o journaling_sim
```

## Observe Real Filesystems

```bash
# Inode info for a file
stat /etc/passwd
ls -i /etc/passwd

# ext4 superblock
sudo dumpe2fs /dev/sda1 | head -30

# Disk usage per inode
df -i

# Trace filesystem calls
strace -e trace=openat,read,write,stat ls
```
