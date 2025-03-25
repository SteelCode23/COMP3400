#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

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

    string getServiceType() { return serviceType; }
    int getProviderID() { return providerID; }
    double getFixedMeterRate() { return fixedMeterRate; }
    double getUsageRate() { return usageRate; }

    void setServiceType(string type) { serviceType = type; }
    void setProviderID(int pid) { providerID = pid; }
    void setFixedMeterRate(double rate) { fixedMeterRate = rate; }
    void setUsageRate(double rate) { usageRate = rate; }

    static vector<UtilityService*> loadServices(const string& filename) {
        vector<UtilityService*> services;
        ifstream file(filename);
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string token, type, subType;
            int pid, serviceId;
            double fixedRate, usageRate;

            getline(ss, token, ','); serviceId = stoi(token);
            getline(ss, token, ','); type = token;
            getline(ss, token, ','); pid = stoi(token);
            getline(ss, token, ','); fixedRate = stod(token);
            getline(ss, token, ','); usageRate = stod(token);
            getline(ss, token, ','); subType = token;

            UtilityService* service = nullptr;
            if (type == "Natural Gas") {
                service = new NaturalGas(pid, fixedRate, usageRate);
            } else if (type == "Internet") {
                service = new InternetService(pid, fixedRate, usageRate, subType);
            } else if (type == "Hydro") {
                service = new HydroService(pid, fixedRate, usageRate, subType);
            }
            if (service) {
                services.push_back(service);
            }
        }
        file.close();
        return services;
    }

    static void createService() {
        const string filename = "data/services.txt";
        vector<UtilityService*> services = loadServices(filename);
        
        int nextServiceId = 1;
        for (auto& service : services) {
            int id = stoi(to_string(reinterpret_cast<uintptr_t>(service))); // Using pointer as temp ID
            if (id >= nextServiceId) nextServiceId = id + 1;
        }

        cout << "Select service type:\n1. Natural Gas\n2. Internet\n3. Hydro\nEnter choice (1-3): ";
        int choice;
        cin >> choice;
        cin.ignore();

        UtilityService* newService = nullptr;
        int pid;
        double fixedRate, usageRate;
        string subType;

        cout << "Enter Provider ID (int): ";
        cin >> pid;
        cout << "Enter Fixed Meter Rate (double): ";
        cin >> fixedRate;
        cout << "Enter Usage Rate (double): ";
        cin >> usageRate;
        cin.ignore();

        switch (choice) {
            case 1:
                newService = new NaturalGas(pid, fixedRate, usageRate);
                break;
            case 2:
                cout << "Enter Internet SubType (string): ";
                getline(cin, subType);
                newService = new InternetService(pid, fixedRate, usageRate, subType);
                break;
            case 3:
                cout << "Enter Hydro SubType (string): ";
                getline(cin, subType);
                newService = new HydroService(pid, fixedRate, usageRate, subType);
                break;
            default:
                cout << "Invalid choice\n";
                return;
        }

        if (newService) {
            services.push_back(newService);
            saveServices(filename, services, true);
        }
    }

    static void saveServices(const string& filename, vector<UtilityService*>& services, bool overwrite) {
        ofstream file(filename, overwrite ? ios::trunc : ios::app);
        if (!file.is_open()) {
            cerr << "Error: Could not open file\n";
            throw runtime_error("Unable to open file");
        }
        if (overwrite) {
            file << "ServiceID,ServiceType,ProviderID,FixedMeterRate,UsageRate,SubType\n";
        }
        for (auto service : services) {
            string subType = "";
            if (InternetService* is = dynamic_cast<InternetService*>(service)) {
                subType = is->getSubType();
            } else if (HydroService* hs = dynamic_cast<HydroService*>(service)) {
                subType = hs->getSubType();
            }
            file << reinterpret_cast<uintptr_t>(service) << "," // Using pointer as temp ID
                 << service->getServiceType() << "," 
                 << service->getProviderID() << "," 
                 << service->getFixedMeterRate() << "," 
                 << service->getUsageRate() << "," 
                 << subType << "\n";
        }
        file.close();
    }

    static UtilityService* readServiceById(int serviceId) {
        string filename = "data/services.txt";
        vector<UtilityService*> services = loadServices(filename);
        for (auto service : services) {
            if (stoi(to_string(reinterpret_cast<uintptr_t>(service))) == serviceId) {
                return service;
            }
        }
        throw runtime_error("Service does not exist!");
    }

    void updateService(int serviceId) {
        cout << "Enter the Service ID to update: ";
        cin >> serviceId;
        cin.ignore();

        UtilityService* service;
        try {
            service = readServiceById(serviceId);
        }
        catch (runtime_error& e) {
            cerr << "Service Does Not Exist\n";
            return;
        }

        cout << "\nSelect the property to update:\n"
             << "1. Provider ID\n"
             << "2. Fixed Meter Rate\n"
             << "3. Usage Rate\n";
        if (dynamic_cast<InternetService*>(service) || dynamic_cast<HydroService*>(service)) {
            cout << "4. SubType\n";
        }
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        string filename = "data/services.txt";
        vector<UtilityService*> services = loadServices(filename);

        switch (choice) {
            case 1: {
                cout << "Enter new Provider ID (int): ";
                int newPid;
                cin >> newPid;
                service->setProviderID(newPid);
                break;
            }
            case 2: {
                cout << "Enter new Fixed Meter Rate (double): ";
                double newFixed;
                cin >> newFixed;
                service->setFixedMeterRate(newFixed);
                break;
            }
            case 3: {
                cout << "Enter new Usage Rate (double): ";
                double newUsage;
                cin >> newUsage;
                service->setUsageRate(newUsage);
                break;
            }
            case 4: {
                if (InternetService* is = dynamic_cast<InternetService*>(service)) {
                    cout << "Enter new SubType (string): ";
                    string newSubType;
                    getline(cin, newSubType);
                    is->setSubType(newSubType);
                } else if (HydroService* hs = dynamic_cast<HydroService*>(service)) {
                    cout << "Enter new SubType (string): ";
                    string newSubType;
                    getline(cin, newSubType);
                    hs->setSubType(newSubType);
                }
                break;
            }
            default:
                cout << "Invalid choice. No changes made.\n";
                return;
        }

        for (auto& s : services) {
            if (stoi(to_string(reinterpret_cast<uintptr_t>(s))) == serviceId) {
                delete s; // Delete old pointer
                s = service;
                break;
            }
        }
        saveServices(filename, services, true);
    }

    static void displayServices() {
        string filename = "data/services.txt";
        vector<UtilityService*> services = loadServices(filename);
        cout << "ServiceID,ServiceType,ProviderID,FixedMeterRate,UsageRate,SubType\n";
        for (auto service : services) {
            string subType = "";
            if (InternetService* is = dynamic_cast<InternetService*>(service)) {
                subType = is->getSubType();
            } else if (HydroService* hs = dynamic_cast<HydroService*>(service)) {
                subType = hs->getSubType();
            }
            cout << reinterpret_cast<uintptr_t>(service) << "," 
                 << service->getServiceType() << "," 
                 << service->getProviderID() << "," 
                 << service->getFixedMeterRate() << "," 
                 << service->getUsageRate() << "," 
                 << subType << "\n";
        }
    }
};

class NaturalGas : public UtilityService {
    public:
        NaturalGas(int pid, double fixedRate, double usageRate) : UtilityService("Natural Gas", pid, fixedRate, usageRate) {}
        double calculateCost(double usage) override { return fixedMeterRate + (usage * usageRate); }
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
        double calculateCost(double usage) override { return fixedMeterRate + usageRate; }
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
        double calculateCost(double usage) override { return fixedMeterRate + (usage * usageRate); }
};
