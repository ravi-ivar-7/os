#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
 * Block header structure. 
 * Hidden just before the memory returned to the user.
 */
typedef struct block_meta {
    size_t size;
    struct block_meta *next;
    int is_free;
} block_meta_t;

#define META_SIZE sizeof(block_meta_t)

// Head of the free list
void *global_base = NULL;

/* 
 * Find a free block that is large enough using First-Fit.
 */
block_meta_t *find_free_block(block_meta_t **last, size_t size) {
    block_meta_t *current = global_base;
    while (current && !(current->is_free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

/*
 * Ask the OS for more memory using sbrk.
 */
block_meta_t *request_space(block_meta_t *last, size_t size) {
    block_meta_t *block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    
    if (request == (void*) -1) {
        return NULL; // sbrk failed
    }

    if (last) {
        last->next = block;
    }
    
    block->size = size;
    block->next = NULL;
    block->is_free = 0;
    
    return block;
}

/*
 * Custom malloc implementation.
 */
void *custom_malloc(size_t size) {
    block_meta_t *block;

    if (size <= 0) {
        return NULL;
    }

    if (!global_base) { // First call
        block = request_space(NULL, size);
        if (!block) return NULL;
        global_base = block;
    } else {
        block_meta_t *last = global_base;
        block = find_free_block(&last, size);
        
        if (!block) { // No matching free block found
            block = request_space(last, size);
            if (!block) return NULL;
        } else { // Found a free block
            block->is_free = 0;
        }
    }

    return (block + 1); // Return pointer to the data region
}

/*
 * Pointer math to get the header from the data pointer.
 */
block_meta_t *get_block_ptr(void *ptr) {
    return (block_meta_t*)ptr - 1;
}

/*
 * Custom free implementation.
 */
void custom_free(void *ptr) {
    if (!ptr) {
        return;
    }

    block_meta_t *block_ptr = get_block_ptr(ptr);
    block_ptr->is_free = 1;
    // Note: A real allocator would try to merge adjacent free blocks here.
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

    printf("Allocating 80 bytes for str2...\n");
    // This should reuse the block previously held by str1
    char *str2 = (char *)custom_malloc(80);
    printf("str2 address: %p\n", str2);
    
    if (str1 == str2) {
        printf("Success: str2 reused the freed memory block!\n");
    } else {
        printf("Failed to reuse block.\n");
    }

    return 0;
}
