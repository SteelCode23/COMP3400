#ifndef BILL_H
#define BILL_H

#include <string>
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Date.h"

class Bill {
private:
    int test;
    int billId;
    int customerId;
    int providerId;
    int serviceId;
    int billCalendarID;
    double billAmount;
    double amountPaid;
    bool paidInFull;
    std::chrono::year_month_day billDate;
    std::chrono::year_month_day dueDate;
    bool overdue;

public:
    Bill();
    Bill(int bid, int cid, int pid, int sid, int bcid, double amount,
         double paid, bool status, std::chrono::year_month_day bdate,
         std::chrono::year_month_day ddate);

    
    int getTest();
    int getBillId();
    int getCustomerId();
    int getProviderId();
    int getServiceId();
    int getBillCalendarID();
    double getBillAmount();
    double getAmountPaid();
    bool getIsPaid();
    std::chrono::year_month_day getBillDate();
    std::chrono::year_month_day getDueDate();
    bool getOverdue();

    
    void setTest(int newTest);
    void setBillId(int id);
    void setCustomerId(int id);
    void setProviderId(int id);
    void setServiceId(int id);
    void setBillCalendarID(int id);
    void setBillAmount(double amount);
    void setAmountPaid(double amount);
    void setPaidInFull(bool status);
    void setBillDate(std::chrono::year_month_day date);
    void setDueDate(std::chrono::year_month_day date);
    void setOverdue(bool status);

    
    std::vector<Bill> loadBills(const std::string& filename);
    Bill readBillById(int billId);
    void displayBill();
    void readAndDisplayBillById(int billId);
    void saveBills(const std::string& filename, const std::vector<Bill>& bills, bool overwrite);
    void updateBill();
    void listOverdueBills();
    void updateBillCalendar();
    bool isOverdue(Date currentDate);
};

#endif