/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#define MAX_BURSTS 10

volatile int simulationRunning = 1;
volatile int activeProcesses = 0;
volatile int fileReadingDone = 0;

typedef struct PCB {
    int id;
    int priority;
    int* cpuBursts;
    int* ioBursts;
    int burstCount;
    int currentBurst;
    struct PCB* next;
    
    // Metrics
    int arrivalTime;
    int completionTime;
    int startTime;
    int totalWaitTime;
    int totalIoTime;
    int lastCpuEndTime;
} PCB;

typedef struct {
    PCB* front;
    PCB* rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

Queue readyQueue, ioQueue;
char* algorithm;
int quantum;
char* inputFileName;

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
}

void enqueue(Queue* q, PCB* pcb) {
    pthread_mutex_lock(&q->mutex);
    pcb->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = pcb;
    } else {
        q->rear->next = pcb;
        q->rear = pcb;
    }
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
}

PCB* dequeue(Queue* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->front == NULL && simulationRunning) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    PCB* temp = q->front;
    if (temp) {
        q->front = q->front->next;
        if (q->front == NULL) {
            q->rear = NULL;
        }
    }
    pthread_mutex_unlock(&q->mutex);
    return temp;
}

void parseArguments(int argc, char* argv[]) {
    int i;
    algorithm = NULL;
    quantum = 30;
    inputFileName = NULL;
    
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-alg") == 0 && i + 1 < argc) {
            algorithm = argv[++i];
        } else if (strcmp(argv[i], "-quantum") == 0 && i + 1 < argc) {
            quantum = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-input") == 0 && i + 1 < argc) {
            inputFileName = argv[++i];
        }
    }

    if (!algorithm || !inputFileName) {
        fprintf(stderr, "Usage: %s -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [file name]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

long getCurrentTimeMillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void* fileReaderThread(void* arg) {
    FILE* file = fopen(inputFileName, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    char line[256]; // Assuming each line won't exceed 255 characters
    int id = 0; // Process ID counter
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "proc", 4) == 0) {
            // Handle process creation (similar to my existing logic)
            
            
            PCB* pcb = parseProcessLine(line, &id); // Implement parseProcessLine to extract process details
            enqueue(&readyQueue, pcb);
        } else if (strncmp(line, "sleep", 5) == 0) {
            int sleepTime;
            sscanf(line, "sleep %d", &sleepTime);
            usleep(sleepTime * 1000); // Convert ms to us and sleep
        } else if (strncmp(line, "stop", 4) == 0) {
            break; // Stop reading the file
        }
    }

    fclose(file);
    fileReadingDone = 1; // Signal the end of file reading
    // Notify other threads possibly waiting on conditions
    pthread_cond_broadcast(&readyQueue.cond);
    pthread_cond_broadcast(&ioQueue.cond);
    return NULL;
}



void* cpuSchedulerThread(void* arg) {
    while (simulationRunning || activeProcesses > 0) {
        PCB* process = NULL;

        if (strcmp(algorithm, "FIFO") == 0) {
            process = dequeue(&readyQueue);
        } else if (strcmp(algorithm, "SJF") == 0) {
            process = selectProcessSJF();
        } else if (strcmp(algorithm, "PR") == 0) {
            process = selectProcessPR(); // Use PR-specific selection
            if (process) {
                printf("Priority Scheduler: Selected Process ID: %d with Priority: %d\n", process->id, process->priority);
            }
        } else if (strcmp(algorithm, "RR") == 0) {
            process = dequeue(&readyQueue); // For RR, simply dequeue the next process
            if (process) {
                int burstTime = process->cpuBursts[process->currentBurst / 2];
                int timeSlice = (quantum < burstTime) ? quantum : burstTime;
                printf("CPU Scheduler (RR): Executing Process ID: %d for %d ms (Quantum: %d ms)\n", process->id, timeSlice, quantum);
                sleep(timeSlice); // Simulate execution

                if (timeSlice < burstTime) {
                    process->cpuBursts[process->currentBurst / 2] -= timeSlice; // Adjust remaining burst time
                    enqueue(&readyQueue, process); // Re-enqueue the process
                } else {
                    process->currentBurst++;
                    if (process->currentBurst < process->burstCount) {
                        enqueue(&ioQueue, process); // More bursts left, move to I/O queue
                    } else {
                        printf("CPU Scheduler (RR): Process ID: %d Completed\n", process->id);
                        activeProcesses--;
                        // Freeing resources and metrics recording for process completion
                    }
                }
                continue; // Skip to the next iteration to prevent executing non-RR logic below
            }
        }

        // Common logic for FIFO, SJF, and PR (non-RR paths)
        if (process) {
            if (process->currentBurst == 0) { // Mark the start time for the first CPU burst
                process->startTime = getCurrentTimeMillis();
            }
            printf("CPU Scheduler: Executing Process ID: %d, CPU Burst: %d ms\n", process->id, process->cpuBursts[process->currentBurst / 2]);
            sleep(process->cpuBursts[process->currentBurst / 2]); // Simulate execution

            process->currentBurst++;
            if (process->currentBurst < process->burstCount) {
                enqueue(&ioQueue, process); // Move to I/O queue if more bursts are pending
            } else {
                process->completionTime = getCurrentTimeMillis(); // Mark completion time
                printf("CPU Scheduler: Process ID: %d Completed\n", process->id);
                activeProcesses--;
                // Free resources and metrics recording for process completion
            }
            } else if (fileReadingDone && readyQueue.front == NULL && ioQueue.front == NULL) {
            // No more processes to handle and file reading is complete
            simulationRunning = 0; // Signal that the simulation can end
        }
    }
    return NULL;
}

void* ioHandlerThread(void* arg) {
    while (simulationRunning || activeProcesses > 0) {
        pthread_mutex_lock(&ioQueue.mutex);
        while (ioQueue.front == NULL && simulationRunning) {
            // Wait for processes to appear in the I/O queue
            pthread_cond_wait(&ioQueue.cond, &ioQueue.mutex);
        }
        PCB* process = dequeue(&ioQueue);
        pthread_mutex_unlock(&ioQueue.mutex);

        if (process) {
            // Simulate I/O operation
            printf("I/O Handler: Processing I/O for Process ID: %d, I/O Burst: %d ms\n", process->id, process->ioBursts[process->currentBurst / 2]);
            sleep(process->ioBursts[process->currentBurst / 2]);

            process->currentBurst++;
            if (process->currentBurst < process->burstCount) {
                // If the process has more CPU bursts, move it back to the ready queue
                pthread_mutex_lock(&readyQueue.mutex);
                enqueue(&readyQueue, process);
                pthread_mutex_unlock(&readyQueue.mutex);
            } else {
                // If the process is complete, update completion time and activeProcesses
                process->completionTime = getCurrentTimeMillis(); // Assuming I have a method to get the current time
                printf("I/O Handler: Process ID: %d I/O Complete and Process Completed\n", process->id);
                // Here, free the process's resources and adjust the active process count
                activeProcesses--;
            }
        }

        if (fileReadingDone && readyQueue.front == NULL && ioQueue.front == NULL && activeProcesses == 0) {
            simulationRunning = 0; // End the simulation if there are no more active processes
        }
    }
    return NULL;
}

void cleanupQueue(Queue* q) {
    PCB* current = q->front;
    while (current != NULL) {
        PCB* next = current->next;
        free(current->cpuBursts);
        free(current->ioBursts);
        free(current);
        current = next;
    }
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->cond);
}

void calculatePerformanceMetrics() {
    // Placeholder values for simulation start and end times
    int simulationStartTime = 0;
    int simulationEndTime = 100; // Assume the simulation lasted 100 units of time

    int totalProcesses = 0; // Initialize total processed count
    int totalCompletionTime = 0;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;

    // Placeholder loop. I need to implement a way to track and iterate over completed processes
    // For now, let's assume these are calculated directly from the processes' data
    PCB* p = NULL; // This needs to point the actual list or collection of completed processes
    while (p != NULL) {
        totalCompletionTime += (p->completionTime - p->arrivalTime);
        totalTurnaroundTime += (p->completionTime - p->arrivalTime);
        //I add a way to calculate waiting time in the PCB struct
        totalWaitingTime += p->totalWaitTime; // This assumes we have a totalWaitTime field
        totalProcesses++;
        p = p->next; // Move to next process in the list
    }

    // Calculate metrics based on placeholder values and printed results
    double cpuUtilization = ((double)totalCompletionTime / (simulationEndTime - simulationStartTime)) * 100.0;
    double throughput = totalProcesses / (double)(simulationEndTime - simulationStartTime);
    double avgTurnaroundTime = totalTurnaroundTime / (double)totalProcesses;
    double avgWaitingTime = totalWaitingTime / (double)totalProcesses;

    printf("CPU Utilization: %.2f%%\n", cpuUtilization);
    printf("Throughput: %.2f processes/unit time\n", throughput);
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime);
}

int main(int argc, char* argv[]) {
    pthread_t fileReaderThread, cpuSchedulerThread, ioHandlerThread;

    parseArguments(argc, argv);
    initQueue(&readyQueue);
    initQueue(&ioQueue);

    if (pthread_create(&fileReaderThread, NULL, fileReaderThread, NULL) != 0) {
        perror("Failed to create fileReader thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&cpuSchedulerThread, NULL, cpuSchedulerThread, NULL) != 0) {
        perror("Failed to create cpuScheduler thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&ioHandlerThread, NULL, ioHandlerThread, NULL) != 0) {
        perror("Failed to create ioHandler thread");
        exit(EXIT_FAILURE);
    }

    pthread_join(fileReaderThread, NULL);
    pthread_join(cpuSchedulerThread, NULL);
    pthread_join(ioHandlerThread, NULL);

    // Cleanup queues
    cleanupQueue(&readyQueue);
    cleanupQueue(&ioQueue);

    // Calculate and print performance metrics
    calculatePerformanceMetrics();

    // Print final output
    printf("Input File Name: %s\n", inputFileName);
    printf("CPU Scheduling Alg: %s", algorithm);
    if (strcmp(algorithm, "RR") == 0) {
        printf(" (%d ms quantum)", quantum);
    }
    printf("\n");

    // Placeholder for actual metrics printing
    printf("CPU utilization: %%...\n");
    printf("Throughput: processes/unit time...\n");
    printf("Avg. Turnaround time: ...\n");
    printf("Avg. Waiting time in R queue: ...\n");

    return 0;
}
