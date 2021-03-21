#pragma once

#include <string>
#include <cctype>
#include <vector>

bool is_equals(const std::string& a, const std::string& b);
std::string readline_or_max(int sockfd, int len);
std::vector<std::string> split_string(const std::string& s, char delim);
