#include "Date.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;
Date::Date() {}
Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

std::chrono::year_month_day Date::parseDate(const string& dateStr) {
    int y, m, d;
    char dash1, dash2;
    stringstream ss(dateStr);
    ss >> y >> dash1 >> m >> dash2 >> d;
    return std::chrono::year_month_day{
        std::chrono::year{y},
        std::chrono::month{static_cast<unsigned int>(m)},
        std::chrono::day{static_cast<unsigned int>(d)}
    };
}

void Date::addDays(int days) {
    day += days;
    if (day > 30) {
        day -= 30;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
}

bool Date::isOverdue(const Date& currentDate) const {
    if (year < currentDate.year) return true;
    if (year == currentDate.year && month < currentDate.month) return true;
    if (year == currentDate.year && month == currentDate.month && day < currentDate.day) return true;
    return false;
}

void Date::display() const {
    std::cout << day << "/" << month << "/" << year;
}
