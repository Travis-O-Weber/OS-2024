/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // For INT_MAX

#define PAGE_SIZE 128 // 128 bytes per page/frame
#define NUM_PAGES 32
#define NUM_FRAMES 8

typedef struct {
    int valid_bit;
    int frame_number;
} PageTableEntry;

PageTableEntry pageTable[NUM_PAGES];
int freeFrames[NUM_FRAMES] = {0, 1, 1, 1, 1, 1, 1, 1}; // Frame 0 is reserved for the OS
int reverseMap[NUM_FRAMES]; // To keep track of which page is in which frame
int LRUcount[NUM_FRAMES]; // To keep track of LRU policy

void initialize() {
    int i;
    for (i = 0; i < NUM_PAGES; i++) {
        pageTable[i].valid_bit = 0;
        pageTable[i].frame_number = -1;
    }
    
    for (i = 0; i < NUM_FRAMES; i++) {
        reverseMap[i] = -1; // -1 indicates that no page is mapped to this frame
        LRUcount[i] = INT_MAX; // Initialize to max value for LRU logic
    }
}

unsigned long translateAddress(unsigned long logicalAddress, int* pageFaultCount) {
    unsigned long pnum = logicalAddress >> 7; // Page number
    unsigned long offset = logicalAddress & 0x7F; // Offset
    unsigned long physicalAddress;
    int frameNumber;
    
    // If the valid_bit is 1, use the frame number from the page table entry
    if (pageTable[pnum].valid_bit == 1) {
        frameNumber = pageTable[pnum].frame_number;
        LRUcount[frameNumber] = 0; // Accessed now, so count is 0
        // Update LRU counts for other frames
        int i;
        for (i = 1; i < NUM_FRAMES; i++) {
            if (freeFrames[i] == 1) {
                LRUcount[i]++;
            }
        }
    } else {
        // Handle page fault
        (*pageFaultCount)++;
        // Find a free frame or use LRU policy to replace a frame
        frameNumber = findFreeFrameOrReplace();
        // If a frame is replaced, invalidate the old page table entry
        if (frameNumber != -1 && reverseMap[frameNumber] != -1) {
            pageTable[reverseMap[frameNumber]].valid_bit = 0;
        }
        // Set up new page table entry
        pageTable[pnum].valid_bit = 1;
        pageTable[pnum].frame_number = frameNumber;
        reverseMap[frameNumber] = pnum; // Update reverse map
        LRUcount[frameNumber] = 0; // Reset LRU count for the new frame
    }
    physicalAddress = (frameNumber << 7) + offset; // Compute the physical address
    return physicalAddress;
}

int findFreeFrameOrReplace() {
    // Search for a free frame
    int i;
    for (i = 1; i < NUM_FRAMES; i++) {
        if (freeFrames[i] == 1) {
            freeFrames[i] = 0; // Frame is now used
            return i;
        }
    }
    // If no free frame, find the LRU frame and replace it
    int lruIndex = 1;
    for (i = 2; i < NUM_FRAMES; i++) {
        if (LRUcount[i] > LRUcount[lruIndex]) {
            lruIndex = i;
        }
    }
    return lruIndex;
}

int main(int argc, char *argv[]) {
        if (argc != 3) {
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        return 1;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        perror("Error opening infile");
        return 1;
    }

    FILE *outfile = fopen(argv[2], "wb");
    if (outfile == NULL) {
        perror("Error opening outfile");
        fclose(infile);
        return 1;
    }

    // Initialize data structures
    initialize();

    int pageFaultCount = 0;
    unsigned long logicalAddress, physicalAddress;

    // Read each logical address from the input file
    while (fread(&logicalAddress, sizeof(unsigned long), 1, infile)) {
        physicalAddress = translateAddress(logicalAddress, &pageFaultCount);
        // Write the physical address to the output file
        fwrite(&physicalAddress, sizeof(unsigned long), 1, outfile);
    }

    // Close the input and output files
    fclose(infile);
    fclose(outfile);

    // Output the number of page faults
    printf("Number of page faults: %d\n", pageFaultCount);

    // Calculate and output the MD5 sum of the output file
    char command[256];
    snprintf(command, sizeof(command), "md5sum %s", argv[2]);
    system(command);

    // Assuming the md5sum should be written to REPORT.txt
    snprintf(command, sizeof(command), "md5sum %s > REPORT.txt", argv[2]);
    system(command);

    // Append the number of page faults to REPORT.txt
    FILE *report = fopen("REPORT.txt", "a");
    if (report == NULL) {
        perror("Error opening REPORT.txt");
        return 1;
    }
    fprintf(report, "Number of page faults: %d\n", pageFaultCount);
    fclose(report);
    
    return 0;
}