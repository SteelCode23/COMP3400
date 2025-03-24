#include <iostream>
#include <vector>
#include <string>
#include "UtilityService.h"

using namespace std;

class UtilityProvider {
    protected:
        int providerID;
        string name;
        vector<UtilityService*> services;

    public:
        UtilityProvider(int id, string n) {
            providerID = id;
            name = n;
        }

        
        int getProviderID() { return providerID; }
        string getName() { return name; }
        void setProviderID(int id) { providerID = id; }
        void setName(string n) { name = n; }

        void addService(UtilityService* service) { services.push_back(service); }

        void displayServices() {
            cout << "Provider: " << name << " offers:\n";
            for (auto service : services) {
                cout << "- " << service->getServiceType() << " (Bill for 100 usage: $"
                    << service->calculateCost(100) << ")\n";
            }
        }
};