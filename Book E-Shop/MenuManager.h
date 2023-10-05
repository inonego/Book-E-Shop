#pragma once

#include <string>
#include <map>
#include <functional>

#include "Menu.h"

using namespace std;

class MenuManager
{
private:
	// 매뉴 매니저가 작동중인지 나타냅니다.
	bool is_working = false;

	// 매뉴들의 목록입니다.
	map<string, Menu*> menu_list;

	// 현재 메뉴를 나타냅니다.
	Menu* current_menu = nullptr;
	// 이전 메뉴를 나타냅니다.
	Menu* previous_menu = nullptr;

	map<char, function<void()>> command_func_list; 

	void CleanUp();
public:
	MenuManager();
	~MenuManager();

	void AppendMenu(string menu_name, Menu* menu);
	void RemoveMenu(string menu_name);

	void AppendCommandFunc(char command, function<void()> command_func);
	void RemoveCommandFunc(char command);

	// 현재 메뉴화면을 설정합니다.
	void SetCurrentMenu(string menu_name);

	void ProcessInput(string input);
	void ProcessCommand(char command);

	// 메뉴화면을 새로고침합니다.
	void RefreshScreen();

	// 메뉴화면 처리를 중지합니다.
	void Quit();
};

