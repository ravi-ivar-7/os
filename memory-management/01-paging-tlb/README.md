# Paging and TLB Simulation

A simulation of how standard hardware translates virtual addresses to physical addresses using a Page Table and a Translation Lookaside Buffer (TLB).

## Core Concepts

- **Virtual Address**: The address the process sees. Divided into a Page Number and an Offset.
- **Physical Address**: The actual RAM address. Divided into a Frame Number and an Offset.
- **Page Table**: Array mapping Page Numbers to Frame Numbers.
- **TLB (Translation Lookaside Buffer)**: Hardware cache for the Page Table. Faster, but tiny.

## The Simulation Workflow

1. A virtual address is generated.
2. The TLB is checked first.
   - **TLB Hit**: The frame is found instantly.
   - **TLB Miss**: The CPU must access main memory to check the Page Table.
3. The Page Table is checked.
   - **Page Hit**: The frame is found in the table. The TLB is updated.
   - **Page Fault**: The page is not in RAM (not implemented in this specific simulation, assumes all pages are loaded).
4. Physical address is calculated as `(Frame Number * Page Size) + Offset`.

## Run Instructions

```bash
gcc paging_sim.c -o paging_sim
./paging_sim
```

Observe the TLB hit rate as the program accesses contiguous memory versus random memory. Contiguous access pattern usually causes higher TLB hit rates due to spatial locality.
