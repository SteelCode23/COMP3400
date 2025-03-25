#include "Rate.h"
#include "InputHelper.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Rate::Rate() = default;

int Rate::getServiceId() const {
    return serviceId;
}

int Rate::getProviderId() const {
    return providerId;
}

double Rate::getFixedRateAmount() const {
    return fixedRateAmount;
}

double Rate::getVariableRateAmount() const {
    return variableRateAmount;
}

void Rate::createRate() {
    Rate r;
    r.rateId = getIntInput("Enter Rate ID: ");
    r.serviceId = getIntInput("Enter Service ID: ");
    r.providerId = getIntInput("Enter Provider ID: ");
    r.rateName = getLineInput("Enter Rate Name: ");
    r.variableRateAmount = getDoubleInput("Enter Variable Rate: ");
    r.fixedRateAmount = getDoubleInput("Enter Fixed Rate: ");
    r.unitOfMeasure = getLineInput("Enter Unit of Measure: ");
    r.MeasuredUsage = getIntInput("Enter Measured Usage: ");

    ofstream file("../data/rates.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Failed to open rates.txt\n";
        return;
    }

    file << "\n" << r.rateId << "," << r.serviceId << "," << r.providerId << "," << r.rateName << ","
         << r.variableRateAmount << "," << r.fixedRateAmount << "," << r.unitOfMeasure << "," << r.MeasuredUsage;
    file.close();
}

void Rate::displayRates() {
    ifstream file("../data/rates.txt");
    string line;
    if (!file.is_open()) {
        cerr << "Failed to open rates.txt\n";
        return;
    }

    getline(file, line);
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void Rate::updateRate() {
    cout << "Rate update not implemented yet.\n";
}

vector<Rate> Rate::loadRates(const string& filename) {
    vector<Rate> rates;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return rates;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Rate r;
        getline(ss, token, ','); r.rateId = stoi(token);
        getline(ss, token, ','); r.serviceId = stoi(token);
        getline(ss, token, ','); r.providerId = stoi(token);
        getline(ss, token, ','); r.rateName = token;
        getline(ss, token, ','); r.variableRateAmount = stod(token);
        getline(ss, token, ','); r.fixedRateAmount = stod(token);
        getline(ss, token, ','); r.unitOfMeasure = token;
        getline(ss, token, ','); r.MeasuredUsage = stoi(token);
        rates.push_back(r);
    }

    file.close();
    return rates;
}
