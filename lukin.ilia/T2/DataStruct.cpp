#include "DataStruct.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>

bool parseULL(const std::string& s, unsigned long long& value) {
    size_t pos = 0;
    try {
        if (s.size() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            // HEX
            value = std::stoull(s.substr(2), &pos, 16);
            return pos + 2 == s.size();
        } else if (s.size() >= 2 && s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) {
            // BIN
            value = std::stoull(s.substr(2), &pos, 2);
            return pos + 2 == s.size();
        } else if (s.size() >= 1 && s[0] == '0') {
            // OCT
            value = std::stoull(s, &pos, 8);
            return pos == s.size();
        } else if (s.size() >= 3 &&
                  (s.substr(s.size() - 3) == "ull" || s.substr(s.size() - 3) == "ULL")) {
            // ULL literal
            value = std::stoull(s.substr(0, s.size() - 3), &pos);
            return pos == s.size() - 3;
        } else if (s.size() >= 2 &&
                  (s.substr(s.size() - 2) == "ll" || s.substr(s.size() - 2) == "LL")) {
            // ULL literal (without 'u')
            value = std::stoull(s.substr(0, s.size() - 2), &pos);
            return pos == s.size() - 2;
        } else {
            // Decimal
            value = std::stoull(s, &pos);
            return pos == s.size();
        }
    } catch (...) {
        return false;
    }
}

bool parseComplex(const std::string& s, std::complex<double>& value) {
    if (s.size() < 5 || s.substr(0, 3) != "#c(" || s.back() != ')') {
        return false;
    }

    std::istringstream iss(s.substr(3, s.size() - 4));
    double real, imag;
    if (!(iss >> real >> imag)) {
        return false;
    }

    value = std::complex<double>(real, imag);
    return true;
}

bool parseDataStruct(const std::string& line, DataStruct& ds) {
    if (line.size() < 2 || line[0] != '(' || line[line.size() - 1] != ')') {
        return false;
    }

    std::string content = line.substr(1, line.size() - 2);
    std::istringstream iss(content);
    std::string token;

    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    while (std::getline(iss, token, ':')) {
        if (token.empty()) continue;

        std::istringstream tokenStream(token);
        std::string key;
        tokenStream >> key;

        if (key == "key1") {
            std::string valueStr;
            std::getline(tokenStream >> std::ws, valueStr);
            if (!parseULL(valueStr, ds.key1)) {
                return false;
            }
            hasKey1 = true;
        } else if (key == "key2") {
            std::string valueStr;
            std::getline(tokenStream >> std::ws, valueStr);
            if (!parseComplex(valueStr, ds.key2)) {
                return false;
            }
            hasKey2 = true;
        } else if (key == "key3") {
            std::string valueStr;
            std::getline(tokenStream >> std::ws, valueStr);
            if (valueStr.size() < 2 || valueStr[0] != '"' || valueStr.back() != '"') {
                return false;
            }
            ds.key3 = valueStr.substr(1, valueStr.size() - 2);
            hasKey3 = true;
        }
    }

    return hasKey1 && hasKey2 && hasKey3;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }

    double modA = std::abs(a.key2);
    double modB = std::abs(b.key2);
    if (modA != modB) {
        return modA < modB;
    }

    return a.key3.size() < b.key3.size();
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
    os << "(:key1 " << ds.key1 << "ull:key2 #c("
       << ds.key2.real() << " " << ds.key2.imag()
       << "):key3 \"" << ds.key3 << "\":)";
    return os;
}