
CC=gcc
CFLAGS=-std=c99 -Wall -Werror
EXECUTABLES=part1 part2
INPUT1=part1sequence
OUTPUT1=part1-output
INPUT2=part2sequence
OUTPUT2=part2-output

.PHONY: all clean

all: $(EXECUTABLES)
	./part1 $(INPUT1) $(OUTPUT1)
	@echo "Part1 MD5SUM:" > REPORT.txt
	@md5sum $(OUTPUT1) >> REPORT.txt
	@rm -f $(OUTPUT1)
	./part2 $(INPUT2) $(OUTPUT2)
	@echo "Part2 MD5SUM:" >> REPORT.txt
	@md5sum $(OUTPUT2) >> REPORT.txt
	@rm -f $(OUTPUT2)

part1: part1.c
	$(CC) $(CFLAGS) -o part1 part1.c

part2: part2.c
	$(CC) $(CFLAGS) -o part2 part2.c

clean:
	rm -f $(EXECUTABLES) $(OUTPUT1) $(OUTPUT2)
