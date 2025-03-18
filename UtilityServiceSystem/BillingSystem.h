#ifndef BILLINGSYSTEM_H
#define BILLINGSYSTEM_H

#include <string>
#include <vector>
#include "UtilityProvider.h"
#include "Customer.h"
#include "Bill.h"

class BillingSystem {
public:
    void addProvider(UtilityProvider provider);
    void addCustomer(Customer customer);
    void generateBill(Bill bill);
    void trackOverduePayments(Date currentDate);
    void displayBills();

private:
    std::vector<UtilityProvider> providers;
    std::vector<Customer> customers;
    std::vector<Bill> bills;
};;

#endif