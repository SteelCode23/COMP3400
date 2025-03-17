#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Customer {
    int customerID;
    string name, address;
    vector<UtilityService*> subscribedServices;
    vector<Bill*> billingHistory;

public:
    Customer(int id, string n, string addr) {
        customerID = id;
        name = n;
        address = addr;
    }

    // Getters and Setters
    int getCustomerID() { return customerID; }
    string getName() { return name; }
    string getAddress() { return address; }

    void subscribeService(UtilityService* service) { subscribedServices.push_back(service); }
    void addBill(Bill* bill) { billingHistory.push_back(bill); }

    void viewBills() {
        cout << "Billing history for " << name << ":\n";
        for (auto bill : billingHistory) bill->displayBill();
    }

    void makePayment(int billID) {
        for (auto bill : billingHistory) {
            if (bill->getBillID() == billID && !bill->getIsPaid()) {
                bill->markPaid();
                cout << "Payment made for Bill ID " << billID << endl;
                return;
            }
        }
        cout << "Bill not found or already paid.\n";
    }
};
