# Define compiler and compiler flag variables
CC = gcc
CFLAGS = -Wall -pthread
JCC = javac

# Define your program names
C_PROGRAM = multithreaded_sorting
JAVA_MAIN = MultithreadedSortingApplication

# Default target to compile both C and Java programs and then run them with different values of N
all: compC compJava runC runJava 

# Compile the C program
compC: $(C_PROGRAM).c
	$(CC) $(CFLAGS) -o $(C_PROGRAM) $(C_PROGRAM).c

# Run the C program with different values of N
runC: $(C_PROGRAM)
	./$(C_PROGRAM) 10
	./$(C_PROGRAM) 1000
	./$(C_PROGRAM) 5000
	./$(C_PROGRAM) 10000

# Compile the Java program
compJava: $(JAVA_MAIN).java
	$(JCC) $(JAVA_MAIN).java

# Run the Java program with different values of N
runJava: $(JAVA_MAIN).class
	java $(JAVA_MAIN) 10
	java $(JAVA_MAIN) 1000
	java $(JAVA_MAIN) 5000
	java $(JAVA_MAIN) 10000

# Clean up compiled outputs
clean:
	rm -f *.o *.class $(C_PROGRAM)