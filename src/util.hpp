#pragma once
#include <vector>
#include <time.h>
#include <string>

bool startsWith(const std::string& s, const std::string& prefix);
size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
std::string nowString(const char* format = "%Y-%m-%d %H:%M:%S");
std::string uptimeString(time_t sec);
