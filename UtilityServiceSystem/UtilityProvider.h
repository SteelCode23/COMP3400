#ifndef UTILITY_PROVIDER_H
#define UTILITY_PROVIDER_H

#include <string>
#include <vector>

class UtilityProvider {
private:
    int providerId;
    std::string providerName;

public:
    UtilityProvider();
    UtilityProvider(int pId, std::string name);
    int getProviderId();
    std::string getProviderName();
    void setProviderId(int id);
    void setProviderName(std::string name);
    void createProvider();
    UtilityProvider readProviderById(int providerId);
    void updateProvider();
    std::vector<UtilityProvider> loadProviders(const std::string& filename);
    void saveProviders(const std::string& filename, std::vector<UtilityProvider>& providers, bool overwrite);
    void displayProviders();
};

#endif