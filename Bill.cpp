#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Bill {
    int billID, customerID, providerID;
    string serviceType;
    double usage, totalCost;
    Date issueDate, dueDate;
    bool isPaid;

public:
    Bill(int bid, int cid, int pid, string type, double usage, Date issue, Date due) 
        : issueDate(issue), dueDate(due) {
        billID = bid;
        customerID = cid;
        providerID = pid;
        serviceType = type;
        this->usage = usage;
        isPaid = false;
        totalCost = 0;
    }

    // Getters and Setters
    int getBillID() { return billID; }
    int getCustomerID() { return customerID; }
    int getProviderID() { return providerID; }
    string getServiceType() { return serviceType; }
    double getUsage() { return usage; }
    double getTotalCost() { return totalCost; }
    bool getIsPaid() { return isPaid; }

    void setTotalCost(double cost) { totalCost = cost; }
    void markPaid() { isPaid = true; }

    bool isOverdue(Date currentDate) {
        return dueDate.isOverdue(currentDate);
    }

    void displayBill() {
        cout << "Bill ID: " << billID << " | Customer ID: " << customerID
             << " | Provider ID: " << providerID << " | Service: " << serviceType
             << " | Amount: $" << totalCost << " | Status: " << (isPaid ? "Paid" : "Unpaid") << " | Due: ";
        dueDate.display();
        cout << endl;
    }
};
