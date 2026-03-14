# Journaling and Crash Recovery

## The Problem: Crash Consistency

A simple file write involves multiple disk operations (update inode, update bitmap, write data blocks). If the system crashes between any of these operations, the filesystem can be left in an **inconsistent state**:

- Data written but inode not updated -> orphaned blocks (space leak).
- Inode updated but data not written -> file points to garbage data.
- Inode updated, data written, but bitmap not updated -> block could get overwritten.

Older file systems (`ext2`, FAT) ran `fsck` (filesystem check) on every unclean boot which scanned the entire disk and could take many minutes on large volumes.

## The Solution: Journaling

A journal (also called a write-ahead log) records intended changes *before* they are applied to the main filesystem. If a crash occurs, the recovery process simply replays or discards the uncommitted journal entries.

### Steps for a Journaled Write

```
1. Write transaction start marker to the journal
2. Write all metadata and data blocks to the journal
3. Write transaction commit marker to the journal
4. Apply (checkpoint) the committed changes to the real filesystem
5. Remove the journal entry
```

This is the "write twice" cost of journaling — slightly lower performance, but instant crash recovery.

## Journaling Modes (ext3/ext4)

| Mode         | What is Journaled        | Performance | Safety |
|--------------|--------------------------|-------------|--------|
| `writeback`  | Metadata only            | Fastest     | Data may be corrupt on crash |
| `ordered`    | Metadata (data first)    | Default     | Data safe, metadata might be stale |
| `journal`    | Metadata + Data          | Slowest     | Fully safe |

Default in ext4 is `ordered`.

## Viewing the Journal

```bash
# Check journal size
sudo dumpe2fs /dev/sda1 | grep -i journal

# See journal inode
# Journal is stored as inode 8 on ext4
sudo debugfs /dev/sda1
debugfs> stat <8>
```

## Run Instructions

The simulation here demonstrates the concept of write-ahead logging. It logs operations to a journal file before applying them, and shows recovery after a simulated crash.

```bash
gcc journaling_sim.c -o journaling_sim
./journaling_sim
```
