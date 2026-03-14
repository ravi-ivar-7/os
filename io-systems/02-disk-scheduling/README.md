# Disk Scheduling Algorithms

Modern block storage devices (like HDDs) have a mechanical arm with a magnetic read/write head that must physically move to the correct "cylinder" (track) to read data. This movement is called a **seek**.

Seek time is often the slowest part of disk I/O (measured in milliseconds). 

Disk scheduling algorithms attempt to order pending read/write requests to minimize the total head movement, improving overall throughput.

## Common Algorithms

### 1. FCFS (First-Come, First-Served)
Requests are serviced exactly in the order they arrive.
- **Pros**: Totally fair. No starvation.
- **Cons**: Extremely inefficient. The head might thrash back and forth across the entire disk repeatedly.

### 2. SSTF (Shortest Seek Time First)
Select the request that is closest to the current head position.
- **Pros**: Minimizes immediate head movement. Much better throughput than FCFS.
- **Cons**: Severe starvation risk. If requests keep arriving near the current head position, requests far away might wait forever.

### 3. SCAN (The Elevator Algorithm)
The head moves in one continuous direction (e.g., towards track 0), servicing all requests in its path. Once it hits the end, it reverses direction and sweeps back.
- **Pros**: Fairer than SSTF, bounded wait times.
- **Cons**: When it turns around at the edge, the requests at the very *other* end of the disk have been waiting the longest, but they are serviced last on the return trip.

### 4. C-SCAN (Circular SCAN)
Similar to SCAN, but when the head reaches the end of the disk, it immediately jumps all the way back to the beginning (without servicing requests on the trip back) and starts sweeping in the same direction again.
- **Pros**: Provides a more uniform wait time than SCAN, treating the disk like a continuous cylinder. The jump back is typically very fast compared to a slow sweep.

## Run Instructions

```bash
gcc disk_sim.c -o disk_sim
./disk_sim
```

Observe the "Total Seek Time" for a provided array of randomized cylinder requests. FCFS is typically the worst, while SSTF/SCAN offer significant optimizations.
