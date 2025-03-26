#include "Menu.h"
#include <iostream>
#include "InputHelper.h"

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void showMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m  Utility Service Provider" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;33m1. Billing" << endl;
    cout << "2. Manage Customers" << endl;
    cout << "3. Manage Provide" << endl;
    cout << "4. Manage Services" << endl;
    cout << "5. Manage Rates" << endl;
    cout << "6. Management Report" << endl;
    cout << "7. Exit" << endl;
    cout << "\033[1;32m=============================" << endl;
}

void showBillingMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Billing Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Run Billing Batches" << endl;
    cout << "2. Calculate Overdue Invoices" << endl;
    cout << "3. Manually Update Bill" << endl;
    cout << "4. View Invoice" << endl;
    cout << "5. List Overdue Bills" << endl;
    cout << "6. Add BillCalendar" << endl;
    cout << "7. View BillCalendar" << endl;
    cout << "8. Update BillCalendar Status" << endl;
    cout << "9. Simulate Usage" << endl;
    cout << "10. Simulate Payments" << endl;
    cout << "11. Back" << endl;
}

void showCustomerMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Customer Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Add Customer" << endl;
    cout << "2. Update Customer" << endl;
    cout << "3. Display Customers" << endl;
    cout << "4. Add Subscription" << endl;
    cout << "5. Update Subscription" << endl;
    cout << "6. Display Customers" << endl;    
    cout << "7. Back" << endl;
}

void showUsageMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Usage Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Add Usage" << endl;
    cout << "2. Search Usage" << endl;
    cout << "3. Update Usage" << endl;
    cout << "4. Back" << endl;
}

void showProviderMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Provider Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Add Provider" << endl;
    cout << "2. Update Provider" << endl;
    cout << "3. Display Providers" << endl;
    cout << "4. Back" << endl;
}

void showServiceMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Services Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Add Service" << endl;
    cout << "2. Update Service" << endl;
    cout << "3. List all Services" << endl;
    cout << "4. Back" << endl;
}

void showRatesMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Rates Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Add Rate" << endl;
    cout << "2. List all Rates" << endl;
    cout << "3. Update Rate" << endl;
    cout << "4. Back" << endl;
}

void showPaymentsMenu() {
    clearScreen();
    cout << "\033[1;32m=============================" << endl;
    cout << "\033[1;34m       Utility Service Provider Payment Management" << endl;
    cout << "\033[1;32m=============================" << endl;
    cout << "Enter your choice: " << endl;
    cout << "\033[1;33m1. Run Payments Batch" << endl;
    cout << "2. Manually Post Payment" << endl;
    cout << "3. Back" << endl;
}

int getChoice() {
    return getIntInput("");
}

void waitForKeyPress() {
    cout << "\nPress Enter to continue...";
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}