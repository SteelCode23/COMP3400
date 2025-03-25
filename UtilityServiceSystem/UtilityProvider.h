// #ifndef UTILITYPROVIDER_H
// #define UTILITYPROVIDER_H

// #include <string>
// #include <vector>
// #include "UtilityService.h"

// class UtilityProvider {
// private:
//     int providerID;
//     std::string providerName;
//     std::string serviceType;
//     double ratePerUnit;
//     std::vector<UtilityService*> services;

// public:

//     UtilityProvider(int id, const std::string& name, const std::string& type, double rate);

//     int getProviderID() const;
//     std::string getProviderName() const;
//     std::string getServiceType() const;
//     double getRatePerUnit() const;

//     void setProviderID(int id);
//     void setProviderName(const std::string& name);
//     void setServiceType(const std::string& type);
//     void setRatePerUnit(double rate);

//     void addService(UtilityService* service);
//     void displayServices() const;

//     static vector<UtilityProvider> loadProviders(const string& filename);
//     void createProvider();
//     static void saveProviders(const string& filename, vector<UtilityProvider>& providers, bool overwrite);
//     static UtilityProvider readProviderById(int providerId);
//     void updateProvider();
//     static void displayProviders();
// };

// #endif
