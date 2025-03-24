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

class BatchService {
public:
    BatchService();

    int getPaymentID(const std::string& filename);
    int getBillingID(const std::string& filename);
    std::vector<Customer> loadCustomers(const std::string& filename);
    std::vector<Usage> loadUsage(const std::string& filename, int BillCalendarID);
    std::vector<Rate> loadRates(const std::string& filename);

    void BillingBatch(int BillCalendarID);
    void SimulatePayments(int BillCalendarID);
    void CalculateOverdue();

    void saveBills(const std::string& filename, const std::vector<Bill>& bills, bool overwrite);
    void postPayments(const string& filename, const vector<Payments>& payments, bool overwrite);


};

#endif