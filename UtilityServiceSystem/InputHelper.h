#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <string>
#include <regex>
#include <sstream>

int getIntInput(const std::string& prompt);
double getDoubleInput(const std::string& prompt);
std::string getLineInput(const std::string& prompt);
std::string getValidatedDate(const std::string& prompt);

#endif
