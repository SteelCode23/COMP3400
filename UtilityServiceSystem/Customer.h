#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "UtilityService.h"
#include "Bill.h"

class Customer {
private:


public:
    int customerID;
    std::string name;
    std::string address;
    std::string email;
    std::string phoneNumber;
    std::vector<UtilityService*> subscribedServices;
    std::vector<Bill*> billingHistory;
    Customer(int id, const std::string& name, const std::string& address, const std::string& phoneNumber);
    Customer();

    int getCustomerID() const;
    std::string getName() const;
    std::string getAddress() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    void setName(const std::string& name);
    void setAddress(const std::string& address);
    void setEmail(const std::string& email);
    void setPhoneNumber(const std::string& phoneNumber);
    void setCustomerID(int id);
    void subscribeService(UtilityService* service);
    void addBill(Bill* bill);
    void viewBills() const;
    void makePayment(int billID);
    static std::vector<Customer> loadCustomers(const std::string& filename);
    void createCustomer();
    static void saveCustomers(const std::string& filename, std::vector<Customer>& customers, bool overwrite);
    static Customer readCustomerById(int customerId);
    void updateCustomer();
    static void displayCustomers();
};

#endif // CUSTOMER_H
