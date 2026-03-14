/**
 * Process Scheduler Simulator
 *
 * Implements and compares multiple scheduling algorithms:
 * - First-Come, First-Served (FCFS)
 * - Shortest Job First (SJF)
 * - Round Robin (RR)
 * - Priority Scheduling
 *
 * Metrics: Average waiting time, turnaround time, response time
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESSES 100
#define TIME_QUANTUM 4  // For Round Robin

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;

    // Computed values
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int first_run_time;
    bool started;
} Process;

typedef struct {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
} Metrics;

void reset_processes(Process procs[], int n, Process original[], int orig_n) {
    memcpy(procs, original, orig_n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].started = false;
        procs[i].first_run_time = -1;
    }
}

Metrics calculate_metrics(Process procs[], int n) {
    Metrics m = {0};
    for (int i = 0; i < n; i++) {
        m.avg_waiting_time += procs[i].waiting_time;
        m.avg_turnaround_time += procs[i].turnaround_time;
        m.avg_response_time += procs[i].response_time;
    }
    m.avg_waiting_time /= n;
    m.avg_turnaround_time /= n;
    m.avg_response_time /= n;
    return m;
}

void print_metrics(const char *algo_name, Metrics m) {
    printf("\n=== %s ===\n", algo_name);
    printf("Average Waiting Time:     %.2f\n", m.avg_waiting_time);
    printf("Average Turnaround Time:  %.2f\n", m.avg_turnaround_time);
    printf("Average Response Time:    %.2f\n", m.avg_response_time);
}

// FCFS: Non-preemptive, run processes in arrival order
Metrics fcfs(Process procs[], int n) {
    int current_time = 0;

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procs[j].arrival_time > procs[j + 1].arrival_time) {
                Process temp = procs[j];
                procs[j] = procs[j + 1];
                procs[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (current_time < procs[i].arrival_time) {
            current_time = procs[i].arrival_time;
        }

        procs[i].response_time = current_time - procs[i].arrival_time;
        current_time += procs[i].burst_time;
        procs[i].completion_time = current_time;
        procs[i].turnaround_time = procs[i].completion_time - procs[i].arrival_time;
        procs[i].waiting_time = procs[i].turnaround_time - procs[i].burst_time;
    }

    return calculate_metrics(procs, n);
}

// SJF: Non-preemptive, run shortest job first
Metrics sjf(Process procs[], int n) {
    int current_time = 0;
    int completed = 0;
    bool is_completed[MAX_PROCESSES] = {false};

    while (completed < n) {
        int shortest = -1;
        int min_burst = 999999;

        // Find shortest job that has arrived
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && procs[i].arrival_time <= current_time) {
                if (procs[i].burst_time < min_burst) {
                    min_burst = procs[i].burst_time;
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            current_time++;
            continue;
        }

        procs[shortest].response_time = current_time - procs[shortest].arrival_time;
        current_time += procs[shortest].burst_time;
        procs[shortest].completion_time = current_time;
        procs[shortest].turnaround_time = procs[shortest].completion_time - procs[shortest].arrival_time;
        procs[shortest].waiting_time = procs[shortest].turnaround_time - procs[shortest].burst_time;
        is_completed[shortest] = true;
        completed++;
    }

    return calculate_metrics(procs, n);
}

// Round Robin: Preemptive, time quantum
Metrics round_robin(Process procs[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;

    // Queue simulation (simple array)
    int queue[MAX_PROCESSES * 10];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROCESSES] = {false};

    // Add processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (procs[i].arrival_time <= current_time) {
            queue[rear++] = i;
            in_queue[i] = true;
        }
    }

    while (completed < n) {
        if (front == rear) {
            current_time++;
            // Add newly arrived processes
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && procs[i].arrival_time <= current_time && procs[i].remaining_time > 0) {
                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }
            continue;
        }

        int idx = queue[front++];
        in_queue[idx] = false;

        if (!procs[idx].started) {
            procs[idx].response_time = current_time - procs[idx].arrival_time;
            procs[idx].started = true;
        }

        int exec_time = (procs[idx].remaining_time > quantum) ? quantum : procs[idx].remaining_time;
        procs[idx].remaining_time -= exec_time;
        current_time += exec_time;

        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && procs[i].arrival_time <= current_time && procs[i].remaining_time > 0 && i != idx) {
                queue[rear++] = i;
                in_queue[i] = true;
            }
        }

        if (procs[idx].remaining_time > 0) {
            queue[rear++] = idx;
            in_queue[idx] = true;
        } else {
            procs[idx].completion_time = current_time;
            procs[idx].turnaround_time = procs[idx].completion_time - procs[idx].arrival_time;
            procs[idx].waiting_time = procs[idx].turnaround_time - procs[idx].burst_time;
            completed++;
        }
    }

    return calculate_metrics(procs, n);
}

// Priority Scheduling: Non-preemptive
Metrics priority_scheduling(Process procs[], int n) {
    int current_time = 0;
    int completed = 0;
    bool is_completed[MAX_PROCESSES] = {false};

    while (completed < n) {
        int highest = -1;
        int max_priority = -1;

        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && procs[i].arrival_time <= current_time) {
                if (procs[i].priority > max_priority) {
                    max_priority = procs[i].priority;
                    highest = i;
                }
            }
        }

        if (highest == -1) {
            current_time++;
            continue;
        }

        procs[highest].response_time = current_time - procs[highest].arrival_time;
        current_time += procs[highest].burst_time;
        procs[highest].completion_time = current_time;
        procs[highest].turnaround_time = procs[highest].completion_time - procs[highest].arrival_time;
        procs[highest].waiting_time = procs[highest].turnaround_time - procs[highest].burst_time;
        is_completed[highest] = true;
        completed++;
    }

    return calculate_metrics(procs, n);
}

int main() {
    // {pid, arrival_time, burst_time, priority}
    Process original_procs[] = {
        {.pid=1, .arrival_time=0, .burst_time=8, .priority=2},
        {.pid=2, .arrival_time=1, .burst_time=4, .priority=1},
        {.pid=3, .arrival_time=2, .burst_time=9, .priority=3},
        {.pid=4, .arrival_time=3, .burst_time=5, .priority=2},
        {.pid=5, .arrival_time=4, .burst_time=2, .priority=4}
    };
    int n = sizeof(original_procs) / sizeof(original_procs[0]);

    printf("=== Process Scheduler Simulator ===\n");
    printf("\nWorkload:\n");
    printf("PID  Arrival  Burst  Priority\n");
    for (int i = 0; i < n; i++) {
        printf("%-4d %-8d %-6d %-8d\n",
               original_procs[i].pid,
               original_procs[i].arrival_time,
               original_procs[i].burst_time,
               original_procs[i].priority);
    }

    Process procs[MAX_PROCESSES];
    Metrics m;

    // Test FCFS
    reset_processes(procs, n, original_procs, n);
    m = fcfs(procs, n);
    print_metrics("First-Come, First-Served (FCFS)", m);

    // Test SJF
    reset_processes(procs, n, original_procs, n);
    m = sjf(procs, n);
    print_metrics("Shortest Job First (SJF)", m);

    // Test Round Robin
    reset_processes(procs, n, original_procs, n);
    m = round_robin(procs, n, TIME_QUANTUM);
    printf("\n=== Round Robin (Quantum = %d) ===\n", TIME_QUANTUM);
    printf("Average Waiting Time:     %.2f\n", m.avg_waiting_time);
    printf("Average Turnaround Time:  %.2f\n", m.avg_turnaround_time);
    printf("Average Response Time:    %.2f\n", m.avg_response_time);

    // Test Priority
    reset_processes(procs, n, original_procs, n);
    m = priority_scheduling(procs, n);
    print_metrics("Priority Scheduling", m);

    printf("\n=== Analysis ===\n");
    printf("Best for waiting time:      Shortest Job First\n");
    printf("Best for response time:     Round Robin (small quantum)\n");
    printf("Most fair:                  Round Robin\n");
    printf("Risk of starvation:         SJF, Priority\n");
    printf("Context switch overhead:    Round Robin (higher)\n");

    return 0;
}

/**
 * KEY INSIGHTS:
 *
 * FCFS:
 * - Simple but suffers from convoy effect
 * - Long process blocks all others
 * - Good for batch systems, bad for interactive
 *
 * SJF:
 * - Optimal average waiting time
 * - But impossible to know burst time in advance
 * - Starvation of long processes
 *
 * Round Robin:
 * - Fair, good response time
 * - Quantum trade-off:
 *   - Too small → excessive context switches
 *   - Too large → degenerates to FCFS
 * - Typical quantum: 10-100ms
 *
 * Priority:
 * - Important processes first
 * - Starvation problem (solved with aging)
 * - Used for real-time systems
 *
 * EXPERIMENTS:
 *
 * 1. Try different workloads:
 *    - All CPU-bound vs mixed I/O
 *    - Uniform burst times vs varied
 *
 * 2. Vary time quantum for RR:
 *    - Compare quantum = 1, 4, 10, 100
 *
 * 3. Add preemptive versions:
 *    - Shortest Remaining Time First (SRTF)
 *    - Preemptive Priority
 *
 * 4. Implement Multi-Level Feedback Queue
 */
