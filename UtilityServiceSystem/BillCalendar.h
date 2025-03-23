#ifndef BILL_CALENDAR_H
#define BILL_CALENDAR_H

#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

class BillCalendar {
public:
    int billCalendarID;
    year_month_day startDate;
    year_month_day endDate;
    bool openClosed; 

    BillCalendar();
    BillCalendar(int id, year_month_day start, year_month_day end, bool open);

    int getBillCalendarID() { return billCalendarID; }
    year_month_day getStartDate() { return startDate; }
    year_month_day getEndDate() { return endDate; }
    bool isOpen() { return openClosed; }


    bool createBillCalendar();
    vector<BillCalendar> readAllBillCalendars();
    BillCalendar readBillCalendarById(int billCalendarID);
    void updateBillCalendar();
    BillCalendar getCurrentBillCalendar();
    void displayCalendar();

private:
    void saveCalendars(const string& filename, const vector<BillCalendar>& calendars);
};

#endif