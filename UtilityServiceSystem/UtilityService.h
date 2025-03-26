#ifndef UTILITY_SERVICE_H
#define UTILITY_SERVICE_H

#include <string>
#include <vector>

class UtilityService {
private:
    int serviceId;
    std::string serviceName;

public:
    UtilityService();
    UtilityService(int sId, std::string name);
    int getServiceId();
    std::string getServiceName();
    void setServiceId(int id);
    void setServiceName(std::string name);
    void createService();
    UtilityService readServiceById(int serviceId);
    void updateService();
    std::vector<UtilityService> loadServices(const std::string& filename);
    void saveServices(const std::string& filename, std::vector<UtilityService>& services, bool overwrite);
    void displayServices();
};

#endif