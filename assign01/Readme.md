# Word Count Program

## Overview
This project consists of a word count program that processes one or more input files to determine the number of words in each file. The project is implemented in C and utilizes process creation and inter-process communication to handle multiple files concurrently.

## Files in the Project

- **`wordcount.c`**: A program that reads a single file and counts the number of words.
- **`multiple_wordcount.c`**: A program that creates child processes to count words in multiple files concurrently.
- **`makefile`**: A script to compile the programs.
- **`input_file_1.txt`** & **`input_file_2.txt`**: Example text files for testing.

## Function Descriptions

### `wordcount.c`
This file implements a program that reads a file and counts the number of words using the `countWords` function.

#### **Functions**
- **`int countWords(FILE *fp)`**
  - Reads the file character by character.
  - Identifies words based on whitespace separation.
  - Returns the total number of words in the file.

- **`int main(int argc, char *argv[])`**
  - Validates input arguments.
  - Opens the provided file.
  - Calls `countWords` and prints the result.
  - Closes the file and exits.

### `multiple_wordcount.c`
This file extends `wordcount.c` to process multiple files concurrently by creating child processes.

#### **Functions**
- **`int main(int argc, char *argv[])`**
  - Validates input arguments.
  - Creates a child process for each file using `fork()`.
  - Executes `wordcount` using `execl()` in the child process.
  - Parent process waits for all child processes to complete.
  - Prints a summary of successfully processed files and failures.

## How to Compile
To compile the project, run:
```sh
make
```
This will generate the executable files `wordcount` and `multiple_wordcount`.

## How to Run
### Single File Word Count
```sh
./wordcount input_file_1.txt
```

### Multiple File Word Count
```sh
./multiple_wordcount input_file_1.txt input_file_2.txt
```

## Expected Output
For `wordcount.c`:
```
wordcount with process <PID> counted words in input_file_1.txt: number of words is <COUNT>
```

For `multiple_wordcount.c`:
```
Parent process <PID> created <N> child processes to count words in <N> files
<X> files have been counted successfully!
<Y> files did not exist
```

## Notes
- If a file does not exist, an error message is displayed, and it is counted as a failure.
- The program considers consecutive spaces, punctuation, and special characters properly when counting words.
- `multiple_wordcount` efficiently utilizes process creation to run word count operations in parallel.

