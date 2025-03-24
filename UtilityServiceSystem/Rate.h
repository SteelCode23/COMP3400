#ifndef RATE_H
#define RATE_H

#include <string>
#include <vector>

class Rate {
public:
    int rateId;
    int serviceId;
    int providerId;
    std::string rateName;
    double variableRateAmount;
    double fixedRateAmount;
    std::string unitOfMeasure;
    int MeasuredUsage;

    Rate();

    void createRate();
    void displayRates();
    void updateRate();

    int getServiceId() const;
    int getProviderId() const;
    double getFixedRateAmount() const;
    double getVariableRateAmount() const;

    static std::vector<Rate> loadRates(const std::string& filename);
};

#endif
