/**
 * Exercise 2: Observing Process States
 *
 * Learn:
 * - How to see process states in Linux
 * - Transitions between states (READY, RUNNING, WAITING)
 * - Using /proc filesystem
 * - CPU-bound vs I/O-bound behavior
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// CPU-bound task - consumes CPU (RUNNING state)
void cpu_bound_task() {
    printf("[CPU-BOUND] PID %d: Computing...\n", getpid());
    printf("Check my state: cat /proc/%d/status | grep State\n", getpid());

    volatile long long sum = 0;
    for (long long i = 0; i < 5000000000LL; i++) {
        sum += i;
    }
    printf("[CPU-BOUND] Done. Sum = %lld\n", sum);
}

// I/O-bound task - waits for input (WAITING state)
void io_bound_task() {
    printf("[I/O-BOUND] PID %d: Waiting for input...\n", getpid());
    printf("Check my state: cat /proc/%d/status | grep State\n", getpid());
    printf("(I should be in 'S' - Interruptible Sleep)\n");

    char buffer[100];
    printf("Type something and press Enter: ");
    fgets(buffer, sizeof(buffer), stdin);
    printf("[I/O-BOUND] Received: %s", buffer);
}

// Demonstrate READY state by creating many competing processes
void demonstrate_ready_state() {
    printf("\n=== READY State Demonstration ===\n");
    printf("Creating 4 CPU-bound processes...\n");
    printf("Run 'top' in another terminal to see them competing for CPU\n\n");

    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            printf("Process %d started (PID: %d)\n", i, getpid());

            // CPU-intensive work
            volatile long long sum = 0;
            for (long long j = 0; j < 3000000000LL; j++) {
                sum += j;
            }

            printf("Process %d finished (PID: %d)\n", i, getpid());
            exit(0);
        }
    }

    // Parent waits for all children
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }
}

// Read and display process state from /proc
void show_process_state(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *f = fopen(path, "r");
    if (!f) {
        perror("fopen");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "State:", 6) == 0) {
            printf("PID %d: %s", pid, line);
            break;
        }
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "cpu") == 0) {
        cpu_bound_task();
    } else if (argc > 1 && strcmp(argv[1], "io") == 0) {
        io_bound_task();
    } else if (argc > 1 && strcmp(argv[1], "ready") == 0) {
        demonstrate_ready_state();
    } else {
        printf("Process State Observer\n\n");
        printf("Usage:\n");
        printf("  %s cpu    - Run CPU-bound task (R - Running)\n", argv[0]);
        printf("  %s io     - Run I/O-bound task (S - Sleeping)\n", argv[0]);
        printf("  %s ready  - Create competing processes (R/S states)\n", argv[0]);
        printf("\nProcess States in Linux:\n");
        printf("  R  - Running or Runnable (READY or RUNNING)\n");
        printf("  S  - Interruptible Sleep (WAITING for I/O)\n");
        printf("  D  - Uninterruptible Sleep (WAITING, can't be interrupted)\n");
        printf("  T  - Stopped (by signal)\n");
        printf("  Z  - Zombie (TERMINATED but not reaped)\n");
    }

    return 0;
}

/**
 * EXPERIMENTS TO RUN:
 *
 * 1. Run CPU-bound task:
 *    ./process_states cpu &
 *    While it runs: ps aux | grep process_states
 *    Should show 'R' or 'S' state
 *
 * 2. Run I/O-bound task:
 *    ./process_states io
 *    In another terminal: cat /proc/$(pgrep process_states)/status | grep State
 *    Should show 'S' (Sleeping)
 *
 * 3. See state transitions:
 *    ./process_states ready
 *    In another terminal: watch -n 0.1 'ps aux | grep process_states'
 *    Watch processes transition between R and S
 *
 * QUESTIONS:
 * 1. Why do running processes sometimes show 'S' instead of 'R'?
 * 2. On a multi-core system, can multiple processes be in 'R' state?
 * 3. What causes a transition from RUNNING to WAITING?
 * 4. What causes a transition from WAITING to READY?
 */
