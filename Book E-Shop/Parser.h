#pragma once 

#include <string>
#include <functional>
#include <any>

using namespace std;

class Parser {
private:
	function<any* (string)> parse = nullptr;

public:
	string regex = ".*";
	string msg_info = "";
	string msg_error = "";

	Parser* set_regex(string regex) { this->regex = regex; return this; }
	Parser* set_msg_info(string msg_info) { this->msg_info = msg_info; return this; }
	Parser* set_msg_error(string msg_error) { this->msg_error = msg_error; return this; }

	Parser* set_parse(function<any* (string)> parse) { this->parse = parse; return this; }

	any* Parse(string input) {
		return parse != nullptr ? parse(input) : nullptr;
	}
};