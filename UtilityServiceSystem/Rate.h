#ifndef RATE_H
#define RATE_H

#include <string>

struct Rate {
    int rateId;
    int serviceId;
    int providerId;
    std::string rateName;
    double variableRateAmount;
    double fixedRateAmount;
    std::string unitOfMeasure;
    int MeasuredUsage;
};

#endif