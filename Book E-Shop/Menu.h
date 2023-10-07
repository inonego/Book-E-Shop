#pragma once

#include <functional>

class Menu
{
private:
	function<void(void)> _print;
public: 
	Menu(function<void(void)> _print) : _print(_print) { }

	void print() {
		_print();
	}
}; 
