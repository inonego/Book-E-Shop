#pragma once

#include <string>

using namespace std;

template <typename T>
string join(T* array, string delimiter = ",") { 
	string result;

	for (T value : array) {
		if (!result.empty()) { 
			result += delimiter;
		}

		result += to_string(value);
	}

	return result;
}