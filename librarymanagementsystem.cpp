#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <string>

using namespace std;

// Structures for data storage
struct Book {
    char title[100];
    char author[50];
    char category[50];
    char isbn[20];
    int totalCopies;
    int availableCopies;
};

struct User {
    int id;
    char name[50];
    Book* borrowedBooks[5]; // Pointers to borrowed books, max 5
    int borrowedCount;
};

// Global arrays and counters
Book books[200];
User users[100];
char categories[50][50];
int bookCount = 0;
int userCount = 0;
int categoryCount = 0;



// Initialize categories
void initializeCategories() {
    strcpy(categories[0], "Programming");
    strcpy(categories[1], "Calculus");
    strcpy(categories[2], "Linear Algebra");
    strcpy(categories[3], "Islamic Studies");
    strcpy(categories[4], "Discrete");
    categoryCount = 5;
}

// Initialize the library with 150 sample books across categories
void initializeBooks() {
    initializeCategories();
    int booksPerCategory = 30;
    int index = 0;

    for (int cat = 0; cat < categoryCount; cat++) {
        for (int i = 1; i <= booksPerCategory; i++) {
            strcpy(books[index].title, categories[cat]);
            strcat(books[index].title, " Book ");
            char num[10];
            itoa(i, num, 10);
            strcat(books[index].title, num);

            strcpy(books[index].author, "Author ");
            strcat(books[index].author, num);

            strcpy(books[index].category, categories[cat]);

            char isbnStr[4];
            sprintf(isbnStr, "%03d", index + 1);
            strcpy(books[index].isbn, isbnStr);

            books[index].totalCopies = 1;
            books[index].availableCopies = 1;
            index++;
        }
    }
    bookCount = 150;
}

// File handling functions
// Load book data from file or initialize if file not found or empty
void loadBooksFromFile() {
    initializeCategories(); // Always initialize categories
    ifstream file("books.txt");
    if (file.is_open()) {
        bookCount = 0;
        while (file >> books[bookCount].title >> books[bookCount].author 
            >> books[bookCount].category >> books[bookCount].isbn 
            >> books[bookCount].totalCopies 
            >> books[bookCount].availableCopies) {
            bookCount++;
            if (bookCount >= 200) break;
        }
        file.close();
        if (bookCount == 0) {
            initializeBooks();
        }
    } else {
        initializeBooks();
    }
}

// Save book data to file
void saveBooksToFile() {
    ofstream file("books.txt");
    if (file.is_open()) {
        for (int i = 0; i < bookCount; i++) {
            file << books[i].title << " " 
            << books[i].author << " " << books[i].category << " " 
            << books[i].isbn << " " << books[i].totalCopies << " " 
            << books[i].availableCopies << endl;
        }
        file.close();
    }
}

// Display all available categories
void displayCategories() {
    cout << "Available categories:" << endl;
    for (int i = 0; i < categoryCount; i++) {
        cout << i + 1 << ". " << categories[i] << endl;
    }
}

// Add a new category
void addCategory() {
    if (categoryCount >= 50) {
        cout << "Maximum categories reached!" << endl;
        return;
    }
    cout << "Enter new category name: ";
    cin.ignore();
    cin.getline(categories[categoryCount], 50);
    categoryCount++;
    cout << "Category added successfully!" << endl;
}

// Book management functions
// Add a new book to the library
void addBook() {
    cout << "Enter title: ";
    cin.ignore();
    char title[100];
    cin.getline(title, 100);
    cout << "Enter author: ";
    char author[50];
    cin.getline(author, 50);
    cout << "Enter category name: ";
    char category[50];
    cin.getline(category, 50);
    cout << "Enter ISBN: ";
    char isbn[20];
    cin.getline(isbn, 20);
    cout << "Enter number of copies: ";
    int copies;
    cin >> copies;
    if (bookCount >= 200) {
        cout << "Library is full! Could not add the book." << endl;
        return;
    }
    strcpy(books[bookCount].title, title);
    strcpy(books[bookCount].author, author);
    strcpy(books[bookCount].category, category);
    strcpy(books[bookCount].isbn, isbn);
    books[bookCount].totalCopies = copies;
    books[bookCount].availableCopies = copies;
    bookCount++;
    cout << copies << " copies of the book added successfully!" << endl;
}

// Remove a book by category, then ISBN or author
void removeBook() {
    char category[50];
    cout << "Enter category name: ";
    cin.ignore();
    cin.getline(category, 50);
    cout << "Remove by (1) ISBN or (2) Author: ";
    int choice;
    cin >> choice;
    if (choice == 1) {
        char isbn[20];
        cout << "Enter ISBN to remove: ";
        cin.ignore();
        cin.getline(isbn, 20);
        for (int i = 0; i < bookCount; i++) {
            if (strcmp(books[i].category, category) == 0 && strcmp(books[i].isbn, isbn) == 0) {
                for (int j = i; j < bookCount - 1; j++) {
                    books[j] = books[j + 1];
                }
                bookCount--;
                cout << "Book removed successfully!" << endl;
                return;
            }
        }
        cout << "Book not found in this category!" << endl;
    } else if (choice == 2) {
        char author[50];
        cout << "Enter author to remove all books by: ";
        cin.ignore();
        cin.getline(author, 50);
        int removed = 0;
        for (int i = 0; i < bookCount; i++) {
            if (strcmp(books[i].category, category) == 0 && strcmp(books[i].author, author) == 0) {
                for (int j = i; j < bookCount - 1; j++) {
                    books[j] = books[j + 1];
                }
                bookCount--;
                removed++;
                i--; // Adjust index after removal
            }
        }
        if (removed > 0) {
            cout << removed << " book(s) by " << author << " removed successfully!" << endl;
        } else {
            cout << "No books by this author found in this category!" << endl;
        }
    } else {
        cout << "Invalid choice!" << endl;
    }
}

// Search books by title or author in a specific category
void searchBook() {
    char category[50];
    cout << "Enter the specific category name to search in: ";
    cin.ignore();
    cin.getline(category, 50);
    char query[100];
    cout << "Enter title or author to search: ";
    cin.getline(query, 100);
    bool found = false;
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].category, category) == 0 && 
        (strstr(books[i].title, query) || strstr(books[i].author, query))) {
            cout << "Title: " << books[i].title << ", Author: " << books[i].author << ", Category: " 
            << books[i].category << ", ISBN: " << books[i].isbn << ", Copies: " << books[i].totalCopies 
            << ", Available: " << books[i].availableCopies << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No books found in this category!" << endl;
    }
}

// Display books by category or all
void displayBooks() {
    char category[50];
    cout << "Enter category (or 'all' for all): ";
    cin.ignore();
    cin.getline(category, 50);
    cout << right << setw(5) << "No." << left << setw(50) 
    << "Title" << setw(30) << "Author" << setw(20) 
    << "Category" << setw(10) << "ISBN" << setw(10) 
    << "Total" << setw(10) << "Available" << endl;
    cout << string(135, '-') << endl;
    int count = 1;
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(category, "all") == 0 || strcmp(books[i].category, category) == 0) {
            cout << right << setw(5) << count << "   " 
            << left << setw(50) << books[i].title << setw(30) 
            << books[i].author << setw(20) << books[i].category 
            << setw(10) << books[i].isbn << setw(10) << books[i].totalCopies \
            << setw(10) << books[i].availableCopies << endl;
            count++;
        }
    }
}

// Function to issue a book
void issueBook() {
    int userId;
    char isbn[20];
    cout << "Enter user ID: ";
    cin >> userId;
    cout << "Enter ISBN to issue: ";
    cin.ignore();
    cin.getline(isbn, 20);
    // Find user
    User* user = NULL;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            user = &users[i];
            break;
        }
    }
    if (!user) {
        // Add new user
        if (userCount >= 100) {
            cout << "Maximum users reached!" << endl;
            return;
        }
        users[userCount].id = userId;
        cout << "Enter user name: ";
        cin.ignore();
        cin.getline(users[userCount].name, 50);
        users[userCount].borrowedCount = 0;
        user = &users[userCount];
        userCount++;
    }
    if (user->borrowedCount >= 5) {
        cout << "User has reached max borrowed books!" << endl;
        return;
    }
    // Find book
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].isbn, isbn) == 0 && books[i].availableCopies > 0) {
            books[i].availableCopies--;
            user->borrowedBooks[user->borrowedCount] = &books[i];
            user->borrowedCount++;
            cout << "Book issued successfully!" << endl;
            return;
        }
    }
    cout << "Book not available!" << endl;
}

// Function to return a book
void returnBook() {
    int userId;
    char isbn[20];
    cout << "Enter user ID: ";
    cin >> userId;
    cout << "Enter ISBN to return: ";
    cin.ignore();
    cin.getline(isbn, 20);
    // Find user
    User* user = NULL;
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == userId) {
            user = &users[i];
            break;
        }
    }
    if (!user) {
        cout << "User not found!" << endl;
        return;
    }
    // Find book in borrowed
    for (int i = 0; i < user->borrowedCount; i++) {
        if (strcmp(user->borrowedBooks[i]->isbn, isbn) == 0) {
            user->borrowedBooks[i]->availableCopies++;
            for (int j = i; j < user->borrowedCount - 1; j++) {
                user->borrowedBooks[j] = user->borrowedBooks[j + 1];
            }
            user->borrowedCount--;
            cout << "Book returned successfully!" << endl;
            return;
        }
    }
    cout << "Book not borrowed by this user!" << endl;
}

// Menu and main program functions
// Display the main menu
void showMenu() {
    cout << "\n";
    cout << "********************************************" << endl;
    cout << "*                                          *" << endl;
    cout << "*        LIBRARY MANAGEMENT SYSTEM         *" << endl;
    cout << "*                                          *" << endl;
    cout << "********************************************" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Remove Book" << endl;
    cout << "3. Search Book" << endl;
    cout << "4. Display Books" << endl;
    cout << "5. Issue Book" << endl;
    cout << "6. Return Book" << endl;
    cout << "7. Add Category" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter choice: ";
}

// Main program entry point
int main() {
    loadBooksFromFile();
    // Add a sample user for testing
    users[0].id = 1;
    strcpy(users[0].name, "Test User");
    userCount = 1;
    int choice;
    do {
        showMenu();
        cin >> choice;
        cout << "==================================================" << endl;
        switch (choice) {
            case 1:
                displayCategories();
                addBook();
                break;
            case 2:
                displayCategories();
                removeBook();
                break;
            case 3:
                displayCategories();
                searchBook();
                break;
            case 4:
                displayCategories();
                displayBooks();
                break;
            case 5:
                issueBook();
                break;
            case 6:
                returnBook();
                break;
            case 7:
                addCategory();
                break;
            case 8:
                saveBooksToFile();
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 8);
    return 0;
}
