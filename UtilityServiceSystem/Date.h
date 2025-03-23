#ifndef DATE_H
#define DATE_H
#include <chrono>
#include <string>

class Date {
public:
    int day, month, year;
    std::chrono::year_month_day parseDate(const std::string& dateStr);
    Date();
    Date(int d, int m, int y);
    void addDays(int days);
    bool isOverdue(const Date& currentDate) const;
    void display() const;
};

#endif // DATE_H
