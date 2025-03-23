#include "Bill.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <fstream>
#include "Date.h"

using namespace std;
using namespace std::chrono;
Bill::Bill() : test(0), billId(0), customerId(0), providerId(0), serviceId(0),
               billCalendarID(0), billAmount(0.0), amountPaid(0.0),
               paidInFull(false), overdue(false) {}

Bill::Bill(int bid, int cid, int pid, int sid, int bcid, double amount,
           double paid, bool status, year_month_day bdate, year_month_day ddate)
    : test(0), billId(bid), customerId(cid), providerId(pid), serviceId(sid),
      billCalendarID(bcid), billAmount(amount), amountPaid(paid),
      paidInFull(status), billDate(bdate), dueDate(ddate), overdue(false) {}

int Bill::getTest() { return test; }
int Bill::getBillId() { return billId; }
int Bill::getCustomerId() { return customerId; }
int Bill::getProviderId() { return providerId; }
int Bill::getServiceId() { return serviceId; }
int Bill::getBillCalendarID() { return billCalendarID; }
double Bill::getBillAmount() { return billAmount; }
double Bill::getAmountPaid() { return amountPaid; }
bool Bill::getIsPaid() { return paidInFull; }
year_month_day Bill::getBillDate() { return billDate; }
year_month_day Bill::getDueDate() { return dueDate; }
bool Bill::getOverdue() { return overdue; }

void Bill::setTest(int newTest) { test = newTest; }
void Bill::setBillId(int id) { billId = id; }
void Bill::setCustomerId(int id) { customerId = id; }
void Bill::setProviderId(int id) { providerId = id; }
void Bill::setServiceId(int id) { serviceId = id; }
void Bill::setBillCalendarID(int id) { billCalendarID = id; }
void Bill::setBillAmount(double amount) { billAmount = amount; }
void Bill::setAmountPaid(double amount) { amountPaid = amount; }
void Bill::setPaidInFull(bool status) { paidInFull = status; }
void Bill::setBillDate(year_month_day date) { billDate = date; }
void Bill::setDueDate(year_month_day date) { dueDate = date; }
void Bill::setOverdue(bool status) { overdue = status; }

vector<Bill> Bill::loadBills(const string& filename) {
    vector<Bill> bills;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        throw runtime_error("Unable to open file: " + filename);
    }    
    string line;

    getline(file, line); 
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Bill b;
        getline(ss, token, ',');
        b.setBillId(stoi(token));
        getline(ss, token, ',');
        b.setCustomerId(stoi(token));
        getline(ss, token, ',');
        b.setProviderId(stoi(token));
        getline(ss, token, ',');
        b.setServiceId(stoi(token));
        getline(ss, token, ',');
        b.setBillCalendarID(stoi(token));
        getline(ss, token, ',');
        b.setBillAmount(stod(token));
        getline(ss, token, ',');
        b.setAmountPaid(stod(token));            
        getline(ss, token, ',');
        b.setPaidInFull(stoi(token));
        getline(ss, token, ',');
        Date d;
        b.setBillDate(d.parseDate(token));
        getline(ss, token, ',');
        b.setDueDate(d.parseDate(token));
        getline(ss, token, ',');
        b.setOverdue(stoi(token));
        bills.push_back(b);
    }
    file.close();
    return bills;
}

void Bill::saveBills(const string& filename, const vector<Bill>& bills, bool overwrite) {
    if (overwrite) {
        ofstream file(filename, ios::trunc);
        file << "BillID,CustomerID,ProviderID,ServiceID,BillCalendarID,BillAmount,AmountPaid,Status,BillDate,DueDate,Overdue";
        for (auto bill : bills) {
            file << "\n" << bill.getBillId() << "," << bill.getCustomerId() << "," << bill.getProviderId() << ","
                 << bill.getServiceId() << "," << bill.getBillCalendarID() << "," << bill.getBillAmount() << ","
                 << bill.getAmountPaid() << "," << bill.getIsPaid() << ","
                 << int(bill.getBillDate().year()) << "-" << unsigned(bill.getBillDate().month()) << "-" << unsigned(bill.getBillDate().day()) << ","
                 << int(bill.getDueDate().year()) << "-" << unsigned(bill.getDueDate().month()) << "-" << unsigned(bill.getDueDate().day()) << ","
                 << bill.getOverdue();
        }
        file.close();              
    } else {
        ofstream file(filename, ios::app);
        for (auto bill : bills) {
            file << "\n" << bill.getBillId() << "," << bill.getCustomerId() << "," << bill.getProviderId() << ","
                 << bill.getServiceId() << "," << bill.getBillCalendarID() << "," << bill.getBillAmount() << ","
                 << bill.getAmountPaid() << "," << bill.getIsPaid() << ","
                 << int(bill.getBillDate().year()) << "-" << unsigned(bill.getBillDate().month()) << "-" << unsigned(bill.getBillDate().day()) << ","
                 << int(bill.getDueDate().year()) << "-" << unsigned(bill.getDueDate().month()) << "-" << unsigned(bill.getDueDate().day()) << ","
                 << bill.getOverdue();
        }
        file.close();
    }
}

Bill Bill::readBillById(int billId) {
    vector<Bill> bills = loadBills("data/bills.txt");
    for (auto& bill : bills) {
        if (bill.getBillId() == billId) {
            return bill;
        }
    }
    throw runtime_error("Bill with ID " + to_string(billId) + " not found");
}

void Bill::updateBill() {
    cout << "Enter the Bill to update: ";
    int billId;
    cin >> billId;
    cin.ignore();
    Bill bill;
    try {
        bill = readBillById(billId);
    } catch (const runtime_error& e) {
        cerr << e.what() << "\n";
        return; 
    }

    cout << "\nSelect the property to update:\n"
         << "1. Provider ID\n"
         << "2. Service ID\n"
         << "3. Bill Amount\n"
         << "4. Amount Paid\n"
         << "5. Paid in Full\n"
         << "6. Due Date\n"
         << "Enter choice (1-6): ";

    int choice;
    cin >> choice;
    cin.ignore(); 
    vector<Bill> bills = loadBills("data/bills.txt");

    switch (choice) {
        case 1: {
            cout << "Enter new Provider ID: ";
            int newId;
            cin >> newId;
            bill.setProviderId(newId);
            break;
        }
        case 2: {
            cout << "Enter new Service ID: ";
            int newId;
            cin >> newId;
            bill.setServiceId(newId);
            break;
        }
        case 3: {
            cout << "Enter new Bill Amount: ";
            double newAmount;
            cin >> newAmount;
            bill.setBillAmount(newAmount);
            break;
        }
        case 4: {
            cout << "Enter new Amount Paid: ";
            double newAmount;
            cin >> newAmount;
            bill.setAmountPaid(newAmount);
            break;
        }
        case 5: {
            cout << "Enter Paid in Full: ";
            int status;
            cin >> status;
            bill.setPaidInFull(status == 1);
            break;
        }
        case 6: {
            cout << "Enter new Due Date (YYYY-MM-DD): ";
            string dateStr;
            getline(cin, dateStr);
            Date d;
            bill.setDueDate(d.parseDate(dateStr));
            break;
        }
        default:
            cout << "Invalid choice. No changes made." << endl;
            return; 
    }
    for (auto& b : bills) {
        if (b.getBillId() == billId) {
            b = bill; 
            break;
        }
    }
    try {
        saveBills("data/bills.txt", bills, true);
        cout << "Bill updated successfully!\n";
    } catch (const exception& e) {
        cerr << "Error saving bills: " << e.what() << "\n";
    }
}

void Bill::displayBill() {
    cout << getTest() << endl;
    cout << "Bill ID: " << getBillId() << endl << " Customer ID: " << getCustomerId()
         << endl << " Provider ID: " << getProviderId() << endl << " Service ID: " << getServiceId()
         << endl << " Calendar ID: " << getBillCalendarID() 
         << endl << " Amount: $" << getBillAmount() << endl << " Paid: $" << getAmountPaid()
         << endl << " Status: " << (getIsPaid() ? "Paid" : "Unpaid") 
         << endl << " Overdue: " << (getOverdue() ? "Yes" : "No")
         << endl << " Bill Date: " << int(getBillDate().year()) << "-" 
         << unsigned(getBillDate().month()) << "-" << unsigned(getBillDate().day())
         << endl << " Due: " << int(getDueDate().year()) << "-" 
         << unsigned(getDueDate().month()) << "-" << unsigned(getDueDate().day()) << endl;
}

void Bill::readAndDisplayBillById(int billId) {
    vector<Bill> bills = loadBills("data/bills.txt");
    for (auto& bill : bills) {
        if (bill.getBillId() == billId) {
            bill.displayBill();
            return;
        }
    }
    cout << "Bill Not Found" << endl;
}

void Bill::listOverdueBills() {
    vector<Bill> bills = loadBills("data/bills.txt");
    cout << "Bill ID, CustomerID, BillAmount, DueDate, AmountPaid" << endl;
    for (auto& bill : bills) {
        if (bill.getOverdue()) {
            cout << bill.getBillId() << "," << bill.getCustomerId() << "," << bill.getBillAmount() << ","
                 << int(bill.getDueDate().year()) << "-" 
                 << unsigned(bill.getDueDate().month()) << "-" << unsigned(bill.getDueDate().day()) << ","
                 << bill.getAmountPaid() << endl;
        }
    }
}

bool Bill::isOverdue(Date currentDate) {
    if (!getIsPaid()) {
        if (currentDate.year > static_cast<int>(getDueDate().year())) {
            return true;
        } else if (currentDate.year == static_cast<int>(getDueDate().year())) {
            if (currentDate.month > unsigned(getDueDate().month())) {
                return true;
            } else if (currentDate.month == unsigned(getDueDate().month())) {
                if (currentDate.day > unsigned(getDueDate().day())) {
                    return true;
                }
            }
        }
    }
    return false;
}