/**
 * Producer-Consumer Problem (Bounded Buffer)
 *
 * Classic synchronization problem:
 * - Producers add items to shared buffer
 * - Consumers remove items from buffer
 * - Buffer has limited capacity
 *
 * Challenges:
 * - Don't overflow buffer (producer must wait if full)
 * - Don't underflow buffer (consumer must wait if empty)
 * - Mutual exclusion for buffer access
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 3
#define ITEMS_TO_PRODUCE 20

// Shared buffer
int buffer[BUFFER_SIZE];
int in = 0;   // Next position to produce
int out = 0;  // Next position to consume

// Semaphores
sem_t empty;  // Counts empty slots
sem_t full;   // Counts full slots
pthread_mutex_t mutex;  // Protects buffer access

int items_produced = 0;
int items_consumed = 0;

void* producer(void* arg) {
    int producer_id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);
        if (items_produced >= ITEMS_TO_PRODUCE) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        items_produced++;
        int item = items_produced;
        pthread_mutex_unlock(&mutex);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Produced item %d at position %d\n", producer_id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        usleep(100000);
    }

    printf("Producer %d: Finished\n", producer_id);
    return NULL;
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;

    while (1) {
        // Fix: check completion BEFORE blocking on sem_wait to avoid deadlock.
        // Producers signal a dummy item on 'full' for each consumer to wake them up.
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (items_consumed >= ITEMS_TO_PRODUCE) {
            // Wake the next consumer in line so all consumers can exit
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            break;
        }

        int item = buffer[out];
        printf("                    Consumer %d: Consumed item %d from position %d\n",
               consumer_id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        items_consumed++;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        usleep(150000);
    }

    printf("Consumer %d: Finished\n", consumer_id);
    return NULL;
}

void demonstrate_producer_consumer() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    printf("=== Producer-Consumer Problem ===\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producers: %d, Consumers: %d\n", NUM_PRODUCERS, NUM_CONSUMERS);
    printf("Items to produce: %d\n\n", ITEMS_TO_PRODUCE);

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots empty
    sem_init(&full, 0, 0);             // Initially no slots full
    pthread_mutex_init(&mutex, NULL);

    // Create producers
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Create consumers
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Wait for all producers
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    // Fix: post one extra 'full' token so the first blocked consumer can wake up
    // and then cascade-wake the remaining consumers via the chain in consumer().
    sem_post(&full);

    // Wait for all consumers
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\n=== Summary ===\n");
    printf("Items produced: %d\n", items_produced);
    printf("Items consumed: %d\n", items_consumed);
}

int main() {
    demonstrate_producer_consumer();

    printf("\n=== How It Works ===\n");
    printf("Semaphores:\n");
    printf("  empty: Counts available slots (starts at BUFFER_SIZE)\n");
    printf("  full:  Counts items in buffer (starts at 0)\n");
    printf("\nProducer:\n");
    printf("  1. wait(empty)  - Decrement empty slots (block if 0)\n");
    printf("  2. lock(mutex)  - Enter critical section\n");
    printf("  3. Add item to buffer\n");
    printf("  4. unlock(mutex)\n");
    printf("  5. post(full)   - Increment full slots\n");
    printf("\nConsumer:\n");
    printf("  1. wait(full)   - Decrement full slots (block if 0)\n");
    printf("  2. lock(mutex)  - Enter critical section\n");
    printf("  3. Remove item from buffer\n");
    printf("  4. unlock(mutex)\n");
    printf("  5. post(empty)  - Increment empty slots\n");

    printf("\n=== Key Insights ===\n");
    printf("1. Semaphores handle buffer full/empty conditions\n");
    printf("2. Mutex protects buffer access (mutual exclusion)\n");
    printf("3. Circular buffer for efficiency\n");
    printf("4. Producers block when buffer full\n");
    printf("5. Consumers block when buffer empty\n");

    return 0;
}

/**
 * COMMON MISTAKES:
 *
 * 1. Using only mutex (no semaphores):
 *    - Can't handle full/empty conditions
 *    - Busy waiting or deadlock
 *
 * 2. Wrong order of operations:
 *    - lock() before wait() → DEADLOCK!
 *    - Always wait() on semaphore first
 *
 * 3. Forgetting to post():
 *    - Processes permanently blocked
 *
 * EXPERIMENTS:
 *
 * 1. Change buffer size:
 *    Small buffer → more blocking
 *    Large buffer → less blocking
 *
 * 2. Vary producer/consumer speeds:
 *    Fast producers + slow consumers → buffer fills
 *    Slow producers + fast consumers → buffer empties
 *
 * 3. Try without semaphores (only mutex):
 *    See the race conditions!
 *
 * 4. Try without mutex:
 *    Race conditions on buffer access
 */
