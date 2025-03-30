#include "Subscription.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;


Subscription::Subscription() : subscriptionId(0), customerId(0), rateId(0), providerId(0), serviceId(0) {}
Subscription::Subscription(int subId, int custId, int rId, int pId, int sId)
    : subscriptionId(subId), customerId(custId), rateId(rId), providerId(pId), serviceId(sId) {}

int Subscription::getSubscriptionId() { return subscriptionId; }
int Subscription::getCustomerId() { return customerId; }
int Subscription::getRateId() { return rateId; }
int Subscription::getProviderId() { return providerId; }
int Subscription::getServiceId() { return serviceId; }
void Subscription::setSubscriptionId(int id) { subscriptionId = id; }
void Subscription::setCustomerId(int id) { customerId = id; }
void Subscription::setRateId(int id) { rateId = id; }
void Subscription::setProviderId(int id) { providerId = id; }
void Subscription::setServiceId(int id) { serviceId = id; }

vector<Subscription> Subscription::loadSubscriptions(const string& filename) {
    vector<Subscription> subscriptions;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Warning: Could not open '" << filename << "'. Returning empty list.\n";
        return subscriptions;
    }
    string line;
    getline(file, line); 
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Subscription s;
        getline(ss, token, ','); s.setSubscriptionId(stoi(token));
        getline(ss, token, ','); s.setCustomerId(stoi(token));
        getline(ss, token, ','); s.setRateId(stoi(token));
        getline(ss, token, ','); s.setProviderId(stoi(token));
        getline(ss, token, ','); s.setServiceId(stoi(token));
        subscriptions.push_back(s);
    }
    file.close();
    return subscriptions;
}


void Subscription::createSubscription() {
    Subscription newSubscription;
    string filename = "data/subscriptions.txt";
    vector<Subscription> subscriptions = loadSubscriptions(filename);
    int nextSubscriptionId = 1;
    for (auto& subscription : subscriptions) {
        if (subscription.getSubscriptionId() >= nextSubscriptionId) {
            nextSubscriptionId = subscription.getSubscriptionId() + 1;
        }
    }
    newSubscription.setSubscriptionId(nextSubscriptionId);
    cout << "Enter Customer ID (int): ";
    int customerId;
    cin >> customerId;
    newSubscription.setCustomerId(customerId);

    cout << "Enter Rate ID (int): ";
    int rateId;
    cin >> rateId;
    newSubscription.setRateId(rateId);

    cout << "Enter Provider ID (int): ";
    int providerId;
    cin >> providerId;
    newSubscription.setProviderId(providerId);

    cout << "Enter Service ID (int): ";
    int serviceId;
    cin >> serviceId;
    newSubscription.setServiceId(serviceId);

    subscriptions.push_back(newSubscription);
    try {
        saveSubscriptions(filename, subscriptions, true);
        cout << "Subscription created successfully!\n";
    } catch (exception& e) {
        cerr << "Error saving subscription: " << e.what() << "\n";
    }
}

void Subscription::saveSubscriptions(const string& filename, vector<Subscription>& subscriptions, bool overwrite) {
    ofstream file(filename, overwrite ? ios::trunc : ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        throw runtime_error("Unable to open file");
    }
    if (overwrite) {
        file << "SubscriptionId,CustomerId,RateID,ProviderID,ServiceID\n";
    }
    for (auto& subscription : subscriptions) {
        file << subscription.getSubscriptionId() << "," << subscription.getCustomerId() << ","
             << subscription.getRateId() << "," << subscription.getProviderId() << ","
             << subscription.getServiceId() << "\n";
    }
    file.close();
}


Subscription Subscription::readSubscriptionById(int subscriptionId) {
    string filename = "data/subscriptions.txt";
    vector<Subscription> subscriptions = loadSubscriptions(filename);
    for (auto& subscription : subscriptions) {
        if (subscription.getSubscriptionId() == subscriptionId) {
            return subscription;
        }
    }
    throw runtime_error("Subscription does not exist!");
}


void Subscription::updateSubscription() {
    cout << "Enter the Subscription ID to update: ";
    int subscriptionId;
    cin >> subscriptionId;
    cin.ignore();
    Subscription subscription;
    try {
        subscription = readSubscriptionById(subscriptionId);
    } catch (runtime_error& e) {
        cerr << "Subscription Does Not Exist\n";
        return;
    }

    cout << "\nSelect the property to update:\n"
         << "1. Customer ID\n"
         << "2. Rate ID\n"
         << "3. Provider ID\n"
         << "4. Service ID\n"
         << "Enter choice (1-4): ";

    int choice;
    cin >> choice;
    cin.ignore();
    string filename = "data/subscriptions.txt";
    vector<Subscription> subscriptions = loadSubscriptions(filename);

    switch (choice) {
        case 1: {
            cout << "Enter new Customer ID (int): ";
            int newId;
            cin >> newId;
            subscription.setCustomerId(newId);
            break;
        }
        case 2: {
            cout << "Enter new Rate ID (int): ";
            int newId;
            cin >> newId;
            subscription.setRateId(newId);
            break;
        }
        case 3: {
            cout << "Enter new Provider ID (int): ";
            int newId;
            cin >> newId;
            subscription.setProviderId(newId);
            break;
        }
        case 4: {
            cout << "Enter new Service ID (int): ";
            int newId;
            cin >> newId;
            subscription.setServiceId(newId);
            break;
        }
        default:
            cout << "Invalid choice. No changes made." << endl;
            return;
    }

    for (auto& s : subscriptions) {
        if (s.getSubscriptionId() == subscriptionId) {
            s = subscription;
            break;
        }
    }
    try {
        saveSubscriptions(filename, subscriptions, true);
        cout << "Subscription updated successfully!\n";
    } catch (exception& e) {
        cerr << "Error saving subscriptions: " << e.what() << "\n";
    }
}


void Subscription::displaySubscriptions() {
    string filename = "data/subscriptions.txt";
    vector<Subscription> subscriptions = loadSubscriptions(filename);
    cout << "SubscriptionId,CustomerId,RateID,ProviderID,ServiceID\n";
    for (auto& subscription : subscriptions) {
        cout << subscription.getSubscriptionId() << "," << subscription.getCustomerId() << ","
             << subscription.getRateId() << "," << subscription.getProviderId() << ","
             << subscription.getServiceId() << "\n";
    }
}