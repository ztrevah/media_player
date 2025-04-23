#include "../inc/utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string msToHHMMSS(long long milliseconds) {
    if (milliseconds < 0) {
        return "Invalid input"; // Handle negative input if needed
    }
  
    long long seconds = milliseconds / 1000;
    long long hours = seconds / 3600;
    long long minutes = (seconds % 3600) / 60;
    long long remainingSeconds = seconds % 60;
  
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << remainingSeconds;
  
    return ss.str();
}

std::string stringToLower(const std::string& input) {
    std::string result = input; // Create a copy to avoid modifying the original
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return std::tolower(c); });
    return result;
}

std::string secondsToHHMMSS(int totalSeconds) {
    if (totalSeconds < 0) {
        return "Invalid input: Seconds cannot be negative";
    }

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::stringstream ss;
    // Use setw and setfill to ensure leading zeros for mm and ss
    ss << std::setw(2) << std::setfill('0') << hours << ":"
       << std::setw(2) << std::setfill('0') << minutes << ":"
       << std::setw(2) << std::setfill('0') << seconds;

    return ss.str();
}

std::string getFileType(const std::string& filename) {
    size_t dotPos = filename.rfind('.');
    if (dotPos != std::string::npos) {
        std::string extension = filename.substr(dotPos + 1);
        if (extension == "mp3" || extension == "wav" || extension == "flac") {
            return "audio";
        } else if (extension == "mp4" || extension == "avi" || extension == "mkv") {
            return "video";
        }
    }
    return "unknown";
}