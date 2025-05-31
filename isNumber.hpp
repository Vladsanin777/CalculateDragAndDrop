#include <cctype>
bool isNumber(const char * str) {
    if (!str) return false;
    char ch{'\0'};
    for (;(ch = *str); str++)
        if (!isdigit(ch)) return false;
    return true;
}