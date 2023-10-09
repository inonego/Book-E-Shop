#pragma once

#include <any>
#include <map>  
#include <set>
#include <string>
#include <functional>

using namespace std;

class Parser;

class Menu;

typedef function<void(void)> action;

class MenuIO {
public:
	class Command {
	public:
		// ��ɾ� ǥ�ø� ���� �̸��Դϴ�.
		string name;

		// ��ɾ �۵��� �� ����Ǵ� �Լ��Դϴ�.
		function<void()> command_func = nullptr;

		Command(string name, action command_func) : name(name), command_func(command_func) {}
	};
private:
	friend class MenuManager;

	// ȭ�鿡 ��µ� �ؽ�Ʈ�� �ӽ÷� �����ϴ� �����Դϴ�.
	string printed = "";

	// �Է��� �޽��ϴ�.
	string input_default();

	// ��ɾ���� ����Դϴ�.
	unordered_map<char, Command*> command_list;

	// ���� �޴����� ��� ������ ��ɾ ���� ����Դϴ�.
	set<char> command_availability;

	// �Է� ���� ��ɾ �۵���ŵ�ϴ�.
	void ProcessCommand(char character);
	// ��ɾ ��Ͽ� �����ϴ����� ��ȯ�մϴ�.
	bool HasCommand(char character);
	// ��ɾ ���� ��� ���������� ��ȯ�մϴ�.
	bool CanCommand(char character);

	// ���� �� �޸𸮸� �����մϴ�.
	void CleanUp();
public:
	MenuIO();
	~MenuIO();

	// ��ɾ ��Ͽ� �߰��մϴ�.
	void AppendCommand(char character, Command* command); 
	// ��ɾ ��Ͽ��� �����մϴ�.
	void RemoveCommand(char character); 
	// ���� �޴����� ��� ������ ��ɾ ���� ����� �����մϴ�.
	void ToggleCommand(set<char> command_availability);

	/*
	- freeze�� ������ ȭ�鿡 ��µ� �ؽ�Ʈ(printed)�� �ӽ÷� �����մϴ�.
	�־��� body �Լ��� �����ϰ� rollback �Լ��� ���ڷ� �����մϴ�.

	- body������ rollback �Լ��� �����ϸ� ������ ����� �ؽ�Ʈ�� �ҷ��ͼ� ����ϴ�.
	freeze�� ������ ����� ��ȿȭ�Ǹ� ���� ���·� ���ư��ϴ�.
	*/
	void freeze(function<void(action)> body);

	// ������ ����� �ؽ�Ʈ�� ���ϴ�.
	void flush();

	// ȭ�鿡 ��µ� ��� �ؽ�Ʈ�� ����ϴ�.
	void clear();

	// ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
	void print(string text);

	// ȭ�鿡 ��µ� ��� �ؽ�Ʈ�� �����, ������ ����� �ؽ�Ʈ�� ȭ�鿡 ����մϴ�.
	void reprint();

	// �Է��� �޽��ϴ�.
	string input();

	// �ʿ��� ������ ����ϰ� �Է��� �޽��ϴ�.
	string input(string msg_info, string pre = "�Է�");

	// Parser�� �̿��� �Է��� �ް�, �ùٸ� �����̸� �Ľ��Ͽ� ��ȯ�մϴ�.
	any input(Parser* parser);

	// "�ƹ�Ű�� �Է��ϼ���..."�� ����ϰ� ����մϴ�.
	void pause();

	// ȭ�鿡 ���м��� �׸��ϴ�.
	void print_line(bool bold = true, int length = 40);

	// ȭ�鿡 ���� �޴����� ��� ������ ��ɾ ���� ����� ����մϴ�.
	void print_available_command();
};

class MenuManager
{
public:
	enum Event
	{
		MENU
	};
private:    
	// �Ŵ�ȭ����� ����Դϴ�.
	map<string, Menu*> menu_list; 

	// ������ ������ �޴�ȭ���Դϴ�.
	Menu* next_menu;
	// ���� �������� �޴�ȭ���Դϴ�.
	Menu* current_menu;

	// ���� �� �޸𸮸� �����մϴ�.
	void CleanUp(); 

	// ������ ������ �޴�ȭ���� �����մϴ�.
	void SetNextMenu(string menu_name);
public:   
	MenuManager();
	~MenuManager();

	// ȭ���� ������� ����ϴ� �޴� �Ŵ��� �����Դϴ�.
	MenuIO IO;

	// �޴�ȭ�� ó���� �����մϴ�.
	void Start(string start_menu_name);

	// �޴�ȭ���� ���¸� �����մϴ�.
	void SetMenu(string title, string menu_name, string prev_menu_name, set<char> command_availability);

	// �޴�ȭ���� ��Ͽ� �߰��մϴ�.
	void AppendMenu(string menu_name, Menu* menu);
	// �޴�ȭ���� ��Ͽ��� �����մϴ�.
	void RemoveMenu(string menu_name);

	// ���� �������� �޴�ȭ���� ��ȯ�մϴ�.
	Menu* GetCurrentMenu();

	// - recursive = true : �������� �޴�ȭ���� �Լ� �ȿ��� ��������� �ٸ� �޴�ȭ���� �Լ��� �����ŵ�ϴ�.
	// ������ �Ϸ�Ǹ� ������ ����� �ؽ�Ʈ�� �ҷ��ͼ� ����� ���� ���·� ���ư��ϴ�.
	// - recursive = false : ��� ����� �޴�ȭ���� �Լ����� ��� �ٸ� �޴�ȭ���� �Լ��� �����ŵ�ϴ�.
	void RunMenu(string menu_name, bool recursive = false);
};