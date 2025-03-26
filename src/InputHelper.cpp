#include "InputHelper.h"
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <sstream>

using namespace std;

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string getLineInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

std::string getValidatedDate(const string& prompt) {
    string input;
    regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!regex_match(input, pattern)) {
            cout << "Invalid format. Use YYYY-MM-DD.\n";
            continue;
        }

        int y, m, d;
        char dash1, dash2;
        stringstream ss(input);
        ss >> y >> dash1 >> m >> dash2 >> d;

        if (dash1 != '-' || dash2 != '-' || m < 1 || m > 12 || d < 1 || d > 31) {
            cout << "Invalid date values.\n";
            continue;
        }

        return input;
    }
}

