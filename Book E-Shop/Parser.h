#pragma once 

#include "util.h"

#include <any>
#include <regex>
#include <string>
#include <functional>

using namespace std;

class Parser {
private:
	function<bool(string)> check = nullptr; 
public:
	string label = "ют╥б";

	wstring regex = L".*";
	string msg_info = "";
	string msg_error = "";

	Parser* set_label(string label) { this->label = label; return this; }

	Parser* set_regex(wstring regex) { this->regex = regex; return this; }

	Parser* set_msg_info(string msg_info) { this->msg_info = msg_info; return this; }
	Parser* set_msg_error(string msg_error) { this->msg_error = msg_error; return this; }

	Parser* set_check(function<bool(string)> check) { this->check = check; return this; }
	
	bool Check(string input) {  
		return regex_match(s2w(input), ::wregex(regex)) && (check == nullptr || check(input));
	}
};