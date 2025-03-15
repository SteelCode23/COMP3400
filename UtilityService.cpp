// Abstract base class
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

    // Getters and Setters
    string getServiceType() { return serviceType; }
    int getProviderID() { return providerID; }
    double getFixedMeterRate() { return fixedMeterRate; }
    double getUsageRate() { return usageRate; }

    void setServiceType(string type) { serviceType = type; }
    void setProviderID(int pid) { providerID = pid; }
    void setFixedMeterRate(double rate) { fixedMeterRate = rate; }
    void setUsageRate(double rate) { usageRate = rate; }
}
