#include "Usage.h"
#include "Customer.h"
#include "Rate.h"
#include "UtilityProvider.h"
#include "Date.h"
#include "BillCalendar.h"
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


void Usage::simulateUsage(){
/*
In the real world usage of electricity, water, and natural gas would be measured by a meter and transmitted
to the billing system likely on a daily basis. Since we don't have meters feeding our application, we will
have to simulate the data. The logic will be that all customers on fixed rate services will just continue on
them the next month and then their usage for all other services will vary plus or minus 5% every month.
*/

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(-0.05, 0.05); // ±5% variation
BillCalendar bc;
Rate r;
//Customer customerObj;
string usageFile = "data/usage.txt";
int currentBillCalendar = bc.getCurrentBillCalendar().getBillCalendarID();
vector<Usage> currentUsage = loadUsage("data/usage.txt", currentBillCalendar);
vector<Rate> rateRecords = r.loadRates("data/rates.txt");
//vector<Customer> customers = customerObj.loadCustomers("data/customers.txt");
int prevBillCalendarId = currentBillCalendar - 1;

vector<Usage> prevUsage = loadUsage("data/usage.txt", prevBillCalendarId);

if (!currentUsage.empty()) {
    cout << "Usage already simulated for Bill Calendar " << currentBillCalendar << "\n";
    return;
}

if (prevUsage.empty()) {
    cout << "Error No Usage ";
    return;
}
vector<Usage> newUsageVector;
int usageId = 0;
for(auto usage: prevUsage){
    if(usage.getUsageId() > usageId){
        usageId = usage.getUsageId();
    }
}
for(auto usage: prevUsage){
    Usage newUsage;
    for(auto rate:rateRecords){
        if(rate.getServiceId() == usage.getServiceId() && rate.getProviderId() == usage.getProviderId()){
            if(rate.getMeasuredUsage() == 0){
                //carry measurement forward.
                newUsage = usage;
            }
            else{
                newUsage = usage;
                //Add + or - 5% to the usage for measured services.
                double variation = dis(gen);
                newUsage.setUsageAmount(usage.getUsageAmount() + variation );
                
            }
            newUsage.setUsageId(usageId++);
            newUsage.setBillCalendarId(currentBillCalendar);
            newUsageVector.push_back(newUsage);
            break;
        }
    }
}
try {
    ofstream file(usageFile, ios::app);
    for (auto& u : newUsageVector) {
        file << u.getUsageId() << "," << u.getCustomerId() << "," << u.getProviderId() << ","<< u.getServiceId() << "," << u.getBillCalendarId() << "," << u.getUsageAmount() << endl;
    }
    file.close();

} catch (exception& e) {
    cerr << "Error saving simulated usage: " << e.what() << "\n";
}

}