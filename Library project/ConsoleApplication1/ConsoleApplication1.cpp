#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <time.h>
using namespace std;

struct Book {
    string title;
    string author;
    string id;
    bool isBorrowed;
};

vector<Book> theBook;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void addBook() {
    clearScreen();
    Book newBook;
    cout << "Enter the title of the book: ";
    getline(cin, newBook.title);
    cout << "Enter the author of the book: ";
    getline(cin, newBook.author);

    bool bookExists = false;
    int existingBookIndex = -1;

    // التحقق من تكرار الكتاب بناءً على العنوان والمؤلف
    for (int i = 0; i < theBook.size(); ++i) {
        if (theBook[i].title == newBook.title && theBook[i].author == newBook.author) {
            bookExists = true;
            existingBookIndex = i;
            break;
        }
    }

    if (bookExists) {
        cout << "A book with the title \"" << newBook.title << "\" by \"" << newBook.author << "\" already exists." << endl;
        cout << "1. Edit the existing book" << endl;
        cout << "2. Cancel" << endl;
        int choice;
        cin >> choice;
        cin.ignore();

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            clearScreen();
            cout << "Invalid input! Operation canceled." << endl;
            return;
        }

        switch (choice) {
        case 1:
            clearScreen();
            cout << "Editing the existing book..." << endl;
            cout << "Enter new author: ";
            getline(cin, theBook[existingBookIndex].author);
            theBook[existingBookIndex].isBorrowed = false;
            break;
        case 2:
            clearScreen();
            cout << "Operation canceled." << endl;
            return;
        default:
            clearScreen();
            cout << "Invalid choice. Operation canceled." << endl;
            return;
        }
    }
    else {
        newBook.id = to_string(rand());
        newBook.isBorrowed = false;
        theBook.push_back(newBook);
        clearScreen();
        cout << "The book has been added." << endl;
    }

    ofstream file("The Library.txt", ios::app);
    file << "Book " << theBook.size() << endl;
    file << "Title: " << newBook.title << endl;
    file << "Author: " << newBook.author << endl;
    file << "ID: " << newBook.id << endl;
    file << "Is Borrowed: " << (newBook.isBorrowed ? "Yes" : "No") << endl;
    file << "--------------------------" << endl << endl;
    file.close();
}

void displayBooks()
{
    clearScreen();
    if (theBook.empty()) {
        cout << "No books available in the library." << endl;
        return;
    }
    cout << "Displaying all books in the library:" << endl;
    cout << "-----------------------------------" << endl;
    int bookNumber = 1;
    for (const auto& book : theBook) {
        cout << "Book " << bookNumber << ":" << endl;
        cout << "  Title      : " << book.title << endl;
        cout << "  Author     : " << book.author << endl;
        cout << "  ID         : " << book.id << endl;
        cout << "  Borrowed   : " << (book.isBorrowed ? "Yes" : "No") << endl;
        cout << "-----------------------------------" << endl;
        ++bookNumber;
    }
    cout << endl;
}

void searchBook()
{
    clearScreen();
    string searchTitle;
    cout << "Enter title to search: ";
    getline(cin, searchTitle);
    cin.ignore();
    bool found = false;
    for (const auto& book : theBook)
    {
        if (book.title == searchTitle)
        {
            clearScreen();
            cout << "Book found:" << endl;
            cout << "----------------------------------------" << endl;
            cout << "Title: " << book.title << endl;
            cout << "Author: " << book.author << endl;
            cout << "ID: " << book.id << endl;
            cout << "Status: " << (book.isBorrowed ? "Borrowed" : "Available") << endl;
            cout << "----------------------------------------" << endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        clearScreen();
        cout << "No book found with the title \"" << searchTitle << "\"." << endl;
    }
}

void borrowBook()
{
    clearScreen();

    string searchTitle;
    cout << "Enter the title of the book to borrow: ";
    getline(cin, searchTitle);
    cin.ignore();
    vector<int> bookIndices;
    for (int i = 0; i < theBook.size(); ++i) {
        if (theBook[i].title == searchTitle && !theBook[i].isBorrowed) {
            bookIndices.push_back(i);
        }
    }

    if (bookIndices.empty()) {
        clearScreen();
        cout << "No available books found with the title \"" << searchTitle << "\"." << endl;
        return;
    }

    cout << "Available books with the title \"" << searchTitle << "\":" << endl;
    for (int i = 0; i < bookIndices.size(); ++i) {
        const Book& b = theBook[bookIndices[i]];
        cout << "Book " << (i + 1) << ":" << endl;
        cout << "Title: " << b.title << endl;
        cout << "Author: " << b.author << endl;
        cout << "ID: " << b.id << endl;
        cout << "Is Borrowed: " << (b.isBorrowed ? "Yes" : "No") << endl;
        cout << "--------------------------" << endl;
    }

    int borrowChoice;
    cout << "Enter the number of the book you want to borrow: ";
    cin >> borrowChoice;

    if (borrowChoice < 1 || borrowChoice > bookIndices.size()) {
        clearScreen();
        cout << "Invalid choice. Operation canceled." << endl;
        return;
    }

    int bookIndexToBorrow = bookIndices[borrowChoice - 1];
    theBook[bookIndexToBorrow].isBorrowed = true;

    clearScreen();
    cout << "Book borrowed successfully!" << endl;
    ofstream file("The Library.txt");
    for (size_t i = 0; i < theBook.size(); ++i) {
        file << "Book " << (i + 1) << endl;
        file << "Title: " << theBook[i].title << endl;
        file << "Author: " << theBook[i].author << endl;
        file << "ID: " << theBook[i].id << endl;
        file << "Is Borrowed: " << (theBook[i].isBorrowed ? "Yes" : "No") << endl;
        file << "--------------------------" << endl;
    }
    file.close();
}
void returnBook()
{
    clearScreen();
    string searchTitle;
    cout << "Enter the title of the book to return: ";
    getline(cin, searchTitle);
    cin.ignore();
    vector<int> bookIndices;
    for (int i = 0; i < theBook.size(); ++i) {
        if (theBook[i].title == searchTitle && theBook[i].isBorrowed) {
            bookIndices.push_back(i);
        }
    }

    if (bookIndices.empty()) {
        clearScreen();
        cout << "No borrowed books found with the title \"" << searchTitle << "\"." << endl;
        return;
    }

    cout << "Borrowed books with the title \"" << searchTitle << "\":" << endl;
    for (int i = 0; i < bookIndices.size(); ++i) {
        const Book& b = theBook[bookIndices[i]];
        cout << "Book " << (i + 1) << ":" << endl;
        cout << "Title: " << b.title << endl;
        cout << "Author: " << b.author << endl;
        cout << "ID: " << b.id << endl;
        cout << "Is Borrowed: " << (b.isBorrowed ? "Yes" : "No") << endl;
        cout << "--------------------------" << endl;
    }

    int returnChoice;
    cout << "Enter the number of the book you want to return: ";
    cin >> returnChoice;
    if (returnChoice < 1 || returnChoice > bookIndices.size()) {
        clearScreen();
        cout << "Invalid choice. Operation canceled." << endl;
        return;
    }

    int bookIndexToReturn = bookIndices[returnChoice - 1];
    theBook[bookIndexToReturn].isBorrowed = false;

    clearScreen();
    cout << "Book returned successfully!" << endl;

    ofstream file("The Library.txt");
    for (size_t i = 0; i < theBook.size(); ++i) {
        file << "Book " << (i + 1) << endl;
        file << "Title: " << theBook[i].title << endl;
        file << "Author: " << theBook[i].author << endl;
        file << "ID: " << theBook[i].id << endl;
        file << "Is Borrowed: " << (theBook[i].isBorrowed ? "Yes" : "No") << endl;
        file << "--------------------------" << endl;
    }
    file.close();
}
void deleteBook()
{
    clearScreen();
    string searchTitle;
    cout << "Enter the title of the book to delete: ";
    getline(cin, searchTitle);
    cin.ignore();
    vector<int> bookIndices;
    for (int i = 0; i < theBook.size(); ++i) {
        if (theBook[i].title == searchTitle) {
            bookIndices.push_back(i);
        }
    }
    if (bookIndices.empty()) {
        clearScreen();
        cout << "No books found with the title \"" << searchTitle << "\"." << endl;
        return;
    }
    cout << "Books with the title \"" << searchTitle << "\":" << endl;
    for (int i = 0; i < bookIndices.size(); ++i) {
        const Book& b = theBook[bookIndices[i]];
        cout << "Book " << (i + 1) << ":" << endl;
        cout << "Title: " << b.title << endl;
        cout << "Author: " << b.author << endl;
        cout << "ID: " << b.id << endl;
        cout << "Is Borrowed: " << (b.isBorrowed ? "Yes" : "No") << endl;
        cout << "--------------------------" << endl;
    }
    int deleteChoice;
    cout << "Enter the number of the book you want to delete: ";
    cin >> deleteChoice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        clearScreen();
        cout << "Invalid input! Please enter a number between 1 and 8." << endl;
    }
    if (deleteChoice < 1 || deleteChoice > bookIndices.size()) {
        clearScreen();
        cout << "Invalid choice. Operation canceled." << endl;
        return;
    }

    int bookIndexToDelete = bookIndices[deleteChoice - 1];
    theBook.erase(theBook.begin() + bookIndexToDelete);

    clearScreen();
    cout << "Book deleted successfully!" << endl;
    ofstream file("The Library.txt");
    for (size_t i = 0; i < theBook.size(); ++i) {
        file << "Book " << (i + 1) << endl;
        file << "Title: " << theBook[i].title << endl;
        file << "Author: " << theBook[i].author << endl;
        file << "ID: " << theBook[i].id << endl;
        file << "Is Borrowed: " << (theBook[i].isBorrowed ? "Yes" : "No") << endl;
        file << "--------------------------" << endl;
    }
    file.close();
}
void saveBooksToFile(const string& filename) {
    clearScreen();
    ofstream file(filename);
    if (!file) {
        cout << "Error opening file for saving." << endl;
        return;
    }

    for (size_t i = 0; i < theBook.size(); ++i) {
        file << "Book " << i + 1 << endl;
        file << "Title: " << theBook[i].title << endl;
        file << "Author: " << theBook[i].author << endl;
        file << "ID: " << theBook[i].id << endl;
        file << "Is Borrowed: " << (theBook[i].isBorrowed ? "Yes" : "No") << endl;
        file << "--------------------------" << endl;
    }
    file.close();
    cout << "Changes saved successfully." << endl;
}

void loadBooksFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file for loading." << endl;
        return;
    }

    theBook.clear();
    string line;
    Book tempBook;

    while (getline(file, line)) {
        if (line.find("Book") != string::npos) {
            if (!tempBook.title.empty()) {
                theBook.push_back(tempBook);
                tempBook = Book();
            }
        }
        else if (line.find("Title:") != string::npos) {
            tempBook.title = line.substr(line.find(":") + 2);
        }
        else if (line.find("Author:") != string::npos) {
            tempBook.author = line.substr(line.find(":") + 2);
        }
        else if (line.find("ID:") != string::npos) {
            tempBook.id = line.substr(line.find(":") + 2);
        }
        else if (line.find("Is Borrowed:") != string::npos) {
            string isBorrowed = line.substr(line.find(":") + 2);
            tempBook.isBorrowed = (isBorrowed == "Yes");
        }
    }

    if (!tempBook.title.empty()) {
        theBook.push_back(tempBook);
    }

    file.close();
    cout << "Books loaded successfully." << endl;
}

int main() {
    int choice;
    loadBooksFromFile("The Library.txt");
    srand(time(0));
    do {
        cout << "\n\t\t\t\t Press \"Save\" or \"Exit\" to save your changes\n";
        cout << "Library Menu:" << endl;
        cout << "1. Add a Book" << endl;
        cout << "2. Display Books" << endl;
        cout << "3. Search for a Book" << endl;
        cout << "4. Borrow a Book" << endl;
        cout << "5. Return a Book" << endl;
        cout << "6. Delete a Book" << endl;
        cout << "7. Save Books" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            clearScreen();
            cout << "Invalid input! Please enter a number between 1 and 8." << endl;
            continue;
        }
        switch (choice)
        {
        case 1: addBook(); break;
        case 2: displayBooks(); break;
        case 3: searchBook(); break;
        case 4: borrowBook(); break;
        case 5: returnBook(); break;
        case 6: deleteBook(); break;
        case 7: saveBooksToFile("The Library.txt"); break;
        case 8: cout << "Exiting..." << endl; break;
        default: cout << "Invalid choice!" << endl; break;
        }
    } while (choice != 8);
    return 0;
}