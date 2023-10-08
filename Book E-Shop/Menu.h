#pragma once

#include <string>
#include <functional>

using namespace std;

class Menu
{
private:
	function<void(MenuManager::MenuIO)> execute_func = nullptr;
public: 
	Menu(function<void(MenuManager::MenuIO)> execute_func) : execute_func(execute_func) {}

	void Execute(MenuManager::MenuIO IO) {
		IO.flush();

		execute_func(IO);
	}
}; 
