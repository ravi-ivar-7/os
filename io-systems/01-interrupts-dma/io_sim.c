#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define DATA_SIZE 5
int data_buffer[DATA_SIZE];
bool device_ready = false;
bool interrupt_received = false;

// Simulated hardware device generating data
void* hardware_device(void* arg) {
    (void)arg;
    for (int i = 0; i < DATA_SIZE; i++) {
        usleep(100000); // Simulate slow hardware (100ms)
        data_buffer[i] = i * 10;
        device_ready = true;
        interrupt_received = true; // For interrupt/DMA modes
    }
    return NULL;
}

void simulate_polling() {
    printf("=== Polling (Programmed I/O) ===\n");
    pthread_t dev_thread;
    pthread_create(&dev_thread, NULL, hardware_device, NULL);

    int cpu_cycles_wasted = 0;
    for (int i = 0; i < DATA_SIZE; i++) {
        device_ready = false;
        // CPU actively waits
        while (!device_ready) {
            cpu_cycles_wasted++;
            usleep(1000); // Avoid freezing the entire terminal
        }
        printf("CPU read data: %d\n", data_buffer[i]);
    }
    printf("-> CPU cycles wasted polling: %d\n\n", cpu_cycles_wasted);
    pthread_join(dev_thread, NULL);
}

void simulate_interrupts() {
    printf("=== Interrupt-Driven I/O ===\n");
    pthread_t dev_thread;
    pthread_create(&dev_thread, NULL, hardware_device, NULL);

    int useful_work_done = 0;
    for (int i = 0; i < DATA_SIZE; i++) {
        interrupt_received = false;
        
        // CPU does other work while waiting
        while (!interrupt_received) {
            useful_work_done++;
            usleep(10000); // Doing useful background work
        }
        
        // ISR (Interrupt Service Routine) context
        printf("Interrupt! CPU saves state, reads data: %d, restores state\n", data_buffer[i]);
    }
    printf("-> Useful work units completed while waiting: %d\n\n", useful_work_done);
    pthread_join(dev_thread, NULL);
}

void simulate_dma() {
    printf("=== Direct Memory Access (DMA) ===\n");
    printf("CPU configured DMA controller. CPU is now free.\n");
    
    pthread_t dev_thread;
    // DMA hardware writes directly to data_buffer without CPU involvement
    pthread_create(&dev_thread, NULL, hardware_device, NULL);

    int useful_work_done = 0;
    interrupt_received = false;
    
    // CPU does other work for the ENTIRE duration
    while (data_buffer[DATA_SIZE - 1] != (DATA_SIZE - 1) * 10) {
        useful_work_done++;
        usleep(10000); // Doing useful background work
    }
    
    printf("Interrupt! DMA transfer complete.\n");
    printf("CPU verifies RAM:\n");
    for(int i = 0; i < DATA_SIZE; i++) {
        printf("  RAM block [%d]: %d\n", i, data_buffer[i]);
    }
    
    printf("-> Useful work units completed during transfer: %d\n\n", useful_work_done);
    pthread_join(dev_thread, NULL);
}

int main() {
    // Reset buffer
    for(int i=0; i<DATA_SIZE; i++) data_buffer[i] = -1;
    simulate_polling();
    
    for(int i=0; i<DATA_SIZE; i++) data_buffer[i] = -1;
    simulate_interrupts();
    
    for(int i=0; i<DATA_SIZE; i++) data_buffer[i] = -1;
    simulate_dma();

    return 0;
}
