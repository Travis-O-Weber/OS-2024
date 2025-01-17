/*
 * File: driver1.c
 * Travis Weber ... YOU NEED TO IMPLEMENT THE main() + additional functions if needed
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include "mylinkedlist.h"



/* 
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline '\n' character that terminates
 * the input is not stored as part of the string.
 */
 
 /* Function prototypes for additional functions */
 

char *ReadLine(void);

int main(int argc, char *arvg[])
{
  // YOU NEED TO IMPLEMENT THIS driver1.c USING FUNCTIONS FROM mylinkedlist.h
  // But before that, implement your ReadLine() function, and test it as shown below. 
  // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE
/*
  char *name;

  printf("Enter a name to test your ReadLine function  : ");
  name = ReadLine();
  printf("User entered : %s \n", name);  
  free(name);

  return 0;*/
  
     linked_list_T *studentList = NewLinkedList(); // Create an empty linked list

    if (!studentList) {
        fprintf(stderr, "Failed to create the linked list.\n");
        return 1;
    }

    while (1) 
    {
        printf("\nEnter your choice:\n");
        printf("1 - Create a new student cell and add it to the linked list\n");
        printf("2 - Remove the first student from the linked list\n");
        printf("3 - Print the number of students in the linked list\n");
        printf("4 - Print information of a student at a specific index\n");
        printf("5 - Print the list of all students in the linked list\n");
        printf("6 - Print the min, average, and max GPAs in the linked list\n");
        printf("7 - Remove the student with the highest GPA\n");
        printf("8 - Exit\n");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) 
        {
            int id;
            double gpa;
            char *name;

            printf("Enter student ID: ");
            scanf("%d", &id);
             getchar(); // Consume the newline character
            printf("Enter student GPA: ");
            scanf("%lf", &gpa);
             getchar(); // Consume the newline character
            printf("Enter student name: ");
            name = ReadLine();

            student_cell_T *element = NewStudentCell(id, gpa, name);
            Enlist(studentList, element);
        } 
        
        else if (choice == 2) 
        {
        student_cell_T *element = Delist(studentList);
            if (element) 
            {
            printf("Removed student: ID: %d, GPA: %.2lf, Name: %s\n",
            element->id, element->gpa, element->name);
            free(element->name);
            free(element);
            }
        }   
        
        else if (choice == 3) 
        {
        int numStudents = LinkedListLength(studentList);
        printf("Number of students in the linked list: %d\n", numStudents);
        } 
 
        else if (choice == 4) 
        {
            int index;
            printf("Enter the index of the student to print: ");
            scanf("%d", &index);
            student_cell_T *element = GetLinkedListElement(studentList, index);
            
            if (element) 
            {
                printf("Student at index %d: ID: %d, GPA: %.2lf, Name: %s\n",
                       index, element->id, element->gpa, element->name);
            }
        } 
        else if (choice == 5) 
        {
            printAllStudentsInfo(studentList);
        } 
        else if (choice == 6) 
        {
            calculateGPASummary(studentList);
        } else if (choice == 7) 
        {
            removeStudentWithMaxGPA(studentList);
        } 
        else if (choice == 8) 
        {
            FreeLinkedList(studentList);
            printf("Exiting the program.\n");
            break;
        }
        else 
        {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
  
}

/* 
 * IMPLEMENTATION of ReadLine();
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline '\n' character that terminates
 * the input is not stored as part of the string.
 *
 * In contrast to standard I/O functions (e.g., scanf with "%s", fgets) 
 * that can read strings into a given static size buffer, ReadLine function 
 * should read the given input line of characters terminated by a newline 
 * character ('\n') into a dynamically allocated and resized buffer based on 
 * the length of the given input line. 
 *
 * When implementing this function you can use standard I/O functions. 
 * We just want you to make sure you allocate enough space for the entered data. 
 * So don't simply allocate 100 or 1000 bytes every time. 
 * If the given input has 5 characters, you need to allocate space for 6 characters.
 *
 * Hint: initially dynamically allocate an array of char with size 10. 
 * Then, read data into that array character by charecter (e.g, you can use getchar()). 
 * If you see '\n' char before reading 10th character, you are done. And you know the 
 * exact length of the input string. So, accordingly allocate enough space and copy the 
 * data into new char array, insert '\0' instead of '\n' and free the original array. 
 * Then return the new string. However, if you DO NOT see '\n' char after 10th character, 
 * then you need larger space. Accordingly, resize your original array and double its size 
 * and continue reading data character by character as in the first step... 
 * Hope you got the idea! 
 *
 * Also please check for possible errors (e.g., no memory etc.) and appropriately handle 
 * them. For example, if malloc returns NULL, free partially allocated space and return 
 * NULL from this function. The program calling this function may take other actions, 
 * e.g., stop the program!
 */
char *ReadLine()
{
  int bufferSize = 10; // Initial buffer size
  int index = 0;
  char *buffer = (char *)malloc(bufferSize * sizeof(char));

  // Check if memory allocation was successful
  if (buffer == NULL) 
  {
    printf("Memory allocation failed.\n");
    return NULL;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF) 
  {
    buffer[index++] = c;
    if (index == bufferSize - 1) 
    {
      // Resize the buffer if it's full
      bufferSize *= 2;
      char *temp = (char *)realloc(buffer, bufferSize * sizeof(char));
      
      // Check if memory reallocation was successful
      if (temp == NULL) 
      {
        printf("Memory reallocation failed.\n");
        free(buffer);
        return NULL;
      }
      buffer = temp;
    }
  }

  // Null-terminate the string
  buffer[index] = '\0';

  return buffer;
}

   