#include <iostream>
#include <cstring>

using namespace std;

int main() {
    // Input string
    const char *input = "1+(6+9*(8-7)/(8^2)*9)";  // Используем строку напрямую, без выделения памяти

    // Find the closing parenthesis ')'
    const char *end = strchr(input, ')');

    // Find the matching opening parenthesis '(' from the last closing parenthesis
    int openCount = 0;
    const char *start = nullptr;
    for (const char *p = end; p >= input; --p) {
        if (*p == ')') {
            openCount++;
        } else if (*p == '(') {
            openCount--;
            if (openCount == 0) {
                start = p;
                break;
            }
        }
    }

    // Debug output to check the positions of start and end
    cout << "Start pointer: " << (start - input) << endl;
    cout << "End pointer: " << (end - input) << endl;

    // Calculate the length of the substring inside the parentheses
    size_t len = size_t(end - start - 1);

    // Debug: print the calculated length
    cout << "Calculated length: " << len << endl;


    // Allocate memory for the substring
    char *substring = new(nothrow) char[len + 1];

    // Copy the substring from input (inside parentheses)
    strncpy(substring, start + 1, len);
    substring[len] = '\0';  // Null terminate the substring

    // Output
    cout << "Input: " << input << endl;
    cout << "Substring: " << substring << endl;

    // Clean up memory
    delete[] substring;

    return 0;
}

