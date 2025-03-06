# Multithreaded Sorting Project

## Overview
This project implements a multithreaded sorting application in both C and Java. The goal is to compare the performance of multithreaded sorting and merging against a single-threaded approach.

## Files in the Project

- **`multithreaded_sorting.c`**: A C program that sorts an array using multithreading and calculates the execution time.
- **`MultithreadedSortingApplication.java`**: A Java program implementing the same sorting and merging techniques.
- **`Makefile`**: A script for compiling the C and Java programs.
- **`report.txt`**: A report summarizing the results and observations.

## Function Descriptions

### `multithreaded_sorting.c`
This program sorts an array using multithreading and compares the performance with a single-threaded approach.

#### **Functions**
- **`void* sorting_avg(void* args)`**
  - Sorts a portion of an array using insertion sort.
  - Computes the average of the sorted portion.
  - Runs in a separate thread.

- **`void* merging_avg(void* args)`**
  - Merges two sorted sub-arrays into one.
  - Computes the overall average of the merged array.
  - Runs in a separate thread.

- **`void insertionSort(double* array, int start, int end)`**
  - Implements insertion sort on a sub-array.

- **`double calculate_average(double* array, int start, int end)`**
  - Computes the average value of a given sub-array.

- **`int main(int argc, char *argv[])`**
  - Parses input arguments.
  - Generates a random array.
  - Creates threads for sorting and merging.
  - Measures execution time for multithreaded and single-threaded sorting.
  - Prints the sorting performance results.

### `MultithreadedSortingApplication.java`
This Java program follows a similar approach to the C implementation but utilizes Java threads.

#### **Methods**
- **`static double[] generateArray(int n)`**
  - Generates an array of random double values.

- **`static class Sorting_avg implements Runnable`**
  - Implements the sorting logic for a sub-array.
  - Computes the average of the sorted sub-array.

- **`static class Merging_avg implements Runnable`**
  - Merges two sorted sub-arrays.
  - Computes the overall average of the merged array.

- **`static void insertionSort(double[] array, int start, int end)`**
  - Implements insertion sort.

- **`static double calculateAverage(double[] array)`**
  - Computes the average value of an array.

- **`static double[] mergeArrays(double[] firstSubArray, double[] secondSubArray)`**
  - Merges two sorted arrays.

- **`public static void main(String[] args)`**
  - Parses input arguments.
  - Generates a random array.
  - Creates threads for sorting and merging.
  - Measures execution time for both multithreaded and single-threaded sorting.
  - Prints performance results.

## How to Compile
To compile the project, run:
```sh
make
```
This will generate the executable files for both C and Java programs.

## How to Run
### C Program
```sh
./multithreaded_sorting <array_size>
```

### Java Program
```sh
java MultithreadedSortingApplication <array_size>
```

## Expected Output
```
Merged array and computed overall average: <VALUE>
Multithreaded sorting and merging took <TIME> seconds.
Single-threaded sorting took <TIME> seconds. Average: <VALUE>
```

## Notes
- The program supports array sizes up to 10,000.
- Multithreading is expected to provide performance improvements for large datasets.
- The report.txt file contains observed execution times and comparisons between C and Java implementations.

This project demonstrates the benefits of multithreading in sorting operations and provides insights into performance differences between C and Java implementations.
