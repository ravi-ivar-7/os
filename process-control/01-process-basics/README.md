# Process Basics

## Learning Objectives

1. What is a process at the OS level
2. How processes are created and destroyed
3. Process states and transitions
4. The Process Control Block (PCB)
5. Parent-child relationships

## Theory (The Essentials)

### Process States
```
NEW → READY → RUNNING → WAITING → TERMINATED
         ↑        ↓
         ←--------
```

- **NEW**: Process being created
- **READY**: Waiting for CPU
- **RUNNING**: Executing on CPU
- **WAITING**: Waiting for I/O or event
- **TERMINATED**: Finished execution

### Process Control Block (PCB)
The OS maintains a PCB for each process containing:
- Process ID (PID)
- Process state
- Program counter
- CPU registers
- Memory management info (page tables)
- I/O status
- Scheduling info (priority, time quantum)

## Exercises

### Exercise 1: Process Creation and States
Understand fork(), exec(), and wait()

See: `01_fork_basics.c`

### Exercise 2: Observe Process States
Use `/proc` filesystem to see real process states

See: `02_process_states.c`

### Exercise 3: Process Tree
Understand parent-child relationships and process hierarchy

See: `03_process_tree.c`

### Exercise 4: Zombie and Orphan Processes
What happens when parent dies? When child is not reaped?

See: `04_zombie_orphan.c`

### Exercise 5: Simulate a PCB
Build a simplified PCB structure

See: `05_pcb_simulator.c`

## Build and Run

```bash
gcc 01_fork_basics.c -o fork_basics
./fork_basics

# For observing states
gcc 02_process_states.c -o process_states
./process_states &
ps aux | grep process_states
cat /proc/$(pgrep process_states)/status
```

## Key Insights to Gain

1. fork() creates a copy - child gets duplicate address space
2. After fork(), parent and child are separate processes
3. exec() replaces process image - this is how programs actually run
4. Zombie processes exist when parent hasn't wait()ed
5. Orphan processes get adopted by init (PID 1)
