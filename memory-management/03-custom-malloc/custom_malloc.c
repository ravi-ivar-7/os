#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct block_meta {
    size_t size;
    struct block_meta *next;
    int is_free;
} block_meta_t;

#define META_SIZE sizeof(block_meta_t)
#define MIN_SPLIT_SIZE 32

void *global_base = NULL;

block_meta_t *find_free_block(block_meta_t **last, size_t size) {
    block_meta_t *current = global_base;
    while (current && !(current->is_free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

block_meta_t *request_space(block_meta_t *last, size_t size) {
    block_meta_t *block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    if (request == (void *)-1) return NULL;
    if (last) last->next = block;
    block->size = size;
    block->next = NULL;
    block->is_free = 0;
    return block;
}

// Improvement: split a large block if the tail is big enough to be reused
static void split_block(block_meta_t *block, size_t size) {
    if (block->size >= size + META_SIZE + MIN_SPLIT_SIZE) {
        block_meta_t *tail = (block_meta_t *)((char *)(block + 1) + size);
        tail->size = block->size - size - META_SIZE;
        tail->next = block->next;
        tail->is_free = 1;
        block->size = size;
        block->next = tail;
    }
}

void *custom_malloc(size_t size) {
    if (size == 0) return NULL; // Fix: was `size <= 0` on unsigned type — always false

    block_meta_t *block;
    if (!global_base) {
        block = request_space(NULL, size);
        if (!block) return NULL;
        global_base = block;
    } else {
        block_meta_t *last = global_base;
        block = find_free_block(&last, size);
        if (!block) {
            block = request_space(last, size);
            if (!block) return NULL;
        } else {
            split_block(block, size);
            block->is_free = 0;
        }
    }
    return (block + 1);
}

block_meta_t *get_block_ptr(void *ptr) {
    return (block_meta_t *)ptr - 1;
}

void custom_free(void *ptr) {
    if (!ptr) return;
    block_meta_t *block_ptr = get_block_ptr(ptr);
    block_ptr->is_free = 1;

    // Improvement: coalesce adjacent free blocks to prevent fragmentation
    block_meta_t *cur = global_base;
    while (cur && cur->next) {
        if (cur->is_free && cur->next->is_free) {
            cur->size += META_SIZE + cur->next->size;
            cur->next = cur->next->next;
        } else {
            cur = cur->next;
        }
    }
}

int main() {
    printf("Allocating 100 bytes for str1...\n");
    char *str1 = (char *)custom_malloc(100);
    strcpy(str1, "Hello from custom allocator!");
    printf("str1 address: %p, value: %s\n\n", str1, str1);

    printf("Allocating 50 bytes for arr1...\n");
    int *arr1 = (int *)custom_malloc(50);
    printf("arr1 address: %p\n\n", arr1);

    printf("Freeing str1 (100 bytes)...\n");
    custom_free(str1);

    printf("Allocating 80 bytes for str2 (should reuse str1's block)...\n");
    char *str2 = (char *)custom_malloc(80);
    printf("str2 address: %p\n", str2);

    if (str1 == str2) {
        printf("Success: str2 reused the freed memory block!\n");
    } else {
        printf("Note: str2 at different address (block may have been split).\n");
    }

    custom_free(str2);
    custom_free(arr1);

    return 0;
}
