#pragma once

#include <format>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
  
std::wstring s2w(const std::string& source);

static int day_diff(std::chrono::system_clock::time_point a, std::chrono::system_clock::time_point b) {
    return abs(std::chrono::duration_cast<std::chrono::days>(a - b).count());
}

static std::chrono::system_clock::time_point today() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // 시간을 tm 구조체로 변환하여 로컬 시간 정보를 얻습니다.
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = {};
    localtime_s(&local_time, &now_time_t);

    std::chrono::year_month_day a(std::chrono::year(1900 + local_time.tm_year) / (local_time.tm_mon + 1) / local_time.tm_mday); 

    return std::chrono::sys_days(a);
}

static std::chrono::system_clock::time_point string_to_date(std::string input) { 
    // 날짜 문자열로부터 파싱 
    std::stringstream sstream(input);

    std::string year, month, day;
    getline(sstream, year, '.');
    getline(sstream, month, '.');
    getline(sstream, day, '.'); 

    // std::chrono::year_month_day를 사용하여 날짜를 설정
    std::chrono::year_month_day a(std::chrono::year(stoi(year) + 2000) / stoi(month) / stoi(day));
     
    return std::chrono::sys_days(a);
}

static std::string date_to_string(std::chrono::system_clock::time_point time_point) {  
    std::chrono::year_month_day date{ floor<std::chrono::days>(time_point) };

   return std::format("{:02}.{:02}.{:02}", ((int)date.year()) % 100, (unsigned int)date.month(), (unsigned int)date.day());
}

static std::string phone_number(std::string input) {
    input.insert(3, "-");
    input.insert(8, "-");

    return input;
}

static std::string join(std::vector<int> array, std::string delimiter = ",") {
    std::string result;

    for (int value : array) {
        if (!result.empty()) {
            result += delimiter;
        }

        result += std::to_string(value);
    }

    return result;
}

static std::string join(std::vector<std::string> array, std::string delimiter = "") {
    std::string result;

    for (std::string value : array) {
        if (!result.empty()) {
            result += delimiter;
        }

        result += value;
    }

    return result;
}

static std::string join(std::vector<std::chrono::system_clock::time_point> array, std::string delimiter = "") {
    std::string result;

    for (std::chrono::system_clock::time_point value : array) {
        if (!result.empty()) {
            result += delimiter;
        }

        result += date_to_string(value);
    }

    return result;
}

static std::string limit(std::string text, int width) {
    return (text.size() > width) ? std::format("{:.{}}...", text, width - 3) : std::format("{:.{}}", text, width);
}