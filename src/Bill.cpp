#include "Bill.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
#include "Date.h"
#include "InputHelper.h"

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
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);
        string line;
        getline(file, line);
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            Bill b;
            getline(ss, token, ','); b.billId = stoi(token);
            getline(ss, token, ','); b.customerId = stoi(token);
            getline(ss, token, ','); b.providerId = stoi(token);
            getline(ss, token, ','); b.serviceId = stoi(token);
            getline(ss, token, ','); b.billCalendarID = stoi(token);
            getline(ss, token, ','); b.billAmount = stod(token);
            getline(ss, token, ','); b.amountPaid = stod(token);
            getline(ss, token, ','); b.paidInFull = stoi(token);
            Date d;
            getline(ss, token, ','); b.billDate = (d.parseDate(token));
            getline(ss, token, ','); b.dueDate = (d.parseDate(token));
            getline(ss, token, ','); b.overdue = stoi(token);
            bills.push_back(b);
        }
        file.close();
    } catch (const exception& e) {
        cerr << "Error in loadBills: " << e.what() << endl;
    }
    return bills;
}

void Bill::saveBills(const string &filename, const vector<Bill>& bills, bool overwrite) {
    try {
        ofstream file;
        if (overwrite) file.open(filename, ios::trunc);
        else file.open(filename, ios::app);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);

        if (overwrite) {
            file << "BillID,CustomerID,ProviderID,ServiceID,BillCalendarID,BillAmount,AmountPaid,Status,BillDate,DueDate,Overdue";
        }

        for (const auto &bill : bills) {
            file << "\n" << bill.billId << "," << bill.customerId << "," << bill.providerId << "," << bill.serviceId << ","
                 << bill.billCalendarID << "," << bill.billAmount << "," << bill.amountPaid << "," << bill.paidInFull << ","
                 << int(bill.billDate.year()) << "-" << unsigned(bill.billDate.month()) << "-" << unsigned(bill.billDate.day()) << ","
                 << int(bill.dueDate.year()) << "-" << unsigned(bill.dueDate.month()) << "-" << unsigned(bill.dueDate.day()) << ","
                 << bill.overdue;
        }

        file.close();
    } catch (const exception& e) {
        cerr << "Error in saveBills: " << e.what() << endl;
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
    int billId = getIntInput("Enter the Bill to update: ");
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
         << "6. Due Date\n";

    int choice = getIntInput("Enter choice (1-6): ");
    cin.ignore(); 
    vector<Bill> bills = loadBills("data/bills.txt");

    switch (choice) {
        case 1: {
            int newId = getIntInput("Enter new Provider ID: ");
            bill.setProviderId(newId);
            break;
        }
        case 2: {
            int newId = getIntInput("Enter new Service ID: ");
            bill.setServiceId(newId);
            break;
        }
        case 3: {
            double newAmount = getDoubleInput("Enter new Bill Amount: ");
            bill.setBillAmount(newAmount);
            break;
        }
        case 4: {
            double newAmount = getDoubleInput("Enter new Amount Paid: ");
            bill.setAmountPaid(newAmount);
            break;
        }
        case 5: {
            int status = getIntInput("Enter Paid in Full (0 or 1): ");
            bill.setPaidInFull(status == 1);
            break;
        }
        case 6: {
            string dateStr = getLineInput("Enter new Due Date (YYYY-MM-DD): ");
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
    //cout << getTest() << endl; antique code
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
        if (currentDate.year > static_cast<int>(getDueDate().year())) return true;
        if (currentDate.year == static_cast<int>(getDueDate().year())) {
            if (currentDate.month > unsigned(getDueDate().month())) return true;
            if (currentDate.month == unsigned(getDueDate().month()) &&
                currentDate.day > unsigned(getDueDate().day())) return true;
        }
    }
    return false;
}

void Bill::generateManagementReport() {
    vector<Bill> bills = loadBills("data/bills.txt");

    map<pair<int, int>, pair<double, double>> reportTotals;

    for (const auto& bill : bills) {
        auto key = make_pair(bill.providerId, bill.serviceId);
        reportTotals[key].first += bill.billAmount;
        reportTotals[key].second += bill.amountPaid;
    }

   cout << "\033[1;34m" << left << setw(15) << "Provider ID"
         << "\033[1;32m" << setw(15) << "Service ID"
         << "\033[1;33m" << setw(20) << "Total Bill Amount"
         << "\033[1;36m" << setw(20) << "Total Amount Paid" << "\033[0m" << endl;
    cout << "\033[1;37m" << string(70, '-') << "\033[0m" << endl;


    for (const auto& [key, totals] : reportTotals) {

    string colorProvider = "\033[1;34m";
    string colorService  = "\033[1;32m";
    string colorBill     = "\033[1;33m";
    string colorPaid     = "\033[1;36m";

    cout << colorProvider << left << setw(15) << key.first
         << colorService  << setw(15) << key.second
         << colorBill     << "$" << setw(19) << fixed << setprecision(2) << totals.first
         << colorPaid     << "$" << setw(19) << fixed << setprecision(2) << totals.second
         << "\033[0m" << endl;
    }
    try {
    ofstream out("report.txt", ios::trunc);
    if (!out.is_open()) throw runtime_error("Failed to open report.txt for writing.");

    out << left << setw(15) << "Provider ID"
        << setw(15) << "Service ID"
        << setw(20) << "Total Bill Amount"
        << setw(20) << "Total Amount Paid" << endl;
    out << string(70, '-') << endl;

    for (const auto& [key, totals] : reportTotals) {
        out << left << setw(15) << key.first
            << setw(15) << key.second
            << "$" << setw(19) << fixed << setprecision(2) << totals.first
            << "$" << setw(19) << fixed << setprecision(2) << totals.second
            << endl;
    }

    out.close();
    cout << "\n\033[1;32mReport written to report.txt\n";
} catch (const exception& e) {
    cerr << "Error writing report.txt: " << e.what() << endl;
}


}

