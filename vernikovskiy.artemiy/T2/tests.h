#ifndef TESTS_H
#define TESTS_H

#include <random>
#include <string>
#include <vector>
#include <algorithm>

double generateRandomDouble(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

char generateRandomChar() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 25);
    return 'a' + dis(gen);
}

std::string generateRandomString(size_t length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, sizeof(charset) - 2);
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result += charset[dis(gen)];
    }
    return result;
}


std::string generateValidRandomPrompt() {
    double key1 = generateRandomDouble(1.0, 100.0);
    char key2 = generateRandomChar();
    std::string key3 = generateRandomString(10);

    std::vector<std::string> keys = {
        "(:key1 " + std::to_string(key1) + "e+1" + ":key2 '" + key2 + "':key3 \"" + key3 + "\":)"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(keys.begin(), keys.end(), gen);

    return keys[0];
}

#endif
