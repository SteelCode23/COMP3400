#include "Bill.h"
#include <iostream>
#include <chrono>
#include "Date.h"

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

void Bill::displayBill() {
    cout << "Bill ID: " << billId << " | Customer ID: " << customerId
         << " | Provider ID: " << providerId << " | Service ID: " << serviceId
         << " | Calendar ID: " << billCalendarID 
         << " | Amount: $" << billAmount << " | Paid: $" << amountPaid
         << " | Status: " << (paidInFull ? "Paid" : "Unpaid") 
         << " | Overdue: " << (overdue ? "Yes" : "No")
         << " | Bill Date: " << int(billDate.year()) << "-" 
         << unsigned(billDate.month()) << "-" << unsigned(billDate.day())
         << " | Due: " << int(dueDate.year()) << "-" 
         << unsigned(dueDate.month()) << "-" << unsigned(dueDate.day()) << endl;
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