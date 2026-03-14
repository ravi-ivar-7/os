# Process Control in Multi-Tasking

How operating systems manage multiple processes. The foundation of everything else in OS.

## What to Build

- Process creation and lifecycle programs
- Context switch cost measurement
- Scheduler simulator (FCFS, SJF, Round Robin, Priority)
- Race condition demonstrations
- Producer-Consumer with semaphores

## Structure

```
01-process-basics/       Process creation, states, zombies
02-context-switching/    Measure context switch cost
03-schedulers/           Implement & compare algorithms + Linux CFS analysis
04-ipc/                  Inter-process communication
05-synchronization/      Race conditions, mutex, semaphores
```

## Learning Path

### Week 1: Process Basics
```bash
cd 01-process-basics
make basics
./fork_basics              # See fork() in action
./process_states cpu       # Observe RUNNING state
./process_states io        # Observe WAITING state
./zombie_orphan zombie     # Create zombie process
```

**Learn**: Process creation, states (NEW/READY/RUNNING/WAITING/TERMINATED), parent-child, zombies/orphans

### Week 2: Context Switching
```bash
cd 02-context-switching
make context
./measure_context_switch   # Measure the cost (1-10 microseconds)
```

**Learn**: What happens during context switch, why it's expensive, voluntary vs involuntary

### Week 3-4: Schedulers (CORE TOPIC)
```bash
cd 03-schedulers
make sched
./scheduler_simulator      # Compare FCFS, SJF, RR, Priority
```

Read: [linux_scheduler_analysis.md](03-schedulers/linux_scheduler_analysis.md) - How Linux CFS actually works

**Learn**: Scheduling algorithms, trade-offs (fairness vs efficiency), why Linux uses CFS

### Week 5-6: Synchronization (CRITICAL)
```bash
cd 05-synchronization
make sync
./race_condition           # See race conditions
./producer_consumer        # Solve with semaphores
```

**Learn**: The problem (race conditions), the solution (mutex/semaphores), classic problems, deadlock

## Quick Reference

### Compile Everything
```bash
make all
```

### Key Programs
```bash
# Process creation
./01-process-basics/fork_basics

# Context switch cost
./02-context-switching/measure_context_switch

# Compare schedulers
./03-schedulers/scheduler_simulator

# Race conditions
./05-synchronization/race_condition

# Producer-Consumer
./05-synchronization/producer_consumer
```

### Observe Real System
```bash
# Process states
ps aux | head -20

# Context switches
cat /proc/$(pgrep firefox)/status | grep ctxt

# Trace system calls
strace -c ls

# Scheduler info
cat /proc/sched_debug
```

## Key Concepts

**Process States**:
```
NEW → READY → RUNNING → WAITING → TERMINATED
         ↑        ↓
         ←--------
```

**Context Switch**: CPU saves current process state, loads next process state
- Cost: 1-10 microseconds + cache pollution
- Triggered by: timer interrupt, I/O, system call

**Schedulers**: Decide which process runs next
- FCFS: Simple, convoy effect
- SJF: Optimal waiting time, starvation
- Round Robin: Fair, context switch overhead
- Linux CFS: Red-black tree, virtual runtime

**Synchronization**: Prevent race conditions
- Problem: Multiple processes access shared data
- Solution: Mutex (mutual exclusion), Semaphores (counting)
- Deadlock: Circular wait for resources

## How to Use

1. **Read** the README in each directory
2. **Study** the code before running
3. **Compile** and run
4. **Answer** the questions in code comments
5. **Experiment** - modify, break, fix
6. **Observe** real systems with tools

## Prerequisites

- C programming (pointers, structs, basic I/O)
- Linux command line
- Patience (OS is hard but rewarding)

## Tools

```bash
# Compile
gcc program.c -o program -lpthread

# Debug
gdb ./program

# Trace syscalls
strace ./program

# Monitor
ps aux | grep program
top
```

## Debugging Tips

```bash
# Compile with debug symbols
gcc -g program.c -o program

# GDB basics
gdb ./program
(gdb) break main
(gdb) run
(gdb) next
(gdb) print variable

# Find memory leaks
valgrind --leak-check=full ./program

# See system calls
strace -c ./program
```

## Learning Goals

After this module, the goals are to be able to:

1. Explain what happens when running `./program`
2. Understand every column in `ps aux` output
3. Know how context switching works and why it's expensive
4. Compare scheduling algorithms (trade-offs)
5. Explain how Linux CFS works
6. Debug race conditions
7. Use mutex and semaphores correctly
8. Identify and prevent deadlock

## Common Questions

**Q: Why does `counter++` cause race conditions?**
A: It's 3 assembly instructions: READ, INCREMENT, WRITE. Another thread can interrupt between them.

**Q: Why is context switching expensive?**
A: Direct cost (save/restore registers) + indirect cost (cache pollution, TLB flush).

**Q: What's the best scheduler?**
A: Depends on workload. Linux CFS is good general-purpose (fairness + good response time).

**Q: Mutex vs Semaphore?**
A: Mutex = binary (lock/unlock). Semaphore = counting (resource management). Use mutex for mutual exclusion.

**Q: How to prevent deadlock?**
A: Break one of 4 conditions: mutual exclusion, hold-and-wait, no preemption, circular wait.

## Next Steps

After mastering process control:
- Memory Management (paging, virtual memory)
- File Systems (inodes, allocation)
- I/O Systems (device drivers, interrupts)

---

**Start**: [01-process-basics/README.md](01-process-basics/README.md)
