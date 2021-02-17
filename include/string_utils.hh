#ifndef string_utils_h
#define string_utils_h 1

#include <locale>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>

// trim from start
static inline std::string &ltrim(std::string &my_string) {
        my_string.erase(my_string.begin(), std::find_if(my_string.begin(), my_string.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return my_string;
}

// trim from end
static inline std::string &rtrim(std::string &my_string) {
        my_string.erase(std::find_if(my_string.rbegin(), my_string.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), my_string.end());
        return my_string;
}

// trim from both ends
static inline std::string &trim(std::string &my_string) {
        return ltrim(rtrim(my_string));
}

static inline std::vector<std::string> &split(const std::string &my_string, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(my_string);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static inline std::vector<std::string> split(const std::string &my_string, char delim) {
    std::vector<std::string> elems;
    split(my_string, delim, elems);
    return elems;
}

#endif
