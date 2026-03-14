/**
 * Demonstrating Race Conditions
 *
 * Shows what happens when multiple processes/threads
 * access shared data without synchronization
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 1000000

// Shared counter (the problem!)
int counter = 0;

// Thread function that increments counter
void* increment_counter(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        // This looks atomic but it's NOT!
        counter++;
        /*
         * Assembly equivalent:
         * 1. mov eax, [counter]    ; Read counter into register
         * 2. inc eax               ; Increment register
         * 3. mov [counter], eax    ; Write back to memory
         *
         * Another thread can interrupt between any of these steps!
         */
    }

    printf("Thread %d finished\n", thread_id);
    return NULL;
}

void demonstrate_race_condition() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("=== Race Condition Demonstration ===\n");
    printf("Starting %d threads, each incrementing counter %d times\n",
           NUM_THREADS, INCREMENTS_PER_THREAD);
    printf("Expected final value: %d\n\n", NUM_THREADS * INCREMENTS_PER_THREAD);

    counter = 0;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, increment_counter, &thread_ids[i]);
    }

    // Wait for all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nActual final value: %d\n", counter);
    printf("Lost updates: %d\n", (NUM_THREADS * INCREMENTS_PER_THREAD) - counter);
    printf("\nWhy? Multiple threads read old value before write completes!\n");
}

// Example of what can happen:
// Thread 1: Read counter=100
// Thread 2: Read counter=100
// Thread 1: Increment to 101
// Thread 1: Write 101
// Thread 2: Increment to 101 (based on old read!)
// Thread 2: Write 101
// Result: Two increments but only increased by 1!

// Now with mutex (fixed version)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int safe_counter = 0;

void* increment_counter_safe(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&mutex);
        safe_counter++;
        pthread_mutex_unlock(&mutex);
    }

    printf("Thread %d finished (safe version)\n", thread_id);
    return NULL;
}

void demonstrate_mutex_solution() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("\n=== Mutex Solution ===\n");
    printf("Same test but with mutex protection\n\n");

    safe_counter = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, increment_counter_safe, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nExpected: %d\n", NUM_THREADS * INCREMENTS_PER_THREAD);
    printf("Actual:   %d\n", safe_counter);
    printf("Lost updates: %d\n", (NUM_THREADS * INCREMENTS_PER_THREAD) - safe_counter);
    printf("\nMutex ensures atomic access to critical section!\n");
}

int main() {
    printf("Race Condition vs Synchronization\n");
    printf("===================================\n\n");

    // Show the problem
    demonstrate_race_condition();

    // Show the solution
    demonstrate_mutex_solution();

    printf("\n=== Key Takeaways ===\n");
    printf("1. counter++ is NOT atomic (3 assembly instructions)\n");
    printf("2. Without synchronization, updates are lost\n");
    printf("3. Mutex ensures only one thread in critical section\n");
    printf("4. Trade-off: Correctness vs Performance\n");

    return 0;
}

/**
 * OBSERVATIONS:
 *
 * Run this multiple times. The lost updates vary because:
 * - Race conditions are non-deterministic
 * - Depends on thread scheduling
 * - CPU architecture (cache coherence)
 *
 * WHY RACE CONDITIONS ARE HARD:
 * - May not happen in testing (works 99% of time)
 * - Heisenbugs: Disappear when you try to debug
 * - Worse under load (production)
 *
 * EXPERIMENTS:
 *
 * 1. Vary number of threads (1, 2, 4, 8, 16)
 *    More threads → more contention → more lost updates
 *
 * 2. Vary increments per thread
 *    More increments → more lost updates
 *
 * 3. Check mutex overhead:
 *    Time both versions
 *    Mutex version is slower (but correct!)
 *
 * 4. Try other atomics:
 *    __atomic_add_fetch(&counter, 1, __ATOMIC_SEQ_CST)
 *    Compare performance vs mutex
 */
