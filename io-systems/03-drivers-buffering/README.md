# Device Drivers & Buffering

Hardware devices operate with vastly different timings, failure modes, and data representations. Operating systems abstract this complexity behind a common interface using **Device Drivers**.

## The Problem: Speed Mismatch

A modern CPU operates at nanosecond speeds (billions of cycles per second).
A spinning hard drive operates at millisecond speeds (thousands of operations per second).

If a process asks to read data from a disk and processes it byte-by-byte as it arrives, the CPU must stop and wait for the mechanical disk between every operation.

## 1. Single Buffering

Instead of byte-by-byte transfer, the OS reserves a block of RAM called a **buffer**.
1. The hardware streams an entire block of data into the single buffer.
2. The hardware signals an interrupt.
3. The CPU wakes up, processes the full buffer, and empties it.
4. The cycle repeats.

- **Con**: While the CPU is processing the buffer, the hardware device must sit idle, because it has nowhere to write new incoming data.

## 2. Double Buffering

The OS reserves *two* blocks of RAM (Buffer A and Buffer B).
1. The hardware starts streaming data into Buffer A.
2. Once A is full, the hardware switches to streaming data into Buffer B immediately.
3. While the hardware fills B, the CPU wakes up and processes the data in Buffer A.
4. When B is full, the hardware switches back to A (which the CPU just finished emptying), and the CPU switches to B.

- **Pro**: True concurrency. The hardware never stops transferring data (assuming CPU processing speed >= hardware transfer speed), maximizing throughput.

## Circular Buffering

An extension of double buffering where multiple (N) buffers are arranged in a ring. Common in continuous streaming applications like audio processing or network packet capture, where brief periods of high latency might cause one buffer to overflow if strict double-buffering is used.

## Run Instructions

```bash
gcc buffer_sim.c -o buffer_sim -pthread
./buffer_sim
```

Observe exactly how the simulated "Device" and "CPU" take turns waiting in Single Buffering mode vs how they overlap perfectly in Double Buffering mode.
