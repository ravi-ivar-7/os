# Context Switching

## Learning Objectives

Context switching is the **heart of multitasking**. This is what allows multiple processes to share a single CPU.

### Key Concepts

1. **What is saved/restored during context switch**
2. **When does context switching happen** (timer interrupt, I/O, syscall)
3. **Cost of context switching** (why it's expensive)
4. **Hardware support** (interrupts, privilege levels)
5. **The actual assembly-level mechanics**

## Context Switch Steps

```
1. Timer interrupt fires (hardware)
2. CPU switches to kernel mode
3. Save current process state to PCB:
   - Program counter (PC/IP)
   - Stack pointer
   - General purpose registers
   - Flags
4. Scheduler selects next process
5. Restore next process state from its PCB
6. Switch memory context (page table base register)
7. Return to user mode
8. Resume execution of new process
```

## Why Context Switching is Expensive

- **Direct costs**: Saving/restoring registers (hundreds of CPU cycles)
- **Indirect costs**: Cache pollution (TLB flush, L1/L2/L3 cache misses)
- **Memory bandwidth**: Loading new process data into cache
- Typically: 1-10 microseconds, but cache effects can last much longer

## Exercises

### Exercise 1: Measure Context Switch Cost
Empirically measure how long a context switch takes

### Exercise 2: Understand Timer Interrupts
See how hardware interrupts trigger scheduling

### Exercise 3: Voluntary vs Involuntary Switches
Track when processes give up CPU vs are preempted

### Exercise 4: Implement User-Space Context Switch
Build cooperative multitasking with setjmp/longjmp or ucontext

### Exercise 5: Examine Real Kernel Context Switch
Read Linux kernel source (arch/x86/kernel/process.c)
