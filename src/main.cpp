#include "Menu.h"
#include "batch.h"
#include "Bill.h"
#include "BillCalendar.h"
#include "InputHelper.h"
#include <iostream>
#include "Usage.h"
#include "UtilityProvider.h"
#include "UtilityService.h"
#include "invoicepdf.h"
using namespace std;

int main() {
    BatchService batchService;
    Bill bill;
    BillCalendar bc;
    UtilityService us;
    UtilityProvider up;
    Customer c;
    BillCalendar openbillcalendar = bc.getCurrentBillCalendar();
    Usage u;
    clearScreen();
    int choice,choice2,choice3,choice4,choice5,choice6;
    Rate r;

    do {
        showMenu();
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1:
                clearScreen();
                showBillingMenu();
                choice2 = getIntInput("Enter billing option: ");
                switch(choice2){
                    case 1:
                        batchService.BillingBatch();
                        cout << "\033[1;32mBilling Batches Completed";
                        break;
                    case 2:
                        batchService.CalculateOverdue();
                        cout << "\033[1;32mOverdue Bills calculated";
                        break;
                    case 3:
                        bill.updateBill();   
                        break;
                    case 4: {
                        int billID = getIntInput("Enter Bill ID: ");
                        bill.readAndDisplayBillById(billID);                    
                        break;
                    }
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
                        u.simulateUsage();
                       break;
                    case 10:
                        batchService.SimulatePayments();
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                clearScreen();
                showCustomerMenu();
                choice2 = getIntInput("Enter customer option: ");
                switch(choice2){
                case 1:
                    c.createCustomer();
                    break;
                case 2:
                    c.updateCustomer();
                    break;
                case 3:
                    c.displayCustomers();
                    break;
                default:
                    break;
                }
                break;
            case 3:
                clearScreen();
                showProviderMenu();
                choice3 = getIntInput("Enter provider option: ");
                switch(choice3){
                case 1:
                    up.createProvider();
                    break;
                case 2:
                    up.updateProvider();
                    break;
                case 3:
                    up.displayProviders();
                    break;
                default:
                    break;
                }
                break;
            case 4:
                clearScreen();
                showServiceMenu();
                choice4 = getIntInput("Enter service option: ");
                switch(choice4){
                case 1:
                    us.createService();
                    break;
                case 2:
                    us.updateService();
                    break;
                case 3:
                    us.displayServices();
                    break;
                default:
                    break;
                }
                break;
            case 5:
                clearScreen();
                showRatesMenu();
                choice5 = getIntInput("Enter rate option: ");
                switch(choice5){
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
            case 6:
                clearScreen();
                bill.generateManagementReport(); 
                break;
            case 7:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "\033[1;31mInvalid choice! Try again." << endl;
        }
        if(choice != 7) 
        waitForKeyPress();
    } while (choice != 7);

    return 0;
}
