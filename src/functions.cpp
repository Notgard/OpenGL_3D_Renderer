#define _CRT_SECURE_NO_WARNINGS

#include "functions.h"

std::string currentTime(std::chrono::time_point<std::chrono::system_clock> now)
{
    // you need to get milliseconds explicitly
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
        ) % 1000;
    // and that's a "normal" point of time with seconds
    auto timeNow = std::chrono::system_clock::to_time_t(now);

    std::ostringstream currentTimeStream;
    currentTimeStream << std::put_time(localtime(&timeNow), "%d.%m.%Y %H:%M:%S")
                      << "." << std::setfill('0') << std::setw(3) << milliseconds.count()
                      << " " << std::put_time(localtime(&timeNow), "%z");

    return currentTimeStream.str();
}

bool endsWith(std::string const &originalString, std::string const &ending)
{
    if (originalString.length() >= ending.length())
    {
        return (
            0 == originalString.compare(
                originalString.length() - ending.length(),
                ending.length(),
                ending
                )
            );
    }
    else
    {
        return false;
    }
}

//function to get filename from path
std::string getFileName(const std::string &s) {
    char sep = '/';
    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }
    return("");
}

float random_float(float min, float max)
{
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}