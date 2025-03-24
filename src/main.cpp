#include "Menu.h"
#include "batch.h"
#include "Bill.h"
#include "BillCalendar.h"
#include <iostream>

using namespace std;

int main() {
    BatchService batchService;
    Bill bill;
    BillCalendar bc;
    BillCalendar  openbillcalendar = bc.getCurrentBillCalendar();
    int choice;
    int choice2;
    Rate r;
    int choice6;    
    do {
        showMenu();
        choice = getChoice();

        switch (choice) {
            case 1:

                showBillingMenu();
                choice2 = getChoice();
                switch(choice2){

                    case 1:
                        
                        batchService.BillingBatch(openbillcalendar.billCalendarID);
                        cout << "Billing Batches Completed";
                        break;
                    case 2:
                        batchService.CalculateOverdue();
                        cout << "Overdue Bills calculated";
                        break;
                    case 3:
                        bill.updateBill();   
                        break;                

                    case 4:
                        int billID;
                        cout << "Enter Bill ID"<< endl;
                        cin >> billID;
                        bill.readAndDisplayBillById(billID);                    
                        break;
                    case 5:
                        bill.listOverdueBills();
                       break;
                    case 6:
                        bc.createBillCalendar();
                        break;
                    case 7:
                        bc.displayCalendar();
                       break;
                    case 8:
                        bc.updateBillCalendar();
                       break;
                    case 9:
                       break;

                }
                break;
            case 2:
                showCustomerMenu();
                choice2 = getChoice();
                batchService.loadCustomers("../data/customers.txt");
                break;
            case 3:
                showUsageMenu();
                cout << "Processing Payments..." << endl;
                batchService.postPayments("../data/payments.txt", {}, false);
                break;
            case 4:
                showProviderMenu();
                cout << "Generating Billing Batch..." << endl;
                batchService.BillingBatch(1);
                break;
            case 5:
                showServiceMenu();
                break;


            case 6:
                showRatesMenu();

                choice6 = getChoice();
                switch(choice6){
                case 1:
                    r.createRate();
                    break;
                case 2:
                    r.displayRates();
                    break;
                case 3:
                    r.updateRate();
                break;
                    default:
                        break;
                }
                break;
            case 7:
                showPaymentsMenu();
                cout << "Exiting..." << endl;
                break;
            case 8:
                cout << "Exiting..." << endl;
                break;                
            default:
                cout << "\033[1;31mInvalid choice! Try again." << endl;
        }

        waitForKeyPress();
    } while (choice != 8);

    return 0;
}