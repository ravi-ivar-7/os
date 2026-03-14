# File System Internals: ext4 and FAT

## ext4 Disk Layout

An ext4 volume is divided into block groups. Each block group has the same structure:

```
| Superblock | Group Descriptors | Block Bitmap | Inode Bitmap | Inode Table | Data Blocks |
```

- **Superblock**: Metadata about the entire filesystem (size, block count, inode count, magic number `0xEF53`, mount time, etc.)
- **Block Bitmap**: One bit per data block. 1 = allocated, 0 = free.
- **Inode Bitmap**: One bit per inode slot. 1 = in use, 0 = free.
- **Inode Table**: The actual array of inode structures.
- **Data Blocks**: The actual file content.

### Inspecting a Real ext4 Filesystem

```bash
# Print superblock info
sudo dumpe2fs /dev/sda1 | head -50

# Print inode info for a file
stat /etc/passwd
ls -i /etc/passwd

# Read raw inode data
sudo debugfs /dev/sda1
debugfs> stat /etc/passwd
debugfs> blocks /etc/passwd
```

## FAT32 Disk Layout

FAT32 is simpler:

```
| Boot Sector | Reserved Sectors | FAT 1 | FAT 2 (backup) | Data Region |
```

- **Boot Sector**: BPB (BIOS Parameter Block) - records cluster size, FAT count, etc.
- **FAT**: The file allocation table. Two copies for redundancy.
- **Data Region**: Clusters (groups of sectors) where actual data lives.

Key difference from ext4: FAT has no concept of inodes or permissions. Directory entries store the file name, size, and starting cluster directly.

### Cluster Size Trade-off

Larger clusters mean:
- Less FAT table overhead.
- More internal fragmentation (a 1-byte file still wastes an entire cluster).

Smaller clusters mean:
- Less wasted space.
- Larger FAT table.

## Run Instructions

```bash
gcc fs_internals.c -o fs_internals
./fs_internals
```
