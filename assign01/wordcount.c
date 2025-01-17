#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 

int countWords(FILE *fp) {

    int count = 0; //holds count
    char ch; // holds character read from the file
    int inWord = 0; // flag to see if we are in the word

    while ((ch = fgetc(fp)) != EOF) 
    {
        if (isspace(ch)) 
        {
            if (inWord) 
            {
                count++;
                inWord = 0;
            }
        } 
        else 
        {
            inWord = 1;
        }
    }
    //loops through each character in the file until the end of the file


    if (inWord) 
    {
        count++;
    }    
    // Count the last word if the file doesn't end with a whitespace

    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    //checks to see if agrument is provided 

    FILE *fp = fopen(argv[1], "r");
    if (!fp) 
    {
        fprintf(stderr, "wordcount with process %d cannot open %s\n", getpid(), argv[1]);
        return 1;
    }
    //attempts to open file in read mode

    int wordCount = countWords(fp);
    printf("wordcount with process %d counted words in %s: number of words is %d\n", getpid(), argv[1], wordCount);
    //printline statements

    fclose(fp); //coses file
    return 0;
}