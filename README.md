## LIBRARY MANAGEMENT SYSTEM
This Library Management System is a simple console-based C++ application that allows users to manage books and members. It enables functionalities such as adding books, issuing and returning books, managing members, and calculating fines for late returns.
1. **Classes**

* LibraryItem (Abstract Class)
   -Defines the base structure for all items in the library
   -Contains a pure virtual function displayDetails().

* Book
  -Stores book details such as ID, title, author, genre, total and available copies.
  -Implements book issuance and return functionality.
  -Tracks how many times a book has been borrowed.


* Student 
  -Stores Student details like ID, name, contact details.
  -Stores fine per day and number of late days and calculates fine.
  -Provides functions to display member details.

* Library
  -Manages collections of books and members.
  -Handles book issuance and return.
  -Uses file handling for data persistence.


2. **Encapsulation**:
  * Data members are private, and accessors (getters/setters) provide controlled access.

3. **Inheritance**: 
* Class Book is inherited from  Parent class LibraryItem.
 
4. **LibraryItem** : 
* It serves as a base class for Book.

5. **Polymorphism**: 
* The function displayDetails() is a virtual function in LibraryItem, allowing different implementations in derived classes.
   * displayMemberInfo() is overridden in class Book.

6. **Abstraction**:
* LibraryItem class abstracts common functionality for different library items.

7. **Exception Handling**: 
* Invalid Member or Book ID:Proper checks ensure valid IDs are used when issuing or returning books.

8. **File Handling**: 
* Books and members are stored in text files (books.txt and members.txt).
* The system reads existing data from these files on startup and writes updated data when exiting.
9. **Data Persistence**:
* Data is stored and retrieved from files to maintain records between sessions.
* Helps in long-term tracking of books and members.

***Main Functionality***
   It provides a menu-driven interface.It allows users to add books, add members, issue books, return books, and view records.


This project serves as a simple yet functional example of OOP principles, exception handling, file management, and data persistence in C++.
