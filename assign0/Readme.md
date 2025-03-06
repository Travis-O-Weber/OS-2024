# **Student Management System (Linked List Implementation)**

## **Overview**
This project implements a **student management system using a linked list** in **C**. It allows users to **add, remove, search, and analyze student records** stored in a **dynamically allocated linked list**. Each student record includes:
- **Student ID**
- **GPA**
- **Name**

The program provides an interactive **menu-driven console interface** for managing student records.

---

## **Features**
- **Dynamically allocated linked list** for storing student records.
- **Create student entries** and add them to the linked list.
- **Remove the first student** from the list.
- **Retrieve student information by index**.
- **Display all students** in the list.
- **Calculate min, average, and max GPAs**.
- **Remove the student with the highest GPA**.
- **Interactive menu-driven interface**.

---

## **Functionality Breakdown**

### **1. Linked List Implementation - `mylinkedlist.c` & `mylinkedlist.h`**
This file contains **linked list operations** for managing student records.

#### **Core Functions:**
- **`NewStudentCell(int id, double gpa, char *name)`**  
  - Creates and returns a new student record.
  
- **`NewLinkedList(void)`**  
  - Creates and returns an empty linked list.

- **`FreeLinkedList(linked_list_T *list)`**  
  - Frees memory associated with the linked list.

- **`Enlist(linked_list_T *list, student_cell_T *element)`**  
  - Adds a student to the **end** of the list.

- **`Delist(linked_list_T *list)`**  
  - Removes and returns the **first** student in the list.

- **`LinkedListIsEmpty(linked_list_T *list)`**  
  - Checks if the list is empty.

- **`LinkedListLength(linked_list_T *list)`**  
  - Returns the number of students in the list.

- **`GetLinkedListElement(linked_list_T *list, int index)`**  
  - Retrieves a student record at a given index.

#### **Additional Functions:**
- **`printAllStudentsInfo(linked_list_T *list)`**  
  - Prints all student records in the linked list.

- **`calculateGPASummary(linked_list_T *list)`**  
  - Calculates and prints the **min, max, and average GPA**.

- **`removeStudentWithMaxGPA(linked_list_T *list)`**  
  - Removes the student with the **highest GPA**.

---

### **2. Interactive Console Menu - `driver1.c`**
This file contains the **main program** that interacts with the user.

#### **Menu Options:**
1. **Add a new student**
2. **Remove the first student**
3. **Print the number of students**
4. **Print student information by index**
5. **Print all students**
6. **Calculate and print GPA statistics**
7. **Remove the student with the highest GPA**
8. **Exit**

#### **Key Functions:**
- **`ReadLine(void)`**  
  - Reads user input as a dynamically allocated string.

- **Main loop** handles user input and calls corresponding **linked list functions**.

---

### **3. Example Input/Output - `driver_output.txt`**
#### **Adding Students**
```
Enter student ID: 1
Enter student GPA: 3.2
Enter student name: Alice

Enter student ID: 2
Enter student GPA: 3.9
Enter student name: Bob
```
#### **Displaying Students**
```
ID: 1, GPA: 3.2, Name: Alice
ID: 2, GPA: 3.9, Name: Bob
```
#### **Removing a Student**
```
Removed student: ID: 1, GPA: 3.2, Name: Alice
```
#### **Calculating GPA Summary**
```
Min GPA: 3.2
Average GPA: 3.55
Max GPA: 3.9
```

---

## **Compilation & Execution**
### **Compile:**
```sh
gcc driver1.c mylinkedlist.c -o student_system
```
### **Run:**
```sh
./student_system
```

---

## **Future Improvements**
- Implement **file-based storage** to save and load student records.
- Improve **error handling** for invalid user inputs.
- Add **search functionality** to find students by name or ID.
