#include "Customer.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

Customer::Customer(int id, const string& name, const string& address, const string& phoneNumber)
    : customerID(id), name(name), address(address), phoneNumber(phoneNumber) {}

Customer::Customer() : customerID(0), name(""), address(""), phoneNumber("") {}

int Customer::getCustomerID() const { return customerID; }
string Customer::getName() const { return name; }
string Customer::getAddress() const { return address; }
string Customer::getPhoneNumber() const { return phoneNumber; }

void Customer::setName(const string& name) { this->name = name; }
void Customer::setAddress(const string& address) { this->address = address; }
void Customer::setPhoneNumber(const string& phoneNumber) { this->phoneNumber = phoneNumber; }
void Customer::setCustomerID(int id) { customerID = id; }

void Customer::subscribeService(UtilityService* service) { subscribedServices.push_back(service); }
void Customer::addBill(Bill* bill) { billingHistory.push_back(bill); }

void Customer::viewBills() const {
    cout << "Billing history for " << name << ":\n";
    // for (const auto& bill : billingHistory) bill->displayBill();
}

void Customer::makePayment(int billID) {
    for (auto& bill : billingHistory) {
        if (bill->getBillId() == billID && !bill->getIsPaid()) {
            bill->setPaidInFull(true);
            cout << "Payment made for Bill ID " << billID << "\n";
            return;
        }
    }
    cout << "Bill not found or already paid.\n";
}

vector<Customer> Customer::loadCustomers(const string& filename) {
    vector<Customer> customers;
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Customer c;
        getline(ss, token, ','); c.customerID = stoi(token);
        getline(ss, token, ','); c.name = token;
        getline(ss, token, ','); c.address = token;
        getline(ss, token, ','); c.phoneNumber = token;
        customers.push_back(c);
    }
    file.close();
    return customers;
}

void Customer::createCustomer() {
    Customer newCustomer;
    const string filename = "data/customers.txt";
    vector<Customer> customers = loadCustomers(filename);
    
    int nextCustomerId = 1;
    for (auto& customer : customers) {
        if (customer.getCustomerID() >= nextCustomerId) {
            nextCustomerId = customer.getCustomerID() + 1;
        }
    }
    newCustomer.setCustomerID(nextCustomerId);
    cin.ignore();
    cout << "Enter Customer Name (string): ";
    string name;
    getline(cin, name);
    newCustomer.setName(name);

    cout << "Enter Address (string): ";
    string address;
    getline(cin, address);
    newCustomer.setAddress(address);

    cout << "Enter Phone Number (string): ";
    string phoneNumber;
    getline(cin, phoneNumber);
    newCustomer.setPhoneNumber(phoneNumber);

    customers.push_back(newCustomer);
    saveCustomers(filename, customers, true);
}

void Customer::saveCustomers(const string& filename, vector<Customer>& customers, bool overwrite) {
    ofstream file(filename, overwrite ? ios::trunc : ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file\n";
        throw runtime_error("Unable to open file");
    }
    if (overwrite) {
        file << "CustomerID,Name,Address,PhoneNumber\n";
    }
    for (auto& customer : customers) {
        file << customer.getCustomerID() << "," << customer.getName() << ","
             << customer.getAddress() << "," << customer.getPhoneNumber() << "\n";
    }
    file.close();
}

Customer Customer::readCustomerById(int customerId) {
    string filename = "data/customers.txt";
    vector<Customer> customers = loadCustomers(filename);
    for (auto& customer : customers) {
        if (customer.getCustomerID() == customerId) {
            return customer;
        }
    }
    throw runtime_error("Customer does not exist!");
}

void Customer::updateCustomer() {
    cout << "Enter the Customer ID to update: ";
    int customerId;
    cin >> customerId;
    cin.ignore();

    Customer customer;
    try {
        customer = readCustomerById(customerId);
    }
    catch (runtime_error& e) {
        cerr << "Customer Does Not Exist\n";
        return;
    }

    cout << "\nSelect the property to update:\n"
         << "1. Name\n"
         << "2. Address\n"
         << "3. Phone Number\n"
         << "Enter choice (1-3): ";

    int choice;
    cin >> choice;
    cin.ignore();

    string filename = "data/customers.txt";
    vector<Customer> customers = loadCustomers(filename);

    switch (choice) {
        case 1: {
            cout << "Enter new Name (string): ";
            string newName;
            getline(cin, newName);
            customer.setName(newName);
            break;
        }
        case 2: {
            cout << "Enter new Address (string): ";
            string newAddress;
            getline(cin, newAddress);
            customer.setAddress(newAddress);
            break;
        }
        case 3: {
            cout << "Enter new Phone Number (string): ";
            string newPhone;
            getline(cin, newPhone);
            customer.setPhoneNumber(newPhone);
            break;
        }
        default:
            cout << "Invalid choice. No changes made.\n";
            return;
    }

    for (auto& c : customers) {
        if (c.getCustomerID() == customerId) {
            c = customer;
            break;
        }
    }
    saveCustomers(filename, customers, true);
}

void Customer::displayCustomers() {
    string filename = "data/customers.txt";
    vector<Customer> customers = loadCustomers(filename);
    cout << "CustomerID,Name,Address,PhoneNumber\n";
    for (auto customer : customers) {
        cout << customer.getCustomerID() << "," << customer.getName() << ","
             << customer.getAddress() << "," << customer.getPhoneNumber() << "\n";
    }
}
