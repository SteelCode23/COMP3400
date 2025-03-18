#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip> 
#include "batch.h"
#include <random>
#include "Bill.h"
#include "Customer.h"
#include "Payments.h" 
#include "Rate.h"
#include "Usage.h"

using namespace std;
using namespace std::chrono;

BatchService::BatchService() {}


    int  BatchService::getPaymentID(const string& filename){
        int nextPaymentID = 1;
        int testPaymentID;
        Payments p;
        ifstream file(filename);
        string line;        
        getline(file,line);//skip headers
        while(getline(file,line)){
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            testPaymentID = stoi(token);
            if(testPaymentID > nextPaymentID){
                nextPaymentID = testPaymentID;
            }
        }
        file.close();
        return nextPaymentID;
    }


   int  BatchService::getBillingID(const string& filename){
        int nextBillingID = 1;
        int testBillingID;
        Bill b;
        ifstream file(filename);
        string line;        
        getline(file,line);//skip headers
        while(getline(file,line)){
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            testBillingID = stoi(token);
            if(testBillingID > nextBillingID){
                nextBillingID = testBillingID;
            }
        }
        file.close();
        return nextBillingID;
    }

    year_month_day BatchService::parseDate(const string& dateStr) {
        int y, m, d;
        char dash1, dash2;
        stringstream ss(dateStr);
        ss >> y >> dash1 >> m >> dash2 >> d;
        return year_month_day{year{y}, month{static_cast<unsigned int>(m)}, day{static_cast<unsigned int>(d)}};
    }   


    vector<Bill>  BatchService::loadBills(const string& filename){
        vector<Bill> bills;
        ifstream file(filename);
        string line;


        getline(file,line); //skip header
        while(getline(file,line)){
            stringstream ss(line);
            string token;
            Bill b;
            getline(ss, token, ',');
            b.billId = stoi(token);
            getline(ss, token, ',');
            b.customerId = stoi(token);
            getline(ss, token, ',');
            b.providerId = stoi(token);
            getline(ss, token, ',');
            b.serviceId = stoi(token);
            getline(ss, token, ',');
            b.billCalendarID = stoi(token);
            getline(ss, token, ',');
            b.billAmount = stod(token);
            getline(ss, token, ',');
            b.amountPaid = stod(token);            
            getline(ss, token, ',');
            b.paidInFull = stoi(token);
            getline(ss, token, ',');
            b.billDate = parseDate(token);

            //b.billDate = token;
            getline(ss, token, ',');
            b.dueDate = parseDate(token);


            //b.dueDate = token;
            bills.push_back(b);
        }
        file.close();

        return bills;
        
    }

    vector<Customer> BatchService::loadCustomers(const string& filename){
        vector<Customer> customers;
        ifstream file(filename);
        string line;


        getline(file,line); //skip header
        while(getline(file,line)){
            stringstream ss(line);
            string token;
            Customer c;
            getline(ss, token, ',');
            c.id = stoi(token);
            getline(ss, token, ',');
            c.name = token;
            customers.push_back(c);
        }
        file.close();

        return customers;
        
    }

    vector<Usage> BatchService::loadUsage(const string& filename, int BillCalendarID){
        vector<Usage> usageData;
        ifstream file(filename);
        string line;       
        getline(file,line); //skip header
        while(getline(file,line)){
            stringstream ss(line);    
            string token;     
            Usage u;
            getline(ss, token, ','); u.customerId = stoi(token);
            getline(ss, token, ','); u.providerId = stoi(token);
            getline(ss, token, ','); u.serviceId = stoi(token);
            getline(ss, token, ','); u.billCalendarId = stoi(token);
            getline(ss, token, ','); u.usageAmount = stoi(token);         
    
            if(u.billCalendarId == BillCalendarID){
                usageData.push_back(u);        //Filter to only this billcalendar.
            }            
        }
        file.close();
        return usageData;
    }

    vector<Rate> BatchService::loadRates(const string& filename){
        vector<Rate> rates;
        ifstream file(filename);
        string line;       
        getline(file,line); //skip header
        while(getline(file,line)){
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
            
            rates.push_back(r);        //Filter to only this billcalendar.

        }
        file.close();
        return rates;
    }

    void BatchService::BillingBatch(int BillCalendarID){
        vector<Bill> bills;
        vector<Customer> customers = loadCustomers("../data/customers.txt");
        vector<Rate> rates = loadRates("../data/rates.txt");
        vector<Usage> usageRecords = loadUsage("../data/usage.txt", BillCalendarID);
        int nextBillingID = getBillingID("../data/bills.txt");
        //Usage Based
        for (const auto& usage : usageRecords){
            if(usage.billCalendarId == BillCalendarID){
                float variableRate, fixedRate = 0.0;
                for(const auto& r: rates){
                    if(r.providerId == usage.providerId && r.serviceId == usage.serviceId){
                        variableRate = r.variableRateAmount;
                        fixedRate = r.fixedRateAmount;
                        break;
                    }
                }

                Bill newBill;
                newBill.billId = nextBillingID++;
                newBill.customerId = usage.customerId;
                newBill.providerId = usage.providerId;
                newBill.billCalendarID = BillCalendarID;
                newBill.serviceId = usage.serviceId;
                newBill.billAmount = (usage.usageAmount * variableRate) + fixedRate;
                newBill.amountPaid = 0;
                newBill.paidInFull = false;
                chrono::sys_days today = floor<days>(system_clock::now()); // Get today's date
                newBill.billDate = year_month_day{today};
                sys_days due = today + days{30}; 
                newBill.dueDate = year_month_day{due};
                newBill.overdue = 0;
                
                bills.push_back(newBill);
            }
        }
 

        saveBills("../data/bills.txt", bills, false);

    }

    void BatchService::saveBills(const string &filename, const vector<Bill> bills, bool overwrite){
        if(overwrite){
            ofstream file(filename, ios::trunc);
            file << 
            "BillID" << "," << "CustomerID"<< "," << "ProviderID" << "," << "ServiceID"<< "," <<
             "BillCalendarID" << "," << "BillAmount"<< "," << "AmountPaid"  << "," << "Status" <<
              "," <<"BillDate" << "," << "DueDate" << "," << "Overdue" ;
              for(const auto & bill:bills){
                file <<"\n"<< bill.billId << "," << bill.customerId << "," << bill.providerId << "," << bill.serviceId << "," << bill.billCalendarID << "," 
                << bill.billAmount << "," << bill.amountPaid <<    "," << bill.paidInFull << ","            
                << int(bill.billDate.year()) << "-" << unsigned(bill.billDate.month()) << "-"<< unsigned(bill.billDate.day()) << "," //BillDate
                << int(bill.dueDate.year()) << "-"<< unsigned(bill.dueDate.month()) << "-"<< unsigned(bill.dueDate.day())  //DueDate
                << ',' << bill.overdue;
            }
            file.close();              
        }
        else{
            ofstream file(filename, ios::app);
            for(const auto & bill:bills){
                file <<"\n"<< bill.billId << "," << bill.customerId << "," << bill.providerId << "," << bill.serviceId << "," << bill.billCalendarID << "," 
                << bill.billAmount << "," << bill.amountPaid <<    "," << bill.paidInFull << ","            
                << int(bill.billDate.year()) << "-" << unsigned(bill.billDate.month()) << "-"<< unsigned(bill.billDate.day()) << "," //BillDate
                << int(bill.dueDate.year()) << "-"<< unsigned(bill.dueDate.month()) << "-"<< unsigned(bill.dueDate.day())  //DueDate
                << ',' << bill.overdue;
            }
            file.close();
        }
        

    }

    void BatchService::SimulatePayments(int BillCalendarID){
        vector<Bill> bills = loadBills("../data/bills.txt");
        vector<Payments> payments;
        /*
            Since this is only a small application to be used by a single user, the purpose of this 
            batch is to simlulate the payments that 100 customers would hypothetically make. It will allow
            95% of customers to make their full payment, and the remaining 5% will either pay nothing, or some
            percentage of their balance, but they will be delinquent.
        */
        random_device  rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(1,100);
        uniform_real_distribution<> dis(0.0,1.0);
        uniform_real_distribution<> partialPay(0.1,.9);
        float paymentAmount = 0.0;
        int paymentId = getPaymentID("../data/payments.txt");
        sys_days todaySys = floor<days>(system_clock::now());
        year_month_day today = year_month_day{todaySys};
        for(auto& bill:bills){
            if(bill.billCalendarID == BillCalendarID){
                bool simulatedPayment = dis(gen) < .95;
                if(simulatedPayment){
                    paymentAmount = bill.billAmount;
                    bill.amountPaid = bill.billAmount;
                    bill.paidInFull = true;                    
                }
                else{
                    if(dis(gen) < .5){ //50% chance of partial vs no payment
                        bill.amountPaid = 0.0;
                        bill.paidInFull = false;
                    }
                    else{
                        paymentAmount = bill.billAmount * partialPay(gen);
                        bill.amountPaid = paymentAmount;
                        bill.paidInFull = false;
                    }
                }
                if(paymentAmount > 0.0){
                    Payments paymentrecord;
                    paymentrecord.paymentId = paymentId++;
                    paymentrecord.billId = bill.billId;
                    paymentrecord.paymentAmount = paymentAmount;
                    payments.push_back(paymentrecord);
                }
            }
        }
        saveBills("../data/bills.txt", bills, true);
        postPayments("../data/payments.txt", payments, false);
        CalculateOverdue();
    }

    void BatchService::postPayments(const string &filename, const vector<Payments> payments, bool overwrite){
        ofstream file(filename, ios::app);
        for (const auto& payment : payments) {
            file << "\n" << payment.paymentId << "," << payment.billId << "," << payment.paymentAmount;
        }
        file.close();
    }

    void BatchService::CalculateOverdue(){    

        vector<Bill> bills = loadBills("../data/bills.txt");

        for(auto& bill:bills){
            sys_days today = floor<days>(system_clock::now());
            sys_days dueSysDays = sys_days(bill.dueDate);
           
            auto daysDifference = duration_cast<days>(today - dueSysDays).count();
            if((bill.billAmount - bill.amountPaid  > 0) && (daysDifference > 30)){              
                bill.overdue = true;
            }
        }

        saveBills("../data/bills.txt", bills, true);

    }
