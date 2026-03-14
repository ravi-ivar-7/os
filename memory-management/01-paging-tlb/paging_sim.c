#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PAGE_SIZE 256
#define NUM_PAGES 256
#define TLB_SIZE 16

// Simplistic TLB structure
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

void init_memory_structures() {
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].valid = false;
    }
    for (int i = 0; i < NUM_PAGES; i++) {
        // Direct mapping simulation: page i -> frame i
        page_table[i] = i; 
    }
}

int translate_address(int virtual_address) {
    total_accesses++;
    
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;
    
    // Check TLB
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_number == page_number) {
            tlb_hits++;
            return (tlb[i].frame_number * PAGE_SIZE) + offset;
        }
    }
    
    // TLB Miss, check Page Table
    page_table_hits++;
    int frame_number = page_table[page_number];
    
    // Update TLB (FIFO replacement)
    tlb[tlb_replace_index].page_number = page_number;
    tlb[tlb_replace_index].frame_number = frame_number;
    tlb[tlb_replace_index].valid = true;
    
    tlb_replace_index = (tlb_replace_index + 1) % TLB_SIZE;
    
    return (frame_number * PAGE_SIZE) + offset;
}

int main() {
    init_memory_structures();
    
    printf("Simulating sequential memory access (High spatial locality)...\n");
    for (int i = 0; i < 1000; i++) {
        translate_address(i); 
    }
    
    printf("TLB Hits: %d\n", tlb_hits);
    printf("Page Table Hits (TLB Misses): %d\n", page_table_hits);
    printf("TLB Hit Rate: %.2f%%\n\n", ((float)tlb_hits / total_accesses) * 100);
    
    // Reset stats
    tlb_hits = 0;
    page_table_hits = 0;
    total_accesses = 0;
    init_memory_structures(); 

    printf("Simulating random memory access (Low spatial locality)...\n");
    for (int i = 0; i < 1000; i++) {
        int random_addr = rand() % (NUM_PAGES * PAGE_SIZE);
        translate_address(random_addr);
    }
    
    printf("TLB Hits: %d\n", tlb_hits);
    printf("Page Table Hits (TLB Misses): %d\n", page_table_hits);
    printf("TLB Hit Rate: %.2f%%\n", ((float)tlb_hits / total_accesses) * 100);
    
    return 0;
}
