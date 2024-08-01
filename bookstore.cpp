#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <iomanip>
#include <numeric>
#include <cctype>
#include <algorithm>
using namespace std;

bool exitCue; // Declare as a global variable
const vector<string> BKCAT = {"Fiction", "NonFiction"}; // Declare category as global var
const vector<string> PAYMETHODS = {"Cash", "Card"}; // Declare payment methods as global const var

/* ---------------------------------
// Utility method
// --------------------------------- */
// Validate the integer based selection is not invalid before further processing
int validateIntSelection(int def, int min, int max) {
    
    string input; // A local string input variable for cin purpose
    int tempval, selection; // A local selection variable to store the final selection value
    
    // Making sure we catch unexpected error, like empty input and string input is given instead of integer
    try {
        getline(cin, input);
        if (input.empty()) {throw invalid_argument("Empty input.");}
        int tempval = stoi(input); // convert string to integer
        if (tempval < min || tempval > max) {
            throw invalid_argument("Input is out of range.");
        } else {
            selection = tempval;
        }
        
    } catch (...) {
        cout << "Invalid selection. Default to " << def << endl;
        selection = def;
    }
    
    return selection;
}

// Function to validate card number using Luhn's algorithm
bool isValidCardNumber(string cardNumber) {
    int sum = 0;
    bool doubleDigit = false;

    // Traverse the card number from the end to the start
    for (int i = cardNumber.size() - 1; i >= 0; --i) {
        char digitChar = cardNumber[i];
        
        // Skip non-digit characters
        if (!isdigit(digitChar)) {
            continue;
        }
        
        // Get the actual integer by subtracting from the char '0'
        int digit = digitChar - '0';

        // Double every second digit from the end
        if (doubleDigit) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9; // If the result is greater than 9, subtract 9
            }
        }

        sum += digit;
        doubleDigit = !doubleDigit; // Toggle the doubleDigit flag
    }

    // A valid card number will have a sum that is a multiple of 10
    return (sum % 10 == 0);
}

// Function to validate a 3-digit CVV
bool isValidCVV(string cvv) {
    // Check if the CVV length is exactly 3
    if (cvv.length() != 3) {
        return false;
    }

    // Check if all characters are digits
    for (char ch : cvv) {
        if (!isdigit(ch)) {
            return false;
        }
    }

    return true;
}

// validate and return card number
string validateCardNumber(){
    string input;
    
    getline(cin, input);
    if(input.empty()) {
        return "Terminate";
    } else {
        if(isValidCardNumber(input)) {
            return input;
        } else {
            return "Again";
        }
    }
}

// validate and return CVV
string validateCVV(){
    string input;
    
    getline(cin, input);
    if(input.empty()) {
        return "Terminate";
    } else {
        if(isValidCVV(input)) {
            return input;
        } else {
            return "Again";
        }
    }
}

// Clear the terminal
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Output the screen name at the start of every screen
void showScreenName(string screenName) {
    cout << endl;
    cout << "[" << screenName << " screen]" << endl;
}

/* ---------------------------------
// Class definition
// --------------------------------- */

// Book class 
// Characterise a book
class Book {
public:
    string title, author, description;
    int category, stock, intermediateStock;
    double price;
    
    Book(string t, string a, string d, int c, int s, double p) {
        title = t;
        author = a;
        description = d;
        category = c;
        stock = s;
        intermediateStock = s;
        price = p;
    }
};

// Cart class 
// Define the characteristic of a cart instance. 
// Each successful transaction should reinitialise a new cart
// Contains some cart specific functions to assist in cart management 
class Cart {
public:
    vector<Book*> selectedBooks;
    vector<int> quantity;
    vector<double> totalAmount;
    double grandTotal;
    
    // Constructor
    Cart(vector<Book*> b = {}, vector<int> q = {}, vector<double> t = {}, double g = 0) : selectedBooks(b), quantity(q), totalAmount(t), grandTotal(g) {}
    
    // Utility function
    // Check if the cart is empty
    bool isEmpty() {
        return selectedBooks.size() > 0? false : true;
    }
    
    // Calculate grand total
    double calculateGrandTotal() {
        return totalAmount.empty()? 0.00 : accumulate(totalAmount.begin(), totalAmount.end(), 0.00);
    }
    
    // Display book titles in current cart
    void displayAllBookTitlesInCart() {
        // For loop to display all book titles in the cart
        for (int i = 0; i < selectedBooks.size(); i++) {
            cout << i + 1 << ". " << selectedBooks[i]->title << endl;
        }
    }
    
    // Main functions
    // This function modifies the cart, i.e. change the quantity of the item selected
    void modifyCart() {
        int selection = 0;
        int modQuantity = 0;
        
        cout << endl;
        cout << "Which item do you want to modify / change the quantity? " << endl;
        
        displayAllBookTitlesInCart(); // From utility function in Cart
        
        // Ask user to select a book to modify
        cout << "Please input a valid index/number: ";
        selection = validateIntSelection(0,0,selectedBooks.size());
        
        if (selection == 0) {
            cout << "Nothing change. Returning back to cart..." << endl;
        } else {
            // Ask for user input on new quantity
            cout << endl;
            cout << "You are currently modifying the quantity of the following book:" << endl;
            cout << selectedBooks[selection - 1]->title << endl;
            cout << "Current quantity: " << quantity[selection - 1] << endl;
            cout << "New quantity [1 - " << selectedBooks[selection - 1]->stock << "]: ";
            modQuantity = validateIntSelection(0,0,selectedBooks[selection - 1]->stock);
            
            if (modQuantity == 0) {
                cout << "Nothing change. Returning back to cart..." << endl;
            } else {
                // Change the quantity, intermediateStock value, totalAmount per item and the grand total
                quantity[selection - 1] = modQuantity;
                selectedBooks[selection - 1]->intermediateStock = selectedBooks[selection - 1]->stock - modQuantity;
                totalAmount[selection - 1] = selectedBooks[selection - 1]->price * modQuantity;
                grandTotal = calculateGrandTotal();
            }
        }
    }
    
    // This function allows user to delete a book from cart
    void deleteBook() {
        int selection = 0;
        
        cout << endl;
        cout << "Which book do you want to delete from cart?" << endl;
        displayAllBookTitlesInCart(); // From utility function in Cart
        cout << "Please input a valid number/index: ";
        selection = validateIntSelection(0,0,selectedBooks.size());
        
        if (selection == 0) {
            cout << "Nothing change. Returning back to cart..." << endl;
        } else {
            // Get the title of the book to be deleted from cart
            string deletingBook = selectedBooks[selection - 1]->title;
            
            // Because the element is being erased, the intermediate stock value should now follow the actual stock value
            selectedBooks[selection - 1]->intermediateStock = selectedBooks[selection - 1]->stock;
            
            // Afterwards, we erase the book element and its corresponding details (quantity, amount) at specified position
            selectedBooks.erase(selectedBooks.begin() + selection - 1);
            quantity.erase(quantity.begin() + selection - 1);
            totalAmount.erase(totalAmount.begin() + selection - 1);
            
            // Then we recalculate the grand total
            grandTotal = calculateGrandTotal();
            
            cout << "The book \"" << deletingBook << "\" has been deleted." << endl;
        }
    }
    
    // This function allows user to clear the cart manually
    void clearCart() {
        int selection = 0;
        
        cout << endl;
        cout << "WARNING: By selecting this option, all the items in the cart will be removed. Are you sure you want to proceed?" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cout << "Please input 1 (Yes) or 2 (No): ";
        selection = validateIntSelection(0,0,2);
        
        // If selection is No or invalid
        if (selection == 0 || selection == 2) {
            cout << "Nothing change. Returning back to cart..." << endl;
        } else {
            // We first need to reinitialise the intermediateStock value to actual stock value as we are going to remove all books from cart
            // We use a for loop with auto& book : selectedBooks to loop through all books
            for (auto* book : selectedBooks) {
                book->intermediateStock = book->stock;
            }
            
            // We clear the vectors storing the selectedBooks, quantity and totalAmount
            selectedBooks.clear();
            quantity.clear();
            totalAmount.clear();
            
            // Then we recalculate the grand total
            grandTotal = calculateGrandTotal();
            
            cout << "The cart has been cleared." << endl;
        }
    }
    
    // This function allows user to clear the cart automatically upon each transaction so that the cart is reinitialised
    void payAndClearCart() {
        // Once paid, the actual stock value should now follow the intermediate stock value
        for (auto* book : selectedBooks) {
            book->stock = book->intermediateStock;
        }
            
        // We clear the vectors storing the selectedBooks, quantity and totalAmount
        selectedBooks.clear();
        quantity.clear();
        totalAmount.clear();
            
        // Then we clear the grand total
        grandTotal = 0;
    }
};

// Templates class 
// Stores templates that display the cart, invoice/receipt, bookwares
class Templates {
public:
    int w_universal; // universal width
    vector<int> wr; // Receipt/Invoice width
    vector<int> ws; // Sales report width
    vector<int> wb; // Sales report width

    // Constructor
    Templates() {
        w_universal = 20;
        wr = {7,25,15,10,13,13}; // Receipt/Invoice width
        ws = {10,20,20,20,8,15}; // Sales report width;
        wb = {10,30,20,10,10}; // Bookware width
    }
    
    // Template for invoice/receipt spacer
    void invoiceReceipt_Spacer() {
        int widthsum = accumulate(wr.begin(), wr.end(), 0);
        for (int i = 0; i < widthsum; i++) {cout << "-";}
        cout << endl;
    }
    
    // Template for invoice/receipt
    void invoiceReceipt(vector<Book*> books, vector<int> quantity, vector<double> totalAmount, double grandTotal, string name="", int paymethodint=0, string cardNum="") {
        
        bool isReceipt = paymethodint == 0? false : true;
        cout << fixed << setprecision(2); // Set precision for price
        
        if (isReceipt) {
            cout << setw(w_universal) << "Name: " 
                 << name
                 << endl;
    
            cout << setw(w_universal) << "Payment by: " 
                 << PAYMETHODS[paymethodint - 1]
                 << endl;
        
            // If payment by card, show card number
            // Else, don't show anything
            if (paymethodint == 2) {
                cout << setw(w_universal) << "Card Number: " 
                     << cardNum
                     << endl;
            }
        }
        
        invoiceReceipt_Spacer();
        
        // Set the header
        cout << setw(wr[0]) << left << "Item"
             << setw(wr[1]) << "Title" 
             << setw(wr[2]) << "Category" 
             << setw(wr[3]) << "Quantity"
             << setw(wr[4]) << "Unit Price" 
             << setw(wr[5]) << "Subtotal" << endl;

        invoiceReceipt_Spacer();
        
        // Set the content
        // 1 row per element
        for (int i = 0; i < books.size() ;i++) { 
            cout << setw(wr[0]) << left << i + 1
                 << setw(wr[1]) << books[i]->title
                 << setw(wr[2]) << BKCAT[books[i]->category]
                 << setw(wr[3]) << quantity[i]
                 << setw(wr[4]) << books[i]->price
                 << setw(wr[5]) << totalAmount[i]
                 << endl;
        }
    
        invoiceReceipt_Spacer(); // Spacer of invoice/receipt from Templates class
        
        // Show grand total in a cart
        cout << setw(wr[0]+wr[1]+wr[2]+wr[3]) << left << " "
             << setw(wr[4]) << "Total" 
             << setw(wr[5]) << grandTotal
             << endl;
        
        invoiceReceipt_Spacer(); // Spacer of invoice/receipt from Templates class
    }
    
    // Template for sales report spacer
    void salesReport_Spacer() {
        int widthsum = accumulate(ws.begin(), ws.end(), 0);
        for (int i = 0; i < widthsum; i++) {cout << "-";}
        cout << endl;
    }
    
    // Template for sales report
    void salesReport(vector<string> customerNames, vector<int>paymethodsint, vector<string> cardNum, vector<string> CVVs, vector<Cart> carts, double grandSalesAmount) {
        
        cout << fixed << setprecision(2); // Set precision for price
        
        salesReport_Spacer(); // Spacer of sales report from Templates class
    
        // Set the header
        cout << setw(ws[0]) << left << "SalesID"
             << setw(ws[1]) << "Customer Names" 
             << setw(ws[2]) << "Payment Methods"
             << setw(ws[3]) << "Card Number"
             << setw(ws[4]) << "CVV"
             << setw(ws[5]) << "Sales Amount"
             << endl;
        
        salesReport_Spacer(); // Spacer of sales report from Templates class
        
        // Set the content
        // 1 row per element
        for (int i = 0; i < customerNames.size(); i++) {
        
        cout << setw(ws[0]) << left << i + 3
             << setw(ws[1]) << customerNames[i]
             << setw(ws[2]) << PAYMETHODS[paymethodsint[i] - 1]
             << setw(ws[3]) << cardNum[i]
             << setw(ws[4]) << CVVs[i]
             << setw(ws[5]) << carts[i].grandTotal
             << endl;
        }
        
        salesReport_Spacer(); // Spacer of sales report from Templates class
        
        // Show grand sales total
        cout << setw(ws[0] + ws[1] + ws[2]) << left << " "
         << setw(ws[3] + ws[4]) << "Grand Sales Amount (RM) "
         << setw(ws[5]) << grandSalesAmount 
         << endl;
    
        salesReport_Spacer();
    }
    
    // Template for salesReport footer option
    void salesReportFooterOption(vector<string> customerNames) {
        // Provide the options for next action below the sales report
        // setw() is to ensure the output is formatted and aligned
        cout << setw(w_universal/2 + 5) << "1. " << "Returning back to previous screen..." << endl;
        cout << setw(w_universal/2 + 5) << "2. " << "View sales report summary again." << endl;
        if(!customerNames.empty()) {
            cout << setw(w_universal/2 + 5) << "[SalesID]." << "A detailed look into individual transaction." << endl;
        }
    }
    
    // Template for bookware spacer
    void bookware_Spacer() {
        int widthsum = accumulate(wb.begin(), wb.end(), 0);
        for (int i = 0; i < widthsum; i++) {cout << "-";}
        cout << endl;
    }
    
    // Template for bookware
    int bookware(int selectedCategory, vector<Book> booksToSell) {
        
        int bookIndex = 1; // This index number will be passed as a function output later for other use.
        
        cout << fixed << setprecision(2); // Set precision for price
        
        cout << "Books available in " << BKCAT[selectedCategory] << " category:" << endl;
        
        bookware_Spacer(); // Spacer of bookware from Templates class
        
        // Set header
        cout << setw(wb[0]) << left << "Item"
             << setw(wb[1]) << "Title" 
             << setw(wb[2]) << "Author" 
             << setw(wb[3]) << "Price" 
             << setw(wb[4]) << "Stock" 
             << endl;
        
        bookware_Spacer(); // Spacer of bookware from Templates class
        
        // Set content
        // auto&: automatically deduces the type of book and ensures that book is a reference to each element in the container booksToSell. The & means that book will not be a copy of the element, but a reference to it. This allows modifications to the actual elements in the container.
        // book: This is the loop variable that represents each element in the booksToSell container during each iteration of the loop.
        for (auto& book : booksToSell) {
            if (book.category == selectedCategory) {
                cout << setw(wb[0]) << left << bookIndex
                     << setw(wb[1]) << book.title 
                     << setw(wb[2]) << book.author 
                     << setw(wb[3]) << book.price 
                     << setw(wb[4]) << book.intermediateStock << endl;
            
                bookIndex++;
            }
        }
        
        bookware_Spacer(); // Spacer of bookware from Templates class
        
        return bookIndex;
    }
};

// SalesManager class 
// Define the characteristic of transactions, register transactions and store
class SalesManager {
    public:
    vector<string> customerNames;
    vector<Cart> carts;
    vector<int> payMethodsInt;
    vector<string> cardNum;
    vector<string> CVVs;
    double grandSalesAmount;
    
    // Constructor
    SalesManager(vector<string> cust = {}, vector<Cart> ct = {}, vector<int> p = {}, vector<string> cN = {}, vector<string> cv = {}, double gsa = 0) : customerNames(cust), carts(ct), payMethodsInt(p), cardNum(cN), CVVs(cv), grandSalesAmount(gsa) {}
    
    // Register a transaction
    void registerSale(string custName, Cart cart, int cashorcard, string cardNumber, string CVV) {
        
        // Add each transaction characteristic to corresponding vector as element
        customerNames.push_back(custName);
        payMethodsInt.push_back(cashorcard);
        cardNum.push_back(cardNumber);
        CVVs.push_back(CVV);
        carts.push_back(cart);
        
        // Reset to 0 so that we can recalculate the grand sales amount
        grandSalesAmount = 0; 
        for (int i = 0; i < carts.size(); i++) {
            grandSalesAmount += carts[i].grandTotal;
        }
    }
};

/* ---------------------------------
// Screen Methods - methods involving screen logics and managing different screens
// --------------------------------- */

// Screen Landing - allow user to select an option, either to order, or to view the total sales on that day.
string screenLanding(int& userSelectedCategory) {

    int selection;
    string input, switch2ScreenName;
    
    // Clear Screen
    // clearScreen();
    
    // Reset this reference variable whenever working on this "Landing" screen
    userSelectedCategory = 0;
    
    // Print the message to let user select
    cout << "Welcome to the Alien BookStore. Please select an option to begin." << endl;
    cout << "1. Proceed to buy books." << endl;
    cout << "2. Proceed to view total daily sales." << endl;
    cout << "3. Exit the program." << endl;
    cout << "Please input a valid number: ";
    selection = validateIntSelection(1,1,3);
    
    // Depending on which selection, assign different next screen name to be passed to switchScreen() function
    switch (selection) {
        case 1:
            switch2ScreenName = "MenuCategory";
            break;
        
        case 2:
            switch2ScreenName = "Reporting";
            break;
            
        case 3:
            cout << "Bye bye!" << endl;
            exitCue = true;
    }
    
    return switch2ScreenName;
}

// Screen Menu Category - to let user to choose which book category they are interested in. this function will return a string that indicates the next screen and another string that indicates 
pair<string, int> screenMenuCategory(string& previousScreenName) {
    int selection;
    string switch2ScreenName;
    int selectedCategory = 0;
    
    // Clear Screen
    // clearScreen();
    
    cout << "Select which categories of books you would like to browse." << endl;
    
    // Loop through the BKCAT array elements to display all the available categories
    int i = 0;
    for (i = 0; i < BKCAT.size() ;i++) {
        cout << i + 1 << ". " << BKCAT[i] << endl;
    }
    
    cout << endl;
    cout << "Alternatively, you can do the following:" << endl;
    cout << i + 1 << ". View Cart." << endl;
    cout << i + 2 << ". Returning back to previous screen..." << endl;
    cout << i + 3 << ". Exit the program." << endl;
    cout << "Please input a valid number: ";
    
    // Get the integer selection value and get the position of the selected category
    selection = validateIntSelection(1,1,i + 3);
    
    // if the selection is anything beyond the size range of the BKCAT array, meaning it has to be either "return to previous screen" or "exit the program" or "view cart".
    if (selection == i + 1) {
        switch2ScreenName = "ViewCart";
    }
    
    else if (selection == i + 2) {
        switch2ScreenName = previousScreenName; // Go back to previous screen
    } 
    
    else if (selection == i + 3) {
        cout << "Bye bye!" << endl;
        exitCue = true;
    } 
    // get the position of the BKCAT and assign the value to "selectedCategory" variable
    else {
        selectedCategory = selection - 1;
        switch2ScreenName = "Bookware";
    }
    
    // return 2 outputs
    return make_pair(switch2ScreenName, selectedCategory);
}

// Screen Bookware - shows all the books from selected categories in table format, allow user to select the book for further action (e.g. specify the quantity)
string screenBookware(vector<Book>& booksToSell, int& userSelectedCategory, string& previousScreenName, Book*& userSelectedBook, Templates& t) {
    
    int selection;
    string switch2ScreenName;
    int bookIndex;
    
    // Clear the screen
    // clearScreen();
    
    // Show bookware and assign the last book index
    bookIndex = t.bookware(userSelectedCategory, booksToSell);
    
    cout << "Select your interested book to add to cart." << endl;
    cout << "Alternatively, select the following:" << endl;
    cout << bookIndex << ". View Cart." << endl;
    cout << bookIndex + 1 << ". Returning back to previous screen..." << endl;
    cout << bookIndex + 2 << ". Exit the program." << endl;
    
    // Ask for user input and get the selection index
    cout << "Please input a valid number: ";
    selection = validateIntSelection(1, 1, bookIndex + 2);
    
    // 3 additional selections, View Cart, return to Menu, Add to Cart
    if (selection == bookIndex) {
        switch2ScreenName = "ViewCart";
    } else if (selection == bookIndex + 1) {
        // Cannot use previousScreenName variable because it may return to the AddToCart screen
        switch2ScreenName = "MenuCategory";
    } else if (selection == bookIndex + 2) {
        cout << "Bye bye!" << endl;
        exitCue = true;
    } else {
        // Get the book reference by comparing the selection value with the bookIndex. if the bookIndex is not equal to the selection value, then keep adding the bookIndex until it reaches the equal value with selection value
        bookIndex = 1;
        for (auto& book : booksToSell) {
            if (book.category == userSelectedCategory) {
                if (selection == bookIndex) {
                    userSelectedBook = &book;
                    break;
                } else {
                    bookIndex++;
                }
            }
        }
        switch2ScreenName = "AddToCart";
    }
    
    return switch2ScreenName;
}

// Screen Add To Cart - Add the book to cart using cart manager
string screenAddToCart(Book*& userSelectedBook, Cart& cart, string& previousScreenName, Templates& t) {
    
    int quantity;
    string switch2ScreenName;
    string bookTitle;
    int findBookIndex = 0;
    bool hasFoundBookIndex = false;
    
    // Clearing the screen
    // clearScreen();
    
    // If there is no available intermediate stock, user is not supposed to add to cart any further.
    if (userSelectedBook->intermediateStock <= 0) {
        cout << "Failed operation. This book is out of stock." << endl;
        cout << "Returning back to previous screen..." << endl;
        return previousScreenName;
    }
    
    // Sometimes, the user can add the same book to the cart again. in this case, we do not want to create a new element, but we need to update the existing element and modify the quantity variable in the element.
    // We need to first find the index of the book in the cart where the current user selected book title matches any of the book title in the cart, if match, change the boolean hasFoundBookIndex to true.
    bookTitle = userSelectedBook->title;
    for (findBookIndex = 0; findBookIndex < cart.selectedBooks.size(); findBookIndex++) {
        if (cart.selectedBooks[findBookIndex]->title == bookTitle) {
            hasFoundBookIndex = true;
            break;
        }
    }
    
    // Set the price to 2 decimal places
    cout << fixed << setprecision(2);
    
    cout << "You are now adding the following book to cart. Just need few more details." << endl;
    
    // Display the details of selected book to be added to cart in formatted way using setw()
    cout << setw(t.w_universal) << "Title" 
         << setw(t.w_universal/10) << ": " << userSelectedBook->title 
         << endl;
    cout << setw(t.w_universal) << "Author" 
         << setw(t.w_universal/10) << ": " << userSelectedBook->author 
         << endl;
    cout << setw(t.w_universal) << "Description" 
         << setw(t.w_universal/10) << ": " << userSelectedBook->description 
         << endl;
    cout << setw(t.w_universal) << "Unit Price" 
         << setw(t.w_universal/10) << ": " << "RM " << userSelectedBook->price 
         << endl;
    cout << setw(t.w_universal) << "Stock Available" 
         << setw(t.w_universal/10) << ": " << userSelectedBook->intermediateStock 
         << endl;
    
    // Ask the users how many do they want
    // Use the intermediate stock value instead of actual stock value since the user hasn't paid yet
    // Once they paid, only then the intermediate stock value will override the actaul stock value
    // If they choose to clear cart, the intermediate stock value should be defaulted back to the actual stock value
    cout << "Specify how many books you want to add to cart (between 1 and " << userSelectedBook->intermediateStock << "): ";
    quantity = validateIntSelection(0, 0, userSelectedBook->intermediateStock);
    
    // If nothing added
    if (quantity == 0) {
        cout << "Nothing added." << endl; 
    }
    else {
        // If there is already existing book in the cart (i.e. via hasFoundBookIndex), modify the existing
        // If not, add new element to vector in Cart
        if (hasFoundBookIndex) {
            cart.quantity[findBookIndex] += quantity;
            cart.totalAmount[findBookIndex] =  cart.selectedBooks[findBookIndex]->price * cart.quantity[findBookIndex];
            cart.grandTotal = cart.calculateGrandTotal();
            cart.selectedBooks[findBookIndex]->intermediateStock -= quantity;
        } else {
            // Add the requested amount into vector for later use
            cart.selectedBooks.push_back(userSelectedBook);
            cart.quantity.push_back(quantity);
            cart.totalAmount.push_back(userSelectedBook->price * quantity);
            cart.grandTotal = cart.calculateGrandTotal();
            userSelectedBook->intermediateStock -= quantity; 
        }
        
        // Let the user know on the amount added to cart
        // Because userSelectedBook and cart.selectedBooks[findBookIndex] fetch the same result due to them being a pointer to same reference, so updating either will update the original variable. therefore, cout userSelectedBook or cart.selectedBooks[findBookIndex] will give the same thing
        cout << "Total amount added to cart: RM " << userSelectedBook->price * quantity << endl;
    }
    
    // Return to previous bookware screen
    cout << "Added to cart. Returning back to previous screen..." << endl;
    switch2ScreenName = previousScreenName;
    
    return switch2ScreenName;
}

// Screen View Cart - Let the user view the cart, and modify the cart
string screenViewCart(Cart& cart, string& previousScreenName, Templates& t) {
    
    string switch2ScreenName;
    int selection = 0;
    int selectionIndex = 0;
    
    // Display the cart invoice
    cout << "Here is your shopping cart invoice." << endl;
    t.invoiceReceipt(cart.selectedBooks, cart.quantity, cart.totalAmount, cart.grandTotal); // Show invoice
    
    cout << endl;
    cout << "What would you like to do?" << endl;
    
    // If cart is empty, don't even bother to show options that is specific to cart management
    if (!cart.isEmpty()) {cout << ++selectionIndex << ". Modify cart." << endl;}
    if (!cart.isEmpty()) {cout << ++selectionIndex << ". Delete a book in cart." << endl;}
    if (!cart.isEmpty()) {cout << ++selectionIndex << ". Delete entire cart." << endl;}
    if (!cart.isEmpty()) {cout << ++selectionIndex << ". Checkout / Pay." << endl;}
    cout << ++selectionIndex << ". Returning back to previous screen..." << endl;
    cout << ++selectionIndex << ". Exit the program." << endl;
    
    // Keep asking for input until a valid number is provided
    while (selection == 0) {
       cout << "Please input a valid number: ";
       selection = validateIntSelection(0, 0, selectionIndex);
    }
    
    // 2 scenarios
    // Non-empty cart - different options will require different cart management logic
    // Empty cart - don't even bother to run any cart management logic
    if (!cart.isEmpty()) {
        switch (selection) {
            case 1:
                // Run modifyCart() function from Cart class
                cart.modifyCart();
                switch2ScreenName = "ViewCart";
                break;
            case 2:
                // Run deleteBook() function from Cart class
                cart.deleteBook();
                switch2ScreenName = "ViewCart";
                break;
            case 3:
                // Run clearCart() function from Cart class
                cart.clearCart();
                cout << "Returning back to previous screen..." << endl;
                switch2ScreenName = "MenuCategory";
                break;
            case 4:
                switch2ScreenName = "Checkout";
                break;
            case 5:
                // There are instances where the screen will get stuck at "ViewCart" screen because it is possible for ViewCart screen to return to itself after the "ModifyCart", "DeleteBook" or "ClearCart" function. hence, the previous screen name and screen name becomes the same and hence will not be able to go back one screen up.
                switch2ScreenName = previousScreenName == "ViewCart"? "MenuCategory" : previousScreenName;
                break;
            case 6:
                cout << "Bye bye!" << endl;
                exitCue = true;
                break;
        }
    }
    else {
        switch (selection) {
            case 1:
                // There are instances where the screen will get stuck at "ViewCart" screen because it is possible for ViewCart screen to return to itself after the "ModifyCart", "DeleteBook" or "ClearCart" function. hence, the previous screen name and screen name becomes the same and hence will not be able to go back one screen up.
                switch2ScreenName = previousScreenName == "ViewCart"? "MenuCategory" : previousScreenName;
                break;
            case 2:
                cout << "Bye bye!" << endl;
                exitCue = true;
                break;
        } 
    }

    return switch2ScreenName;
}

// Screen Checkout - Allow users to choose payment method, pay, clear cart, and register into daily sales report
string screenCheckout(Cart& cart, SalesManager& salesManager, string& previousScreenName) {
    
    string switch2ScreenName;
    string custName;
    string cardNumber = "Again";
    string cvv = "Again";
    int cashorcard = 0;
    
    cout << "Thank you from choosing to buy from us." << endl;
    cout << endl;
    cout << "Total amount: RM " << cart.grandTotal << endl;
    
    // Allow user to choose between cash or bank card
    cout << "You can choose to pay with cash or credit/debit card. What is your option?" << endl;
    cout << "1. Cash" << endl;
    cout << "2. Credit/Debit Card" << endl;
    cout << "3. Returning back to previous screen..." << endl;
    cout << "Please select either 1 (Cash) or 2 (Credit/Debit card) or 3 (Return): ";
    cashorcard = validateIntSelection(0, 0, 3);
    
    // 0 or 3 are indicators of returning to previous screen
    if (cashorcard == 0 || cashorcard == 3) {
        cout << "Payment aborted. Returning back to previous screen..." << endl;
        return previousScreenName;
        
    } 
    // if customer selects to pay via cash
    else if (cashorcard == 1) {
        
        // ask for the name of customer
        cout << "Next, tell us your name: ";
        cin >> custName;
        
        // Register as part of daily sales
        salesManager.registerSale(custName, cart, cashorcard, "N/A", "0");
        
        // Clear cart and reinitialise the value
        cart.payAndClearCart();
        
    } 
    // if customer selects to pay via bank card
    else if (cashorcard == 2) {
        
        // Keep getting user input on card number until a valid card number is provided
        // Validate against the input using Luhn's algorithm
        while (cardNumber == "Again") {
            cout << "Input your Card Number: ";
            cardNumber = validateCardNumber(); // Valdate if the card number provided is a valid card number
        }
        
        // if no card number is presented, the validateCardNumber() will return "Terminate", and the payment will be aborted.
        if (cardNumber == "Terminate") {
            cout << "Payment aborted. Returning back to previous screen..." << endl;
            return previousScreenName;
        }
        
        // Keep getting user input on CVV until a valid CVV is provided
        // Validate againt the input using custom validation function
        while (cvv == "Again") {
            cout << "Input your CVV: ";
            cvv = validateCVV(); // Valdate if the CVV provided is a valid CVV
        }
        
        // if no CVV is presented, the validateCVV() will return "Terminate", and the payment will be aborted.
        if (cvv == "Terminate") {
            cout << "Payment aborted. Returning back to previous screen..." << endl;
            return previousScreenName;
        }
        
        // Get user input on their name, i.e. customer name
        cout << "Next, tell us your name: ";
        cin >> custName;
        
        // Register as part of daily sales
        salesManager.registerSale(custName, cart, cashorcard, cardNumber, cvv);
        
        // Clear cart and reinitialise the value
        cart.payAndClearCart();
        
    }
    
    switch2ScreenName = "Receipt";
    
    return switch2ScreenName;
}

// Receipt screen - show receipt to customer upon paid and proceed to landing screen upon pressing enter
string screenReceipt(SalesManager& salesManager, Templates& t) {
    
    string junk;
    string switch2ScreenName;
    int salesVecLastPos = salesManager.customerNames.size() - 1;
    
    cout << "Thank you for shopping with us. We hope you have a pleasant shopping experience." << endl;
    cout << endl;
    cout << "Here is your receipt." << endl;
    
    // Show Receipt
    t.invoiceReceipt(salesManager.carts[salesVecLastPos].selectedBooks, salesManager.carts[salesVecLastPos].quantity, salesManager.carts[salesVecLastPos].totalAmount, salesManager.carts[salesVecLastPos].grandTotal, salesManager.customerNames[salesVecLastPos], salesManager.payMethodsInt[salesVecLastPos], salesManager.cardNum[salesVecLastPos]);
    
    // Press Enter to move back to landing screen
    cout << "Press \"Enter\" to continue..." << endl;
    cin.ignore();
    getline(cin, junk);
    switch2ScreenName = "Landing";
    
    return switch2ScreenName;
}

// Screen reporting - shows sales report summary and detailed transactions. Also this screen is only accessible by admin so it needs to be protected via a password. 3 failed login attempt will send the user back to "Landing" screen
string screenReporting(SalesManager& salesManager, Templates& t) {
    
    string switch2ScreenName;
    int selection = 0;
    const string PASS = "admin";
    bool isAuthorised = false;
    int loginAttempt = 0;
    string input = "";
    
    cout << "Only authorised personnel can login to this screen." << endl;
    cout << "Input password: ";
    getline(cin, input);
    
    // Compare the input and password. If match, change to true, else remain as false
    isAuthorised = input == PASS? true : false;
    
    // If wrong password, add a counter to loginAttempt
    // If 3 login attempts are encountered and yet the boolean isAuthorised is still false, break the while loop to go to next logic.
    // For every password input, validate if it matches the PASS variable. if matches, change the boolean to true and proceed with viewing the sales report
    while (!isAuthorised) {
        if (++loginAttempt == 3) {break;};
        cout << "WARNING: Wrong password! Login attempt [" << loginAttempt << "/3]" << endl;
        cout << "Input password: ";
        getline(cin, input);
        isAuthorised = input == PASS? true : false;
    }
    
    // Force user to return to "Landing" screen if unauthorised
    if(!isAuthorised) {
        cout << "Exceeded 3 failed login attempts. Unauthorised access detected." << endl;
        cout << "Returning back to previous screen..." << endl;
        return "Landing";
    }
    
    cout << "Authorised Access." << endl;
    cout << endl;
    cout << "Welcome, admin. Here is your sales report." << endl;
    
    // Display sales report and footer options
    t.salesReport(salesManager.customerNames, salesManager.payMethodsInt, salesManager.cardNum, salesManager.CVVs, salesManager.carts, salesManager.grandSalesAmount);
    t.salesReportFooterOption(salesManager.customerNames);
    
    // Keep getting input until a selection = 1 is invoked (i.e. return to previous screen)
    // If there is any other selection other than 1 (i.e. return), ...
    // a) if the selection does not fall under the range of validateIntSelection, as usual it will default back to 0 and loop in while loop
    // b) if the selection falls under the range of validateIntSelection, it will run the specified logic and then reinitialise the selection = 0, so it will loop again in while loop
    while (selection == 0) {
        cout << "Please input a valid number: ";
        selection = validateIntSelection(0, 0, salesManager.customerNames.size() + 2);
        
        // Run the logic depending on different selections
        if (selection == 1) {
            return "Landing";
        } else if (selection == 2) {
            cout << endl;
            cout << "Here is the sales report summary for your perusal again:" << endl;
            
            // Display sales report and footer options
            t.salesReport(salesManager.customerNames, salesManager.payMethodsInt, salesManager.cardNum, salesManager.CVVs, salesManager.carts, salesManager.grandSalesAmount);
            t.salesReportFooterOption(salesManager.customerNames);
            
            // Reset the selection so it can loop in while loop
            selection = 0;
        } else if (selection > 2) {
            cout << endl;
            cout << "Here is the requested detailed transaction:" << endl;
            
            // Display the receipt and footer options
            // selection - 3 is because we start the salesID at 3 due to 1 and 2 being used for other purpose (i.e. return, view sales summary)
            t.invoiceReceipt(salesManager.carts[selection - 3].selectedBooks, salesManager.carts[selection - 3].quantity, salesManager.carts[selection - 3].totalAmount, salesManager.carts[selection - 3].grandTotal, salesManager.customerNames[selection - 3], salesManager.payMethodsInt[selection - 3], salesManager.cardNum[selection - 3]);
            
            t.salesReportFooterOption(salesManager.customerNames);
            
            // Reset the selection so it can loop in while loop
            selection = 0;
        }
    }
    
    switch2ScreenName = "Reporting";
    
    return switch2ScreenName;
}

// Function to switch screen
string switchScreen(string& screenName, vector<Book>& booksToSell, int& userSelectedCategory, Book*& userSelectedBook, Cart& cart, string& previousScreenName, SalesManager& salesManager, Templates& templates) {

    string switchScreenName;
    
    // show the screen name for every new screen
    showScreenName(screenName);
    
    if (screenName == "Landing") {
        switchScreenName = screenLanding(userSelectedCategory);
    } else if (screenName == "Reporting") {
        switchScreenName = screenReporting(salesManager, templates);
    } else if (screenName == "Bookware") {
        switchScreenName = screenBookware(booksToSell, userSelectedCategory, previousScreenName, userSelectedBook, templates);
    } else if (screenName == "AddToCart") {
        switchScreenName = screenAddToCart(userSelectedBook, cart, previousScreenName, templates);
    } else if (screenName == "ViewCart") {
        switchScreenName = screenViewCart(cart, previousScreenName, templates);
    } else if (screenName == "Checkout") {
        switchScreenName = screenCheckout(cart, salesManager, previousScreenName);
    } else if (screenName == "Receipt") {
        switchScreenName = screenReceipt(salesManager, templates);
    } else if (screenName == "MenuCategory") {
        
        // Since screenMenuCategory logic provides 1 string and 1 integer in a pair, so auto type is used.
        auto res = screenMenuCategory(previousScreenName);
        switchScreenName = res.first;
        userSelectedCategory = res.second;
    } 
    
    return switchScreenName;
}

/* ---------------------------------
// Main method - running the program
// --------------------------------- */

int main() {
    
    // Define all the constants
    const string DEF_SCREENNAME = "Landing";
    
    // Initialise variables
    string screenName = DEF_SCREENNAME;
    string previousScreenName = DEF_SCREENNAME;
    int userSelectedCategory = 0;
    bool isAuthorised = false;
    exitCue = false;
    
    // Initialise classes
    Cart cart;
    SalesManager salesManager;
    Templates templates;
    
    // Initialise object instances of class Book and store in vector
    vector<Book> booksToSell = {
        Book("1984", "George Orwell", "A dystopian novel depicting a totalitarian regime that uses surveillance and propaganda to maintain control.", 0, 56, 11.50),
        
        Book("To Kill a Mockingbird", "Harper Lee", "A powerful tale of racial injustice and childhood innocence in the American South.", 0, 64, 12),
        
        Book("The Catcher in the Rye", "J.D. Salinger", "A young man's journey through New York City as he grapples with teenage angst and alienation.", 0, 33, 8.13),
        
        Book("Sapiens", "Yuval Noah Harari", "An exploration of the history and impact of Homo sapiens from ancient times to the present.", 1, 92, 19.99),
        
        Book("The Power of Habit", "Charles Duhigg", "An insightful look into the science of habits and how they can be changed to improve our lives.", 1, 88, 18)
    };
    
    // Initialize userSelectedBook pointer to the address of the first element of booksToSell to be used later
    Book *userSelectedBook = &booksToSell[0]; 
    
    // while loop to keep resetting different screen after running the logic of each screen
    while (!exitCue) {
        
        // Store current screen name to temporary variable
        string tempSN = screenName;
        
        // Pass the current screen name into switchScreen() function, and overwrite screenName variable with the new screen name 
        screenName = switchScreen(screenName, booksToSell, userSelectedCategory, userSelectedBook, cart, previousScreenName, salesManager, templates);
        
        // Assign the value in temporary variable (which is essentially the previous screen name) to the previousScreenName variable
        previousScreenName = tempSN;
    }

    return 0;
}
