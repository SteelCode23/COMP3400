#include "Date.h"
#include <iostream>

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

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
