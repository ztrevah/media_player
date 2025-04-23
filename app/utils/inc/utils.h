#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string trim(const std::string& str);
std::string msToHHMMSS(long long milliseconds);
std::string stringToLower(const std::string& input);
std::string secondsToHHMMSS(int totalSeconds);
std::string getFileType(const std::string& filename);

#endif