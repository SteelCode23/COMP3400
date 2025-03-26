#include <iostream>
#include <vector>
#include <string>
#include "UtilityProvider.h"
#include "Customer.h"
#include "Bill.h"
#include "Date.h"

using namespace std;

class BillingSystem {
 //   vector<UtilityProvider> providers;
    vector<Customer> customers;
    vector<Bill> bills;

public:
 //   void addProvider(UtilityProvider provider) { providers.push_back(provider); }
    void addCustomer(Customer customer) { customers.push_back(customer); }
    void generateBill(Bill bill) { bills.push_back(bill); }

    void trackOverduePayments(Date currentDate) {
        cout << "Overdue Bills:\n";
        for (auto &bill : bills) {
            if (!bill.getIsPaid() && bill.isOverdue(currentDate)) {
                bill.displayBill();
            }
        }
    }

    void displayBills() {
        cout << "\nAll Bills:\n";
        for (auto &bill : bills) bill.displayBill();
    }
};