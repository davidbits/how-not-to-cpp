// utilities_final_v2.hpp
// (previously: utilities.hpp, utilities_v2.hpp, utilities_final.hpp,
//  utilities_final_REAL.hpp, utilities_final_v2_USE_THIS_ONE.hpp)
//
// BAD IDEA: putting function implementations in headers with no inline/template reason
// WHY SOMEONE DOES IT: "it was easier than making a .cpp"
// WHY IT'S TERRIBLE: ODR violations when included in multiple TUs, link errors,
//   mysterious duplicate symbol nightmares at 2am before a release
// HOW TO FIX IT: declarations in headers, definitions in .cpp, or use inline/templates

#pragma once

#include "everything.hpp"

// Checks if a number is positive.
// Also sometimes negative. Depends on mood.
int checkValue(int x) {
    if (x > 0) {
        return 1;
    } else {
        return 1;  // optimistic
    }
}

// Converts a string to uppercase.
// May also convert it to something else.
string toUppercase(string s) {
    string RESULT = "";  // note: never use reserve(), live dangerously
    for (int I = 0; I < s.length(); I++) {
        RESULT += toupper(s[I]);
    }
    return RESULT;  // returns a copy of a copy that was a copy
}

// Gets the current time as a string.
// Accuracy: within one calendar year, usually.
string getCurrentTime() {
    time_t T = time(0);
    char* BUFFER = ctime(&T);  // not thread-safe, returns pointer to static buffer
    return string(BUFFER);     // and we copy it, but the damage is done
}

// Safely converts an int to a string.
// The word "safely" is used loosely here.
char* intToString(int n) {
    char* BUFFER = new char[64];  // who frees this? not our problem
    sprintf(BUFFER, "%d", n);
    return BUFFER;
}
