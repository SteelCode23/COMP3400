#include "Rate.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

Rate::Rate() : rateId(0), serviceId(0), providerId(0), rateName(""),
               variableRateAmount(0.0), fixedRateAmount(0.0), unitOfMeasure(""),
               MeasuredUsage(0) {}

Rate::Rate(int rId, int sId, int pId, string rName, double varRate, double fixRate,
           string uom, int mUsage)    : rateId(rId), serviceId(sId), providerId(pId), rateName(rName),
      variableRateAmount(varRate), fixedRateAmount(fixRate), unitOfMeasure(uom),
      MeasuredUsage(mUsage) {}

int Rate::getRateId() { return rateId; }
int Rate::getServiceId() { return serviceId; }
int Rate::getProviderId() { return providerId; }
string Rate::getRateName() { return rateName; }
double Rate::getVariableRateAmount() { return variableRateAmount; }
double Rate::getFixedRateAmount() { return fixedRateAmount; }
string Rate::getUnitOfMeasure() { return unitOfMeasure; }
int Rate::getMeasuredUsage() { return MeasuredUsage; }
void Rate::setRateId(int id) { rateId = id; }
void Rate::setServiceId(int id) { serviceId = id; }
void Rate::setProviderId(int id) { providerId = id; }
void Rate::setRateName(string name) { rateName = name; }
void Rate::setVariableRateAmount(double amount) { variableRateAmount = amount; }
void Rate::setFixedRateAmount(double amount) { fixedRateAmount = amount; }
void Rate::setUnitOfMeasure(string uom) { unitOfMeasure = uom; }
void Rate::setMeasuredUsage(int usage) { MeasuredUsage = usage; }

vector<Rate> Rate::loadRates(const string& filename){
    vector<Rate> rates;
    ifstream file(filename);
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


void Rate::createRate(){
    Rate newRate;
    const string filename = "data/rates.txt";
    vector<Rate> rates;
    rates = loadRates(filename);
    int nextRateId = 1;
    for (auto& rate : rates) {
        if (rate.getRateId() >= nextRateId) {
            nextRateId = rate.getRateId() + 1;
        }
    }
    newRate.setRateId(nextRateId);
    cout << "Enter Service ID (int): ";
    int serviceId;
    cin >> serviceId;
    newRate.setServiceId(serviceId);
    cout << "Enter Provider ID (int): ";
    int providerId;
    cin >> providerId;
    newRate.setProviderId(providerId);

    cin.ignore(); 
    cout << "Enter Rate Name (string): ";
    string rateName;
    getline(cin, rateName);
    newRate.setRateName(rateName);

    cout << "Enter Variable Rate Amount (double): ";
    double varRate;
    cin >> varRate;
    newRate.setVariableRateAmount(varRate);

    cout << "Enter Fixed Rate Amount (double): ";
    double fixRate;
    cin >> fixRate;
    newRate.setFixedRateAmount(fixRate);

    cin.ignore(); 
    cout << "Enter Unit of Measure (string): ";
    string unitOfMeasure;
    getline(cin, unitOfMeasure);
    newRate.setUnitOfMeasure(unitOfMeasure);

    cout << "Enter Measured Usage (int): ";
    int measuredUsage;
    cin >> measuredUsage;
    newRate.setMeasuredUsage(measuredUsage);
    rates.push_back(newRate); 
    saveRates("data/rates.txt", rates, true);
}

void Rate::saveRates(const string& filename, vector<Rate>& rates, bool overwrite) {
    ofstream file(filename, overwrite ? ios::trunc : ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file \n";
        throw runtime_error("Unable to open file");
    }
    if (overwrite) {
        file << "RateID,ServiceID,ProviderID,RateName,VariableRateAmount,FixedRateAmount,UnitofMeasure,MeasuredService\n";
    }    
    for (auto& rate : rates) {
        file << rate.getRateId() << "," << rate.getServiceId() << "," << rate.getProviderId() << ","
             << rate.getRateName() << "," << rate.getVariableRateAmount() << "," << rate.getFixedRateAmount() << ","
             << rate.getUnitOfMeasure() << "," << rate.getMeasuredUsage() << "\n";
    }    
}
Rate Rate::readRateById(int rateId){
    string filename = "data/rates.txt";
    vector<Rate> rates = loadRates(filename);
    for (auto& rate : rates) {
        if (rate.getRateId() == rateId) {
            return rate;
        }
    }
    throw runtime_error("Rate does not exist!");
}


void Rate::updateRate(){
    cout << "Enter the Rate ID to update: ";
    int rateId;
    cin >> rateId;
    cin.ignore();
    Rate rate;
    try{
    rate = readRateById(rateId);
    }
    catch (runtime_error& e) {
        cerr << "Rate Does Not Exist";
        return;
    }
    cout << "\nSelect the property to update:\n"
    << "1. Service ID\n"
    << "2. Provider ID\n"
    << "3. Rate Name\n"
    << "4. Variable Rate Amount\n"
    << "5. Fixed Rate Amount\n"
    << "6. Unit of Measure\n"
    << "7. Measured Usage\n"
    << "Enter choice (1-7): ";

    int choice;
    cin >> choice;
    cin.ignore();
    string filename = "data/rates.txt";
    vector<Rate> rates = loadRates(filename);
    switch (choice) {
        case 1: {
            cout << "Enter new Service ID (int): ";
            int newId;
            cin >> newId;
            rate.setServiceId(newId);
            break;
        }
        case 2: {
            cout << "Enter new Provider ID (int): ";
            int newId;
            cin >> newId;
            rate.setProviderId(newId);
            break;
        }
        case 3: {
            cout << "Enter new Rate Name (string): ";
            string newName;
            getline(cin, newName);
            rate.setRateName(newName);
            break;
        }
        case 4: {
            cout << "Enter new Variable Rate Amount (double): ";
            double newAmount;
            cin >> newAmount;
            rate.setVariableRateAmount(newAmount);
            break;
        }
        case 5: {
            cout << "Enter new Fixed Rate Amount (double): ";
            double newAmount;
            cin >> newAmount;
            rate.setFixedRateAmount(newAmount);
            break;
        }
        case 6: {
            cout << "Enter new Unit of Measure (string): ";
            string newUom;
            getline(cin, newUom);
            rate.setUnitOfMeasure(newUom);
            break;
        }
        case 7: {
            cout << "Enter new Measured Usage (int): ";
            int newUsage;
            cin >> newUsage;
            rate.setMeasuredUsage(newUsage);
            break;
        }
        default:
            cout << "Invalid choice. No changes made." << endl;
            return;
    }
    for (auto& r : rates) {
        if (r.getRateId() == rateId) {
            r = rate;
            break;
        }
    }    
    saveRates(filename, rates, true);
}

void Rate::displayRates(){
    string filename = "data/rates.txt";
    vector<Rate> rates = loadRates(filename);
    cout << "RateID,RateName,VariableRateAmount,FixedRateAmount,UnitofMeasure" << endl;
    for(auto rate:rates){
        cout<< rate.getRateId() << "," << rate.getRateName() << "," << rate.getVariableRateAmount() << "," << rate.getFixedRateAmount() << ","<< rate.getUnitOfMeasure() << endl;
    }
    return;   
}