#include "BillCalendar.h"
#include "Date.h"
#include "InputHelper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

using namespace std;

// Constructors
BillCalendar::BillCalendar() : billCalendarID(0), openClosed(false) {}

BillCalendar::BillCalendar(int id, year_month_day start, year_month_day end, bool open)
    : billCalendarID(id), startDate(start), endDate(end), openClosed(open) {}

int BillCalendar::getBillCalendarID()  { return billCalendarID; }
year_month_day BillCalendar::getStartDate()  { return startDate; }
year_month_day BillCalendar::getEndDate()  { return endDate; }
bool BillCalendar::isOpen()  { return openClosed; }
void BillCalendar::setBillCalendarID(int id) { billCalendarID = id; }
void BillCalendar::setStartDate(year_month_day start) { startDate = start; }
void BillCalendar::setEndDate(year_month_day end) { endDate = end; }
void BillCalendar::setOpenClosed(bool open) { openClosed = open; }

vector<BillCalendar> BillCalendar::readAllBillCalendars() {
    vector<BillCalendar> calendars;
    ifstream file("data/billcalendar.txt");
    if (!file.is_open()) {
        return calendars; 
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        BillCalendar bc;

        try {
            Date d;
            getline(ss, token, ','); bc.billCalendarID = stoi(token);
            getline(ss, token, ','); bc.startDate = d.parseDate(token); // Non-static, called on instance
            getline(ss, token, ','); bc.endDate = d.parseDate(token);   // Non-static, called on instance
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
    if (!file.is_open()) {
        throw runtime_error("Unable to open file for writing: " + filename);
    }
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
    vector<BillCalendar> calendars = readAllBillCalendars();
    string startDateStr, endDateStr;
    int maxId = 0;
    for (const auto& cal : calendars) {
        if (cal.billCalendarID > maxId) {
            maxId = cal.billCalendarID;
        }
    }
    int newId = maxId + 1; 
    cin.ignore();
    startDateStr = getValidatedDate("Enter Start Date (YYYY-MM-DD): ");
    endDateStr = getValidatedDate("Enter End Date (YYYY-MM-DD): ");
    Date d;
    BillCalendar newCalendar(newId, d.parseDate(startDateStr), d.parseDate(endDateStr), false);
    calendars.push_back(newCalendar);
    saveCalendars("data/billcalendar.txt", calendars);
    return true;
}


BillCalendar BillCalendar::readBillCalendarById(int billCalendarID) {
    vector<BillCalendar> calendars = readAllBillCalendars();
    for (const auto& calendar : calendars) {
        if (calendar.billCalendarID == billCalendarID) {
            return calendar;
        }
    }
    throw runtime_error("BillCalendar with ID " + to_string(billCalendarID) + " not found");
}

BillCalendar BillCalendar::getCurrentBillCalendar() {
    vector<BillCalendar> calendars = readAllBillCalendars();
    for (const auto& calendar : calendars) {
        if (calendar.openClosed) {
            return calendar;
        }
    }
    throw runtime_error("No open BillCalendar found");
}


void BillCalendar::updateBillCalendar() {
    vector<BillCalendar> calendars = readAllBillCalendars();
    bool found = false;
    int billCalendarID;
    int newOpenClosed;
    cout << "Enter Bill Calendar ID to Update" << endl;
    cin >> billCalendarID;
    cout << "Enter new status" << endl;
    cout << "1. Open" << endl;
    cout << "2. Closed" << endl;
    cin >> newOpenClosed;
    switch(newOpenClosed){
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
        default:
            cout << "Not a valid response";
            break;
        }


    if (!found) {
        throw runtime_error("BillCalendar with ID " + to_string(billCalendarID) + " not found");
    }
    saveCalendars("data/billcalendar.txt", calendars);
}


void BillCalendar::displayCalendar() {
    vector<BillCalendar> calendars = readAllBillCalendars();
    cout << "The Current Open Bill Calendar is:" << endl;
    for(const auto calendar:calendars){
        if(calendar.openClosed){
            cout << "Bill Calendar ID: " << calendar.billCalendarID
            << " | Start Date: " << int(calendar.startDate.year()) << "-" << setw(2) << setfill('0') << unsigned(calendar.startDate.month()) << "-" << setw(2) << unsigned(calendar.startDate.day())
            << " | End Date: " << int(calendar.endDate.year()) << "-" << setw(2) << setfill('0') << unsigned(calendar.endDate.month()) << "-" << setw(2) << unsigned(calendar.endDate.day())
            << " | Status: " << (calendar.openClosed ? "Open" : "Closed") << "\n";
        }
    }
}
