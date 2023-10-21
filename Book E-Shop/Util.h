#pragma once

#include <string>
#include <vector>
#include <functional>

using namespace std;

static string join(vector<int> array, string delimiter = ",") {
    string result;

    for (int value : array) {
        if (!result.empty()) {
            result += delimiter;
        }

        result += to_string(value);
    }

    return result;
}

static string join(vector<string> array, string delimiter = "") {
    string result;

    for (string value : array) {
        if (!result.empty()) {
            result += delimiter;
        }

        result += value;
    }

    return result;
}