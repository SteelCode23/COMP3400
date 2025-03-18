#include "Customer.h"
#include <iostream>

Customer::Customer(int id, const std::string& name, const std::string& address, const std::string& phoneNumber)
    : customerID(id), name(name), address(address), phoneNumber(phoneNumber) {}

Customer::Customer() : customerID(0), name(""), address(""), phoneNumber("") {}

int Customer::getCustomerID() const { return customerID; }
std::string Customer::getName() const { return name; }
std::string Customer::getAddress() const { return address; }
std::string Customer::getPhoneNumber() const { return phoneNumber; }

void Customer::setName(const std::string& name) { this->name = name; }
void Customer::setAddress(const std::string& address) { this->address = address; }
void Customer::setPhoneNumber(const std::string& phoneNumber) { this->phoneNumber = phoneNumber; }

void Customer::subscribeService(UtilityService* service) { subscribedServices.push_back(service); }
void Customer::addBill(Bill* bill) { billingHistory.push_back(bill); }

void Customer::viewBills() const {
    std::cout << "Billing history for " << name << ":\n";
    for (const auto& bill : billingHistory) bill->displayBill();
}

void Customer::makePayment(int billID) {
    for (auto& bill : billingHistory) {
        if (bill->getBillId() == billID && !bill->getIsPaid()) {
            bill->setPaidInFull(true);
            std::cout << "Payment made for Bill ID " << billID << "\n";
            return;
        }
    }
    std::cout << "Bill not found or already paid.\n";
}