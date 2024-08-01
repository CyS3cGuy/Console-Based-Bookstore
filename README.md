# Console-Based-Bookstore
> Simulate a console-based bookstore, where users can order books and store owner can view their sales summary.

This application is written in `C++` programming language. Some of the key functionalities that are being simulated in this bookstore application include:
<ins>**General functionality**</ins>
This application, in general, can automatically do the following, depending on the code logics being run:
1. Screen switching
  - This app uses the concept of *screen*. After user selects an option, it switches to different *screen* for further interaction.
  - Each screen has its own logic which presents user with different options/selections to choose from before switching to next *screen*.

2. Bookware display
   - Display all books, including the titles, authors, descriptions, unit prices and available stocks.
   - Display in table format.

3. Invoice/Receipt generation
   - Generate invoice based on items in the cart before payment
   - Generate receipt after payment
  
4. Data validation
   - Validate the user input is valid so to not crash the system.
   - Also validate if the input is a valid card number using Luhn's Algorithm and CVV if user chooses to pay using card.
  
5. Unauthorised Access Prevention
   - Certain features can only be accessed by certain privileged individuals. These features are protected by password and only correct password will provide authorised access.
   - It also contains feature to detect number of failed login attempts and 'forcefully' kick the user out of the *screen*.

<ins>**User functionality**</ins>
This application allow users interaction on certain activities:
1. Choose book categories.
2. Order books and add ordered books to cart before checkout.
3. Pay using cash or card.

<ins>**Store Owner functionality**</ins>
Store owner has higher privileges, especially viewing sales report. This application allow store owner interaction on certain activities:
1. View sales report summary after providing correct password.
2. View detailed transaction data from the sales report.
