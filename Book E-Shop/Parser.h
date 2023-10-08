#pragma once 

#include <string>
#include <functional>
#include <regex>
#include <any>

using namespace std;

class Parser {
private:
	function<any(string)> parse = nullptr;
	function<bool(string)> check = nullptr;

	regex re;
public:
	string pre = "ют╥б";

	string regex = ".*";
	string msg_info = "";
	string msg_error = "";

	Parser* set_pre(string pre) { this->pre = pre; return this; }

	Parser* set_regex(string regex) { re = ::regex(this->regex = regex); return this; }

	Parser* set_msg_info(string msg_info) { this->msg_info = msg_info; return this; }
	Parser* set_msg_error(string msg_error) { this->msg_error = msg_error; return this; }

	Parser* set_check(function<bool(string)> check) { this->check = check; return this; }
	Parser* set_parse(function<any(string)> parse) { this->parse = parse; return this; }

	bool Check(string input) { 
		return regex_match(input, re) && (check == nullptr || check(input));
	}

	any Parse(string input) {
		return parse != nullptr ? parse(input) : any(input);
	}
};