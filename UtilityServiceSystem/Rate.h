#ifndef RATE_H
#define RATE_H

#include <string>
#include <vector>
class  Rate {

    private:
    int rateId;
    int serviceId;
    int providerId;
    std::string rateName;
    double variableRateAmount;
    double fixedRateAmount;
    std::string unitOfMeasure;
    int MeasuredUsage;
    
    public:
    Rate();
    Rate(int rId, int sId, int pId, std::string rName, double varRate, double fixRate, std::string uom, int mUsage);
    Rate getRates();
    int getRateId();
    int getServiceId();
    int getProviderId();
    std::string getRateName();
    double getVariableRateAmount();
    double getFixedRateAmount();
    std::string getUnitOfMeasure();
    int getMeasuredUsage();
    void setRateId(int id);
    void setServiceId(int id);
    void setProviderId(int id);
    void setRateName(std::string name);
    void setVariableRateAmount(double amount);
    void setFixedRateAmount(double amount);
    void setUnitOfMeasure(std::string uom);
    void setMeasuredUsage(int usage);
    void createRate();
    Rate readRateById(int rateId);
    void updateRate();
    std::vector<Rate> loadRates(const std::string& filename);
    void saveRates(const std::string& filename, std::vector<Rate>& rates, bool overwrite);
    void displayRates();
};

#endif