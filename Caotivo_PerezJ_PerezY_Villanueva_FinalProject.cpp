#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <list>
#include <climits>


using namespace std;

const int MAX_NAME_LENGTH = 30;
int transactionID = 1000;

void Menu();
void initializeFirearms();
void ViewAvailableFirearmsMenu();
void AddTransaction();
void DisplayTransactions();
void UpdateTransaction();
void ProcessExpiredTransactions();
void SaveTransactionsToFile(const string& filename);
void LoadTransactionsFromFile(const string& filename);
void RecomputeInventoryFromTransactions();
string CapitalizeFirstLetter(const string& input);
void SearchTransaction();
void Reminder();
int ConfirmExit();

struct Firearm {
    string id;
    string model;
    string caliber;
    int quantity;
};

std::list<Firearm> firearms;

struct Transaction {
    int id;
    string firearmId;
    string customerName;
    int quantityRented;
    time_t timestamp;
    bool returned;
};

struct TransactionNode {
    Transaction data;
    TransactionNode* next;
    TransactionNode(const Transaction& t) : data(t), next(NULL) {}
};

struct TransactionQueue {
    TransactionNode* front;
    TransactionNode* rear;
    int size;

    TransactionQueue() : front(NULL), rear(NULL), size(0) {}

    void enqueue(const Transaction& t) {
        TransactionNode* node = new TransactionNode(t);
        if (!rear) {
            front = rear = node;
        } else {
            rear->next = node;
            rear = node;
        }
        size++;
    }

};

TransactionQueue transactionQueue;

TransactionNode* FindTransactionNodeById(int id) {
    TransactionNode* curr = transactionQueue.front;
    while (curr) {
        if (curr->data.id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

void UpdateFirearmAndQuantity(TransactionNode* node);
void UpdateCustomerName(TransactionNode* node);
void SortTransactionArray(Transaction* arr, int n, int sortOpt);


int main(){
    initializeFirearms();
    LoadTransactionsFromFile("transactions.txt");
    ProcessExpiredTransactions(); 
    RecomputeInventoryFromTransactions(); 

    bool exit = false;
    int choice;

    do {
        Menu();
        cout << "=======================================\n\n";
        
        while (true) {
            cout << "Select an option (1 - 7): ";
            if (!(cin >> choice)) {
            
                cin.clear();               
                cin.ignore(10000, '\n'); 
                cout << "Invalid input. Please enter a number between 1 and 7.\n";
            }
            else if (choice < 1 || choice > 7) {
                
                cout << "Invalid option. Please choose from 1 to 7.\n";
            }
            else {
                
                break;
            }
        }
        cin.ignore(10000, '\n');

        switch (choice) {
            case 1:
                ViewAvailableFirearmsMenu();
                cout << "\nPress Enter to return to the main menu...";
                cin.get();
                break;
            case 2:
                AddTransaction();
                break;
            case 3:
                UpdateTransaction();
                break;
            case 4:
                DisplayTransactions();
                break;
            case 5:
                SearchTransaction();
                break;
            case 6:
                Reminder();
                break;
            case 7:
                SaveTransactionsToFile("transactions.txt"); 
                exit = (ConfirmExit() == 1); 
                break;
        }

    } while (!exit);

    return 0;
}

void initializeFirearms() {
    firearms.clear();

    Firearm f;
    f.id       = "G17A";
    f.model    = "Glock 17";
    f.caliber  = "9mm Pistol";
    f.quantity = 5;
    firearms.push_back(f);

    f.id       = "R870";
    f.model    = "Remington 870";
    f.caliber  = "12ga Pump-Action Shotgun";
    f.quantity = 3;
    firearms.push_back(f);

    f.id       = "AR15";
    f.model    = "AR-15";
    f.caliber  = "5.56mm Semi-Automatic Rifle";
    f.quantity = 4;
    firearms.push_back(f);

    f.id       = "QBZ191";
    f.model    = "QBZ-191";
    f.caliber  = "5.8mm Assault Rifle";
    f.quantity = 6;
    firearms.push_back(f);

    f.id       = "VSK94";
    f.model    = "VSK-94";
    f.caliber  = "9mm Sniper Rifle";
    f.quantity = 2;
    firearms.push_back(f);

    f.id       = "SPAS12";
    f.model    = "Franchi SPAS-12";
    f.caliber  = "12ga Dual-Mode Shotgun";
    f.quantity = 4;
    firearms.push_back(f);

    f.id       = "6P50";
    f.model    = "Kord 6P50";
    f.caliber  = "12.7mm Heavy Machine Gun";
    f.quantity = 1;
    firearms.push_back(f);

    f.id       = "VECTOR";
    f.model    = "KRISS Vector";
    f.caliber  = "9mm Submachine Gun";
    f.quantity = 5;
    firearms.push_back(f);
}

void Menu(){
    system("CLS");
    cout << "======================================\n";
    cout << "||      FIREARM RENTAL SYSTEM       ||\n";
    cout << "======================================\n";
    cout << "||  1.) View Available Firearms     ||\n";
    cout << "||  2.) Add Transaction             ||\n";
    cout << "||  3.) Update Transaction          ||\n";
    cout << "||  4.) Display Transaction List    ||\n";
    cout << "||  5.) Transaction Search          ||\n";
    cout << "||  6.) Important Reminder          ||\n";
    cout << "||  7.) Exit                        ||\n";
    cout << "======================================\n\n";
}

void Reminder() {
    system("CLS");
    cout << "================================================================================\n";
    cout << "||                             IMPORTANT REMINDER                             ||\n";
    cout << "================================================================================\n";
    cout << "|| Each transaction will automatically expire after 2 minutes (120 seconds)   ||\n";
    cout << "|| from the time it was added or last updated.                                ||\n";
    cout << "||                                                                            ||\n";
    cout << "|| Once expired:                                                              ||\n";
    cout << "|| - The transaction will be marked as 'Expired'.                             ||\n";
    cout << "|| - The rented firearm will be automatically returned to the inventory.      ||\n";
    cout << "||                                                                            ||\n";
    cout << "|| NOTE:                                                                      ||\n";
    cout << "|| The 2-minute time limit is set only for testing purposes, so you can       ||\n";
    cout << "|| easily verify that expiration and auto-return features are working.        ||\n";
    cout << "|| We could modify the expiration duration to be much longer if needed.       ||\n";
    cout << "================================================================================\n";
    cout << "|| Press Enter to return to the main menu...                                  ||\n";
    cout << "================================================================================\n";
    cin.get();
}


void ViewAvailableFirearmsMenu() {
    ProcessExpiredTransactions();
    system("CLS");
    cout << "\n===============================================================================\n";
    cout << "||                            AVAILABLE FIREARMS                             ||\n";
    cout << "===============================================================================\n";
    cout << "|| " << left << setw(10) << "ID"
         << "| " << setw(20) << "Model"
         << "| " << setw(30) << "Caliber"
         << "| " << setw(8) << "Qty" << "||\n";
    cout << "||-----------|---------------------|-------------------------------|---------||\n";

    for (std::list<Firearm>::iterator it = firearms.begin(); it != firearms.end(); ++it) {
        cout << "|| " << left << setw(10)  << it->id
             << "| " << setw(20) << it->model
             << "| " << setw(30) << it->caliber
             << "| " << right << setw(6) << it->quantity << "  ||\n";
    }

    cout << "===============================================================================\n";
}


void AddTransaction() {
    system("CLS");
    ProcessExpiredTransactions();
    ViewAvailableFirearmsMenu();

    cout << "============================================================\n";
    cout << "||                    ADD TRANSACTION                     ||\n";
    cout << "============================================================\n";
    string id, customer;
    int qty;
    std::list<Firearm>::iterator it = firearms.end();

    
    if (transactionID >= INT_MAX - 1) {
        cout << "|| Error: Maximum transactions reached!                   ||\n";
        cout << "============================================================\n";
        system("pause");
        return;
    }

    while (true) {
        cout << " Enter Firearm ID (or C to cancel): ";
        cin >> id;

        if (id == "C" || id == "c") {
            cout << "Transaction cancelled. Returning to main menu.\n";
            system("pause");
            return;
        }

        for (size_t i = 0; i < id.length(); ++i) {
            id[i] = toupper((unsigned char)id[i]);
        }

        it = firearms.end();
        for (std::list<Firearm>::iterator fit = firearms.begin(); fit != firearms.end(); ++fit) {
            if (fit->id == id) {
                it = fit;
                break;
            }
        }

        if (it == firearms.end()) {
            cout << " Firearm not found! Please try again.\n";
        }
        else if (it->quantity == 0) {
            cout << " Sorry, \"" << it->model << "\" is currently out of stock.\n"
                << " Enter another ID, or C to cancel.\n";
        }
        else {
            break;
        }
    }

    cin.ignore(); 
    while (true) {
        cout << " Enter Customer Name (max " << MAX_NAME_LENGTH << " chars): ";
        getline(cin, customer);
        
        
        size_t start = customer.find_first_not_of(" \t");
        if (start == string::npos) {
            customer = "";
        } else {
            size_t end = customer.find_last_not_of(" \t");
            customer = customer.substr(start, end - start + 1);
        }
        
        if (customer.empty()) {
            cout << " Customer name cannot be empty! \n";
        } else if (customer.length() > MAX_NAME_LENGTH) {
            cout << " Name too long! Maximum " << MAX_NAME_LENGTH << " characters allowed. \n";
        } else {
            break;
        }
    }
    customer = CapitalizeFirstLetter(customer);

    
    ProcessExpiredTransactions();
    
    cout << " Enter Quantity to Rent: ";
    
    while (true){
        if (!(cin >> qty)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " Invalid input. Please enter an integer. \n";
        }    
        else if (qty <= 0 || qty > it->quantity) {
            cout << " Invalid quantity! Must be 1 - " << it->quantity << " \n";
        }   
        else {
            break;
        }
        cout << " Enter Quantity to Rent: ";
    }

    
    ProcessExpiredTransactions();
    if (qty > it->quantity) {
        cout << " Error: Not enough inventory! Available: " << it->quantity << "               ||\n";
        cout << "============================================================\n";
        system("pause");
        return;
    }

    
    time_t currentTime = time(NULL);
    if (currentTime == -1) {
        cout << "Error: Unable to get current time! \n";
        cout << "============================================================\n";
        system("pause");
        return;
    }

    Transaction t;
    t.id = transactionID++;
    t.firearmId = id;
    t.customerName = customer;
    t.quantityRented = qty;
    t.timestamp = currentTime;
    t.returned = false;

    transactionQueue.enqueue(t);
    it->quantity -= qty;

    cout << "============================================================\n";
    cout << "|| Transaction ID: " << t.id 
         << "                                   ||\n";
    cout << "|| Transaction added successfully!                        ||\n";
    cout << "============================================================\n";
    system("pause");
}

void DisplayTransactions() {
    ProcessExpiredTransactions(); 
    
    if (transactionQueue.size == 0) {
        system("CLS");
        cout << "=============================================================\n";
        cout << "||                No transactions yet.                     ||\n";
        cout << "=============================================================\n";
        cout << "Press Enter to return to the main menu...";
        cin.ignore();
        cin.get();
        return;
    }

    int sortOpt;
    char repeat;

    do {
        system("CLS");
        
        if (transactionQueue.size == 0) {
            cout << "=============================================================\n";
            cout << "||           All transactions have been processed.         ||\n";
            cout << "=============================================================\n";
            break;
        }

        cout << "======================================================================\n";
        cout << "||                          SORTING OPTION                          ||\n";
        cout << "======================================================================\n";
        cout << "||   [1] Transaction ID   (Low - High)                              ||\n";
        cout << "||   [2] Transaction ID   (High - Low)                              ||\n";
        cout << "||   [3] Quantity         (Low - High)                              ||\n";
        cout << "||   [4] Quantity         (High - Low)                              ||\n";
        cout << "||   [5] Customer Name    (A - Z)                                   ||\n";
        cout << "||   [6] Customer Name    (Z - A)                                   ||\n";
        cout << "||   [7] Cancel / Exit                                              ||\n";
        cout << "======================================================================\n";
        
        while (true) {
            cout << "Select a sorting option (1 - 7): ";
            
            if (!(cin >> sortOpt)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "|| Invalid input. Please enter a number (1-7).            ||\n";
            }
            else if (sortOpt < 1 || sortOpt > 7) {
                cout << "|| Invalid option. Please select 1-7.                     ||\n";
            }
            else {
                break;
            }
        }

        if (sortOpt == 7) break;

        int n = transactionQueue.size;
        Transaction* arr = new Transaction[n];
        
        TransactionNode* temp = transactionQueue.front;
        int actualCount = 0;
        for (int i = 0; i < n && temp; ++i, temp = temp->next) {
            arr[i] = temp->data;
            actualCount++;
        }

        SortTransactionArray(arr, actualCount, sortOpt);

        system("CLS");
        cout << "==========================================================================\n";
        cout << "||                           TRANSACTION LIST                           ||\n";
        cout << "==========================================================================\n";
        cout << "|| TID    | Firearm | Customer Name                  | Qty |   Status   ||\n";
        cout << "||--------|---------|--------------------------------|-----|------------||\n";

        time_t currentTime = time(NULL);
        if (currentTime == -1) {
            cout << "|| Error: Unable to determine transaction status.          ||\n";
            delete[] arr;
            break;
        }

       for (int i = 0; i < actualCount; ++i) {
            Transaction& t = arr[i];
            double elapsed = difftime(currentTime, t.timestamp);
            string status = (t.returned || elapsed >= 120.0) ? "Expired" : "Ongoing";
            
            cout << "|| "
                << setw(6) << left << t.id << " | "
                << setw(7) << left << t.firearmId << " | "
                << setw(30) << left << t.customerName << " | " 
                << setw(3) << left << t.quantityRented << " | "
                << setw(11) << left << status << "||\n";
        }

cout << "==========================================================================\n";

        delete[] arr;
        
        while (true) {
            cout << "Do you want to sort again? (Y/N): ";
            if (!(cin >> repeat)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter Y or N.\n";
                continue;
            }
            cin.ignore(10000, '\n');
            if (repeat=='Y' || repeat=='y' || repeat=='N' || repeat=='n')
                break;
            cout << "Invalid input. Please enter Y or N.\n";
        }
    } while (repeat=='Y' || repeat=='y');

    cout << "Press Enter to return to the main menu...";
    cin.get();
}



void UpdateTransaction() {
    system("CLS");
    ProcessExpiredTransactions();
    if (transactionQueue.size == 0) {
        cout << "======================================\n";
        cout << "||     No transactions to update!   ||\n";
        cout << "======================================\n";
        system("pause");
        return;
    }

    int updateID;
    cout << "======================================\n";
    cout << "||         UPDATE TRANSACTION       ||\n";
    cout << "======================================\n";
    cout << "Enter transaction ID to update: ";
    
    
    while (!(cin >> updateID)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter an integer: ";
    }

    TransactionNode* node = FindTransactionNodeById(updateID);
    if (!node) {
        cout << "||  Transaction not found!          ||\n";
        cout << "======================================\n";
        system("pause");
        return;
    }

    time_t now = time(NULL);
    double elapsed = difftime(now, node->data.timestamp);
    string status = (node->data.returned || elapsed >= 120.0) ? "Expired" : "Ongoing";
    cout << "Current Status: " << status << endl;

    if (node->data.returned || elapsed >= 120.0) {
        cout << "======================================\n";
        cout << "||  Cannot update - transaction is  ||\n";
        cout << "||  already expired/returned!       ||\n";
        cout << "======================================\n";
        system("pause");
        return;
    }
    int choice;
    do {
        system("CLS");
        cout << "======================================\n";
        cout << "||      UPDATE TRANSACTION MENU      ||\n";
        cout << "======================================\n";
        cout << "||  1.) Update Firearm & Quantity    ||\n";
        cout << "||  2.) Update Customer Name         ||\n";
        cout << "||  3.) Back to Main Menu            ||\n";
        cout << "======================================\n";
        cout << "Current Transaction Details:\n";
        cout << "Firearm: " << node->data.firearmId << endl;
        cout << "Customer: " << node->data.customerName << endl;
        cout << "Quantity: " << node->data.quantityRented << endl;
        string loopStatus = (node->data.returned || difftime(time(NULL), node->data.timestamp) >= 120.0) ? "Expired" : "Ongoing";
        cout << "Status: " << loopStatus << endl;
        cout << "======================================\n";
        cout << "Select an option: ";
        
        
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number (1-3): ";
        }

        switch (choice) {
            case 1:
                UpdateFirearmAndQuantity(node);
                break;
            case 2:
                UpdateCustomerName(node);
                break;
            case 3:
                break;
            default:
                cout << "Invalid choice! Please select 1-3.\n";
                system("pause");
        }
    } while (choice != 3);
}


void UpdateFirearmAndQuantity(TransactionNode* node) {
    system("CLS");
    cout << "======================================\n";
    cout << "||   UPDATE FIREARM & QUANTITY      ||\n";
    cout << "======================================\n";
    cout << "Current Firearm: " << node->data.firearmId << endl;
    cout << "Current Quantity: " << node->data.quantityRented << endl;
    ViewAvailableFirearmsMenu();

    string newFirearmId;
    int newQty;
    std::list<Firearm>::iterator newIt = firearms.end();
    
   while (true) {
        cout << "Enter new Firearm ID (or C to cancel): ";
        cin >> newFirearmId;

        if (newFirearmId == "C" || newFirearmId == "c") {
            cout << "Update cancelled. Returning to update menu.\n";
            system("pause");
            return;          
        }

        for (size_t i = 0; i < newFirearmId.length(); ++i) {
            newFirearmId[i] = toupper((unsigned char)newFirearmId[i]);
        }

        newIt = firearms.end();
        for (std::list<Firearm>::iterator it = firearms.begin(); it != firearms.end(); ++it) {
            if (it->id == newFirearmId) {
                newIt = it;
                break;
            }
        }

        if (newIt == firearms.end()) {
            cout << "Firearm not found! Please try again.\n";
        }
        else if (newIt->quantity == 0) {
            cout << "Sorry, \"" << newIt->model << "\" is out of stock.\n"
                << "Enter another ID or C to cancel.\n";
        }
        else {
            break;
        }
    }

    
    std::list<Firearm>::iterator oldIt = firearms.end();
    for (std::list<Firearm>::iterator it = firearms.begin(); it != firearms.end(); ++it) {
        if (it->id == node->data.firearmId) {
            oldIt = it;
            break;
        }
    }

    
    if (!node->data.returned && oldIt != firearms.end()) {
        oldIt->quantity += node->data.quantityRented;
    }

    
    ProcessExpiredTransactions();
    
    cout << "Available quantity: " << newIt->quantity << endl;
    
    while (true) {
        cout << "Enter new quantity: ";
        
        if (!(cin >> newQty)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter an integer. \n";
        }
        else if (newQty <= 0 || newQty > newIt->quantity) {
            cout << "Invalid quantity! Must be 1 - " << newIt->quantity << "\n";
        }
        else {
            break;
        }
    }

    
    ProcessExpiredTransactions();
    if (newQty > newIt->quantity) {
        cout << "Error: Not enough inventory! Available: " << newIt->quantity << "\n";
        
        if (oldIt != firearms.end()) {
            oldIt->quantity -= node->data.quantityRented;
        }
        system("pause");
        return;
    }

    
    time_t currentTime = time(NULL);
    if (currentTime == -1) {
        cout << "Error: Unable to get current time!\n";
        
        if (oldIt != firearms.end()) {
            oldIt->quantity -= node->data.quantityRented;
        }
        system("pause");
        return;
    }

    
    newIt->quantity -= newQty;
    node->data.firearmId = newFirearmId;
    node->data.quantityRented = newQty;
    node->data.timestamp = currentTime;
    node->data.returned = false;

    cout << "======================================\n";
    cout << "|| Firearm and quantity updated      ||\n";
    cout << "======================================\n";

    system("pause");
}


void UpdateCustomerName(TransactionNode* node) {
    system("CLS");
    cout << "======================================\n";
    cout << "||      UPDATE CUSTOMER NAME        ||\n";
    cout << "======================================\n";
    cout << "Current Customer Name: " << node->data.customerName << endl;
    
    string newCustomerName;
    cin.ignore(); 
    
    
    while (true) {
        cout << "Enter new Customer Name (max " << MAX_NAME_LENGTH << " chars): ";
        getline(cin, newCustomerName);
        
        
        size_t start = newCustomerName.find_first_not_of(" \t");
        if (start == string::npos) {
            newCustomerName = "";
        } else {
            size_t end = newCustomerName.find_last_not_of(" \t");
            newCustomerName = newCustomerName.substr(start, end - start + 1);
        }
        
        if (newCustomerName.empty()) {
            cout << "Customer name cannot be empty!\n";
        } else if (newCustomerName.length() > MAX_NAME_LENGTH) {
            cout << "Name too long! Maximum " << MAX_NAME_LENGTH << " characters allowed.\n";
        } else {
            break;
        }
    }
    
    
    time_t currentTime = time(NULL);
    if (currentTime == -1) {
        cout << "Error: Unable to get current time!\n";
        system("pause");
        return;
    }
    
    
    node->data.customerName = CapitalizeFirstLetter(newCustomerName);
    node->data.timestamp = currentTime;
    node->data.returned = false;
    
    cout << "======================================\n";
    cout << "||  Customer name updated!          ||\n";
    cout << "======================================\n";
    system("pause");
}


void SortTransactionArray(Transaction* arr, int n, int sortOpt) {
    for (int i = 1; i < n; ++i) {
        Transaction key = arr[i];
        int j = i - 1;
        bool go;
        while (j >= 0) {
            switch (sortOpt) {
                case 1: go = (arr[j].id > key.id); break;                          
                case 2: go = (arr[j].id < key.id); break;                          
                case 3: go = (arr[j].quantityRented > key.quantityRented); break; 
                case 4: go = (arr[j].quantityRented < key.quantityRented); break; 
                case 5: go = (arr[j].customerName > key.customerName); break;     
                case 6: go = (arr[j].customerName < key.customerName); break;     
                default: go = false;
            }
            if (!go) break;
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void SearchTransaction() {
    system("CLS");
    ProcessExpiredTransactions();

    if (transactionQueue.size == 0) {
        cout << "========================================\n";
        cout << "||     No transactions to search      ||\n";
        cout << "========================================\n";
        system("pause");
        return;
    }

    int id;
    cout << "========================================\n";
    cout << "||   BINARY SEARCH (TRANSACTION)      ||\n";
    cout << "========================================\n";
    cout << "Enter Transaction ID: ";
    while (!(cin >> id)) {
        cout << "Invalid input. Please enter an integer: ";
        cin.clear();
        cin.ignore(10000, '\n');
    } 

    int n = transactionQueue.size;
    Transaction* arr = new Transaction[n];
    TransactionNode* temp = transactionQueue.front;
    
    for (int i = 0; i < n && temp; ++i, temp = temp->next) {
        arr[i] = temp->data;
    }
    
    SortTransactionArray(arr, n, 1);

    int left = 0, right = n - 1, idx = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2; 
        if (arr[mid].id == id) {
            idx = mid;
            break;
        }
        else if (arr[mid].id < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    system("CLS");

cout << "=========================================================================\n";
cout << "||                              SEARCH RESULT                          ||\n";
cout << "=========================================================================\n";

if (idx != -1) {
    Transaction& t = arr[idx];
    double elapsed = difftime(time(NULL), t.timestamp);
    string status = (t.returned || elapsed >= 120.0) ? "Expired" : "Ongoing";

    cout << "|| TID    | Firearm | Customer Name                  | Qty |  Status   ||\n";
    cout << "||--------|---------|--------------------------------|-----|-----------||\n";
    cout << "|| "
        << setw(6) << t.id << " | "
        << setw(7) << t.firearmId << " | "
        << setw(30) << t.customerName << " | " 
        << setw(3) << t.quantityRented << " | "
        << setw(9) << status << " ||\n";
}
else {
    cout << "||                  No matching transaction found.                     ||\n";
}
cout << "=========================================================================\n";
    
    delete[] arr;
    cout << "Press Enter to return to the main menu...";
    cin.ignore(10000, '\n');
    cin.get();
}

void ProcessExpiredTransactions() {
    time_t now = time(NULL);
    for (TransactionNode* curr = transactionQueue.front; curr; curr = curr->next) {
        double elapsed = difftime(now, curr->data.timestamp);
        if (elapsed >= 120.0 && !curr->data.returned) {
           
            for (std::list<Firearm>::iterator it = firearms.begin(); it != firearms.end(); ++it) {
                if (it->id == curr->data.firearmId) {
                    it->quantity += curr->data.quantityRented;
                    break;
                }
            }
            curr->data.returned = true;
        }
    }
}
void SaveTransactionsToFile(const string& filename) {
    ofstream fout(filename.c_str());
    if (!fout) {
        cerr << "Failed to save transactions to file: " << filename << endl;
        return;
    }

    for (TransactionNode* curr = transactionQueue.front; curr; curr = curr->next) {
        fout << curr->data.id << "|"
             << curr->data.firearmId << "|"
             << curr->data.customerName << "|"
             << curr->data.quantityRented << "|"
             << curr->data.timestamp << "|"
             << (curr->data.returned ? 1 : 0) << "\n";
    }
}

void LoadTransactionsFromFile(const string& filename) {
    ifstream fin(filename.c_str());
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Transaction t;
        int    field = 0;
        size_t pos;
        while ((pos = line.find('|')) != string::npos) {
            string token = line.substr(0, pos);
            switch (field) {
                case 0: t.id           = atoi(token.c_str()); break;
                case 1: t.firearmId    = token;               break;
                case 2: t.customerName = token;               break;
                case 3: t.quantityRented = atoi(token.c_str()); break;
                case 4: t.timestamp    = (time_t)atol(token.c_str()); break;
            }
            line.erase(0, pos+1);
            ++field;
        }
        t.returned = (atoi(line.c_str()) != 0);
        transactionQueue.enqueue(t);
        if (t.id >= transactionID) transactionID = t.id + 1;
    }
}

void RecomputeInventoryFromTransactions() {
    initializeFirearms();
    time_t now = time(NULL);
    for (TransactionNode* curr = transactionQueue.front; curr; curr = curr->next) {
        double elapsed = difftime(now, curr->data.timestamp);
        if (!(curr->data.returned || elapsed >= 120.0)) {
          
            for (std::list<Firearm>::iterator it = firearms.begin(); it != firearms.end(); ++it) {
                if (it->id == curr->data.firearmId) {
                    it->quantity -= curr->data.quantityRented;
                    if (it->quantity < 0) it->quantity = 0;
                    break;
                }
            }
        }
    }
}


string CapitalizeFirstLetter(const string& input) {
    if (input.empty()) return input;
    string result = input;
    bool capitalizeNext = true;
    
    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] == ' ' || result[i] == '\t') {
            capitalizeNext = true;
        } else if (capitalizeNext) {
            result[i] = toupper((unsigned char)result[i]);
            capitalizeNext = false;
        } else {
            result[i] = tolower((unsigned char)result[i]);
        }
    }
    
    return result;
}

int ConfirmExit() {
    char choice;
    while (true) {
        cout << "\nDo you want to exit the program? (Y/N): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter Y or N.\n";
            continue;
        }
        cin.ignore(10000, '\n');
        switch (choice) {
            case 'Y': case 'y':
                cout << "\nClosing the program... Goodbye!\n";
                return 1;
            case 'N': case 'n':
                return 0;
            default:
                cout << "Invalid input. Please enter Y or N.\n";
        }
    }
}

