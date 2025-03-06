# Process Scheduling Simulation

## Overview
This project simulates a CPU scheduling system with multiple scheduling algorithms using multithreading in C. It processes an input file describing processes with CPU and I/O bursts, handles scheduling via different algorithms, and computes performance metrics.

## Files in the Project

- **`assign3.c`**: The main C program implementing the process scheduling simulation.
- **`Report.txt`**: A report documenting collaboration, correctness, issues faced, and challenges.

## Function Descriptions

### `assign3.c`
This file implements a multithreaded simulation for process scheduling. The system supports different CPU scheduling algorithms: FIFO, SJF, PR, and RR with a configurable quantum.

#### **Functions**

- **`void initQueue(Queue* q)`**
  - Initializes a queue with a mutex and condition variable.

- **`void enqueue(Queue* q, PCB* pcb)`**
  - Adds a process to the queue while handling synchronization.

- **`PCB* dequeue(Queue* q)`**
  - Removes and returns the front process from the queue.

- **`void parseArguments(int argc, char* argv[])`**
  - Parses command-line arguments to set the scheduling algorithm, quantum, and input file.

- **`long getCurrentTimeMillis()`**
  - Returns the current time in milliseconds.

- **`void* fileReaderThread(void* arg)`**
  - Reads and parses the input file to create processes and populate the ready queue.

- **`void* cpuSchedulerThread(void* arg)`**
  - Implements the CPU scheduling logic based on the chosen algorithm.
  - Uses FIFO, SJF, PR (priority scheduling), or RR (round-robin scheduling).

- **`void* ioHandlerThread(void* arg)`**
  - Simulates I/O processing for processes.
  - Moves processes back to the ready queue when I/O is complete.

- **`void cleanupQueue(Queue* q)`**
  - Frees allocated memory and destroys synchronization objects.

- **`void calculatePerformanceMetrics()`**
  - Computes CPU utilization, throughput, average turnaround time, and average waiting time.

- **`int main(int argc, char* argv[])`**
  - Initializes queues and threads for file reading, CPU scheduling, and I/O handling.
  - Waits for all threads to complete before computing performance metrics.

## How to Compile
```sh
gcc -pthread -o assign3 assign3.c
```

## How to Run
```sh
./assign3 -alg <FIFO|SJF|PR|RR> [-quantum <time>] -input <filename>
```
Example:
```sh
./assign3 -alg RR -quantum 50 -input processes.txt
```

## Expected Output
```
CPU Scheduler: Executing Process ID: 3, CPU Burst: 40 ms
CPU Scheduler: Process ID: 3 Completed
I/O Handler: Processing I/O for Process ID: 2, I/O Burst: 20 ms
...
CPU Utilization: 85.67%
Throughput: 3.2 processes/unit time
Average Turnaround Time: 125.4
Average Waiting Time: 40.2
```

## Notes
- The program supports FIFO, SJF, PR, and RR scheduling.
- Multithreading is used to parallelize file reading, CPU scheduling, and I/O processing.
- Report.txt documents issues and challenges faced during the assignment.
