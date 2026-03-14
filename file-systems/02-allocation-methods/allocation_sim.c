#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_BLOCKS 30
#define FAT_SIZE 30

// ---- CONTIGUOUS ALLOCATION ----

int contiguous_disk[TOTAL_BLOCKS]; // -1 = free, otherwise file_id

void init_contiguous() {
    memset(contiguous_disk, -1, sizeof(contiguous_disk));
}

// Returns start block or -1 if no fit
int contiguous_allocate(int file_id, int blocks_needed) {
    int start = -1, count = 0;
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        if (contiguous_disk[i] == -1) {
            if (start == -1) start = i;
            count++;
            if (count == blocks_needed) {
                for (int j = start; j < start + blocks_needed; j++)
                    contiguous_disk[j] = file_id;
                return start;
            }
        } else {
            start = -1;
            count = 0;
        }
    }
    return -1;
}

void print_contiguous() {
    printf("Contiguous disk: [");
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        if (contiguous_disk[i] == -1) printf(" . ");
        else printf(" %d ", contiguous_disk[i]);
    }
    printf("]\n");
}

// ---- FAT (LINKED) ALLOCATION ----

int fat[FAT_SIZE]; // -1 = free, -2 = EOF, otherwise next_block

void init_fat() {
    for (int i = 0; i < FAT_SIZE; i++) fat[i] = -1;
}

// Returns start block or -1 if not enough space
int fat_allocate(int blocks_needed) {
    int start = -1, prev = -1;
    int allocated = 0;
    for (int i = 0; i < FAT_SIZE && allocated < blocks_needed; i++) {
        if (fat[i] == -1) {
            if (start == -1) start = i;
            if (prev != -1) fat[prev] = i;
            prev = i;
            allocated++;
        }
    }
    if (allocated < blocks_needed) return -1;
    fat[prev] = -2; // EOF marker
    return start;
}

void print_fat(int start, const char *label) {
    printf("FAT chain for '%s': ", label);
    int cur = start;
    while (cur != -2 && cur != -1) {
        printf("[%d]", cur);
        cur = fat[cur];
        if (cur != -2) printf("->");
    }
    printf(" (EOF)\n");
}

// ---- INDEXED ALLOCATION ----

#define MAX_DIRECT 5
typedef struct {
    int direct[MAX_DIRECT]; // Direct block pointers
    int used;
} IndexNode;

int indexed_disk[TOTAL_BLOCKS];
int indexed_next_free = 0;

int indexed_allocate(IndexNode *node, int blocks_needed) {
    node->used = 0;
    for (int i = 0; i < blocks_needed && i < MAX_DIRECT; i++) {
        if (indexed_next_free >= TOTAL_BLOCKS) return -1;
        node->direct[i] = indexed_next_free++;
        node->used++;
    }
    return node->direct[0];
}

void print_indexed(IndexNode *node, const char *label) {
    printf("Index node for '%s': direct_blocks=[", label);
    for (int i = 0; i < node->used; i++) {
        printf("%d", node->direct[i]);
        if (i < node->used - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("===== Allocation Method Comparison =====\n\n");

    // Contiguous
    printf("--- Contiguous Allocation ---\n");
    init_contiguous();
    int s1 = contiguous_allocate(1, 4);
    int s2 = contiguous_allocate(2, 3);
    printf("File A (4 blocks) starts at block %d\n", s1);
    printf("File B (3 blocks) starts at block %d\n", s2);

    // Free file A to create a gap (fragmentation demo)
    for (int i = s1; i < s1 + 4; i++) contiguous_disk[i] = -1;
    int s3 = contiguous_allocate(3, 5); // 5 blocks - won't fit in the gap
    if (s3 == -1)
        printf("File C (5 blocks) FAILED: fragmented gap too small!\n");
    else
        printf("File C (5 blocks) starts at block %d\n", s3);
    print_contiguous();

    // FAT
    printf("\n--- FAT (Linked) Allocation ---\n");
    init_fat();
    int fa = fat_allocate(4);
    int fb = fat_allocate(3);
    printf("File A (4 blocks) start: %d | ", fa); print_fat(fa, "A");
    printf("File B (3 blocks) start: %d | ", fb); print_fat(fb, "B");

    // Indexed
    printf("\n--- Indexed Allocation ---\n");
    IndexNode nodeA, nodeB;
    indexed_allocate(&nodeA, 4);
    indexed_allocate(&nodeB, 3);
    print_indexed(&nodeA, "A");
    print_indexed(&nodeB, "B");

    return 0;
}
