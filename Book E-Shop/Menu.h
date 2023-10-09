#pragma once

#include <set>
#include <string>
#include <functional>

using namespace std;

class Menu
{
private:
	function<void(MenuIO)> execute_func = nullptr;

	string title;

	string prev_menu_name; 

	set<char> command_availability;
public:
	Menu(function<void(MenuIO)> execute_func) : execute_func(execute_func) {}

	string GetPrevMenuName() {
		return prev_menu_name;
	}

	void SetValue(string title, string prev_menu_name, set<char> command_availability) {
		this->title = title;

		this->prev_menu_name = prev_menu_name;

		this->command_availability = command_availability;
	}

	void Execute(MenuIO IO) {
		IO.flush();

		IO.ToggleCommand(command_availability);

		if (execute_func != nullptr) {
			execute_func(IO);
		}
	}
}; 
