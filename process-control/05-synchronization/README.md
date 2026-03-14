# Process Synchronization

## The Core Problem

**Race Condition**: Multiple processes access shared data concurrently, and the outcome depends on execution order.

```c
// Shared variable
int counter = 0;

// Two processes both do:
counter++;  // NOT ATOMIC! Actually 3 steps:
            // 1. Read counter
            // 2. Increment
            // 3. Write back

// Result: Lost updates!
```

## Critical Section Problem

**Critical Section**: Code that accesses shared resource

Requirements:
1. **Mutual Exclusion**: Only one process in critical section
2. **Progress**: If no one is in CS, someone waiting should get in
3. **Bounded Waiting**: No starvation

## Synchronization Primitives

### 1. Mutex (Mutual Exclusion Lock)
```c
pthread_mutex_lock(&mutex);
// Critical section
pthread_mutex_unlock(&mutex);
```

### 2. Semaphore
- Integer variable + atomic operations
- **Binary semaphore** (mutex-like)
- **Counting semaphore** (resource counting)

```c
sem_wait(&sem);   // P() operation, decrement
// Critical section
sem_post(&sem);   // V() operation, increment
```

### 3. Condition Variables
- Wait for specific condition
- Used with mutex

```c
pthread_cond_wait(&cond, &mutex);
pthread_cond_signal(&cond);
```

## Classic Synchronization Problems

### 1. Producer-Consumer (Bounded Buffer)
- Producers add items to buffer
- Consumers remove items
- Buffer has limited size
- **Challenge**: Don't overflow/underflow, coordinate access

### 2. Readers-Writers
- Multiple readers can read simultaneously
- Writers need exclusive access
- **Challenge**: Reader/writer priority, starvation

### 3. Dining Philosophers
- 5 philosophers, 5 forks
- Need 2 forks to eat
- **Challenge**: Deadlock, starvation

## Deadlock

Four necessary conditions (all must hold):
1. **Mutual Exclusion**: Resources can't be shared
2. **Hold and Wait**: Process holds resource while waiting for another
3. **No Preemption**: Resources can't be forcibly taken
4. **Circular Wait**: Circular chain of processes waiting for resources

**Prevention**: Break one of the four conditions

## What to Implement

1. Race conditions (demonstrate the problem)
2. Mutex-based solutions
3. Semaphore-based solutions
4. Producer-Consumer
5. Readers-Writers
6. Dining Philosophers
7. Deadlock demonstration and prevention
