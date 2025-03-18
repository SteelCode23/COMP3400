#include "Menu.h"
#include "batch.h"
#include <iostream>

using namespace std;

int main() {
    BatchService batchService;
    int choice;
    do {
        showMenu();
        choice = getChoice();

        switch (choice) {
            case 1:
                cout << "Loading Bills..." << endl;
                batchService.loadBills("../data/bills.txt");
                break;
            case 2:
                cout << "Loading Customers..." << endl;
                batchService.loadCustomers("../data/customers.txt");
                break;
            case 3:
                cout << "Processing Payments..." << endl;
                batchService.postPayments("../data/payments.txt", {}, false);
                break;
            case 4:
                cout << "Generating Billing Batch..." << endl;
                batchService.BillingBatch(1);
                break;
            case 5:
                cout << "Simulating Payments..." << endl;
                batchService.SimulatePayments(1);
                break;
            case 6:
                cout << "Calculating Overdue Bills..." << endl;
                batchService.CalculateOverdue();
                break;
            case 7:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "\033[1;31mInvalid choice! Try again." << endl;
        }

        waitForKeyPress();
    } while (choice != 7);

    return 0;
}
