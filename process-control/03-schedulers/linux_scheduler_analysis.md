# Understanding Linux's Completely Fair Scheduler (CFS)

## Why CFS Exists

Traditional schedulers like Round Robin have problems:
- **Fixed time quantum**: One size doesn't fit all
- **Priority inversion**: High priority process blocked by low priority
- **Poor interactivity**: Can't distinguish interactive vs batch processes

Linux CFS (since 2.6.23) solves these issues.

## Core Concept: Virtual Runtime

Instead of tracking time slices, CFS tracks **virtual runtime** (`vruntime`).

```
vruntime = actual_runtime × (NICE_0_LOAD / process_weight)
```

- Low priority (nice +19) → higher vruntime growth
- High priority (nice -20) → lower vruntime growth

**Scheduler picks process with lowest vruntime**

## The Red-Black Tree

CFS uses a red-black tree to organize processes:

```
              [vruntime=50]
              /            \
    [vruntime=30]        [vruntime=80]
       /      \             /      \
  [vr=10]  [vr=40]    [vr=60]  [vr=90]
```

- Leftmost node = next process to run
- O(log n) insertion/deletion
- Self-balancing

## Scheduling Decision

```c
// Simplified CFS logic
pick_next_task() {
    // Get leftmost node (lowest vruntime)
    task = rb_tree_leftmost();

    // Calculate time slice based on number of runnable tasks
    timeslice = sched_period / nr_running;

    // Min timeslice to avoid excessive context switching
    if (timeslice < min_granularity)
        timeslice = min_granularity;

    return task;
}

update_curr() {
    // After task runs, update its vruntime
    delta_exec = now - task->exec_start;
    task->vruntime += calc_delta_fair(delta_exec, task);

    // Reinsert into tree at new position
    reposition_task_in_tree(task);
}
```

## Key Parameters

```bash
# See CFS tunables
cat /proc/sys/kernel/sched_latency_ns         # 6ms default
cat /proc/sys/kernel/sched_min_granularity_ns  # 0.75ms default
cat /proc/sys/kernel/sched_wakeup_granularity_ns
```

- **sched_latency**: Target for how long all tasks should run once
- **sched_min_granularity**: Minimum time slice (avoid thrashing)

## Example Scenario

3 processes with different nice values:

| Process | Nice | Weight | Actual Time | vruntime Growth |
|---------|------|--------|-------------|-----------------|
| A       | 0    | 1024   | 10ms        | 10ms            |
| B       | +5   | 335    | 10ms        | 30ms            |
| C       | -5   | 3121   | 10ms        | 3.3ms           |

After 10ms each:
- Process C (vruntime=3.3) runs next
- Process A (vruntime=10) runs second
- Process B (vruntime=30) runs last

Over time, they get CPU proportional to weight → **fairness**

## Handling I/O-bound Processes

Problem: Process sleeps (I/O), wakes up with very low vruntime, monopolizes CPU

Solution: On wakeup, set vruntime to minimum in tree (with small penalty)

```c
place_entity() {
    if (task_was_sleeping) {
        // Don't let it starve CPU-bound processes
        vruntime = max(task->vruntime, min_vruntime - threshold);
    }
}
```

## Comparing to Round Robin

| Aspect              | Round Robin         | CFS                      |
|---------------------|---------------------|--------------------------|
| Time quantum        | Fixed               | Dynamic (based on load)  |
| Fairness            | Equal time          | Weighted fairness        |
| Priority handling   | Separate queues     | Weight affects vruntime  |
| Complexity          | O(1)                | O(log n)                 |
| Starvation          | Possible with prio  | Prevented                |
| Interactivity       | Poor                | Good (sleep bonus)       |

## Observing CFS in Action

### 1. Check current scheduler
```bash
cat /sys/block/sda/queue/scheduler
# Should show [mq-deadline] or similar for I/O
# CPU scheduling is always CFS for SCHED_NORMAL

cat /proc/sched_debug
# Shows detailed scheduler state
```

### 2. Monitor vruntime
```bash
# Create CPU-bound process
stress --cpu 1 &

# Check scheduler stats
cat /proc/$(pgrep stress)/sched
# Look for se.vruntime
```

### 3. Test nice values
```bash
# Start 3 CPU-bound processes with different priorities
nice -n -10 stress --cpu 1 &  # High priority
nice -n 0 stress --cpu 1 &    # Normal
nice -n 10 stress --cpu 1 &   # Low priority

# Monitor CPU usage
top -p $(pgrep stress | tr '\n' ',' | sed 's/,$//')
# High priority gets ~75%, Normal ~20%, Low ~5%
```

### 4. See context switches
```bash
# Watch vruntime and context switches
watch -n 1 'cat /proc/$(pgrep stress | head -1)/sched | grep -E "vruntime|nr_switches"'
```

## Real-Time Scheduling

CFS is for `SCHED_NORMAL`. Linux also has:

- **SCHED_FIFO**: Real-time, first-in-first-out
- **SCHED_RR**: Real-time, round-robin
- **SCHED_DEADLINE**: Real-time with deadlines

```c
#include <sched.h>

struct sched_param param;
param.sched_priority = 99;  // Max priority

// Set real-time scheduling
sched_setscheduler(0, SCHED_FIFO, &param);
```

Real-time processes always preempt CFS processes.

## Why CFS Wins

1. **Fairness**: Guaranteed proportional share
2. **Scalability**: O(log n) is fine even with 10,000 processes
3. **No starvation**: All processes eventually run
4. **Self-tuning**: Adapts to number of runnable tasks
5. **Good interactivity**: I/O-bound tasks feel responsive

## Implementation Details

Source code location: `kernel/sched/fair.c` (Linux kernel)

Key functions:
- `pick_next_task_fair()`: Select next process
- `update_curr()`: Update vruntime after execution
- `enqueue_task_fair()`: Add process to red-black tree
- `dequeue_task_fair()`: Remove from tree

## Further Reading

```bash
# Read the original CFS design document
# https://www.kernel.org/doc/Documentation/scheduler/sched-design-CFS.txt

# Explore kernel source
git clone https://github.com/torvalds/linux.git
cd linux/kernel/sched
less fair.c
```

## Exercises

1. **Measure fairness**:
   - Run 10 CPU-bound processes
   - Check if they get equal CPU over time

2. **Test nice values**:
   - Vary nice values from -20 to +19
   - Measure actual CPU percentages
   - Compare to theoretical weights

3. **Compare with other OS**:
   - Windows: Multilevel Feedback Queue
   - FreeBSD: ULE scheduler
   - macOS: Mach scheduler

4. **Simulate CFS**:
   - Implement simplified CFS
   - Use binary search tree
   - Track vruntime
   - Compare to Round Robin performance

## Key Takeaway

**CFS doesn't try to predict the future** (like SJF). It just ensures fairness by tracking the past (vruntime). Simple, elegant, effective.

This is why Linux is used everywhere from embedded systems to supercomputers - the scheduler adapts to any workload.
