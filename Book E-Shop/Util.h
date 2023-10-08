#pragma once

#include <string>
#include <vector>

using namespace std;

template <typename T>
string join(vector<T> array, string delimiter = ",") {
    string result;

    for (T value : array) {
        if (!result.empty()) {
            result += delimiter;
        }

        result += to_string(value);
    }

    return result;
}