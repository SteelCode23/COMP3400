#include "UtilityService.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class UtilityProvider {
    protected:
        int providerID;
        string name;
        vector<UtilityService*> services;

    public:
        UtilityProvider();
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


        static vector<UtilityProvider> loadProviders(const string& filename) {
            vector<UtilityProvider> providers;
            ifstream file(filename);
            string line;
            getline(file, line);
            while (getline(file, line)) {
                stringstream ss(line);
                string token;
                int id;
                string name;
                getline(ss, token, ','); id = stoi(token);
                getline(ss, token, ','); name = token;
                providers.push_back(UtilityProvider(id, name));
            }
            file.close();
            return providers;
        }

        void createProvider() {
            const string filename = "data/providers.txt";
            vector<UtilityProvider> providers = loadProviders(filename);
            
            int nextProviderId = 1;
            for (auto& provider : providers) {
                if (provider.getProviderID() >= nextProviderId) {
                    nextProviderId = provider.getProviderID() + 1;
                }
            }
            providerID = nextProviderId;

            cin.ignore();
            cout << "Enter Provider Name (string): ";
            string providerName;
            getline(cin, providerName);
            setName(providerName);

            providers.push_back(*this);
            saveProviders(filename, providers, true);
        }

        static void saveProviders(const string& filename, vector<UtilityProvider>& providers, bool overwrite) {
            ofstream file(filename, overwrite ? ios::trunc : ios::app);
            if (!file.is_open()) {
                cerr << "Error: Could not open file\n";
                throw runtime_error("Unable to open file");
            }
            if (overwrite) {
                file << "ProviderID,Name\n";
            }
            for (auto& provider : providers) {
                file << provider.getProviderID() << "," << provider.getName() << "\n";
            }
            file.close();
        }

        static UtilityProvider readProviderById(int providerId) {
            string filename = "data/providers.txt";
            vector<UtilityProvider> providers = loadProviders(filename);
            for (auto& provider : providers) {
                if (provider.getProviderID() == providerId) {
                    return provider;
                }
            }
            throw runtime_error("Provider does not exist!");
        }

        void updateProvider() {
            cout << "Enter the Provider ID to update: ";
            int providerId;
            cin >> providerId;
            cin.ignore();

            UtilityProvider provider;
            try {
                provider = readProviderById(providerId);
            }
            catch (runtime_error& e) {
                cerr << "Provider Does Not Exist\n";
                return;
            }

            cout << "\nSelect the property to update:\n"
                 << "1. Provider ID\n"
                 << "2. Name\n"
                 << "Enter choice (1-2): ";

            int choice;
            cin >> choice;
            cin.ignore();

            string filename = "data/providers.txt";
            vector<UtilityProvider> providers = loadProviders(filename);

            switch (choice) {
                case 1: {
                    cout << "Enter new Provider ID (int): ";
                    int newId;
                    cin >> newId;
                    provider.setProviderID(newId);
                    break;
                }
                case 2: {
                    cout << "Enter new Name (string): ";
                    string newName;
                    getline(cin, newName);
                    provider.setName(newName);
                    break;
                }
                default:
                    cout << "Invalid choice. No changes made.\n";
                    return;
            }

            for (auto& p : providers) {
                if (p.getProviderID() == providerId) {
                    p = provider;
                    break;
                }
            }
            saveProviders(filename, providers, true);
        }

        static void displayProviders() {
            string filename = "data/providers.txt";
            vector<UtilityProvider> providers = loadProviders(filename);
            cout << "ProviderID,Name\n";
            for (auto provider : providers) {
                cout << provider.getProviderID() << "," << provider.getName() << "\n";
            }
        }
};

