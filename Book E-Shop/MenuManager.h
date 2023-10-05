#pragma once

#include <string>
#include <map>
#include <functional>

#include "Menu.h"

using namespace std;

class MenuManager
{
private:
	// �Ŵ� �Ŵ����� �۵������� ��Ÿ���ϴ�.
	bool is_working = false;

	// �Ŵ����� ����Դϴ�.
	map<string, Menu*> menu_list;

	// ���� �޴��� ��Ÿ���ϴ�.
	Menu* current_menu = nullptr;
	// ���� �޴��� ��Ÿ���ϴ�.
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

	// ���� �޴�ȭ���� �����մϴ�.
	void SetCurrentMenu(string menu_name);

	void ProcessInput(string input);
	void ProcessCommand(char command);

	// �޴�ȭ���� ���ΰ�ħ�մϴ�.
	void RefreshScreen();

	// �޴�ȭ�� ó���� �����մϴ�.
	void Quit();
};

