#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAMES 3
#define REFERENCE_STRING_LEN 20

int reference_string[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};

bool is_in_memory(int page, int frames[], int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

int simulate_fifo() {
    int frames[FRAMES];
    for (int i = 0; i < FRAMES; i++) frames[i] = -1;

    int page_faults = 0;
    int index = 0;

    for (int i = 0; i < REFERENCE_STRING_LEN; i++) {
        int page = reference_string[i];
        if (!is_in_memory(page, frames, FRAMES)) {
            frames[index] = page;
            index = (index + 1) % FRAMES;
            page_faults++;
        }
    }
    return page_faults;
}

int simulate_lru() {
    int frames[FRAMES];
    int last_used[FRAMES];

    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
        last_used[i] = 0;
    }

    int page_faults = 0;
    int timestamp = 0; // renamed from 'time' to avoid shadowing stdlib time()

    for (int i = 0; i < REFERENCE_STRING_LEN; i++) {
        timestamp++;
        int page = reference_string[i];

        bool found = false;
        for (int j = 0; j < FRAMES; j++) {
            if (frames[j] == page) {
                last_used[j] = timestamp;
                found = true;
                break;
            }
        }

        if (!found) {
            // Fix: first look for a genuinely empty slot, THEN find LRU victim
            int victim = -1;
            for (int j = 0; j < FRAMES; j++) {
                if (frames[j] == -1) {
                    victim = j;
                    break;
                }
            }
            if (victim == -1) {
                int min_time = last_used[0];
                victim = 0;
                for (int j = 1; j < FRAMES; j++) {
                    if (last_used[j] < min_time) {
                        min_time = last_used[j];
                        victim = j;
                    }
                }
            }
            frames[victim] = page;
            last_used[victim] = timestamp;
            page_faults++;
        }
    }
    return page_faults;
}

int simulate_clock() {
    int frames[FRAMES];
    int use_bit[FRAMES];

    for (int i = 0; i < FRAMES; i++) {
        frames[i] = -1;
        use_bit[i] = 0;
    }

    int page_faults = 0;
    int pointer = 0;

    for (int i = 0; i < REFERENCE_STRING_LEN; i++) {
        int page = reference_string[i];

        bool found = false;
        for (int j = 0; j < FRAMES; j++) {
            if (frames[j] == page) {
                use_bit[j] = 1;
                found = true;
                break;
            }
        }

        if (!found) {
            while (true) {
                if (frames[pointer] == -1 || use_bit[pointer] == 0) {
                    frames[pointer] = page;
                    use_bit[pointer] = 1;
                    pointer = (pointer + 1) % FRAMES;
                    page_faults++;
                    break;
                } else {
                    use_bit[pointer] = 0;
                    pointer = (pointer + 1) % FRAMES;
                }
            }
        }
    }
    return page_faults;
}

int main() {
    printf("Reference String: ");
    for (int i = 0; i < REFERENCE_STRING_LEN; i++) {
        printf("%d ", reference_string[i]);
    }
    printf("\n\n");

    printf("FIFO  Page Faults: %d\n", simulate_fifo());
    printf("LRU   Page Faults: %d\n", simulate_lru());
    printf("Clock Page Faults: %d\n", simulate_clock());

    return 0;
}
