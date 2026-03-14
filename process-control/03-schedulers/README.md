# Process Schedulers

## The Core of Multi-Tasking

The scheduler decides **which process runs next**. This is one of the most critical OS components.

## Scheduling Goals (Trade-offs!)

1. **CPU Utilization**: Keep CPU busy
2. **Throughput**: Maximize processes completed per time unit
3. **Turnaround Time**: Minimize time from submission to completion
4. **Waiting Time**: Minimize time in ready queue
5. **Response Time**: Minimize time to first response (interactive systems)
6. **Fairness**: All processes get fair share

**It is impossible to optimize all of these!** Different schedulers make different trade-offs.

## Scheduling Algorithms

### 1. First-Come, First-Served (FCFS)
- Simplest algorithm
- Non-preemptive
- **Problem**: Convoy effect (short processes wait for long ones)

### 2. Shortest Job First (SJF)
- Run shortest process first
- Optimal average waiting time
- **Problem**: Starvation (long processes may never run)
- **Problem**: Need to predict future CPU burst

### 3. Round Robin (RR)
- Each process gets time slice (quantum)
- Preemptive FCFS
- **Problem**: Context switch overhead if quantum too small
- **Problem**: Poor response time if quantum too large

### 4. Priority Scheduling
- Each process has priority
- Run highest priority first
- **Problem**: Starvation (aging can fix this)

### 5. Multi-Level Feedback Queue (MLFQ)
- Multiple queues with different priorities
- Processes move between queues based on behavior
- Used by most modern OS (with variations)

### 6. Completely Fair Scheduler (CFS)
- Linux's current scheduler
- Red-black tree of processes
- Tracks "virtual runtime"
- Guarantees fair CPU time

## What to Build

1. **Simulator**: Implement all algorithms with metrics
2. **User-space scheduler**: Cooperative multitasking
3. **Analysis tools**: Compare algorithm performance
4. **Real-world**: Understand Linux CFS

## Key Questions

1. Why does Linux use CFS instead of Round Robin?
2. How to schedule real-time processes vs interactive processes?
3. What's the right time quantum for Round Robin?
4. How to prevent starvation?
5. How to handle priority inversion?
