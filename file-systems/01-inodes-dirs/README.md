# Inodes and Directories

## What is an Inode?

An inode (Index Node) is the fundamental data structure in Unix-based file systems. Every file and directory is represented by exactly one inode. The file or directory **name is not stored in the inode**; names live in directory entries that simply point to inode numbers.

An inode stores:
- File type (regular file, directory, symlink, etc.)
- Permissions (rwx bits for owner, group, others)
- Owner (UID, GID)
- Timestamps (created, modified, accessed)
- File size
- Hard link count
- Pointers to data blocks

## Block Pointers

Data is stored in fixed-size disk blocks. The inode contains pointers to these blocks:

```
Inode
├── 12 Direct Block Pointers   -> point directly to data blocks
├── 1 Single Indirect Pointer  -> points to a block of block numbers
├── 1 Double Indirect Pointer  -> points to a block of indirect pointers
└── 1 Triple Indirect Pointer  -> two levels of indirection
```

A file larger than `12 * BLOCK_SIZE` needs indirect block pointers.

## How Directories Work

A directory is just a file whose data is a list of **(name, inode_number)** pairs:

```
Directory data:
  .  -> inode 5    (the directory itself)
  .. -> inode 1    (parent directory)
  file1.txt -> inode 12
  subdir    -> inode 20
```

When resolving the path `/home/ravi/file.txt`:
1. Start at inode 1 (root `/`)
2. Look up `home` in root's directory data -> inode N
3. Look up `ravi` in inode N's data -> inode M
4. Look up `file.txt` in inode M's data -> inode X
5. Read the actual file data from inode X's block pointers

## Run Instructions

```bash
gcc inode_sim.c -o inode_sim
./inode_sim
```
