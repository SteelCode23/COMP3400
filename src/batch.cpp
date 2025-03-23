#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <random>
#include "batch.h"
#include "Bill.h"
#include "Customer.h"
#include "Payments.h"
#include "Rate.h"
#include "Usage.h"

using namespace std;
using namespace std::chrono;

BatchService::BatchService() {}

int BatchService::getPaymentID(const string& filename) {
    int nextPaymentID = 1;
    int testPaymentID;
    Payments p;
    ifstream file(filename);
    string line;
    getline(file, line); // Skip headers
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        testPaymentID = stoi(token);
        if (testPaymentID > nextPaymentID) {
            nextPaymentID = testPaymentID;
        }
    }
    file.close();
    return nextPaymentID;
}

int BatchService::getBillingID(const string& filename) {
    int nextBillingID = 1;
    int testBillingID;
    Bill b;
    ifstream file(filename);
    string line;
    getline(file, line); // Skip headers
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        testBillingID = stoi(token);
        if (testBillingID > nextBillingID) {
            nextBillingID = testBillingID;
        }
    }
    file.close();
    return nextBillingID;
}


vector<Customer> BatchService::loadCustomers(const string& filename) {
    vector<Customer> customers;
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Customer c;
        getline(ss, token, ',');
        c.customerID = stoi(token);
        getline(ss, token, ',');
        c.name = token;
        customers.push_back(c);
    }
    file.close();
    return customers;
}

vector<Usage> BatchService::loadUsage(const string& filename, int BillCalendarID) {
    vector<Usage> usageData;
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Usage u;
        getline(ss, token, ','); u.customerId = stoi(token);
        getline(ss, token, ','); u.providerId = stoi(token);
        getline(ss, token, ','); u.serviceId = stoi(token);
        getline(ss, token, ','); u.billCalendarId = stoi(token);
        getline(ss, token, ','); u.usageAmount = stoi(token);
        if (u.billCalendarId == BillCalendarID) {
            usageData.push_back(u);
        }
    }
    file.close();
    return usageData;
}

// vector<Rate> BatchService::loadRates(const string& filename) {
//     vector<Rate> rates;
//     ifstream file(filename);
//     string line;
//     getline(file, line); // Skip header
//     while (getline(file, line)) {
//         stringstream ss(line);
//         string token;
//         Rate r;
//         getline(ss, token, ','); r.rateId = stoi(token);
//         getline(ss, token, ','); r.serviceId = stoi(token);
//         getline(ss, token, ','); r.providerId = stoi(token);
//         getline(ss, token, ','); r.rateName = token;
//         getline(ss, token, ','); r.variableRateAmount = stod(token);
//         getline(ss, token, ','); r.fixedRateAmount = stod(token);
//         getline(ss, token, ','); r.unitOfMeasure = token;
//         getline(ss, token, ','); r.MeasuredUsage = stoi(token);
//         rates.push_back(r);
//     }
//     file.close();
//     return rates;
// }

void BatchService::BillingBatch(int BillCalendarID) {
    vector<Bill> bills;
    Rate rateObj;
    vector<Customer> customers = loadCustomers("data/customers.txt");
    vector<Rate> rates = rateObj.loadRates("data/rates.txt");
    vector<Usage> usageRecords = loadUsage("data/usage.txt", BillCalendarID);
    int nextBillingID = getBillingID("data/bills.txt");

    for (const auto& usage : usageRecords) {
        if (usage.billCalendarId == BillCalendarID) {
            float variableRate = 0.0, fixedRate = 0.0;
            for ( auto& r : rates) {
                if (r.getProviderId() == usage.providerId && r.getServiceId() == usage.serviceId) {
                    variableRate = r.getVariableRateAmount();
                    fixedRate = r.getFixedRateAmount();
                    break;
                }
            }

            Bill newBill;
            newBill.setBillId(nextBillingID++);
            newBill.setCustomerId(usage.customerId);
            newBill.setProviderId(usage.providerId);
            newBill.setBillCalendarID(BillCalendarID);
            newBill.setServiceId(usage.serviceId);
            newBill.setBillAmount((usage.usageAmount * variableRate) + fixedRate);
            newBill.setAmountPaid(0.0);
            newBill.setPaidInFull(false);
            chrono::sys_days today = floor<days>(system_clock::now());
            newBill.setBillDate(year_month_day{today});
            sys_days due = today + days{30};
            newBill.setDueDate(year_month_day{due});
            newBill.setOverdue(false);

            bills.push_back(newBill);
        }
    }
    saveBills("data/bills.txt", bills, false);
}

void BatchService::saveBills(const string& filename, const vector<Bill>& bills, bool overwrite) {
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
        for ( auto bill : bills) {
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
void BatchService::postPayments(const string& filename, const vector<Payments>& payments, bool overwrite) {
    ofstream file(filename, ios::app);
    for (auto payment : payments) {
        file << "\n" << payment.paymentId << "," << payment.billId << "," << payment.paymentAmount;
    }
    file.close();
}


void BatchService::SimulatePayments(int BillCalendarID) {
    Bill billObj;
    vector<Bill> bills = billObj.loadBills("data/bills.txt");
    vector<Payments> payments;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100);
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_real_distribution<> partialPay(0.1, 0.9);
    float paymentAmount = 0.0;
    int paymentId = getPaymentID("data/payments.txt");
    sys_days todaySys = floor<days>(system_clock::now());
    year_month_day today = year_month_day{todaySys};

    for (auto& bill : bills) {
        if (bill.getBillCalendarID() == BillCalendarID) {
            bool simulatedPayment = dis(gen) < 0.95;
            if (simulatedPayment) {
                paymentAmount = bill.getBillAmount();
                bill.setAmountPaid(bill.getBillAmount());
                bill.setPaidInFull(true);
            } else {
                if (dis(gen) < 0.5) {
                    bill.setAmountPaid(0.0);
                    bill.setPaidInFull(false);
                } else {
                    paymentAmount = bill.getBillAmount() * partialPay(gen);
                    bill.setAmountPaid(paymentAmount);
                    bill.setPaidInFull(false);
                }
            }
            if (paymentAmount > 0.0) {
                Payments paymentrecord;
                paymentrecord.paymentId = paymentId++;
                paymentrecord.billId = bill.getBillId();
                paymentrecord.paymentAmount = paymentAmount;
                payments.push_back(paymentrecord);
            }
        }
    }
    saveBills("data/bills.txt", bills, true);
    postPayments("data/payments.txt", payments, false);
    CalculateOverdue();
}


void BatchService::CalculateOverdue() {
    Bill billObj;
    vector<Bill> bills = billObj.loadBills("data/bills.txt");
    for (auto& bill : bills) {
        sys_days today = floor<days>(system_clock::now());
        sys_days dueSysDays = sys_days(bill.getDueDate());
        auto daysDifference = duration_cast<days>(today - dueSysDays).count();
        if ((bill.getBillAmount() - bill.getAmountPaid() > 0) && (daysDifference > 30)) {
            bill.setOverdue(true);
        }
    }
    saveBills("data/bills.txt", bills, true);
}