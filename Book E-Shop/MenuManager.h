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
	class MenuIO {
	private:
		friend class MenuManager;

		// ȭ�鿡 ��µ� �ؽ�Ʈ�� �����մϴ�.
		string saved = ""; 
	public: 
		// ȭ�鿡 ��µ� �ؽ�Ʈ�� �����ϰ� �Լ��� �����մϴ�.
		// �Լ� �ȿ��� rollback �Լ��� �����ϸ�, ������ ����� ȭ�� ������� �ѹ��Ͽ� ������մϴ�.
		void freeze(function<void(function<void(void)>)> body);

		// ȭ���� ��� �ؽ�Ʈ�� ����ϴ�.
		void clear();

		// ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
		void print(string text);

		// ������ ����� ȭ�� ������� ������մϴ�.
		void reprint();

		// �Է��� �޽��ϴ�.
		string input(); 
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

	MenuIO IO;

	// ��ɾ �ش��ϴ� �Լ��� ����Դϴ�.
	map<char, function<void()>> command_func_list;

	void CleanUp(); 
public: 

	MenuManager();
	~MenuManager();
	
	// ��ɾ �ش��ϴ� �Լ��� �߰��մϴ�.
	void AppendCommandFunc(char command, function<void()> command_func);
	// ��ɾ �ش��ϴ� �Լ��� �����մϴ�.
	void RemoveCommandFunc(char command);

	void Start(string start_menu_name);

	// �޴�ȭ���� �߰��մϴ�.
	void AppendMenu(string menu_name, Menu* menu);
	// �޴�ȭ���� �����մϴ�.
	void RemoveMenu(string menu_name);

	// ���� �޴�ȭ���� �����մϴ�.
	void CallMenu(string menu_name);
	 
	// �޴�ȭ�� ó���� �����մϴ�.
	void Quit();
};

typedef MenuManager::MenuIO MenuIO;