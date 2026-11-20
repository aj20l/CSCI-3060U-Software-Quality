// This header file defines utility functions for text processing, including trimming whitespace, padding text, formatting money amounts, and parsing money amounts from strings. These functions are used throughout the project to handle user input and display data in a consistent format.
#pragma once

#include <cctype>    // isdigit: is this character a digit
#include <iomanip>   // setw, setfill, setprecision: format numbers
#include <sstream>   // ostringstream: build text piece by piece
#include <string>
using namespace std;

// Removes whitespace from the beginning and end of a piece of text. Returns the cleaned-up text
inline string trim(const string& text) {
    const string whitespace = " \t\r\n";
    size_t first = text.find_first_not_of(whitespace);
    if (first == string::npos) return "";
    size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Pads a piece of text with spaces on the right so it is exactly the specified width
inline string padRight(const string& text, size_t width) {
    string padded = text.substr(0, width);
    padded.resize(width, ' ');
    return padded;
}

// Converts a piece of text to lowercase. Returns the lowercase version of the text
inline string toLower(string text) {
    for (char& character : text) {
        character = static_cast<char>(tolower(static_cast<unsigned char>(character)));
    }
    return text;
}

// Returns true if the query string is found in the text string, ignoring case
inline bool containsText(const string& text, const string& query) {
    return toLower(text).find(toLower(query)) != string::npos;
}

// Formats a money amount as text with exactly two decimal places and a fixed width.
inline string formatMoney(double amount, size_t width) {
    ostringstream text;
    text << fixed << setprecision(2) << setfill('0') << setw(static_cast<int>(width)) << amount;
    return text.str();
}

// Tries to turn a piece of text into a money amount. Returns true if successful, false if not
// Example: parseMoney("  15.5  ", amount) gives true and sets amount to 15.50. parseMoney("abc", amount) gives false and leaves amount unchanged
inline bool parseMoney(const string& text, double& amount) {
    string cleaned = trim(text);

    // Split at the dot "15.5" -> dollars "15" and cents "5".
    size_t dotPosition = cleaned.find('.');
    string dollars = cleaned.substr(0, dotPosition);
    string cents = (dotPosition == string::npos) ? "" : cleaned.substr(dotPosition + 1);

    // Reject empty dollars, absurdly long numbers and more than 2 decimal places
    if (dollars.empty() || dollars.size() > 7 || cents.size() > 2) return false;

    // Every remaining character must be a digit
    for (char character : dollars + cents) {
        if (!isdigit(static_cast<unsigned char>(character))) return false;
    }

    cents.resize(2, '0');   // "5" means 50 cents
    amount = stol(dollars) + stol(cents) / 100.0;
    return true;
}