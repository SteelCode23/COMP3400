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

    BillCalendar();
    BillCalendar(int id, year_month_day start, year_month_day end, bool open);

    void setBillCalendarID(int id);
    void setStartDate(year_month_day start);
    void setEndDate(year_month_day end);
    void setOpenClosed(bool open);
    int getBillCalendarID(); 
    year_month_day getStartDate();
    year_month_day getEndDate();
    bool isOpen(); 



    bool createBillCalendar();
    vector<BillCalendar> readAllBillCalendars();
    BillCalendar readBillCalendarById(int billCalendarID);
    void updateBillCalendar();
    BillCalendar getCurrentBillCalendar();
    void displayCalendar();

private:
    void saveCalendars(const string& filename, const vector<BillCalendar>& calendars);
    int billCalendarID;
    std::chrono::year_month_day startDate;
    std::chrono::year_month_day endDate;
    bool openClosed; 

};

#endif