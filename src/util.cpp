#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

string strip_extension(string inStr) {
    size_t offsetPeriod = inStr.find_last_of(".");
    if (offsetPeriod == string::npos)
        return inStr;

    size_t offsetSlash = inStr.find_last_of("/\\");
    if (offsetSlash != string::npos && offsetSlash > offsetPeriod)
        return inStr;

    return inStr.substr(0, offsetPeriod);
}

string replace_spaces(string inStr) {
    size_t offsetSlash = inStr.find_last_of("/\\");
    if (offsetSlash == string::npos)
        offsetSlash = 0;

    for (; offsetSlash < inStr.length(); offsetSlash++) {
        if (inStr[offsetSlash] == ' ')
            inStr[offsetSlash] = '_';
    }

    return inStr;
}

