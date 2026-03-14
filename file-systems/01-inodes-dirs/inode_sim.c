#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INODES 64
#define MAX_FILENAME 64
#define MAX_DIRECT_BLOCKS 12
#define BLOCK_SIZE 512

// Inode structure (simplified ext4-style)
typedef struct {
    int inode_num;
    int is_used;
    int is_dir;
    int size;
    int link_count;
    int uid;
    int gid;
    int permissions;        // Unix-style e.g. 0644
    time_t created_at;
    time_t modified_at;
    int direct_blocks[MAX_DIRECT_BLOCKS];
    int single_indirect;    // Points to a block of block numbers
} Inode;

// Directory entry
typedef struct {
    int inode_num;
    char name[MAX_FILENAME];
} DirEntry;

// Directory (simplified: max 16 entries)
typedef struct {
    DirEntry entries[16];
    int count;
} Directory;

Inode inode_table[MAX_INODES];
int inode_bitmap[MAX_INODES]; // 0 = free, 1 = used

void init_fs() {
    memset(inode_bitmap, 0, sizeof(inode_bitmap));
    memset(inode_table, 0, sizeof(inode_table));

    // Inode 0 is reserved. Inode 1 is the root directory.
    inode_bitmap[0] = 1;
    inode_bitmap[1] = 1;

    inode_table[1].inode_num = 1;
    inode_table[1].is_used = 1;
    inode_table[1].is_dir = 1;
    inode_table[1].link_count = 2; // "." and ".."
    inode_table[1].permissions = 0755;
    inode_table[1].created_at = time(NULL);
    inode_table[1].modified_at = time(NULL);

    printf("Filesystem initialized. Root inode: 1\n\n");
}

int allocate_inode() {
    for (int i = 2; i < MAX_INODES; i++) {
        if (!inode_bitmap[i]) {
            inode_bitmap[i] = 1;
            inode_table[i].inode_num = i;
            inode_table[i].is_used = 1;
            inode_table[i].created_at = time(NULL);
            inode_table[i].modified_at = time(NULL);
            return i;
        }
    }
    return -1; // No free inodes
}

void create_file(const char *name, int size, int permissions) {
    int inum = allocate_inode();
    if (inum == -1) {
        printf("Error: Inode table is full!\n");
        return;
    }
    inode_table[inum].is_dir = 0;
    inode_table[inum].size = size;
    inode_table[inum].permissions = permissions;
    inode_table[inum].link_count = 1;

    // Assign simulated block numbers
    int blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i = 0; i < blocks_needed && i < MAX_DIRECT_BLOCKS; i++) {
        inode_table[inum].direct_blocks[i] = 1000 + inum * 10 + i;
    }

    printf("Created file: '%s' -> inode %d | size: %d bytes | permissions: %04o\n",
           name, inum, size, permissions);
}

void create_dir(const char *name) {
    int inum = allocate_inode();
    if (inum == -1) {
        printf("Error: Inode table is full!\n");
        return;
    }
    inode_table[inum].is_dir = 1;
    inode_table[inum].size = BLOCK_SIZE;
    inode_table[inum].permissions = 0755;
    inode_table[inum].link_count = 2;
    printf("Created dir:  '%s' -> inode %d | permissions: %04o\n", name, inum, inode_table[inum].permissions);
}

void print_inode(int inum) {
    Inode *n = &inode_table[inum];
    if (!n->is_used) {
        printf("Inode %d is not in use.\n", inum);
        return;
    }
    printf("\n--- Inode %d ---\n", inum);
    printf("  Type:        %s\n", n->is_dir ? "directory" : "regular file");
    printf("  Size:        %d bytes\n", n->size);
    printf("  Permissions: %04o\n", n->permissions);
    printf("  Hard Links:  %d\n", n->link_count);
    printf("  Blocks:      ");
    for (int i = 0; i < MAX_DIRECT_BLOCKS; i++) {
        if (n->direct_blocks[i]) printf("%d ", n->direct_blocks[i]);
    }
    printf("\n");
}

void dump_inode_table() {
    printf("\n--- Inode Table Dump ---\n");
    printf("%-5s %-8s %-6s %-8s\n", "Inode", "Type", "Size", "Perms");
    printf("----------------------------------\n");
    for (int i = 1; i < MAX_INODES; i++) {
        if (inode_table[i].is_used) {
            printf("%-5d %-8s %-6d %-8o\n",
                   i,
                   inode_table[i].is_dir ? "dir" : "file",
                   inode_table[i].size,
                   inode_table[i].permissions);
        }
    }
}

int main() {
    init_fs();

    printf("Creating files and directories...\n");
    create_dir("home");
    create_dir("etc");
    create_file("passwd", 2048, 0644);
    create_file("hosts", 512, 0644);
    create_file("kernel.img", 8192000, 0755);
    create_dir("usr");
    create_file("main.c", 8096, 0644);

    print_inode(2); // Inspect the first created inode
    dump_inode_table();

    return 0;
}
