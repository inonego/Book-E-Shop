#pragma once

#include <any>
#include <map> 
#include <string>
#include <functional>

using namespace std;

class Parser;

class Menu;

class MenuManager
{
public:
	enum Event
	{
		MENU
	};

	class MenuIO {
	private:
		friend class MenuManager; 

		// ȭ�鿡 ��µ� �ؽ�Ʈ�� �����մϴ�.
		string saved = "";

		// ��ɾ �ش��ϴ� �Լ��� ����Դϴ�.
		map<char, function<void()>> command_func_list;		  

		// �Է��� �޽��ϴ�.
		string input_raw(); 

		// ��ɾ �ش��ϴ� �Լ��� �����մϴ�.
		void ProcessCommand(char command);
		// ��ɾ �����ϴ��� ��ȯ�մϴ�.
		bool HasCommand(char command);
	public:   
		MenuIO();
		~MenuIO();

		// ��ɾ �ش��ϴ� �Լ��� �߰��մϴ�.
		void AppendCommandFunc(char command, function<void()> command_func);
		// ��ɾ �ش��ϴ� �Լ��� �����մϴ�.
		void RemoveCommandFunc(char command);

		// ȭ�鿡 ��µ� �ؽ�Ʈ�� �����ϰ� �Լ��� �����մϴ�.
		// �Լ� �ȿ��� rollback �Լ��� �����ϸ�, ������ ����� ȭ�� ������� �ѹ��Ͽ� ������մϴ�.
		void freeze(function<void(function<void(void)>)> body);

		void flush();

		// ȭ���� ��� �ؽ�Ʈ�� ����ϴ�.
		void clear();

		// ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
		void print(string text);

		// ������ ����� ȭ�� ������� ������մϴ�.
		void reprint();

		// �Է��� �ް� ��ɾ� ó���� �մϴ�.
		string input();

		// �ʿ��� ������ ����ϰ� �Է��� �޽��ϴ�.
		string input(string msg_info, string pre = "�Է�"); 

		// Parser�� �̿��� �Է��� �ް�, �ùٸ� �����̸� �Ľ��մϴ�.
		any input(Parser* parser);

		// "�ƹ�Ű�� �Է��ϼ���..."�� ����ϰ� ����մϴ�.
		void pause();
	};
private:  
	// �Ŵ����� ����Դϴ�.
	map<string, Menu*> menu_list; 

	Menu* next_menu;

	// ���� �� �޸𸮸� �����մϴ�.
	void CleanUp();

	void SetNextMenu(string menu_name);
public:   
	MenuManager();
	~MenuManager();

	MenuIO IO;

	void Start(string start_menu_name);

	// �޴�ȭ���� �߰��մϴ�.
	void AppendMenu(string menu_name, Menu* menu);
	// �޴�ȭ���� �����մϴ�.
	void RemoveMenu(string menu_name);

	// - ��������� �޴��� �����ϸ� �������� �޴� �Լ� �ȿ��� �ٸ� �޴��� �Լ��� ����˴ϴ�.
	// ������ �Ϸ�Ǹ� ���� �޴��� �����ϰ� ����� ����� ������մϴ�.
	// - ��������� �޴��� �������� ������ ��� ����� �޴� �Լ����� ��� �ٸ� �޴��� �Լ��� ����˴ϴ�.
	void RunMenu(string menu_name, bool recursive = false);

	// �޴�ȭ�� ó���� �����մϴ�.
	void Quit();
};

typedef MenuManager::MenuIO MenuIO;