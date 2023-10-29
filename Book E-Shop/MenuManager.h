#pragma once

#include <map>  
#include <unordered_set>
#include <string>
#include <functional>

#include "IO.h"
#include "Menu.h"

using namespace std;

typedef function<void(void)> action; 

using RunFunc = function<void(MenuIO&)>;

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
	map<MenuCode, IMenu*> menu_list;

	vector<MenuCode> menu_stack;

	// ��ɾ���� ����Դϴ�.
	unordered_map<char, Command*> command_list;
	unordered_set<char> command_availability;

	void RunMenuInternal(MenuCode menu_code); 
public:   
	MenuManager();
	~MenuManager();

	// ȭ���� ������� ����ϴ� �޴� �Ŵ��� �����Դϴ�.
	MenuIO IO;

	// �޴�ȭ�� ó���� �����մϴ�.
	void Start(MenuCode menu_code);

	// �޴�ȭ���� ��Ͽ� �߰��մϴ�.
	void AppendMenu(MenuCode menu_code, IMenu* menu);
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
	void ToggleCommand(unordered_set<char> command_availability) {
		this->command_availability = command_availability;
	}	
	
	// ��ɾ��� ��� ���� ���θ� �����մϴ�.
	template<typename... TP, typename = char>
	void ToggleCommand(TP... command) {
		ToggleCommand({ command... });
	}

	// ���� ��� ������ ��ɾ���� ����մϴ�.
	void PrintCommand();

	IMenu* GetMenu(MenuCode menu_code);

	// �������� �޴����� ��� ���ο� �޴�ȭ���� �����մϴ�.
	template<typename... TP> 
	void RunMenu(MenuCode menu_code, TP... args);

	// �������� �޴����� ��������� ���ο� �޴�ȭ���� �����մϴ�.
	template<typename ...TP>
	void RunRecursiveMenu(MenuCode menu_code, TP ...args);

	void RunPreviousMenu();

	IMenu* operator[](MenuCode menu_code);
}; 

inline void MenuManager::RunMenuInternal(MenuCode menu_code)
{
	menu_stack.erase(find(menu_stack.begin(), menu_stack.end(), menu_code), menu_stack.end());

	menu_stack.push_back(menu_code);

	GetMenu(menu_code)->Run(IO);
}

template<typename ...TP>
inline void MenuManager::RunMenu(MenuCode menu_code, TP ...args)
{
	GetMenu(menu_code)->SetArgs(args...);

	throw menu_code;
}

template<typename ...TP>
inline void MenuManager::RunRecursiveMenu(MenuCode menu_code, TP ...args)
{ 
	IO::Buffer* prev_buffer = this->IO.get_buffer();
	IO::Buffer* next_buffer = new IO::Buffer();

	// ���� ���� ���� �� ���� ���� ����
	auto command_availability = this->command_availability;

	this->IO.set_buffer(next_buffer);

	ToggleCommand('z', 'l', 'q');

	// �޴� ����
	GetMenu(menu_code)->SetArgs(args...)->Run(IO);

	// ���� ���·� ����
	this->command_availability = command_availability;

	this->IO.set_buffer(prev_buffer);

	delete next_buffer;
} 

inline void MenuManager::RunPreviousMenu()
{
	MenuCode menu_code = MENU_NONE;

	if (menu_stack.size() > 1) {
		menu_stack.pop_back();

		menu_code = menu_stack.back();
	} 

	throw menu_code;
}