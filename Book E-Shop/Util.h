#pragma once

#include <format>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

using namespace std;
using namespace chrono;

static int day_diff(system_clock::time_point a, system_clock::time_point b) {
    return abs(std::chrono::duration_cast<std::chrono::days>(a - b).count());
}

static chrono::system_clock::time_point today() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // �ð��� tm ����ü�� ��ȯ�Ͽ� ���� �ð� ������ ����ϴ�.
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = {};
    localtime_s(&local_time, &now_time_t);

    std::chrono::year_month_day a(std::chrono::year(1900 + local_time.tm_year) / (local_time.tm_mon + 1) / local_time.tm_mday); 

    return std::chrono::sys_days(a);
}

static chrono::system_clock::time_point string_to_date(string input) { 
    // ��¥ ���ڿ��κ��� �Ľ� 
    stringstream sstream(input);

    string year, month, day;
    getline(sstream, year, '.');
    getline(sstream, month, '.');
    getline(sstream, day, '.'); 

    // std::chrono::year_month_day�� ����Ͽ� ��¥�� ����
    std::chrono::year_month_day a(std::chrono::year(stoi(year) + 2000) / stoi(month) / stoi(day));
     
    return std::chrono::sys_days(a);
}

static string date_to_string(chrono::system_clock::time_point time_point) {  
    std::chrono::year_month_day date{ floor<std::chrono::days>(time_point) };

   return format("{:02}.{:02}.{:02}", ((int)date.year()) % 100, (unsigned int)date.month(), (unsigned int)date.day());
}

static string phone_number(string input) {
    input.insert(3, "-");
    input.insert(8, "-");

    return input;
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