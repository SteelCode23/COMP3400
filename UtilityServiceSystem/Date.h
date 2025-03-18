#ifndef DATE_H
#define DATE_H

class Date {
public:
    int day, month, year;

    Date(int d, int m, int y);
    void addDays(int days);
    bool isOverdue(const Date& currentDate) const;
    void display() const;
};

#endif // DATE_H
