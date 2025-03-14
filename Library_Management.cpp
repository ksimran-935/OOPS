#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class LibraryItem
{
public:
    virtual void displayDetails() const = 0;
};

class Book : public LibraryItem
{
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

class Member
{
protected:
    int memberID;
    string name;
    string contactInfo;

public:
    Member(int memberID, string name, string contactInfo)
        : memberID(memberID), name(name), contactInfo(contactInfo) {}

    Member() : memberID(0) {}

    int getMemberID() const { return memberID; }
    string getName() const { return name; }
    string getContactInfo() const { return contactInfo; }

    virtual void displayMemberInfo() const;
    virtual string getType() const { return "Member"; }

    friend ofstream &operator<<(ofstream &os, const Member &member);
    friend ifstream &operator>>(ifstream &is, Member &member);
};

class Student : public Member
{
private:
    float finePerDay;
    int daysLate;

public:
    Student(int memberID, string name, string contactInfo, float finePerDay = 1.0, int daysLate = 0)
        : Member(memberID, name, contactInfo), finePerDay(finePerDay), daysLate(daysLate) {}

    Student() : finePerDay(1.0), daysLate(0) {}

    void setDaysLate(int days) { daysLate = days; }
    float calculateFine() const;
    void displayMemberInfo() const override;
    string getType() const override { return "Student"; }

    friend ofstream &operator<<(ofstream &os, const Student &student);
    friend ifstream &operator>>(ifstream &is, Student &student);
};

class Library
{
private:
    vector<Book> books;
    vector<Member *> members;
    map<int, int> issuedBooks;

    void saveBooksToFile();
    void saveMembersToFile();
    void loadBooksFromFile();
    void loadMembersFromFile();

public:
    Library();
    ~Library();

    void addBook(const Book &book);
    void addMember(Member *member);
    void displayAllBooks() const;
    void displayAllMembers() const;
    void issueBook(int bookID, int memberId);
    void returnBook(int bookID, int memberId);
    void showMenu();
    Member *findMemberByID(int memberID) const
    {
        for (const auto &member : members)
        {
            if (member->getMemberID() == memberID)
            {
                return member;
            }
        }
        return nullptr;
    }
};

int main()
{
    Library library;
    library.showMenu();
    return 0;
}

void Library::showMenu()
{
    while (true)
    {
        cout << "\n--- Library Menu ---\n";
        cout << "1. Add Book\n2. Add Member\n3. Display All Books\n4. Display All Members\n5. Issue Book\n6. Return Book\n7. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id, copies;
            string title, author, genre;
            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Title: ";
            getline(cin, title);
            cout << "Enter Author: ";
            getline(cin, author);
            cout << "Enter Genre: ";
            getline(cin, genre);
            cout << "Enter Total Copies: ";
            cin >> copies;
            addBook(Book(id, title, author, genre, copies));
            break;
        }
        case 2:
        {
            int id;
            string name, contactInfo, type;
            cout << "Enter Member ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Contact Info: ";
            getline(cin, contactInfo);
            cout << "Enter Member Type (Member/Student): ";
            cin >> type;

            if (type == "Student")
            {
                addMember(new Student(id, name, contactInfo));
            }
            else
            {
                addMember(new Member(id, name, contactInfo));
            }
            break;
        }

        case 3:
            displayAllBooks();
            break;
        case 4:
            displayAllMembers();
            break;
        case 5:
        {
            int bookID, memberID;
            cout << "Enter Book ID to issue: ";
            cin >> bookID;
            cout << "Enter Member ID to issue the book to: ";
            cin >> memberID;
            issueBook(bookID, memberID);
            break;
        }
        case 6:
        {
            int bookID, memberID;
            cout << "Enter Book ID to return: ";
            cin >> bookID;
            cout << "Enter Member ID returning the book: ";
            cin >> memberID;
            returnBook(bookID, memberID);
            break;
        }

        case 7:
        {
            cout << "Exiting the library system. Goodbye!" << endl;
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
        }
    }
}


void Book::addCopies(int count)
{
    totalCopies += count;
    availableCopies += count;
}

void Book::issueBook()
{
    if (availableCopies > 0)
    {
        availableCopies--;
        incrementBorrowCount();
        cout << "Book \"" << title << "\" has been issued successfully." << endl;
    }
    else
    {
        cout << "All copies of \"" << title << "\" are currently unavailable." << endl;
    }
}

void Book::returnBook()
{
    if (availableCopies < totalCopies)
    {
        availableCopies++;
    }
    else
    {
        cout << "All copies of \"" << title << "\" are already in the library." << endl;
    }
}

void Book::displayDetails() const
{
    cout << "\n--- Book Details ---\n";
    cout << "Book ID: " << bookId << "\nTitle: " << title << "\nAuthor: " << author
         << "\nGenre: " << genre
         << "\nTotal Copies: " << totalCopies
         << "\nAvailable Copies: " << availableCopies
         << "\nTimes Borrowed: " << borrowCount;
         
}

ofstream &operator<<(ofstream &os, const Book &book)
{
    os << book.bookId << "|" << book.title << "|" << book.author << "|"
       << book.genre << "|" << book.totalCopies << "|" << book.availableCopies << "|";
      
    return os;
}

ifstream &operator>>(ifstream &is, Book &book)
{
    string line;
    if (getline(is, line))
    {
        istringstream ss(line);
        string temp;

        getline(ss, temp, '|');
        book.bookId = stoi(temp);

        getline(ss, book.title, '|');
        getline(ss, book.author, '|');
        getline(ss, book.genre, '|');

        getline(ss, temp, '|');
        book.totalCopies = stoi(temp);

        getline(ss, temp, '|');
        book.availableCopies = stoi(temp);

        getline(ss, temp, '|');
        book.borrowCount = stoi(temp);

       
    }
    return is;
}


void Member::displayMemberInfo() const
{
    cout << "\n--- Member Details ---\n";
    cout << "Member ID: " << memberID << "\nName: " << name << "\nContact Info: " << contactInfo << endl;
}

ofstream &operator<<(ofstream &os, const Member &member)
{
    os << member.memberID << "|" << member.name << "|" << member.contactInfo << "|"
       << member.getType() << endl;
    return os;
}

ifstream &operator>>(ifstream &is, Member &member)
{
    string line;
    if (getline(is, line))
    {
        istringstream ss(line);
        string temp;

        getline(ss, temp, '|');
        member.memberID = stoi(temp);

        getline(ss, member.name, '|');
        getline(ss, member.contactInfo, '|');
    }
    return is;
}


float Student::calculateFine() const
{
    return daysLate * finePerDay;
}

void Student::displayMemberInfo() const
{
    Member::displayMemberInfo();
    cout << "Fine Per Day: " << finePerDay << "\nDays Late: " << daysLate
         << "\nCalculated Fine: " << calculateFine() << endl;
}

ofstream &operator<<(ofstream &os, const Student &student)
{
    os << student.memberID << "|" << student.name << "|" << student.contactInfo << "|"
       << student.finePerDay << "|" << student.daysLate << "|Student" << endl;
    return os;
}

ifstream &operator>>(ifstream &is, Student &student)
{
    string line;
    if (getline(is, line))
    {
        istringstream ss(line);
        string temp;

        getline(ss, temp, '|');
        student.memberID = stoi(temp);

        getline(ss, student.name, '|');
        getline(ss, student.contactInfo, '|');

        getline(ss, temp, '|');
        student.finePerDay = stof(temp);

        getline(ss, temp, '|');
        student.daysLate = stoi(temp);
    }
    return is;
}


Library::Library()
{
    loadBooksFromFile();
    loadMembersFromFile();
}

Library::~Library()
{
    saveBooksToFile();
    saveMembersToFile();
    
}

void Library::saveBooksToFile()
{
    ofstream outFile("books.txt");
    for (const auto &book : books)
    {
        outFile << book;
    }
}

void Library::saveMembersToFile()
{
    ofstream outFile("members.txt");
    for (const auto &member : members)
    {
        outFile << *member;
    }
}

void Library::loadBooksFromFile()
{
    ifstream inFile("books.txt");
    if (!inFile)
        return;

    Book book;
    while (inFile >> book)
    {
        books.push_back(book);
    }
}

void Library::loadMembersFromFile()
{
    ifstream inFile("members.txt");
    if (!inFile)
    {
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        istringstream ss(line);
        string memberType;
        getline(ss, memberType, '|');

        if (memberType == "Student")
        {
            string memberID, name, contactInfo, finePerDay, daysLate;

            if (getline(ss, memberID, '|') &&
                getline(ss, name, '|') &&
                getline(ss, contactInfo, '|') &&
                getline(ss, finePerDay, '|') &&
                getline(ss, daysLate, '|'))
            {

                int id = stoi(memberID);
                float fine = stof(finePerDay);
                int late = stoi(daysLate);

                Student *student = new Student(id, name, contactInfo, fine, late);
                members.push_back(student);
            }
            else
            {
                cerr << "Error: Invalid Student data format in line: " << line << endl;
            }
        }
        else if (memberType == "Member")
        {
            string memberID, name, contactInfo;

            if (getline(ss, memberID, '|') &&
                getline(ss, name, '|') &&
                getline(ss, contactInfo, '|'))
            {

                int id = stoi(memberID);
                Member *member = new Member(id, name, contactInfo);
                members.push_back(member);
            }
            else
            {
                cerr << "Error: Invalid Member data format in line: " << line << endl;
            }
        }
        else
        {
            cerr << "Error: Unknown member type in line: " << line << endl;
        }
    }
}

void Library::addBook(const Book &book)
{
    books.push_back(book);
    saveBooksToFile();
}

void Library::addMember(Member *member)
{
    members.push_back(member);
    saveMembersToFile();
}

void Library::displayAllBooks() const
{
    for (const auto &book : books)
    {
        book.displayDetails();
    }
}

void Library::displayAllMembers() const
{
    for (const auto &member : members)
    {
        if (member->getMemberID())
        {
            member->displayMemberInfo();
        }
        else
        {
            Student *student = dynamic_cast<Student *>(member);
            student->displayMemberInfo();
        }
    }
}

void Library::issueBook(int bookID, int memberID)
{
    
    Member *member = findMemberByID(memberID);
    if (!member)
    {
        cout << "Error: Member with ID " << memberID << " does not exist." << endl;
        return;
    }

    
    auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book &book)
                          { return book.getBookID() == bookID; });

    if (bookIt != books.end())
    {
        if (issuedBooks.find(bookID) == issuedBooks.end()) 
        {
            bookIt->issueBook();
            issuedBooks[bookID] = memberID; 
            saveBooksToFile();
            cout << "Book with ID " << bookID << " issued to Member ID " << memberID << "." << endl;
        }
        else
        {
            cout << "Book with ID " << bookID << " is already issued to Member ID " << issuedBooks[bookID] << "." << endl;
        }
    }
    else
    {
        cout << "Book with ID " << bookID << " not found." << endl;
    }
}

void Library::returnBook(int bookID, int memberID)
{
    
    Member *member = findMemberByID(memberID);
    if (!member)
    {
        cout << "Error: Member with ID " << memberID << " does not exist." << endl;
        return;
    }

    
    auto bookIt = find_if(books.begin(), books.end(), [bookID](const Book &book)
                          { return book.getBookID() == bookID; });

    if (bookIt != books.end())
    {
        auto issueIt = issuedBooks.find(bookID);
        if (issueIt != issuedBooks.end() && issueIt->second == memberID)
        {
            bookIt->returnBook();
            issuedBooks.erase(issueIt); // Remove the association
            saveBooksToFile();

            
            if (member->getType() == "Student")
            {
                Student *student = dynamic_cast<Student *>(member);
                if (student)
                {
                    int daysLate;
                    cout << "Enter the number of days late: ";
                    cin >> daysLate;
                    student->setDaysLate(daysLate);
                    cout << "Fine for late return: Rs." << student->calculateFine() << endl;
                }
            }

            cout << "Book with ID " << bookID << " returned by Member ID " << memberID << "." << endl;
        }
        else if (issueIt == issuedBooks.end())
        {
            cout << "Book with ID " << bookID << " is not issued to any member." << endl;
        }
        else
        {
            cout << "Book with ID " << bookID << " was not issued to Member ID " << memberID << "." << endl;
        }
    }
    else
    {
        cout << "Book with ID " << bookID << " not found." << endl;
    }
}
