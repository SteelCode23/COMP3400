#ifndef BATCH_SERVICE_H
#define BATCH_SERVICE_H
#include "batch.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip> 

using namespace std;
using namespace std::chrono;

struct Customer{
    int id;
    string name;
 };

struct Usage {
    int customerId;
    int providerId;
    int serviceId;
    int billCalendarId;
    float usageAmount;
};

struct Rate {
    int rateId;
    int serviceId;
    int providerId;
    string rateName;
    float variableRateAmount;
    float fixedRateAmount;
    string unitOfMeasure; 
    bool MeasuredUsage;
};

struct Bill {
    int billId;
    int customerId;
    int providerId;
    int serviceId;
    int billCalendarID;
    float billAmount;
    float amountPaid;
    bool paidInFull;
    std::chrono::year_month_day dueDate;
    std::chrono::year_month_day billDate;
    bool overdue;

};

struct Payments{
    int paymentId;
    int billId;
    float paymentAmount;
};



class BatchService{

    public:
    BatchService();
    void BillingBatch(int BillCalendarID);
    void SimulatePayments(int BillCalendarID);
    void CalculateOverdue();

    private:
    int getBillingID(const string& filename);
    int getPaymentID(const string& filename);
    vector<Customer> loadCustomers(const string& filename);

    vector<Usage> loadUsage(const string& filename, int BillCalendarID);
    vector<Rate> loadRates(const string& filename);
    vector<Bill> loadBills(const string& filename);
    void saveBills(const string &filename, const vector<Bill> bills, bool overwrite);
    void postPayments(const string &filename, const vector<Payments> payments, bool overwrite);
    year_month_day parseDate(const string& dateStr);


};

#endif // BATCH_SERVICE_H