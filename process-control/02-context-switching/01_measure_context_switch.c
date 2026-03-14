/**
 * Exercise 1: Measuring Context Switch Cost
 *
 * Empirically measure how expensive context switching is.
 * We'll use pipes to force context switches between two processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

#define ITERATIONS 100000

long get_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void measure_context_switch_cost() {
    int pipe1[2], pipe2[2];
    char byte = 'X';
    long start, end;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        close(pipe1[1]);  // Close write end of pipe1
        close(pipe2[0]);  // Close read end of pipe2

        for (int i = 0; i < ITERATIONS; i++) {
            // Wait for parent (blocks, causes context switch)
            read(pipe1[0], &byte, 1);
            // Wake up parent (causes context switch)
            write(pipe2[1], &byte, 1);
        }

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else {
        // Parent process
        close(pipe1[0]);  // Close read end of pipe1
        close(pipe2[1]);  // Close write end of pipe2

        start = get_microseconds();

        for (int i = 0; i < ITERATIONS; i++) {
            // Wake up child (causes context switch)
            write(pipe1[1], &byte, 1);
            // Wait for child (blocks, causes context switch)
            read(pipe2[0], &byte, 1);
        }

        end = get_microseconds();

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL);

        long total_time = end - start;
        double time_per_switch = (double)total_time / (ITERATIONS * 2);

        printf("=== Context Switch Cost Measurement ===\n");
        printf("Iterations: %d\n", ITERATIONS);
        printf("Total time: %ld microseconds\n", total_time);
        printf("Context switches: %d (2 per iteration)\n", ITERATIONS * 2);
        printf("Average time per context switch: %.2f microseconds\n", time_per_switch);
        printf("\nNote: This includes pipe overhead, so actual context switch is slightly less\n");
    }
}

void measure_system_call_cost() {
    long start, end;
    int iterations = 1000000;

    start = get_microseconds();
    for (int i = 0; i < iterations; i++) {
        getpid();  // Simple syscall
    }
    end = get_microseconds();

    double time_per_syscall = (double)(end - start) / iterations;
    printf("\n=== System Call Cost ===\n");
    printf("Average time per getpid() syscall: %.3f microseconds\n", time_per_syscall);
    printf("(Syscalls are cheaper than context switches)\n");
}

void demonstrate_voluntary_vs_involuntary() {
    printf("\n=== Voluntary vs Involuntary Context Switches ===\n");
    printf("Check context switch counters:\n");
    printf("  cat /proc/%d/status | grep ctxt\n", getpid());
    printf("  voluntary_ctxt_switches   - Process blocked (I/O, lock, etc)\n");
    printf("  nonvoluntary_ctxt_switches - Preempted by scheduler\n\n");

    long vol_before = 0, invol_before = 0, vol_after = 0, invol_after = 0;
    FILE *f;
    char line[256];

    // Read initial counters
    snprintf(line, sizeof(line), "/proc/%d/status", getpid());
    f = fopen(line, "r");
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "voluntary_ctxt_switches: %ld", &vol_before) == 1) continue;
        if (sscanf(line, "nonvoluntary_ctxt_switches: %ld", &invol_before) == 1) break;
    }
    fclose(f);

    // Do some I/O (causes voluntary switches)
    printf("Performing I/O operations (causes voluntary switches)...\n");
    for (int i = 0; i < 100; i++) {
        usleep(100);  // Voluntary yield
    }

    // Read final counters
    snprintf(line, sizeof(line), "/proc/%d/status", getpid());
    f = fopen(line, "r");
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "voluntary_ctxt_switches: %ld", &vol_after) == 1) continue;
        if (sscanf(line, "nonvoluntary_ctxt_switches: %ld", &invol_after) == 1) break;
    }
    fclose(f);

    printf("\nResults:\n");
    printf("  Voluntary switches: %ld → %ld (delta: %ld)\n",
           vol_before, vol_after, vol_after - vol_before);
    printf("  Involuntary switches: %ld → %ld (delta: %ld)\n",
           invol_before, invol_after, invol_after - invol_before);
}

int main() {
    printf("Context Switch Cost Analysis\n");
    printf("=============================\n\n");

    measure_context_switch_cost();
    measure_system_call_cost();
    demonstrate_voluntary_vs_involuntary();

    return 0;
}

/**
 * WHAT'S HAPPENING:
 *
 * 1. Two processes ping-pong a byte through pipes
 * 2. Each read() blocks the reader, forcing context switch
 * 3. Each write() wakes the other process, forcing context switch
 * 4. We measure total time and divide by number of switches
 *
 * OBSERVATIONS:
 * - Context switch typically takes 1-10 microseconds
 * - Much more expensive than a system call
 * - Varies based on CPU, cache state, process size
 *
 * WHY IT'S EXPENSIVE:
 * - Save/restore CPU registers
 * - Switch page tables (TLB flush)
 * - Cache pollution (new process data replaces old in cache)
 * - Kernel overhead (scheduler logic)
 *
 * EXPERIMENTS:
 *
 * 1. Compare with system call cost:
 *    Context switches should be 10-100x more expensive
 *
 * 2. Run on different CPUs:
 *    taskset -c 0 ./measure_context_switch
 *
 * 3. Monitor cache misses:
 *    perf stat -e cache-misses ./measure_context_switch
 *
 * 4. Check voluntary vs involuntary:
 *    CPU-bound process → more involuntary
 *    I/O-bound process → more voluntary
 */
