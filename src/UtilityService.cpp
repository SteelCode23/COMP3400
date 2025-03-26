#include "UtilityService.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

UtilityService::UtilityService() : serviceId(0), serviceName("") {}
UtilityService::UtilityService(int sId, string name) : serviceId(sId), serviceName(name) {}

int UtilityService::getServiceId() { return serviceId; }
string UtilityService::getServiceName() { return serviceName; }
void UtilityService::setServiceId(int id) { serviceId = id; }
void UtilityService::setServiceName(string name) { serviceName = name; }

vector<UtilityService> UtilityService::loadServices(const string& filename) {
    vector<UtilityService> services;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Warning: Could not open '" << filename << "'. Returning empty list.\n";
        return services;
    }
    string line;
    getline(file, line); 
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        UtilityService s;
        getline(ss, token, ','); s.setServiceId(stoi(token));
        getline(ss, token, ','); s.setServiceName(token);
        services.push_back(s);
    }
    file.close();
    return services;
}

void UtilityService::createService() {
    UtilityService newService;
    string filename = "data/services.txt";
    vector<UtilityService> services = loadServices(filename);


    int nextServiceId = 1;
    for (auto& service : services) {
        if (service.getServiceId() >= nextServiceId) {
            nextServiceId = service.getServiceId() + 1;
        }
    }
    newService.setServiceId(nextServiceId);
    cin.ignore(); 
    cout << "Enter Service Name (string): ";
    string serviceName;
    getline(cin, serviceName);
    newService.setServiceName(serviceName);
    services.push_back(newService);
    try {
        saveServices(filename, services, true);
        cout << "Service created successfully!\n";
    } catch (exception& e) {
        cerr << "Error saving service: " << e.what() << "\n";
    }
}

void UtilityService::saveServices(const string& filename, vector<UtilityService>& services, bool overwrite) {
    ofstream file(filename, overwrite ? ios::trunc : ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        throw runtime_error("Unable to open file");
    }
    if (overwrite) {
        file << "ServiceID,ServiceName\n";
    }
    for (auto& service : services) {
        file << service.getServiceId() << "," << service.getServiceName() << "\n";
    }
    file.close();
}

UtilityService UtilityService::readServiceById(int serviceId) {
    string filename = "data/services.txt";
    vector<UtilityService> services = loadServices(filename);
    for (auto& service : services) {
        if (service.getServiceId() == serviceId) {
            return service;
        }
    }
    throw runtime_error("Service does not exist!");
}

void UtilityService::updateService() {
    cout << "Enter the Service ID to update: ";
    int serviceId;
    cin >> serviceId;
    cin.ignore();
    UtilityService service;
    try {
        service = readServiceById(serviceId);
    } catch (runtime_error& e) {
        cerr << "Service Does Not Exist\n";
        return;
    }

    cout << "\nSelect the property to update:\n"
         << "1. Service Name\n"
         << "Enter choice (1): ";

    int choice;
    cin >> choice;
    cin.ignore();
    string filename = "data/services.txt";
    vector<UtilityService> services = loadServices(filename);

    switch (choice) {
        case 1: {
            cout << "Enter new Service Name (string): ";
            string newName;
            getline(cin, newName);
            service.setServiceName(newName);
            break;
        }
        default:
            cout << "Invalid choice. No changes made." << endl;
            return;
    }

    for (auto& s : services) {
        if (s.getServiceId() == serviceId) {
            s = service;
            break;
        }
    }
    try {
        saveServices(filename, services, true);
        cout << "Service updated successfully!\n";
    } catch (exception& e) {
        cerr << "Error saving services: " << e.what() << "\n";
    }
}

void UtilityService::displayServices() {
    string filename = "data/services.txt";
    vector<UtilityService> services = loadServices(filename);
    cout << "ServiceID,ServiceName\n";
    for (auto& service : services) {
        cout << service.getServiceId() << "," << service.getServiceName() << "\n";
    }
}