#include "BillCalendar.h"
#include "Date.h"
#include "InputHelper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include "Menu.h"

using namespace std::chrono;
using namespace std;

BillCalendar::BillCalendar() : billCalendarID(0), openClosed(false) {}

BillCalendar::BillCalendar(int id, year_month_day start, year_month_day end, bool open)
    : billCalendarID(id), startDate(start), endDate(end), openClosed(open) {}

vector<BillCalendar> BillCalendar::readAllBillCalendars() {
    vector<BillCalendar> calendars;
    ifstream file("data/billcalendar.txt");
    if (!file.is_open()) return calendars;

    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        BillCalendar bc;
        try {
            Date d;
            getline(ss, token, ','); bc.billCalendarID = stoi(token);
            getline(ss, token, ','); bc.startDate = d.parseDate(token);
            getline(ss, token, ','); bc.endDate = d.parseDate(token);
            getline(ss, token, ','); bc.openClosed = stoi(token);
            calendars.push_back(bc);
        } catch (const exception& e) {
            cerr << "Error parsing line: " << e.what() << "\n";
        }
    }
    file.close();
    return calendars;
}

void BillCalendar::saveCalendars(const string& filename, const vector<BillCalendar>& calendars) {
    ofstream file(filename);
    if (!file.is_open()) throw runtime_error("Unable to open file for writing: " + filename);

    file << "BillCalendarID,StartDate,EndDate,OpenClosed\n";
    for (const auto& calendar : calendars) {
        file << calendar.billCalendarID << ","
             << int(calendar.startDate.year()) << "-" << unsigned(calendar.startDate.month()) << "-" << unsigned(calendar.startDate.day()) << ","
             << int(calendar.endDate.year()) << "-" << unsigned(calendar.endDate.month()) << "-" << unsigned(calendar.endDate.day()) << ","
             << (calendar.openClosed ? "1" : "0") << "\n";
    }
    file.close();
}

bool BillCalendar::createBillCalendar() {
    clearScreen();
    vector<BillCalendar> calendars = readAllBillCalendars();
    string startDateStr = getLineInput("Enter Start Date (YYYY-MM-DD): ");
    string endDateStr = getLineInput("Enter End Date (YYYY-MM-DD): ");

    int maxId = 0;
    for (const auto& cal : calendars) {
        if (cal.billCalendarID > maxId) maxId = cal.billCalendarID;
    }
    int newId = maxId + 1;

    try {
        Date d;
        year_month_day start = d.parseDate(startDateStr);
        year_month_day end = d.parseDate(endDateStr);
        BillCalendar newCalendar(newId, start, end, false);
        calendars.push_back(newCalendar);
        saveCalendars("data/billcalendar.txt", calendars);
        return true;
    } catch (const exception& e) {
        cerr << "Invalid date format: " << e.what() << endl;
        return false;
    }
}

BillCalendar BillCalendar::readBillCalendarById(int billCalendarID) {
    vector<BillCalendar> calendars = readAllBillCalendars();
    for (const auto& calendar : calendars) {
        if (calendar.billCalendarID == billCalendarID) return calendar;
    }
    throw runtime_error("BillCalendar with ID " + to_string(billCalendarID) + " not found");
}

BillCalendar BillCalendar::getCurrentBillCalendar() {
    vector<BillCalendar> calendars = readAllBillCalendars();
    for (const auto& calendar : calendars) {
        if (calendar.openClosed) return calendar;
    }
    throw runtime_error("No open BillCalendar found");
}

void BillCalendar::updateBillCalendar() {
    clearScreen();
    vector<BillCalendar> calendars = readAllBillCalendars();
    bool found = false;

    int billCalendarID = getIntInput("Enter Bill Calendar ID to Update: ");
    cout << "Enter new status:\n1. Open\n2. Closed\n";
    int newOpenClosed = getIntInput("Choice: ");

    switch (newOpenClosed) {
        case 1:
            for (const auto& cal : calendars) {
                if (cal.openClosed && cal.billCalendarID != billCalendarID) {
                    throw runtime_error("Another calendar (ID " + to_string(cal.billCalendarID) + ") is already open");
                }
            }
            for (auto& calendar : calendars) {
                if (calendar.billCalendarID == billCalendarID) {
                    calendar.openClosed = 1;
                    found = true;
                    break;
                }
            }
            break;

        case 2:
            for (auto& calendar : calendars) {
                if (calendar.billCalendarID == billCalendarID) {
                    calendar.openClosed = 0;
                    found = true;
                    break;
                }
            }
            break;

        default:
            cout << "Not a valid response\n";
            return;
    }

    if (!found) {
        throw runtime_error("BillCalendar with ID " + to_string(billCalendarID) + " not found");
    }

    saveCalendars("data/billcalendar.txt", calendars);
}

void BillCalendar::displayCalendar() {
    clearScreen();
    vector<BillCalendar> calendars = readAllBillCalendars();
    cout << "The Current Open Bill Calendar is:\n";
    for (const auto& calendar : calendars) {
        if (calendar.openClosed) {
            cout << "Bill Calendar ID: " << calendar.billCalendarID
                 << " | Start Date: " << int(calendar.startDate.year()) << "-" << setw(2) << setfill('0') << unsigned(calendar.startDate.month()) << "-" << setw(2) << unsigned(calendar.startDate.day())
                 << " | End Date: " << int(calendar.endDate.year()) << "-" << setw(2) << setfill('0') << unsigned(calendar.endDate.month()) << "-" << setw(2) << unsigned(calendar.endDate.day())
                 << " | Status: " << (calendar.openClosed ? "Open" : "Closed") << "\n";
        }
    }
}
