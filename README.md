# Description of the Project
	The Firearm Rental System is a C++ program, which helps the user in managing weapon inventory and rentals. The program features multiple options displayed in the starting text-based interface menu.

1. ) View Available Firearms - Displays a formatted list of every available firearm in stock. The list is organized with the following details: Firearm ID, model, caliber, and quantity.

2. ) Add Transaction	- Takes user input to save in the system. Allows the user to: Select firearm by ID, input their name, and choose quantity. After the user inputs details, the system automatically increments transaction ID and deducts the quantity of firearm from the inventory.

3. ) Update Transaction - Allows the user to update an "Ongoing" transaction and change input details. The system requests for the transaction ID, then the user can update the following details: Firearm, quantity, or customer's name. Upon updating, the timer automatically resets.

4. ) Display Transaction List - Upon selecting this option, it offers various sorting options: By transaction ID, quantity, or customer name. These can be either ascending or descending. The transaction list shows the full list of transactions in an organized table. The order of details are as followed: Transaction ID, firearm ID, customer name, quantity rented, and status.


5. ) Transaction Search - User is required to input a Transaction ID, then the system locates and displays the details of any available transactions with the matching ID. The system uses a binary search on a sorted array of transaction IDs.

6. ) Important Reminder - Displays an important reminder before using the program. It is regarding the expiration of transactions, which were set to 2 minutes for testing purposes to the system's expiration and auto-return feature.

7. ) Exit - Prompts a Yes/No choice before closing the system. It saves a text file of the previously-made transactions to ensure that the user does not lose data.

# Objectives

- To develop a program using C++ that enables customers to rent their desired firearms.
- To utilize advanced programming concepts, including data structures, algorithms, and file handling, to implement the system.
- To provide an efficient and automated solution for managing firearms, ensuring accurate tracking, secure tracking processing, and seamless customer interactions.
To help the developers improve their programming efficiency and problem-solving abilities.
