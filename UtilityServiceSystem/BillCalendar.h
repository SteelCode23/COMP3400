#ifndef BILL_CALENDAR_H
#define BILL_CALENDAR_H

#include <string>
#include <chrono>
#include <vector>

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
};

#endif
