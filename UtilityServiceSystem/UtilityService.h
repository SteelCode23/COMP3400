#ifndef UTILITYSERVICE_H
#define UTILITYSERVICE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


class UtilityService {
protected:
    string serviceType;
    int providerID;
    double fixedMeterRate;
    double usageRate;

public:
    UtilityService(string type, int pid, double fixedRate, double usageRate) {
        this->serviceType = type;
        this->providerID = pid;
        this->fixedMeterRate = fixedRate;
        this->usageRate = usageRate;
    }
    
    virtual double calculateCost(double usage) = 0;
    virtual ~UtilityService() {}

    
    string getServiceType() { return serviceType; }
    int getProviderID() { return providerID; }
    double getFixedMeterRate() { return fixedMeterRate; }
    double getUsageRate() { return usageRate; }

    void setServiceType(string type) { serviceType = type; }
    void setProviderID(int pid) { providerID = pid; }
    void setFixedMeterRate(double rate) { fixedMeterRate = rate; }
    void setUsageRate(double rate) { usageRate = rate; }

    static vector<UtilityService*> loadServices(const string& filename);
    static void createService();
    static void saveServices(const string& filename, vector<UtilityService*>& services, bool overwrite);
    static UtilityService* readServiceById(int serviceId);
    void updateService(int serviceId);
    static void displayServices();
};

class NaturalGas : public UtilityService {
    public:
        NaturalGas(int pid, double fixedRate, double usageRate) : UtilityService("Natural Gas", pid, fixedRate, usageRate) {}
    
        double calculateCost(double usage) override {
            return fixedMeterRate + (usage * usageRate);
        }
};

class InternetService : public UtilityService {
    protected:
        string subType;
    public:
        InternetService(int pid, double fixedRate, double usageRate, string type) : UtilityService("Internet", pid, fixedRate, usageRate) {
            subType = type;
        }
    
        void setSubType(string type) { subType = type; }
        string getSubType() { return subType; }
    
        double calculateCost(double usage) override {
            return fixedMeterRate + usageRate;
        }
};

class HydroService : public UtilityService {
    protected:
        string subType;
    public:
        HydroService(int pid, double fixedRate, double usageRate, string type) : UtilityService("Hydro", pid, fixedRate, usageRate) {
            subType = type;
        }
    
        void setSubType(string type) { subType = type; }
        string getSubType() { return subType; }
    
        double calculateCost(double usage) override {
            return fixedMeterRate + (usage * usageRate);
        }
};

#endif