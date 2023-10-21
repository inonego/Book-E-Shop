#pragma once

#include <map>  
#include <unordered_set>
#include <string>
#include <functional>

#include "IO.h"
#include "Menu.h"

using namespace std;

typedef function<void(void)> action;

class MenuManager
{
public:	
	class Command {
	public:
		// ��ɾ� ǥ�ø� ���� �̸��Դϴ�.
		string name;

		// ��ɾ �۵��� �� ����Ǵ� �Լ��Դϴ�.
		action command_func = nullptr;

		Command(string name, action command_func) : name(name), command_func(command_func) {}
	};
private:   
	// �Ŵ�ȭ����� ����Դϴ�.
	map<MenuCode, Menu*> menu_list;

	Menu* current_menu;

	// ��ɾ���� ����Դϴ�.
	unordered_map<char, Command*> command_list;
	unordered_set<char> command_availability;
public:   
	MenuManager();
	~MenuManager();

	// ȭ���� ������� ����ϴ� �޴� �Ŵ��� �����Դϴ�.
	MenuIO IO;

	// �޴�ȭ�� ó���� �����մϴ�.
	void Start(MenuCode menu_code);

	// �޴�ȭ���� ��Ͽ� �߰��մϴ�.
	void AppendMenu(MenuCode menu_code, Menu* menu);
	// �޴�ȭ���� ��Ͽ��� �����մϴ�.
	void RemoveMenu(MenuCode menu_code);

	// ��ɾ ��Ͽ� �߰��մϴ�.
	void AppendCommand(char character, Command* command);
	// ��ɾ ��Ͽ��� �����մϴ�.
	void RemoveCommand(char character);

	// ��ɾ ��Ͽ� �����ϴ����� ��ȯ�մϴ�.
	bool HasCommand(char character);
	// ��ɾ ���� ��� ���������� ��ȯ�մϴ�.
	bool CanCommand(char character);
	// �Է� ���� ��ɾ �۵���ŵ�ϴ�.
	void ProcessCommand(char character);
	// ��ɾ��� ��� ���� ���θ� �����մϴ�.
	template<typename... TP, typename = char>
	void ToggleCommand(TP... command) {
		command_availability = unordered_set<char>{ command... };
	}

	// ���� ��� ������ ��ɾ���� ����մϴ�.
	void PrintCommand();

	Menu* GetCurrentMenu();

	// �������� �޴����� ��� ���ο� �޴�ȭ���� �����մϴ�.
	template<typename... TP> 
	void RunMenu(MenuCode menu_code, TP... args);

	// �������� �޴����� ��������� ���ο� �޴�ȭ���� �����մϴ�.
	template<typename ...TP>
	void RunRecursiveMenu(MenuCode menu_code, TP ...args);

	Menu* operator[](MenuCode menu_code);
}; 

template<typename ...TP>
inline void MenuManager::RunMenu(MenuCode menu_code, TP ...args)
{
	vector<any> v{ args... };

	throw make_pair(menu_code, v);
}

template<typename ...TP>
inline void MenuManager::RunRecursiveMenu(MenuCode menu_code, TP ...args)
{
	IO::Buffer* prev_buffer = this->IO.get_buffer();
	IO::Buffer* next_buffer = new IO::Buffer();

	// ���� ���� ���� �� ���� ���� ����
	auto command_availability = this->command_availability;

	auto current_menu = this->current_menu;
	 
	this->IO.set_buffer(next_buffer);

	ToggleCommand('z', 'l', 'q');

	// �޴� ����
	vector<any> v{ args... };

	menu_list[menu_code]->Run(IO, v);

	// ���� ���·� ����
	this->command_availability = command_availability;

	this->current_menu = current_menu;

	this->IO.set_buffer(prev_buffer);

	delete next_buffer;
}
