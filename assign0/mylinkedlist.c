/*
 * File: mylinkedlist.c
 * Travis Weber
 * YOU NEED TO IMPLEMENT THE FUNCTIONS  here
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include "mylinkedlist.h"
 
/*
 * Function: NewStudentCell
 * Usage: student_cell_T *element;
 *        element = NewStudentCell(int id, double gpa, char *name);
 * --------------------------
 * This function allocates space for a student cell and intilize its fileds
 */
student_cell_T *NewStudentCell(int id, double gpa, char *name)
{
   student_cell_T *element;
  
   element = (student_cell_T *) malloc( sizeof(student_cell_T) );
   if( !element)
   {
     fprintf(stderr,"NewStudentCell cannot allocate memory\n");
     return NULL;
   }
   element->id = id;
   element->gpa = gpa;
   element->name = name;

  return element;
}


/*
 * Function: NewLinkedList
 * Usage: linked_list_T *list;
 *        list = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list.
 */
linked_list_T *NewLinkedList(void)
{
   linked_list_T *list;
  
   list = (linked_list_T *) malloc( sizeof(linked_list_T) );
   
   if( !list)
   {
     fprintf(stderr,"NewLinkedList cannot allocate memory\n");
     return NULL;
   }

   list->head = NULL;
   list->tail = NULL;

  return list;
}


/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(list);
 * ------------------------
 * This function frees the storage associated with list.
 */
void FreeLinkedList(linked_list_T *list)
{
    if (!list) 
    {
        fprintf(stderr, "FreeLinkedList received NULL pointer\n");
        return;
    }

    // Implement the freeing of the linked list here
    while (list->head) 
    {
        student_cell_T *temp = list->head;
        list->head = list->head->next;
        free(temp->name); // Free the name
        free(temp);       // Free the cell
    }

    // Free the linked list structure itself
    free(list);
}

/*
 * Function: Enlist
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a student cell pointed by element to the end of the list.
 */
void Enlist(linked_list_T *list, student_cell_T *element)
{
    if (!list || !element) 
    {
        fprintf(stderr, "Enlist received NULL pointer(s)\n");
        return;
    }

    // Implement adding a student cell to the end of the list here
    element->next = NULL;
    if (list->tail) 
    {
        list->tail->next = element;
        list->tail = element;
    } 
    else 
    {
        list->head = element;
        list->tail = element;
    }
}

/*
 * Function: Delist
 * Usage: element = Delist(list);
 * --------------------------------
 * This function removes the student cell at the head of the list
 * and returns its address to the caller (client).  If the list is empty, Delist
 * prints an Error with an appropriate message and returns NULL.
 */
student_cell_T *Delist(linked_list_T *list)
{
    if (!list) 
    {
        fprintf(stderr, "Delist received NULL pointer\n");
        return NULL;
    }

    // Implement removing a student cell from the head of the list here
    if (!list->head) 
    {
        fprintf(stderr, "Delist: List is empty\n");
        return NULL;
    }

    student_cell_T *temp = list->head;
    list->head = list->head->next;
    if (!list->head) 
    {
        list->tail = NULL;
    }
    temp->next = NULL;
    return temp;
}

/*
 * Functions: LinkedListIsEmpty, LinkedListIsFull
 * Usage: if (LinkedListIsEmpty(list)) . . .
 *        if (LinkedListIsFull(list)) . . .
 * -------------------------------------
 * These functions test whether the list is empty or full.
 */
int LinkedListIsEmpty(linked_list_T *list)
{
    if (!list) 
    {
        fprintf(stderr, "LinkedListIsEmpty received NULL pointer\n");
        return 1;
    }

    // Implement checking if the list is empty here
    return (list->head == NULL);
}

int LinkedListIsFull(linked_list_T *list)
{
    if (!list) 
    {
        fprintf(stderr, "LinkedListIsFull received NULL pointer\n");
        return 0;
    }
    return 0; // because we have linked list
}

/*
 * Function: LinkedListLength
 * Usage: n = LinkedListLength(list);
 * ------------------------------
 * This function returns the number of elements in the list.
 */
int LinkedListLength(linked_list_T  *list)
{
    if (!list) 
    {
        fprintf(stderr, "LinkedListLength received NULL pointer\n");
        return 0;
    }

    // Implement counting the number of elements in the list here
    int length = 0;
    student_cell_T *current = list->head;
    while (current) 
    {
        length++;
        current = current->next;
    }
    return length;
}

/*
 * Function: GetLinkedListElement
 * Usage: element = GetLinkedListElement(list, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * list, where the head of the list is defined as index 0. For
 * example, calling GetLinkedListElement(list, 0) returns the initial
 * element from the list without removing it.  If the caller tries
 * to select an element that is out of range, GetLinkedListElement prints
 * Error and returns NULL.  Note: This function is not a fundamental list operation
 * and is instead provided mainly to facilitate debugging.
 */
student_cell_T *GetLinkedListElement(linked_list_T *list, int index)
{
    if (!list) 
    {
        fprintf(stderr, "GetLinkedListElement received NULL pointer\n");
        return NULL;
    }

    // Implement getting the element at the specified index here
    if (index < 0) 
    {
        fprintf(stderr, "GetLinkedListElement: Invalid index\n");
        return NULL;
    }

    int current_index = 0;
    student_cell_T *current = list->head;
    while (current) 
    {
        if (current_index == index) 
        {
            return current;
        }
        current_index++;
        current = current->next;
    }

    fprintf(stderr, "GetLinkedListElement: Index out of range\n");
    return NULL;
}



/* OTHER FUNCTIONS YOU WOULD NEED....
 * EXPORT THEM HERE, BUT IMPLMENT THEM in mylinkedlist.c 
 */
 
 // Additional function to print all students' information
void printAllStudentsInfo(linked_list_T *list)
{
    if (!list) 
    {
        fprintf(stderr, "printAllStudentsInfo received NULL pointer\n");
        return;
    }

    student_cell_T *current = list->head;
    while (current) 
    {
        printf("ID: %d, GPA: %.2lf, Name: %s\n", current->id, current->gpa, current->name);
        current = current->next;
    }
}

// Additional function to calculate and print GPA statistics
void calculateGPASummary(linked_list_T *list)
{
    if (!list)
    {
        fprintf(stderr, "calculateGPASummary received NULL pointer\n");
        return;
    }

    if (LinkedListIsEmpty(list))
    {
        printf("No students in the list.\n");
        return;
    }

    double minGPA = list->head->gpa;
    double maxGPA = list->head->gpa;
    double totalGPA = 0.0;
    int count = 0;

    student_cell_T *current = list->head;
    while (current) 
    {
        double gpa = current->gpa;
        if (gpa < minGPA)
        {
            minGPA = gpa;
        }
        if (gpa > maxGPA)
        {
            maxGPA = gpa;
        }
        totalGPA += gpa;
        count++;
        current = current->next;
    }

    double averageGPA = totalGPA / count;

    printf("Min GPA: %.2lf\n", minGPA);
    printf("Average GPA: %.2lf\n", averageGPA);
    printf("Max GPA: %.2lf\n", maxGPA);
}

// Additional function to remove the student with the highest GPA
void removeStudentWithMaxGPA(linked_list_T *list)
{
    if (!list)
    {
        fprintf(stderr, "removeStudentWithMaxGPA received NULL pointer\n");
        return;
    }

    if (LinkedListIsEmpty(list)) 
    {
        printf("No students in the list to remove.\n");
        return;
    }

    student_cell_T *current = list->head;
    student_cell_T *prev = NULL;
    student_cell_T *maxGPAStudent = list->head;
    double maxGPA = list->head->gpa;

    while (current) 
    {
        if (current->gpa > maxGPA)
        {
            maxGPA = current->gpa;
            maxGPAStudent = current;
            prev = current; //was prev = prev I was trying to update the pointer but assigned it to itself by accident 
        }
        prev = current;
        current = current->next;
    }

    if (maxGPAStudent == list->head)
    {
        list->head = list->head->next;
        if (!list->head)
        {
            list->tail = NULL;
        }
    } 
    else
    {
        prev->next = maxGPAStudent->next;
        if (!prev->next)
        {
            list->tail = prev;
        }
    }

    printf("Removed student with the highest GPA: ID: %d, GPA: %.2lf, Name: %s\n",
           maxGPAStudent->id, maxGPAStudent->gpa, maxGPAStudent->name);
    free(maxGPAStudent->name);
    free(maxGPAStudent);
}


  