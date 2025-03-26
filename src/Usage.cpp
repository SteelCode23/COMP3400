#include "Usage.h"
#include "Customer.h"
#include "Rate.h"
#include "UtilityProvider.h"
#include "Date.h"
#include "BillCalendar.h"
#include "Subscription.h"
#include <random>
using namespace std;
using namespace std::chrono;
Usage::Usage() : usageId(0), customerId(0), providerId(0), serviceId(0), billCalendarId(0),
                                   usageAmount(0.0) {}

Usage::Usage(int uId, int cId, int pId, int sId, int bcId, int amount)
    : usageId(uId), customerId(cId), providerId(pId), serviceId(sId), billCalendarId(bcId),
      usageAmount(amount) {}

int Usage::getUsageId() { return usageId; }
int Usage::getCustomerId() { return customerId; }
int Usage::getProviderId() { return providerId; }
int Usage::getServiceId() { return serviceId; }
int Usage::getBillCalendarId() { return billCalendarId; }
int Usage::getUsageAmount() { return usageAmount; }
void Usage::setUsageId(int id) { usageId = id; }
void Usage::setCustomerId(int id) { customerId = id; }
void Usage::setProviderId(int id) { providerId = id; }
void Usage::setServiceId(int id) { serviceId = id; }
void Usage::setBillCalendarId(int id) { billCalendarId = id; }
void Usage::setUsageAmount(int amount) { usageAmount = amount; }

vector<Usage> Usage::loadUsage(const std::string& filename, int BillCalendarID) {
    vector<Usage> usageData;
    ifstream file(filename);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Usage u;
        
        getline(ss, token, ','); u.setUsageId(stoi(token));
        
        getline(ss, token, ','); u.setCustomerId(stoi(token));
        getline(ss, token, ','); u.setProviderId(stoi(token));
        getline(ss, token, ','); u.setServiceId(stoi(token));
        getline(ss, token, ','); u.setBillCalendarId(stoi(token));
        getline(ss, token, ','); u.setUsageAmount(stod(token)); 
        if (u.getBillCalendarId() == BillCalendarID) {
            usageData.push_back(u);
        }
    }
    file.close();
    return usageData;
}


void Usage::simulateUsage() {
    /*
    In the real world usage of electricity, water, and natural gas would be measured by a meter and transmitted
    to the billing system likely on a daily basis. Since we don't have meters feeding our application, we will
    have to simulate the data. The logic will be that all customers on fixed rate services will pull their monthly fee
    based on which servie they're subscribed to and then their usage for all other services will vary plus or minus 5% every month.
    */
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-0.05, 0.05); // ±5% variation
    BillCalendar bc;
    Rate r;
    Customer customerObj;
    Subscription subObj;
    string usageFile = "data/usage.txt";
    int currentBillCalendar = bc.getCurrentBillCalendar().getBillCalendarID();
    vector<Usage> currentUsage = loadUsage("data/usage.txt", currentBillCalendar);
    vector<Rate> rateRecords = r.loadRates("data/rates.txt");
    vector<Customer> customers = customerObj.loadCustomers("data/customers.txt");
    vector<Subscription> subscriptions = subObj.loadSubscriptions("data/subscriptions.txt");
    int prevBillCalendarId = currentBillCalendar - 1;
    vector<Usage> prevUsage = loadUsage("data/usage.txt", prevBillCalendarId);

    for(auto use:currentUsage){
        cout << use.getUsageId() << endl;
    }
    if (!currentUsage.empty()) {
        cout << "Usage already simulated for Bill Calendar " << currentBillCalendar << "\n";
        return;
    }

    // Seed initial usage if none exists for the previous bill calendar
    if (prevUsage.empty()) {
        cout << "No previous usage found. Seeding initial usage for Bill Calendar " << prevBillCalendarId << "\n";
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(100.0, 2000.0); // Initial range
        int usageId = 1;
        vector<Usage> initialUsage;
        for (auto& customer : customers) {
            vector<Subscription> customerSubs;
            for (auto& sub : subscriptions) {
                if (sub.getCustomerId() == customer.getCustomerID()) {
                    customerSubs.push_back(sub);
                }
            }
            for (auto& sub : customerSubs) {
                for (auto& rate : rateRecords) {
                    if (rate.getRateId() == sub.getRateId()) {
                        double initialAmount;
                        if (rate.getMeasuredUsage() == 1) {
                            // Measured services: random initial usage
                            if (rate.getServiceId() == 6) { // Hydro Electric (kWh)
                                uniform_real_distribution<> dis(300.0, 1000.0); // Range: 300-1000 kWh
                                initialAmount = dis(gen);
                            } else if (rate.getServiceId() == 7 || rate.getServiceId() == 8 || rate.getServiceId() == 1) { // Water, Sewage, Gas (m3)
                                uniform_real_distribution<> dis(5.0, 20.0); // Range: 5-20 m³
                                initialAmount = dis(gen);
                            } else {
                                uniform_real_distribution<> dis(100.0, 2000.0); // Default range
                                initialAmount = dis(gen);
                            }
                        } else {
                            // Non-measured services: use FixedRateAmount
                            initialAmount = rate.getFixedRateAmount();
                        }
                        Usage u(usageId++, customer.getCustomerID(), sub.getProviderId(), sub.getServiceId(),
                                prevBillCalendarId, initialAmount);
                        initialUsage.push_back(u);
                        break;
                    }
                }
            }
        }
        ofstream file(usageFile, ios::trunc);
        file << "UsageID,CustomerID,ProviderID,ServiceID,BillCalendarID,UsageAmount\n";
        for (auto& u : initialUsage) {
            file << u.getUsageId() << "," << u.getCustomerId() << "," << u.getProviderId() << ","
                 << u.getServiceId() << "," << u.getBillCalendarId() << "," << u.getUsageAmount() << "\n";
        }
        file.close();
        prevUsage = loadUsage("data/usage.txt", prevBillCalendarId); // Reload after seeding
    }

    vector<Usage> newUsageVector;
    int usageId = 0;
    for (auto usage : prevUsage) {
        if (usage.getUsageId() > usageId) {
            usageId = usage.getUsageId();
        }
    }
    usageId++;

    // Process each customer
    for (auto& customer : customers) {
        int customerId = customer.getCustomerID();
        // Find all subscriptions for this customer
        vector<Subscription> customerSubscriptions;
        for (auto& sub : subscriptions) {
            if (sub.getCustomerId() == customerId) {
                customerSubscriptions.push_back(sub);
            }
        }

        // Skip if the customer has no subscriptions
        if (customerSubscriptions.empty()) {
            continue;
        }

        for (auto& sub : customerSubscriptions) {
            Rate rate;
            bool rateFound = false;
            for (auto& r : rateRecords) {
                if (r.getRateId() == sub.getRateId()) {
                    rate = r;
                    rateFound = true;
                    break;
                }
            }
            if (!rateFound) {
                cout << "Warning: No rate found for RateID " << sub.getRateId() << ". Skipping subscription.\n";
                continue;
            }

            Usage newUsage;
            newUsage.setUsageId(usageId++);
            newUsage.setCustomerId(customerId);
            newUsage.setProviderId(sub.getProviderId());
            newUsage.setServiceId(sub.getServiceId());
            newUsage.setBillCalendarId(currentBillCalendar);
            if (rate.getMeasuredUsage() == 0) {
                // Non-measured service: use the FixedRateAmount from the rate
                newUsage.setUsageAmount(rate.getFixedRateAmount());
            } else {
                // Measured service: find previous usage for this customer and service type
                double prevAmount = 0.0;
                bool prevUsageFound = false;
                for (auto& prev : prevUsage) {
                    if (prev.getCustomerId() == customerId && prev.getServiceId() == sub.getServiceId()) {
                        prevAmount = prev.getUsageAmount();
                        prevUsageFound = true;
                        break;
                    }
                }
                if (!prevUsageFound) {
                    // If no previous usage, seed an initial value
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_real_distribution<> dis(100.0, 2000.0);
                    if (rate.getServiceId() == 6) { // Hydro Electric (kWh)
                        uniform_real_distribution<> dis(300.0, 1000.0); // Range: 300-1000 kWh
                        prevAmount = dis(gen);
                    } else if (rate.getServiceId() == 7 || rate.getServiceId() == 8 || rate.getServiceId() == 1) { // Water, Sewage, Gas (m3)
                        uniform_real_distribution<> dis(5.0, 20.0); // Range: 5-20 m³
                        prevAmount = dis(gen);
                    } else {
                        uniform_real_distribution<> dis(100.0, 2000.0); // Default range
                        prevAmount = dis(gen);
                    }
                }
                // Apply ±5% variation to the previous amount
                double variation = dis(gen);
                newUsage.setUsageAmount(prevAmount * (1 + variation));
            }

            newUsageVector.push_back(newUsage);
        }
    }

    try {
        ofstream file(usageFile, ios::app);
        if (!file.is_open()) throw runtime_error("Unable to open usage file for appending");
        for (auto& u : newUsageVector) {
            file << u.getUsageId() << "," << u.getCustomerId() << "," << u.getProviderId() << ","
                 << u.getServiceId() << "," << u.getBillCalendarId() << "," << u.getUsageAmount() << endl;
        }
        file.close();
    } catch (exception& e) {
        cerr << "Error saving simulated usage: " << e.what() << "\n";
    }
}