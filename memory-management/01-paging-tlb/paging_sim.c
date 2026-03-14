#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define PAGE_SIZE 256
#define NUM_PAGES 256
#define TLB_SIZE 16

typedef struct {
    int page_number;
    int frame_number;
    bool valid;
} TLBEntry;

TLBEntry tlb[TLB_SIZE];
int page_table[NUM_PAGES];

int tlb_hits = 0;
int page_table_hits = 0;
int total_accesses = 0;
int tlb_replace_index = 0;

void reset_stats() {
    tlb_hits = 0;
    page_table_hits = 0;
    total_accesses = 0;
}

void init_memory_structures() {
    // Fix: clear all TLB fields, not just valid bit. Also reset FIFO pointer.
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].valid = false;
        tlb[i].page_number = -1;
        tlb[i].frame_number = -1;
    }
    tlb_replace_index = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i] = i; // Direct map: page i -> frame i
    }
}

int translate_address(int virtual_address) {
    total_accesses++;
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_number == page_number) {
            tlb_hits++;
            return (tlb[i].frame_number * PAGE_SIZE) + offset;
        }
    }

    page_table_hits++;
    int frame_number = page_table[page_number];

    tlb[tlb_replace_index].page_number = page_number;
    tlb[tlb_replace_index].frame_number = frame_number;
    tlb[tlb_replace_index].valid = true;
    tlb_replace_index = (tlb_replace_index + 1) % TLB_SIZE;

    return (frame_number * PAGE_SIZE) + offset;
}

void print_stats() {
    printf("  TLB Hits:               %d\n", tlb_hits);
    printf("  Page Table Hits:        %d\n", page_table_hits);
    printf("  TLB Hit Rate:           %.2f%%\n\n",
           total_accesses > 0 ? ((float)tlb_hits / total_accesses) * 100 : 0.0f);
}

int main() {
    srand((unsigned int)time(NULL)); // Seed for true random results

    init_memory_structures();
    reset_stats();

    printf("Sequential access (high spatial locality):\n");
    for (int i = 0; i < 1000; i++) {
        translate_address(i);
    }
    print_stats();

    init_memory_structures(); // Full reset — also clears TLB data and FIFO ptr
    reset_stats();

    printf("Random access (low spatial locality):\n");
    for (int i = 0; i < 1000; i++) {
        translate_address(rand() % (NUM_PAGES * PAGE_SIZE));
    }
    print_stats();

    return 0;
}
