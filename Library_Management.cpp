#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class LibraryItem {
public:
    virtual void displayDetails() const = 0;
};

class Book : public LibraryItem {
private:
    int bookId;
    string title;
    string author;
    string genre;
    int totalCopies;
    int availableCopies;
    int borrowCount;

public:
    Book(int id, string title, string author, string genre, int copies)
        : bookId(id), title(title), author(author), genre(genre),
          totalCopies(copies), availableCopies(copies), borrowCount(0) {}

    Book() : bookId(0), totalCopies(0), availableCopies(0), borrowCount(0) {}

    int getBookID() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getGenre() const { return genre; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }
    int getBorrowCount() const { return borrowCount; }

    void incrementBorrowCount() { borrowCount++; }
    void addCopies(int count);
    void issueBook();
    void returnBook();
    void displayDetails() const override;

    friend ofstream &operator<<(ofstream &os, const Book &book);
    friend ifstream &operator>>(ifstream &is, Book &book);
};

class Student {
private:
    int memberID;
    string name;
    string contactInfo;
    float finePerDay;
    int daysLate;

public:
    Student(int id, string name, string contact, float fine = 1.0, int late = 0)
        : memberID(id), name(name), contactInfo(contact), finePerDay(fine), daysLate(late) {}

    Student() : memberID(0), finePerDay(1.0), daysLate(0) {}

    int getMemberID() const { return memberID; }
    string getName() const { return name; }
    string getContactInfo() const { return contactInfo; }
    void setDaysLate(int days) { daysLate = days; }
    float calculateFine() const { return daysLate * finePerDay; }
    void displayMemberInfo() const;

    friend ofstream &operator<<(ofstream &os, const Student &student);
    friend ifstream &operator>>(ifstream &is, Student &student);
};

class Library {
private:
    vector<Book> books;
    vector<Student> members;
    map<int, int> issuedBooks;

    void saveBooksToFile();
    void saveMembersToFile();
    void loadBooksFromFile();
    void loadMembersFromFile();

public:
    Library();
    ~Library();

    void addBook(const Book &book);
    void addMember(const Student &student);
    void displayAllBooks() const;
    void displayAllMembers() const;
    void issueBook(int bookID, int memberId);
    void returnBook(int bookID, int memberId);
    void showMenu();
    Student *findMemberByID(int id);
};

int main() {
    Library library;
    library.showMenu();
    return 0;
}

// --- Function Definitions ---

void Book::addCopies(int count) {
    totalCopies += count;
    availableCopies += count;
}

void Book::issueBook() {
    if (availableCopies > 0) {
        availableCopies--;
        incrementBorrowCount();
        cout << "Book \"" << title << "\" has been issued successfully." << endl;
    } else {
        cout << "All copies of \"" << title << "\" are currently unavailable." << endl;
    }
}

void Book::returnBook() {
    if (availableCopies < totalCopies) {
        availableCopies++;
    } else {
        cout << "All copies of \"" << title << "\" are already in the library." << endl;
    }
}

void Book::displayDetails() const {
    cout << "\n--- Book Details ---\n";
    cout << "Book ID: " << bookId << "\nTitle: " << title << "\nAuthor: " << author
         << "\nGenre: " << genre
         << "\nTotal Copies: " << totalCopies
         << "\nAvailable Copies: " << availableCopies
         << "\nTimes Borrowed: " << borrowCount << endl;
}

ofstream &operator<<(ofstream &os, const Book &book) {
    os << book.bookId << "|" << book.title << "|" << book.author << "|"
       << book.genre << "|" << book.totalCopies << "|" << book.availableCopies << "|"
       << book.borrowCount << "\n";
    return os;
}

ifstream &operator>>(ifstream &is, Book &book) {
    string line;
    if (getline(is, line)) {
        istringstream ss(line);
        string temp;

        getline(ss, temp, '|'); book.bookId = stoi(temp);
        getline(ss, book.title, '|');
        getline(ss, book.author, '|');
        getline(ss, book.genre, '|');
        getline(ss, temp, '|'); book.totalCopies = stoi(temp);
        getline(ss, temp, '|'); book.availableCopies = stoi(temp);
        getline(ss, temp, '|'); book.borrowCount = stoi(temp);
    }
    return is;
}

void Student::displayMemberInfo() const {
    cout << "\n--- Student Details ---\n";
    cout << "ID: " << memberID << "\nName: " << name << "\nContact: " << contactInfo
         << "\nFine/Day: Rs." << finePerDay << "\nDays Late: " << daysLate
         << "\nTotal Fine: Rs." << calculateFine() << endl;
}

ofstream &operator<<(ofstream &os, const Student &student) {
    os << student.memberID << '|' << student.name << '|' << student.contactInfo << '|' << student.finePerDay << '|' << student.daysLate << '\n';
    return os;
}

ifstream &operator>>(ifstream &is, Student &student) {
    string line;
    if (getline(is, line)) {
        istringstream ss(line);
        string temp;
        getline(ss, temp, '|'); student.memberID = stoi(temp);
        getline(ss, student.name, '|');
        getline(ss, student.contactInfo, '|');
        getline(ss, temp, '|'); student.finePerDay = stof(temp);
        getline(ss, temp, '|'); student.daysLate = stoi(temp);
    }
    return is;
}

Library::Library() {
    loadBooksFromFile();
    loadMembersFromFile();
}

Library::~Library() {
    saveBooksToFile();
    saveMembersToFile();
}

void Library::saveBooksToFile() {
    ofstream outFile("books.txt");
    for (const auto &book : books) {
        outFile << book;
    }
}

void Library::saveMembersToFile() {
    ofstream outFile("members.txt");
    for (const auto &student : members) {
        outFile << student;
    }
}

void Library::loadBooksFromFile() {
    ifstream inFile("books.txt");
    Book book;
    while (inFile >> book) {
        books.push_back(book);
    }
}

void Library::loadMembersFromFile() {
    ifstream inFile("members.txt");
    Student student;
    while (inFile >> student) {
        members.push_back(student);
    }
}

void Library::addBook(const Book &book) {
    books.push_back(book);
    saveBooksToFile();
}

void Library::addMember(const Student &student) {
    members.push_back(student);
    saveMembersToFile();
}

void Library::displayAllBooks() const {
    for (const auto &book : books) {
        book.displayDetails();
    }
}

void Library::displayAllMembers() const {
    for (const auto &member : members) {
        member.displayMemberInfo();
    }
}

Student *Library::findMemberByID(int id) {
    for (auto &m : members) {
        if (m.getMemberID() == id) return &m;
    }
    return nullptr;
}

void Library::issueBook(int bookID, int memberID) {
    Student *student = findMemberByID(memberID);
    if (!student) {
        cout << "No such student ID." << endl;
        return;
    }

    auto it = find_if(books.begin(), books.end(), [bookID](const Book &b) { return b.getBookID() == bookID; });
    if (it != books.end()) {
        if (issuedBooks.find(bookID) == issuedBooks.end()) {
            it->issueBook();
            issuedBooks[bookID] = memberID;
            saveBooksToFile();
            cout << "Issued book ID " << bookID << " to student ID " << memberID << ".\n";
        } else {
            cout << "Book is already issued.\n";
        }
    } else {
        cout << "Book not found.\n";
    }
}

void Library::returnBook(int bookID, int memberID) {
    Student *student = findMemberByID(memberID);
    if (!student) {
        cout << "No such student ID." << endl;
        return;
    }

    auto it = find_if(books.begin(), books.end(), [bookID](const Book &b) { return b.getBookID() == bookID; });
    if (it != books.end()) {
        if (issuedBooks.find(bookID) != issuedBooks.end() && issuedBooks[bookID] == memberID) {
            it->returnBook();
            issuedBooks.erase(bookID);
            saveBooksToFile();
            int days;
            cout << "Enter days late: ";
            cin >> days;
            student->setDaysLate(days);
            cout << "Late fine: Rs." << student->calculateFine() << endl;
        } else {
            cout << "Book not issued to this student.\n";
        }
    } else {
        cout << "Book not found.\n";
    }
}

void Library::showMenu() {
    while (true) {
        cout << "\n--- Library Menu ---\n";
        cout << "1. Add Book\n2. Add Member\n3. Display All Books\n4. Display All Members\n5. Issue Book\n6. Return Book\n7. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            int id, copies;
            string title, author, genre;
            cout << "Enter Book ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Title: "; getline(cin, title);
            cout << "Enter Author: "; getline(cin, author);
            cout << "Enter Genre: "; getline(cin, genre);
            cout << "Enter Total Copies: "; cin >> copies;
            addBook(Book(id, title, author, genre, copies));
            break;
        }
        case 2: {
            int id;
            string name, contactInfo;
            cout << "Enter Student ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Name: "; getline(cin, name);
            cout << "Enter Contact Info: "; getline(cin, contactInfo);
            addMember(Student(id, name, contactInfo));
            break;
        }
        case 3:
            displayAllBooks();
            break;
        case 4:
            displayAllMembers();
            break;
        case 5: {
            int bookID, memberID;
            cout << "Enter Book ID to issue: "; cin >> bookID;
            cout << "Enter Member ID to issue the book to: "; cin >> memberID;
            issueBook(bookID, memberID);
            break;
        }
        case 6: {
            int bookID, memberID;
            cout << "Enter Book ID to return: "; cin >> bookID;
            cout << "Enter Member ID returning the book: "; cin >> memberID;
            returnBook(bookID, memberID);
            break;
        }
        case 7:
            cout << "Exiting the library system. Goodbye!" << endl;
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
