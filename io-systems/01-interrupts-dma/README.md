# Interrupts, Polling, and DMA

The CPU is incredibly fast compared to hardware devices (disks, network cards, keyboards). How should the CPU interact with them without wasting time?

## 1. Polling (Programmed I/O)

The simplest approach. The CPU issues an I/O command and then enters a tight loop, constantly checking the device's status register until the operation is complete.

- **Pros**: Easy to implement. Good for very fast devices where the wait time is minimal.
- **Cons**: Extremely inefficient for slow devices (like hard drives). The CPU wastes billions of cycles doing no useful work while waiting.

## 2. Interrupt-Driven I/O

The CPU issues the command and immediately goes to do other work (like running another process). When the device finishes, it sends a hardware interrupt signal to the CPU over the system bus.

- **Pros**: CPU cycles are not wasted. Multitasking is possible.
- **Cons**: For large data transfers, an interrupt is triggered for *every single byte or word* transferred, which adds massive overhead.

## 3. Direct Memory Access (DMA)

For bulk data transfers (like reading a 5MB file from a disk), the CPU offloads the work to a dedicated hardware component called the DMA Controller.

1. CPU tells the DMA controller: "Read block X from disk and place it at RAM address Y, length Z."
2. CPU goes to do other work.
3. The DMA controller takes control of the system bus and transfers the data directly from the device to RAM, bypassing the CPU entirely.
4. When all Z bytes are transferred, the DMA controller sends *one* interrupt to the CPU.

- **Pros**: Frees the CPU during massive transfers. Only one interrupt per large block.
- **Cons**: Requires specialized hardware. DMA controller occasionally "steals" bus cycles from the CPU to write to RAM.

## Run Instructions

```bash
gcc io_sim.c -o io_sim -pthread
./io_sim
```

Observe how many "useful work units" the simulated CPU can accomplish while waiting for the hardware in the different modes.
