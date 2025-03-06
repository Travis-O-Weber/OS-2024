# Virtual Memory Management Simulation

## Overview
This project simulates virtual memory management using a page table. It consists of two parts:

- **Part 1**: Implements fixed page-to-frame mapping.
- **Part 2**: Implements dynamic paging with an LRU (Least Recently Used) page replacement algorithm.

## Files in the Project

- **`part1.c`**: Implements fixed page mapping translation.
- **`part2.c`**: Implements paging with LRU replacement.
- **`Makefile`**: Used to compile the programs.
- **`part1sequence`** & **`part2sequence`**: Input test sequences.
- **`Report.txt`**: Contains observations and MD5 checksums of output files.

## Function Descriptions

### `part1.c`
This program translates logical addresses to physical addresses using a predefined page table.

#### **Functions**
- **`void initializePageTable()`**
  - Initializes a static page-to-frame mapping.
- **`unsigned long translateAddress(unsigned long logicalAddress)`**
  - Translates a logical address to a physical address using the page table.
- **`int main(int argc, char *argv[])`**
  - Reads input addresses, translates them, and writes the output.

### `part2.c`
This program simulates virtual memory with LRU page replacement.

#### **Functions**
- **`void initialize()`**
  - Initializes the page table, frame tracking, and LRU counters.
- **`unsigned long translateAddress(unsigned long logicalAddress, int* pageFaultCount)`**
  - Translates logical to physical addresses and handles page faults.
- **`int findFreeFrameOrReplace()`**
  - Finds a free frame or replaces the least recently used frame.
- **`int main(int argc, char *argv[])`**
  - Reads input addresses, translates them, counts page faults, and writes output.

## How to Compile
```sh
make
```

## How to Run
### Fixed Mapping (Part 1)
```sh
./part1 part1sequence part1-out-test
```

### LRU Paging (Part 2)
```sh
./part2 part2sequence part2-out-test
```

## Expected Output
The programs generate binary output files containing translated addresses. The report provides MD5 checksums for validation:
```
Part1 MD5SUM:
d41d8cd98f00b204e9800998ecf8427e  part1-output
Part2 MD5SUM:
d41d8cd98f00b204e9800998ecf8427e  part2-output
```

## Notes
- **Part 1** implements static page mapping, ensuring valid pages are mapped.
- **Part 2** dynamically assigns frames and replaces the least recently used page.
- The **MD5SUM** output in `Report.txt` helps verify correctness.
- The project highlights the impact of LRU replacement and limited frame availability.
