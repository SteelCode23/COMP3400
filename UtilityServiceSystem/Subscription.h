#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <string>
#include <vector>

class Subscription {
private:
    int subscriptionId;
    int customerId;
    int rateId;
    int providerId;
    int serviceId;

public:
    Subscription();
    Subscription(int subId, int custId, int rId, int pId, int sId);
    int getSubscriptionId();
    int getCustomerId();
    int getRateId();
    int getProviderId();
    int getServiceId();
    void setSubscriptionId(int id);
    void setCustomerId(int id);
    void setRateId(int id);
    void setProviderId(int id);
    void setServiceId(int id);
    void createSubscription();
    Subscription readSubscriptionById(int subscriptionId);
    void updateSubscription();
    std::vector<Subscription> loadSubscriptions(const std::string& filename);
    void saveSubscriptions(const std::string& filename, std::vector<Subscription>& subscriptions, bool overwrite);
    void displaySubscriptions();
};

#endif