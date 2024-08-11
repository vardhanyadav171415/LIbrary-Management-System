#include <bits/stdc++.h>
using namespace std;

// Constants
const int MAX_BORROW_DAYS = 14;
const double FINE_PER_DAY = 5.0;

// Book Class
class Book {
private:
    int bookID;
    string title;
    string author;
    bool isIssued;
    time_t issueDate;

public:
    Book(int id = 0, string t = "", string a = "") : bookID(id), title(t), author(a), isIssued(false), issueDate(0) {}

    int getBookID() { return bookID; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    bool getStatus() { return isIssued; }
    time_t getIssueDate() { return issueDate; }

    void issueBook() { 
        isIssued = true;
        issueDate = time(nullptr);  // Store the current time as issue date
    }

    void returnBook() {
        isIssued = false;
        issueDate = 0;  // Reset issue date
    }

    void displayBookInfo() {
        cout << "Book ID: " << bookID << "\nTitle: " << title << "\nAuthor: " << author << "\nStatus: " << (isIssued ? "Issued" : "Available") << endl;
        if (isIssued) {
            tm *ltm = localtime(&issueDate);
            cout << "Issued Date: " << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << endl;
        }
    }
};

// Member Class
class Member {
private:
    int memberID;
    string name;
    vector<int> borrowedBooks;

public:
    Member(int id = 0, string n = "") : memberID(id), name(n) {}

    int getMemberID() { return memberID; }
    string getName() { return name; }
    vector<int> getBorrowedBooks() { return borrowedBooks; }

    void borrowBook(int bookID) { borrowedBooks.push_back(bookID); }
    void returnBook(int bookID) {
        borrowedBooks.erase(remove(borrowedBooks.begin(), borrowedBooks.end(), bookID), borrowedBooks.end());
    }

    void displayMemberInfo() {
        cout << "Member ID: " << memberID << "\nName: " << name << "\nBorrowed Books: ";
        if (borrowedBooks.empty()) {
            cout << "None" << endl;
        } else {
            for (int bookID : borrowedBooks) {
                cout << bookID << " ";
            }
            cout << endl;
        }
    }
};

// Library Class
class Library {
private:
    vector<Book> books;
    vector<Member> members;

    void saveBooks() {
        ofstream file("books.dat", ios::out | ios::binary);
        for (Book &book : books) {
            file.write((char *)&book, sizeof(Book));
        }
        file.close();
    }

    void saveMembers() {
        ofstream file("members.dat", ios::out | ios::binary);
        for (Member &member : members) {
            file.write((char *)&member, sizeof(Member));
        }
        file.close();
    }

public:
    Library() {
        loadBooks();
        loadMembers();
    }

    ~Library() {
        saveBooks();
        saveMembers();
    }

    void loadBooks() {
        ifstream file("books.dat", ios::in | ios::binary);
        Book book;
        while (file.read((char *)&book, sizeof(Book))) {
            books.push_back(book);
        }
        file.close();
    }

    void loadMembers() {
        ifstream file("members.dat", ios::in | ios::binary);
        Member member;
        while (file.read((char *)&member, sizeof(Member))) {
            members.push_back(member);
        }
        file.close();
    }

    void addBook() {
        int id;
        string title, author;
        cout << "Enter Book ID: ";
        cin >> id;
        cout << "Enter Book Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);
        books.push_back(Book(id, title, author));
        cout << "Book Added Successfully!" << endl;
    }

    void addMember() {
        int id;
        string name;
        cout << "Enter Member ID: ";
        cin >> id;
        cout << "Enter Member Name: ";
        cin.ignore();
        getline(cin, name);
        members.push_back(Member(id, name));
        cout << "Member Added Successfully!" << endl;
    }

    Book* findBook(int bookID) {
        for (auto &book : books) {
            if (book.getBookID() == bookID) {
                return &book;
            }
        }
        return nullptr;
    }

    Member* findMember(int memberID) {
        for (auto &member : members) {
            if (member.getMemberID() == memberID) {
                return &member;
            }
        }
        return nullptr;
    }

    void issueBook(int bookID, int memberID) {
        Book *book = findBook(bookID);
        Member *member = findMember(memberID);

        if (book == nullptr) {
            cout << "Book not found!" << endl;
            return;
        }

        if (member == nullptr) {
            cout << "Member not found!" << endl;
            return;
        }

        if (book->getStatus()) {
            cout << "Book is already issued!" << endl;
        } else {
            book->issueBook();
            member->borrowBook(bookID);
            cout << "Book Issued Successfully!" << endl;
        }
    }

    void returnBook(int bookID, int memberID) {
        Book *book = findBook(bookID);
        Member *member = findMember(memberID);

        if (book == nullptr) {
            cout << "Book not found!" << endl;
            return;
        }

        if (member == nullptr) {
            cout << "Member not found!" << endl;
            return;
        }

        if (!book->getStatus()) {
            cout << "Book is not issued!" << endl;
        } else {
            time_t currentTime = time(nullptr);
            time_t issueDate = book->getIssueDate();
            double daysBorrowed = difftime(currentTime, issueDate) / (60 * 60 * 24);
            double fine = 0.0;

            if (daysBorrowed > MAX_BORROW_DAYS) {
                fine = (daysBorrowed - MAX_BORROW_DAYS) * FINE_PER_DAY;
            }

            if (fine > 0.0) {
                cout << "Late return! Fine: $" << fine << endl;
            }

            book->returnBook();
            member->returnBook(bookID);
            cout << "Book Returned Successfully!" << endl;
        }
    }

    void searchBookByTitle(string title) {
        bool found = false;
        for (auto &book : books) {
            if (book.getTitle() == title) {
                book.displayBookInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "No books found with the title \"" << title << "\"" << endl;
        }
    }

    void searchBookByAuthor(string author) {
        bool found = false;
        for (auto &book : books) {
            if (book.getAuthor() == author) {
                book.displayBookInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "No books found by the author \"" << author << "\"" << endl;
        }
    }

    void searchMemberByName(string name) {
        bool found = false;
        for (auto &member : members) {
            if (member.getName() == name) {
                member.displayMemberInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "No members found with the name \"" << name << "\"" << endl;
        }
    }

    void viewAllBooks() {
        for (auto &book : books) {
            book.displayBookInfo();
            cout << "-----------------------" << endl;
        }
    }

    void viewAllMembers() {
        for (auto &member : members) {
            member.displayMemberInfo();
            cout << "-----------------------" << endl;
        }
    }

    void listAllIssuedBooks() {
        bool found = false;
        for (auto &book : books) {
            if (book.getStatus()) {
                book.displayBookInfo();
                cout << "-----------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No books are currently issued." << endl;
        }
    }
};

// Main Function
int main() {
    Library library;
    int choice, bookID, memberID;
    string searchQuery;

    while (true) {
        cout << "========== Library Management System ==========" << endl;
        cout << "1. Add Book\n2. Add Member\n3. Issue Book\n4. Return Book\n5. Search Book by Title\n6. Search Book by Author\n7. Search Member by Name\n8. View All Books\n9. View All Members\n10. List All Issued Books\n11. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.addMember();
                break;
            case 3:
                cout << "Enter Book ID: ";
                cin >> bookID;
                cout << "Enter Member ID: ";
                cin >> memberID;
                library.issueBook(bookID, memberID);
                break;
            case 4:
                cout << "Enter Book ID: ";
                cin >> bookID;
                cout << "Enter Member ID: ";
                cin >> memberID;
                library.returnBook(bookID, memberID);
                break;
            case 5:
                cout << "Enter Book Title: ";
                cin.ignore();
                getline(cin, searchQuery);
                library.searchBookByTitle(searchQuery);
                break;
            case 6:
                cout << "Enter Author Name: ";
                cin.ignore();
                getline(cin, searchQuery);
                library.searchBookByAuthor(searchQuery);
                break;
            case 7:
                cout << "Enter Member Name: ";
                cin.ignore();
                getline(cin, searchQuery);
                library.searchMemberByName(searchQuery);
                break;
            case 8:
                library.viewAllBooks();
                break;
            case 9:
                library.viewAllMembers();
                break;
            case 10:
                library.listAllIssuedBooks();
                break;
            case 11:
                exit(0);
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
