#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define DATA_CHUNKS 5
#define CHUNK_SIZE 1024

// Single Buffering Setup
int single_buffer[CHUNK_SIZE];
int single_buffer_full = 0; // 0=empty, 1=full
pthread_mutex_t single_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t single_cond = PTHREAD_COND_INITIALIZER;

// Double Buffering Setup
int buffer_A[CHUNK_SIZE];
int buffer_B[CHUNK_SIZE];
int *writing_buffer = buffer_A;
int *reading_buffer = buffer_B;
int buffers_ready = 0;
pthread_mutex_t double_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t double_cond = PTHREAD_COND_INITIALIZER;

int total_time_single = 0;
int total_time_double = 0;

// --- SINGLE BUFFERING ---

void* single_device_write(void* arg) {
    (void)arg;
    for (int i = 0; i < DATA_CHUNKS; i++) {
        // Wait for buffer to be empty
        pthread_mutex_lock(&single_mutex);
        while (single_buffer_full == 1) {
            pthread_cond_wait(&single_cond, &single_mutex);
        }
        
        // Simulate hardware filling buffer (e.g. disk read)
        printf("[Hardware] Filling single buffer (Chunk %d)...\n", i+1);
        usleep(200000); // 200ms device time
        
        single_buffer_full = 1;
        pthread_cond_signal(&single_cond);
        pthread_mutex_unlock(&single_mutex);
    }
    return NULL;
}

void* single_cpu_read(void* arg) {
    (void)arg;
    for (int i = 0; i < DATA_CHUNKS; i++) {
        // Wait for buffer to be full
        pthread_mutex_lock(&single_mutex);
        while (single_buffer_full == 0) {
            pthread_cond_wait(&single_cond, &single_mutex);
        }
        
        // Simulate CPU processing data
        printf("  [CPU]      Processing single buffer (Chunk %d)...\n", i+1);
        usleep(100000); // 100ms CPU time
        
        single_buffer_full = 0;
        pthread_cond_signal(&single_cond);
        pthread_mutex_unlock(&single_mutex);
    }
    return NULL;
}

// --- DOUBLE BUFFERING ---

void* double_device_write(void* arg) {
    (void)arg;
    for (int i = 0; i < DATA_CHUNKS; i++) {
        // Simulate hardware filling the writing buffer
        printf("[Hardware] Filling buffer %c (Chunk %d)...\n", 
               (writing_buffer == buffer_A) ? 'A' : 'B', i+1);
        usleep(200000); // 200ms device time
        
        pthread_mutex_lock(&double_mutex);
        
        // Swap buffers
        int *temp = writing_buffer;
        writing_buffer = reading_buffer;
        reading_buffer = temp;
        
        buffers_ready++;
        pthread_cond_signal(&double_cond);
        pthread_mutex_unlock(&double_mutex);
    }
    return NULL;
}

void* double_cpu_read(void* arg) {
    (void)arg;
    for (int i = 0; i < DATA_CHUNKS; i++) {
        pthread_mutex_lock(&double_mutex);
        while (buffers_ready == 0) {
            pthread_cond_wait(&double_cond, &double_mutex);
        }
        buffers_ready--;
        pthread_mutex_unlock(&double_mutex);
        
        // Simulate CPU processing data from the reading buffer
        printf("  [CPU]      Processing buffer %c (Chunk %d)...\n", 
               (reading_buffer == buffer_A) ? 'A' : 'B', i+1);
        usleep(100000); // 100ms CPU time (concurrent with next hardware fill!)
    }
    return NULL;
}

int main() {
    pthread_t io_thread, cpu_thread;

    printf("=== SIMULATING SINGLE BUFFERING ===\n");
    printf("Hardware and CPU must take turns. Total time should be ~ (200ms + 100ms) * %d = 1500ms\n\n", DATA_CHUNKS);
    
    pthread_create(&io_thread, NULL, single_device_write, NULL);
    pthread_create(&cpu_thread, NULL, single_cpu_read, NULL);
    
    pthread_join(io_thread, NULL);
    pthread_join(cpu_thread, NULL);

    printf("\n=== SIMULATING DOUBLE BUFFERING ===\n");
    printf("Hardware and CPU can overlap. Total time should be ~ 200ms * %d + 100ms = 1100ms\n\n", DATA_CHUNKS);
    
    pthread_create(&io_thread, NULL, double_device_write, NULL);
    pthread_create(&cpu_thread, NULL, double_cpu_read, NULL);
    
    pthread_join(io_thread, NULL);
    pthread_join(cpu_thread, NULL);
    
    printf("\nConclusion: Double buffering allows the slow I/O device to keep producing\n");
    printf("data seamlessly while the CPU processes the previous chunk.\n");

    return 0;
}
