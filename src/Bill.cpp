#include "Bill.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include "Date.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Date.h"

using namespace std;

Bill::Bill() : billId(0), customerId(0), providerId(0), serviceId(0),
               billCalendarID(0), billAmount(0.0), amountPaid(0.0),
               paidInFull(false), overdue(false) {}

Bill::Bill(int bid, int cid, int pid, int sid, int bcid, double amount,
           double paid, bool status, year_month_day bdate, year_month_day ddate)
    : billId(bid), customerId(cid), providerId(pid), serviceId(sid),
      billCalendarID(bcid), billAmount(amount), amountPaid(paid),
      paidInFull(status), billDate(bdate), dueDate(ddate), overdue(false) {}

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

vector<Bill>  Bill::loadBills(const string& filename){
    vector<Bill> bills;
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        throw std::runtime_error("Unable to open file: " + filename);
    }    
    string line;

    getline(file,line); //skip header
    while(getline(file,line)){
        stringstream ss(line);
        string token;
        Bill b;
        getline(ss, token, ',');
        b.billId = stoi(token);
        getline(ss, token, ',');
        b.customerId = stoi(token);
        getline(ss, token, ',');
        b.providerId = stoi(token);
        getline(ss, token, ',');
        b.serviceId = stoi(token);
        getline(ss, token, ',');
        b.billCalendarID = stoi(token);
        getline(ss, token, ',');
        b.billAmount = stod(token);
        getline(ss, token, ',');
        b.amountPaid = stod(token);            
        getline(ss, token, ',');
        b.paidInFull = stoi(token);
        getline(ss, token, ',');
        Date d;
        b.billDate = d.parseDate(token);

        //b.billDate = token;
        getline(ss, token, ',');


        b.dueDate = d.parseDate(token);
        getline(ss, token, ',');
        b.overdue = stoi(token);
        //b.dueDate = token;
        bills.push_back(b);
    }
    file.close();

    return bills;
    
}

void Bill::saveBills(const string &filename, const vector<Bill> bills, bool overwrite){
    if(overwrite){
        ofstream file(filename, ios::trunc);
        file << 
        "BillID" << "," << "CustomerID"<< "," << "ProviderID" << "," << "ServiceID"<< "," <<
         "BillCalendarID" << "," << "BillAmount"<< "," << "AmountPaid"  << "," << "Status" <<
          "," <<"BillDate" << "," << "DueDate" << "," << "Overdue" ;
          for(const auto & bill:bills){
            file <<"\n"<< bill.billId << "," << bill.customerId << "," << bill.providerId << "," << bill.serviceId << "," << bill.billCalendarID << "," 
            << bill.billAmount << "," << bill.amountPaid <<    "," << bill.paidInFull << ","            
            << int(bill.billDate.year()) << "-" << unsigned(bill.billDate.month()) << "-"<< unsigned(bill.billDate.day()) << "," //BillDate
            << int(bill.dueDate.year()) << "-"<< unsigned(bill.dueDate.month()) << "-"<< unsigned(bill.dueDate.day())  //DueDate
            << ',' << bill.overdue;
        }
        file.close();              
    }
    else{
        ofstream file(filename, ios::app);
        for(const auto & bill:bills){
            file <<"\n"<< bill.billId << "," << bill.customerId << "," << bill.providerId << "," << bill.serviceId << "," << bill.billCalendarID << "," 
            << bill.billAmount << "," << bill.amountPaid <<    "," << bill.paidInFull << ","            
            << int(bill.billDate.year()) << "-" << unsigned(bill.billDate.month()) << "-"<< unsigned(bill.billDate.day()) << "," //BillDate
            << int(bill.dueDate.year()) << "-"<< unsigned(bill.dueDate.month()) << "-"<< unsigned(bill.dueDate.day())  //DueDate
            << ',' << bill.overdue;
        }
        file.close();
    }
    

}
Bill Bill::readBillById(int billId) {
    std::vector<Bill> bills = loadBills("data/bills.txt");

    for (auto& bill : bills) {
        if (bill.getBillId() == billId) {
            return bill;
        }
    }
    throw std::runtime_error("Bill with ID " + std::to_string(billId) + " not found");
}



void Bill::updateBill(){
    cout << "Enter the Bill ID to update: ";
    int billId;
    cin >> billId;
    cin.ignore();;
    Bill bill;
    try{
        
        bill = readBillById(billId);
        cout << "Found bill" << endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << "\n";
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
            cout << "Enter new Provider ID (int): ";
            int newId;
            cin >> newId;
            bill.providerId = newId;
            break;
        }
        case 2: {
            cout << "Enter new Service ID (int): ";
            int newId;
            cin >> newId;
            bill.serviceId = newId;
            break;
        }
        case 3: {
            cout << "Enter new Bill Amount (double): ";
            double newAmount;
            cin >> newAmount;
            bill.billAmount = newAmount;
            break;
        }
        case 4: {
            cout << "Enter new Amount Paid (double): ";
            double newAmount;
            cin >> newAmount;
            bill.amountPaid = newAmount;
            break;
        }
        case 5: {
            cout << "Enter Paid in Full (1 for true, 0 for false): ";
            int status;
            cin >> status;
            bill.paidInFull = (status == 1);
            break;
        }
        case 6: {
            cout << "Enter new Due Date (YYYY-MM-DD): ";
            string dateStr;
            getline(cin, dateStr);
            Date d;
            bill.dueDate = d.parseDate(dateStr);
            break;
        }
        default:
            cout << "Invalid choice. No changes made." << endl;
            return; 
    }
    for (auto& b : bills) {
        if (b.billId == billId) {
            b = bill; 
            break;
        }
    }
    try {
        saveBills("data/bills.txt", bills, true);
        std::cout << "Bill updated successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving bills: " << e.what() << "\n";
    }
}





void Bill::displayBill() {
    cout << "Bill ID: " << billId <<  endl << " Customer ID: " << customerId
         <<  endl << " Provider ID: " << providerId <<  endl << " Service ID: " << serviceId
         <<  endl << " Calendar ID: " << billCalendarID 
         <<  endl << " Amount: $" << billAmount <<  endl << " Paid: $" << amountPaid
         <<  endl << " Status: " << (paidInFull ? "Paid" : "Unpaid") 
         <<  endl << " Overdue: " << (overdue ? "Yes" : "No")
         <<  endl << " Bill Date: " << int(billDate.year()) << "-" 
         << unsigned(billDate.month()) << "-" << unsigned(billDate.day())
         <<  endl << " Due: " << int(dueDate.year()) << "-" 
         << unsigned(dueDate.month()) << "-" << unsigned(dueDate.day()) << endl;
}

void Bill::displayBill(Bill bill) {
    cout << "Bill ID: " << bill.billId <<  endl << " Customer ID: " << bill.customerId
         <<  endl << " Provider ID: " << bill.providerId <<  endl << " Service ID: " << bill.serviceId
         <<  endl << " Calendar ID: " << bill.billCalendarID 
         <<  endl << " Amount: $" << bill.billAmount <<  endl << " Paid: $" << bill.amountPaid
         <<  endl << " Status: " << (bill.paidInFull ? "Paid" : "Unpaid") 
         <<  endl << " Overdue: " << (bill.overdue ? "Yes" : "No")
         <<  endl << " Bill Date: " << int(bill.billDate.year()) << "-" 
         << unsigned(bill.billDate.month()) << "-" << unsigned(bill.billDate.day())
         <<  endl << " Due: " << int(bill.dueDate.year()) << "-" 
         << unsigned(bill.dueDate.month()) << "-" << unsigned(bill.dueDate.day()) << endl;
}

void Bill::listOverdueBills(){
    std::vector<Bill> bills = loadBills("data/bills.txt");
    cout << "Bill ID, CustomerID, BillAmount, DueDate, AmountPaid " << endl;
    for (auto& bill : bills) {
        if( bill.overdue){
            cout << bill.billId << "," << bill.customerId << "," << bill.billAmount <<","
            << int(bill.dueDate.year()) << "-" 
            << unsigned(bill.dueDate.month()) << "-" << unsigned(bill.dueDate.day()) <<"," << bill.amountPaid << endl;
        }
    }
    return;
}

void Bill::updateBillCalendar(){

}

void Bill::readAndDisplayBillById(int billId) {
    std::vector<Bill> bills = loadBills("data/bills.txt");
    for (auto& bill : bills) {
        if (bill.billId == billId) {
            displayBill(bill);
            return;
        }
    }
    cout << "Bill Not Found";
    return;
}


bool Bill::isOverdue(Date currentDate) {
    if (!paidInFull) {
        if (currentDate.year > static_cast<int>(dueDate.year())) {
            return true;
        } else if (currentDate.year == static_cast<int>(dueDate.year())) {
            if (currentDate.month > unsigned(dueDate.month())) {
                return true;
            } else if (currentDate.month == unsigned(dueDate.month())) {
                if (currentDate.day > unsigned(dueDate.day())) {
                    return true;
                }
            }
        }
    }
    return false;
}