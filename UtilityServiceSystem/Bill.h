#ifndef BILL_H
#define BILL_H

#include <string>
#include <chrono>
#include <iostream>
#include "Date.h"

using namespace std;
using namespace std::chrono;

class Bill {
    

public:
    int billId;
    int customerId;
    int providerId;
    int serviceId;
    int billCalendarID;
    double billAmount;
    double amountPaid;
    bool paidInFull;
    year_month_day billDate;
    year_month_day dueDate;
    bool overdue;
    Bill();
    Bill(int bid, int cid, int pid, int sid, int bcid, double amount,
         double paid, bool status, year_month_day bdate, year_month_day ddate);

    int getBillId();
    int getCustomerId();
    int getProviderId();
    int getServiceId();
    int getBillCalendarID();
    double getBillAmount();
    double getAmountPaid();
    bool getIsPaid();
    year_month_day getBillDate();
    year_month_day getDueDate();
    bool getOverdue();

    void setBillId(int id);
    void setCustomerId(int id);
    void setProviderId(int id);
    void setServiceId(int id);
    void setBillCalendarID(int id);
    void setBillAmount(double amount);
    void setAmountPaid(double amount);
    void setPaidInFull(bool status);
    void setBillDate(year_month_day date);
    void setDueDate(year_month_day date);
    void setOverdue(bool status);

    void displayBill();
    bool isOverdue(Date currentDate);
};

#endif