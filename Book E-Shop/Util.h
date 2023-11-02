#pragma once

#include <format>
#include <string>
#include <vector>
#include <functional>
#include <ctime>

using namespace std;

static string today() {
    time_t curr_time = time(nullptr);
    struct tm date;

    localtime_s(&date, &curr_time);

    return format("{}.{:02}.{:02}", (date.tm_year) % 100, date.tm_mon + 1, date.tm_mday);
}

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

static string limit(string text, int width) {
    return (text.size() > width) ? std::format("{:.{}}...", text, width - 3) : std::format("{:.{}}", text, width);
}