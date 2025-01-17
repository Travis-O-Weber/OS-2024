#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10000

typedef struct {
    double* array;
    int start;
    int end;
    double average;
} Parameters;

void* sorting_avg(void* args);
void* merging_avg(void* args);
void insertionSort(double* array, int start, int end);
double calculate_average(double* array, int start, int end);

int main(int argc, char *argv[]) {
    int i, n; 
    double A[MAX_SIZE];
    Parameters arg1, arg2, merge_args;
    pthread_t tid1, tid2, tid3;
    pthread_attr_t attr;
    clock_t start, end;
    double time_multithread, time_singlethread;

    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n > MAX_SIZE || n <= 0) {
        printf("Please enter a number between 1 and %d.\n", MAX_SIZE);
        return 1;
    }

    srand(time(NULL));
    for (i = 0; i < n; i++) {
        A[i] = 1.0 + (rand() / (RAND_MAX / (1000.0 - 1.0)));
    }

    // Multithreading sort and merge
    start = clock();
    pthread_attr_init(&attr);

    arg1.array = A;
    arg1.start = 0;
    arg1.end = n / 2;
    pthread_create(&tid1, &attr, sorting_avg, &arg1);

    arg2.array = A;
    arg2.start = n / 2;
    arg2.end = n;
    pthread_create(&tid2, &attr, sorting_avg, &arg2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    merge_args.array = A;
    merge_args.start = 0;
    merge_args.end = n;
    pthread_create(&tid3, &attr, merging_avg, &merge_args);
    pthread_join(tid3, NULL);
    end = clock();
    time_multithread = (double)(end - start) / CLOCKS_PER_SEC;

    // Reset array for single-threaded sort
      for (i = 0; i < n; i++) {
        A[i] = 1.0 + (rand() / (RAND_MAX / (1000.0 - 1.0)));
    }

    // Single-threaded sort and average
    start = clock();
    insertionSort(A, 0, n);
    double average = calculate_average(A, 0, n);
    end = clock();
    time_singlethread = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Multithreaded sorting and merging took %f seconds.\n", time_multithread);
    printf("Single-threaded sorting took %f seconds. Average: %f\n", time_singlethread, average);

    return 0;
}

void* sorting_avg(void* args) {
    int i;
    Parameters* p = (Parameters*)args;
    insertionSort(p->array, p->start, p->end);
    
    double sum = 0;
    for (i = p->start; i < p->end; i++) {
        sum += p->array[i];
    }
    p->average = sum / (p->end - p->start);
    pthread_exit(0);
}

void* merging_avg(void* args) {
     Parameters* p = (Parameters*)args;
    int total_elements = p->end - p->start;
    double* temp = (double*)malloc(total_elements * sizeof(double));
    int mid = (p->start + p->end) / 2;
    int i = p->start;
    int j = mid;
    int k = 0;
    double sum = 0;

    // Merge the two sorted sub-arrays into temp[]
    while (i < mid && j < p->end) {
        if (p->array[i] < p->array[j]) {
            temp[k++] = p->array[i++];
        } else {
            temp[k++] = p->array[j++];
        }
    }
    while (i < mid) {
        temp[k++] = p->array[i++];
    }
    while (j < p->end) {
        temp[k++] = p->array[j++];
    }

    // Copy the merged temp[] back into the original array
    for (i = 0; i < total_elements; i++) {
        p->array[p->start + i] = temp[i];
        sum += temp[i];
    }

    // Compute the overall average of the merged array
    p->average = sum / total_elements;
    printf("Merged array and computed overall average: %f\n", p->average);

    free(temp); // Don't forget to free the allocated memory
    pthread_exit(0);
}

void insertionSort(double* array, int start, int end) {
    int i, j;
    double key;
    for (i = start + 1; i < end; i++) {
        key = array[i];
        j = i - 1;

        while (j >= start && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

double calculate_average(double* array, int start, int end) {
    double sum = 0;
    int i; 
    for (i = start; i < end; i++) { 
        sum += array[i];
    }
    return sum / (end - start);
}