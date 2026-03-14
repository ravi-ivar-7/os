# Inter-Process Communication (IPC)

## Why IPC Matters

Processes are isolated by default (separate address spaces). IPC allows them to:
- **Share data**
- **Coordinate actions**
- **Send signals**

## IPC Mechanisms

### 1. Pipes
- Unidirectional byte stream
- `pipe()` for related processes
- `mkfifo()` for named pipes (unrelated processes)
- **Use case**: Shell pipelines (ls | grep)

### 2. Message Queues
- Structured messages
- Multiple readers/writers
- Message priorities
- **Use case**: Producer-consumer patterns

### 3. Shared Memory
- Fastest IPC (no kernel copying)
- Requires synchronization
- **Use case**: High-throughput data sharing

### 4. Sockets
- Network-capable IPC
- Unix domain sockets for local IPC
- **Use case**: Client-server, distributed systems

### 5. Signals
- Asynchronous notifications
- Limited data (just signal number)
- **Use case**: Process control (kill, interrupt)

## Key Concepts

- **Blocking vs Non-blocking**
- **Buffering**
- **Race conditions** (need synchronization!)
- **Producer-Consumer problem**

## Exercises

Each mechanism with practical examples and gotchas.
