#include "UtilityProvider.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;


UtilityProvider::UtilityProvider() : providerId(0), providerName("") {}

UtilityProvider::UtilityProvider(int pId, string name) : providerId(pId), providerName(name) {}
int UtilityProvider::getProviderId() { return providerId; }
string UtilityProvider::getProviderName() { return providerName; }
void UtilityProvider::setProviderId(int id) { providerId = id; }
void UtilityProvider::setProviderName(string name) { providerName = name; }

vector<UtilityProvider> UtilityProvider::loadProviders(const string& filename) {
    vector<UtilityProvider> providers;
    ifstream file(filename);

    string line;
    getline(file, line); 
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        UtilityProvider p;
        getline(ss, token, ','); p.setProviderId(stoi(token));
        getline(ss, token, ','); p.setProviderName(token);
        providers.push_back(p);
    }
    file.close();
    return providers;
}

void UtilityProvider::createProvider() {
    UtilityProvider newProvider;
    string filename = "data/providers.txt";
    vector<UtilityProvider> providers = loadProviders(filename);
    int nextProviderId = 1;
    for (auto& provider : providers) {
        if (provider.getProviderId() >= nextProviderId) {
            nextProviderId = provider.getProviderId() + 1;
        }
    }
    newProvider.setProviderId(nextProviderId);
    cin.ignore(); 
    cout << "Enter Provider Name (string): ";
    string providerName;
    getline(cin, providerName);
    newProvider.setProviderName(providerName);

    providers.push_back(newProvider);
    try {
        saveProviders(filename, providers, true);
        cout << "Provider created successfully!\n";
    } catch (exception& e) {
        cerr << "Error saving provider: " << e.what() << "\n";
    }
}

void UtilityProvider::saveProviders(const string& filename, vector<UtilityProvider>& providers, bool overwrite) {
    ofstream file(filename, overwrite ? ios::trunc : ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        throw runtime_error("Unable to open file");
    }
    if (overwrite) {
        file << "ProviderID,ProviderName\n";
    }
    for (auto& provider : providers) {
        file << provider.getProviderId() << "," << provider.getProviderName() << "\n";
    }
    file.close();
}

UtilityProvider UtilityProvider::readProviderById(int providerId) {
    string filename = "data/providers.txt";
    vector<UtilityProvider> providers = loadProviders(filename);
    for (auto& provider : providers) {
        if (provider.getProviderId() == providerId) {
            return provider;
        }
    }
    throw runtime_error("Provider does not exist!");
}

void UtilityProvider::updateProvider() {
    cout << "Enter the Provider ID to update: ";
    int providerId;
    cin >> providerId;
    cin.ignore();
    UtilityProvider provider;
    try {
        provider = readProviderById(providerId);
    } catch (runtime_error& e) {
        cerr << "Provider Does Not Exist\n";
        return;
    }

    cout << "\nSelect the property to update:\n"
         << "1. Provider Name\n"
         << "Enter choice (1): ";

    int choice;
    cin >> choice;
    cin.ignore();
    string filename = "data/providers.txt";
    vector<UtilityProvider> providers = loadProviders(filename);
    switch (choice) {
        case 1: {
            cout << "Enter new Provider Name (string): ";
            string newName;
            getline(cin, newName);
            provider.setProviderName(newName);
            break;
        }
        default:
            cout << "Invalid choice. No changes made." << endl;
            return;
    }

    for (auto& p : providers) {
        if (p.getProviderId() == providerId) {
            p = provider;
            break;
        }
    }
    try {
        saveProviders(filename, providers, true);
        cout << "Provider updated successfully!\n";
    } catch (exception& e) {
        cerr << "Error saving providers: " << e.what() << "\n";
    }
}

void UtilityProvider::displayProviders() {
    string filename = "data/providers.txt";
    vector<UtilityProvider> providers = loadProviders(filename);
    cout << "ProviderID,ProviderName\n";
    for (auto& provider : providers) {
        cout << provider.getProviderId() << "," << provider.getProviderName() << "\n";
    }
}