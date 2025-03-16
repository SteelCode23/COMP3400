#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Date {
    public:
      int day, month, year;

    Date(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
    }

    void addDays(int days) {
        day += days;
        if (day > 30) { // Simplified month handling
            day -= 30;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    bool isOverdue(Date currentDate) {
        if (year < currentDate.year){
            return true;
        } 
        if (year == currentDate.year && month < currentDate.month){
            return true;
        } 
        if (year == currentDate.year && month == currentDate.month && day < currentDate.day){
            return true;
        } 
        return false;
    }

    void display() {
        cout << day << "/" << month << "/" << year;
    }
};
