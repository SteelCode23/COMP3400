#ifndef USAGE_H
#define USAGE_H
#include <vector>
#include <string>
class Usage {

private:
int usageId;
int customerId;
int providerId;
int serviceId;
int billCalendarId;
    public:

    int getUsageId();
    int getCustomerId();
    int getProviderId();
    int getServiceId();
    int getBillCalendarId();
    int getUsageAmount();
    void setUsageId(int id);
    void setCustomerId(int id);
    void setProviderId(int id);
    void setServiceId(int id);
    void setBillCalendarId(int id);
    void setUsageAmount(int amount);

    int usageAmount;
        Usage();
        Usage(int uId, int cId, int pId, int sId, int bcId, int amount);
        std::vector<Usage> loadUsage(const std::string& filename, int BillCalendarID);
        void simulateUsage();

};

#endif