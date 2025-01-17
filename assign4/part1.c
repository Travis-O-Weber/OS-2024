/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// Declare a structure to represent a page table entry
typedef struct {
    int valid_bit;
    int frame_number;
} PageTableEntry;

// Declare the page table
PageTableEntry pageTable[32];

// Function to initialize the page table with fixed mapping
void initializePageTable() {
    // Initialize all page table entries to invalid
    int i;
    for (i = 0; i < 32; i++) {
        pageTable[i].valid_bit = 0;
        pageTable[i].frame_number = -1;
    }
    
    // Update the page table with fixed mappings based on the figure provided
    pageTable[1].valid_bit = 1;
    pageTable[1].frame_number = 2;
    // Add the rest of the fixed mappings here...
}

// Function to translate a logical address to a physical address
unsigned long translateAddress(unsigned long logicalAddress) {
    unsigned long pnum = logicalAddress >> 7; // Get page number (d=7 bits)
    unsigned long dnum = logicalAddress & 0x7F; // Get offset (7 bits)
    
    // Check if the page number is valid and has a valid mapping
    if (pageTable[pnum].valid_bit != 1) {
        fprintf(stderr, "Invalid page reference: %lu\n", pnum);
        exit(EXIT_FAILURE);
    }

    unsigned long fnum = pageTable[pnum].frame_number;
    unsigned long physicalAddress = (fnum << 7) | dnum;
    return physicalAddress;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s infile outfile\n", argv[0]);
        return 1;
    }

    // Open the input file for reading in binary mode
    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing in binary mode
    FILE *outfile = fopen(argv[2], "wb");
    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile);
        return 1;
    }

    // Initialize the page table
    initializePageTable();
    int numberOfPagesProcessed = 0; // Declare and initialize the page counter
    // Read logical addresses from the input file and translate them
    unsigned long logicalAddress, physicalAddress;
    // Inside your loop that reads and translates addresses
    while (fread(&logicalAddress, sizeof(unsigned long), 1, infile) == 1) {
    physicalAddress = translateAddress(logicalAddress);
    fwrite(&physicalAddress, sizeof(unsigned long), 1, outfile);
    
    // Print the addresses in hexadecimal format to the console
    printf("The LA is %lx and Translated PA is %lx\n", logicalAddress, physicalAddress);
    fflush(stdout); // Force the output to be written to the terminal
}

    // After the loop
    printf("total number of pages = %d\n", numberOfPagesProcessed);

    // Close the files
    fclose(infile);
    fclose(outfile);

    return 0;
}