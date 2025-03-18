#include "Menu.h"
#include <iostream>

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void showMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;33m1. Load Bills" << endl;
    cout << "2. Load Customers" << endl;
    cout << "3. Process Payments" << endl;
    cout << "4. Generate Billing Batch" << endl;
    cout << "5. Simulate Payments" << endl;
    cout << "6. Calculate Overdue Bills" << endl;
    cout << "7. Exit" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: ";
}

int getChoice() {
    int choice;
    cin >> choice;
    return choice;
}

void waitForKeyPress() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}
