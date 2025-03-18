#ifndef BATCH_H
#define BATCH_H

#include <vector>
#include <string>
#include <chrono>
#include "Bill.h"
#include "Customer.h"
#include "Payments.h"
#include "Rate.h"
#include "Usage.h"

using namespace std;
using namespace std::chrono;

class BatchService {
public:
    BatchService();

    int getPaymentID(const string& filename);
    int getBillingID(const string& filename);
    year_month_day parseDate(const string& dateStr);

    vector<Bill> loadBills(const string& filename);
    vector<Customer> loadCustomers(const string& filename);
    vector<Usage> loadUsage(const string& filename, int BillCalendarID);
    vector<Rate> loadRates(const string& filename);

    void BillingBatch(int BillCalendarID);
    void SimulatePayments(int BillCalendarID);
    void CalculateOverdue();

    void saveBills(const string &filename, const vector<Bill> bills, bool overwrite);
    void postPayments(const string &filename, const vector<Payments> payments, bool overwrite);
};

#endif
