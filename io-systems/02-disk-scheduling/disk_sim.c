#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define CYLINDERS 200
#define NUM_REQUESTS 8

int requests[NUM_REQUESTS] = {98, 183, 37, 122, 14, 124, 65, 67};
int initial_head = 53;

void simulate_fcfs() {
    printf("=== FCFS (First-Come, First-Served) ===\n");
    int head = initial_head;
    int total_movement = 0;
    
    printf("Head movement: %d", head);
    for (int i = 0; i < NUM_REQUESTS; i++) {
        total_movement += abs(head - requests[i]);
        head = requests[i];
        printf(" -> %d", head);
    }
    printf("\nTotal Seek Time: %d tracks\n\n", total_movement);
}

void simulate_sstf() {
    printf("=== SSTF (Shortest Seek Time First) ===\n");
    int head = initial_head;
    int total_movement = 0;
    bool completed[NUM_REQUESTS] = {false};
    
    printf("Head movement: %d", head);
    for (int i = 0; i < NUM_REQUESTS; i++) {
        int closest_index = -1;
        int min_distance = CYLINDERS * 2; // Infinity
        
        // Find closest uncompleted request
        for (int j = 0; j < NUM_REQUESTS; j++) {
            if (!completed[j]) {
                int distance = abs(head - requests[j]);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_index = j;
                }
            }
        }
        
        completed[closest_index] = true;
        total_movement += min_distance;
        head = requests[closest_index];
        printf(" -> %d", head);
    }
    printf("\nTotal Seek Time: %d tracks\n\n", total_movement);
}

// Helper for sorting required by SCAN/C-SCAN
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void simulate_scan() {
    // SCAN moves in one direction to the end (0), then reverses
    printf("=== SCAN (Elevator Algorithm, moving toward 0 first) ===\n");
    
    int sorted_requests[NUM_REQUESTS];
    for (int i=0; i<NUM_REQUESTS; i++) sorted_requests[i] = requests[i];
    qsort(sorted_requests, NUM_REQUESTS, sizeof(int), compare);
    
    int head = initial_head;
    int total_movement = 0;
    
    printf("Head movement: %d", head);
    
    // Move towards 0
    for (int i = NUM_REQUESTS - 1; i >= 0; i--) {
        if (sorted_requests[i] <= head) {
            total_movement += abs(head - sorted_requests[i]);
            head = sorted_requests[i];
            printf(" -> %d", head);
        }
    }
    
    // Jump to 0 (end of disk)
    if (head != 0) {
        total_movement += head;
        head = 0;
        printf(" -> 0(end)");
    }
    
    // Move towards CYLINDERS - 1
    for (int i = 0; i < NUM_REQUESTS; i++) {
        if (sorted_requests[i] > initial_head) {
            total_movement += abs(head - sorted_requests[i]);
            head = sorted_requests[i];
            printf(" -> %d", head);
        }
    }
    printf("\nTotal Seek Time: %d tracks\n\n", total_movement);
}

void simulate_c_scan() {
    // C-SCAN moves in one direction to the end (0), jumps to other end, and scans again
    printf("=== C-SCAN (Circular SCAN, moving toward 0 first) ===\n");
    
    int sorted_requests[NUM_REQUESTS];
    for (int i=0; i<NUM_REQUESTS; i++) sorted_requests[i] = requests[i];
    qsort(sorted_requests, NUM_REQUESTS, sizeof(int), compare);
    
    int head = initial_head;
    int total_movement = 0;
    
    printf("Head movement: %d", head);
    
    // Move towards 0
    for (int i = NUM_REQUESTS - 1; i >= 0; i--) {
        if (sorted_requests[i] <= head) {
            total_movement += abs(head - sorted_requests[i]);
            head = sorted_requests[i];
            printf(" -> %d", head);
        }
    }
    
    // Jump to 0, then jump to CYLINDERS - 1
    if (head != 0) {
        total_movement += head;
        head = 0;
        printf(" -> 0(end)");
    }
    printf(" -> (jump) -> %d(end)", CYLINDERS - 1);
    
    // C-SCAN jump is often counted or not counted depending on the model.
    // In this simulation, we'll count the sweeping return trip for accuracy.
    total_movement += (CYLINDERS - 1);
    head = CYLINDERS - 1; 
    
    // Sweep back down
    for (int i = NUM_REQUESTS - 1; i >= 0; i--) {
        if (sorted_requests[i] > initial_head) {
            total_movement += abs(head - sorted_requests[i]);
            head = sorted_requests[i];
            printf(" -> %d", head);
        }
    }
    printf("\nTotal Seek Time: %d tracks\n\n", total_movement);
}


int main() {
    printf("Disk Queue: ");
    for(int i=0; i<NUM_REQUESTS; i++) printf("%d ", requests[i]);
    printf("\nInitial Head Position: %d\n\n", initial_head);
    
    simulate_fcfs();
    simulate_sstf();
    simulate_scan();
    simulate_c_scan();
    
    return 0;
}
