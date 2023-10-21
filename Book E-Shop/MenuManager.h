#pragma once

#include <map>  
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

	// ��ɾ���� ����Դϴ�.
	unordered_map<char, Command*> command_list;
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

	// �������� �޴����� ��� ���ο� �޴�ȭ���� �����մϴ�.
	template<typename... TP> 
	void RunMenu(MenuCode menu_code, TP... args);

	// �������� �޴����� ��������� ���ο� �޴�ȭ���� �����մϴ�.
	template<typename ...TP>
	void RunRecursiveMenu(MenuCode menu_code, TP ...args);
};

template<typename ...TP>
inline void MenuManager::RunMenu(MenuCode menu_code, TP ...args)
{
	throw menu_code;
}

template<typename ...TP>
inline void MenuManager::RunRecursiveMenu(MenuCode menu_code, TP ...args)
{
	IO::Buffer* prev_buffer = this->IO.get_buffer();
	IO::Buffer* next_buffer = new IO::Buffer();

	this->IO.set_buffer(next_buffer);

	menu_list[menu_code]->Run(IO, args...);

	this->IO.set_buffer(prev_buffer);

	delete next_buffer;
}
