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
#include "BillCalendar.h"
#include "Subscription.h"
#include "invoicepdf.h"

using namespace std;
using namespace std::chrono;

BatchService::BatchService() {}

int BatchService::getPaymentID(const string& filename) {
    int nextPaymentID = 1;
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);
        string line;
        getline(file, line);
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            int testPaymentID = stoi(token);
            if (testPaymentID > nextPaymentID) {
                nextPaymentID = testPaymentID;
            }
        }
        file.close();
    } catch (const exception& e) {
        cerr << "Error in getPaymentID: " << e.what() << endl;
    }
    return nextPaymentID;
}


int BatchService::getBillingID(const string& filename) {
    int nextBillingID = 1;
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);
        string line;
        getline(file, line);
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            int testBillingID = stoi(token);
            if (testBillingID > nextBillingID) {
                nextBillingID = testBillingID;
            }
        }
        file.close();
    } catch (const exception& e) {
        cerr << "Error in getBillingID: " << e.what() << endl;
    }
    return nextBillingID;
}



vector<Customer> BatchService::loadCustomers(const string& filename) {
    vector<Customer> customers;
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);
        string line;
        getline(file, line);
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
    } catch (const exception& e) {
        cerr << "Error in loadCustomers: " << e.what() << endl;
    }
    return customers;
}


void BatchService::BillingBatch() {
    vector<Bill> bills;
    Rate rateObj;
    Subscription subObj;
    BillCalendar bc;
    Usage u;
    int currentBillCalendar = bc.getCurrentBillCalendar().getBillCalendarID();
    vector<Customer> customers = loadCustomers("data/customers.txt");
    vector<Rate> rates = rateObj.loadRates("data/rates.txt");
    vector<Subscription> subscriptions = subObj.loadSubscriptions("data/subscriptions.txt");
    vector<Usage> usageRecords = u.loadUsage("data/usage.txt", currentBillCalendar);
    int nextBillingID = getBillingID("data/bills.txt");
    for (auto& customer : customers) {
        int customerId = customer.getCustomerID();
        vector<Subscription> customerSubscriptions;
        for (auto& sub : subscriptions) {
            if (sub.getCustomerId() == customerId) {
                customerSubscriptions.push_back(sub);
            }
        }
        if (customerSubscriptions.empty()) {
            continue;
        }

        float totalBillAmount = 0.0;
        int providerId = -1; 
        int serviceId = -1; 
        for (auto& sub : customerSubscriptions) {
            bool usageFound = false;
            for (auto& usage : usageRecords) {
                if (usage.getCustomerId() == customerId && usage.getProviderId() == sub.getProviderId() && usage.getServiceId() == sub.getServiceId() && usage.getBillCalendarId() == currentBillCalendar) {
                    for (auto& rate : rates) {
                        if (rate.getRateId() == sub.getRateId()) {
                            float charge = 0.0;
                            if (rate.getMeasuredUsage() == 0) {
                                // Non-measured service: use FixedRateAmount only
                                charge = rate.getFixedRateAmount();
                            } else {
                                // Measured service: use current usage * VariableRateAmount + FixedRateAmount
                                float variableRate = rate.getVariableRateAmount();
                                float fixedRate = rate.getFixedRateAmount();
                                charge = (usage.getUsageAmount() * variableRate) + fixedRate;
                            }
                            totalBillAmount += charge;
                            if (providerId == -1) {
                                providerId = sub.getProviderId();
                                serviceId = sub.getServiceId();
                            }
                            usageFound = true;
                            break;
                        }
                    }
                    break;
                }
            }
            if (!usageFound) {
                cerr << "Warning: No usage found for CustomerID " << customerId                     << ", ProviderID " << sub.getProviderId()
                     << ", ServiceID " << sub.getServiceId()
                     << " in BillCalendarID " << currentBillCalendar << "\n";
            }
        }
        if (totalBillAmount == 0.0) {
            continue;
        }
        Bill newBill;
        newBill.setBillId(nextBillingID++);
        newBill.setCustomerId(customerId);
        newBill.setProviderId(providerId);
        newBill.setBillCalendarID(currentBillCalendar);
        newBill.setServiceId(serviceId);
        newBill.setBillAmount(totalBillAmount);
        newBill.setAmountPaid(0.0);
        newBill.setPaidInFull(false);
        chrono::sys_days today = floor<days>(system_clock::now());
        newBill.setBillDate(year_month_day{today});
        sys_days due = today + days{30};
        newBill.setDueDate(year_month_day{due});
        newBill.setOverdue(false);
        bills.push_back(newBill);
    }

    saveBills("data/bills.txt", bills, false);
    generatePDFInvoices(bills);
}

void BatchService::generatePDFInvoices(const std::vector<Bill>& bills){
    for(auto bill:bills){
        std::string beginning, end;
        int middle;
        beginning = "./invoices/";
        middle = bill.getBillId();
        end = ".pdf";
        std::string filepath = beginning + std::to_string(middle) + end;
        InvoicePDF i(filepath,bill);
        i.generate();   
    }
}


void BatchService::saveBills(const string &filename, const vector<Bill>& bills, bool overwrite) {
    try {
        ofstream file;
        if (overwrite) file.open(filename, ios::trunc);
        else file.open(filename, ios::app);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);

        if (overwrite) {
            file << "BillID,CustomerID,ProviderID,ServiceID,BillCalendarID,BillAmount,AmountPaid,Status,BillDate,DueDate,Overdue";
        }

        for (auto bill : bills) {
            file << "\n" << bill.getBillId() << "," << bill.getCustomerId() << "," << bill.getProviderId() << "," << bill.getServiceId() << ","
                 << bill.getBillCalendarID() << "," << bill.getBillAmount() << "," << bill.getAmountPaid() << "," << bill.getIsPaid() << ","
                 << int(bill.getBillDate().year()) << "-" << unsigned(bill.getBillDate().month()) << "-" << unsigned(bill.getBillDate().day()) << ","
                 << int(bill.getDueDate().year()) << "-" << unsigned(bill.getDueDate().month()) << "-" << unsigned(bill.getDueDate().day()) << ","
                 << bill.getOverdue();
        }

        file.close();
    } catch (const exception& e) {
        cerr << "Error in saveBills: " << e.what() << endl;
    }
}


void BatchService::postPayments(const string &filename, const vector<Payments>& payments, bool overwrite) {
    try {
        ofstream file;
        if (overwrite) file.open(filename, ios::trunc);
        else file.open(filename, ios::app);
        if (!file.is_open()) throw runtime_error("Failed to open file: " + filename);
        for (const auto& payment : payments) {
            file << "\n" << payment.paymentId << "," << payment.billId << "," << payment.paymentAmount;
        }
        file.close();
    } catch (const exception& e) {
        cerr << "Error in postPayments: " << e.what() << endl;
    }
}



void BatchService::SimulatePayments() {
    Bill billObj;
    vector<Bill> bills = billObj.loadBills("data/bills.txt");
    vector<Payments> payments;
    BillCalendar bc;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100);
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_real_distribution<> partialPay(0.1, 0.9);
    float paymentAmount = 0.0;
    int paymentId = getPaymentID("data/payments.txt");
    sys_days todaySys = floor<days>(system_clock::now());
    year_month_day today = year_month_day{todaySys};
    int currentBillCalendar = bc.getCurrentBillCalendar().getBillCalendarID();
    for (auto& bill : bills) {
        if (bill.getBillCalendarID() == currentBillCalendar) {
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
