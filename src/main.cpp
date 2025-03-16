#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip> 
#include "batch.h"

using namespace std;
using namespace std::chrono;


int main(){

    BatchService service;
    //service.BillingBatch(1);
    service.SimulatePayments(1);
    //service.CalculateOverdue();

    
    return 0;
}