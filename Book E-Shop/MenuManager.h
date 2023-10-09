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
		// 명령어 표시를 위한 이름입니다.
		string name;

		// 명령어가 작동될 때 실행되는 함수입니다.
		function<void()> command_func = nullptr;

		Command(string name, action command_func) : name(name), command_func(command_func) {}
	};
private:
	friend class MenuManager;

	// 화면에 출력된 텍스트를 임시로 저장하는 변수입니다.
	string printed = "";

	// 입력을 받습니다.
	string input_default();

	// 명령어들의 목록입니다.
	unordered_map<char, Command*> command_list;

	// 현재 메뉴에서 사용 가능한 명령어를 담은 목록입니다.
	set<char> command_availability;

	// 입력 받은 명령어를 작동시킵니다.
	void ProcessCommand(char character);
	// 명령어가 목록에 존재하는지를 반환합니다.
	bool HasCommand(char character);
	// 명령어가 현재 사용 가능한지를 반환합니다.
	bool CanCommand(char character);

	// 종료 전 메모리를 해제합니다.
	void CleanUp();
public:
	MenuIO();
	~MenuIO();

	// 명령어를 목록에 추가합니다.
	void AppendCommand(char character, Command* command); 
	// 명령어를 목록에서 제거합니다.
	void RemoveCommand(char character); 
	// 현재 메뉴에서 사용 가능한 명령어를 담은 목록을 수정합니다.
	void ToggleCommand(set<char> command_availability);

	/*
	- freeze한 시점에 화면에 출력된 텍스트(printed)를 임시로 저장합니다.
	주어진 body 함수를 실행하고 rollback 함수를 인자로 제공합니다.

	- body내에서 rollback 함수를 실행하면 이전에 저장된 텍스트를 불러와서 덮어씁니다.
	freeze한 이후의 출력은 무효화되며 이전 상태로 돌아갑니다.
	*/
	void freeze(function<void(action)> body);

	// 이전에 저장된 텍스트를 비웁니다.
	void flush();

	// 화면에 출력된 모든 텍스트를 지웁니다.
	void clear();

	// 화면에 텍스트를 출력합니다.
	void print(string text);

	// 화면에 출력된 모든 텍스트를 지우고, 이전에 저장된 텍스트를 화면에 출력합니다.
	void reprint();

	// 입력을 받습니다.
	string input();

	// 필요한 정보를 출력하고 입력을 받습니다.
	string input(string msg_info, string pre = "입력");

	// Parser를 이용해 입력을 받고, 올바른 형식이면 파싱하여 반환합니다.
	any input(Parser* parser);

	// "아무키나 입력하세요..."를 출력하고 대기합니다.
	void pause();

	// 화면에 구분선을 그립니다.
	void print_line(bool bold = true, int length = 40);

	// 화면에 현재 메뉴에서 사용 가능한 명령어를 담은 목록을 출력합니다.
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
	// 매뉴화면들의 목록입니다.
	map<string, Menu*> menu_list; 

	// 다음에 실행할 메뉴화면입니다.
	Menu* next_menu;
	// 현재 실행중인 메뉴화면입니다.
	Menu* current_menu;

	// 종료 전 메모리를 해제합니다.
	void CleanUp(); 

	// 다음에 실행할 메뉴화면을 설정합니다.
	void SetNextMenu(string menu_name);
public:   
	MenuManager();
	~MenuManager();

	// 화면의 입출력을 담당하는 메뉴 매니저 도구입니다.
	MenuIO IO;

	// 메뉴화면 처리를 시작합니다.
	void Start(string start_menu_name);

	// 메뉴화면의 상태를 설정합니다.
	void SetMenu(string title, string menu_name, string prev_menu_name, set<char> command_availability);

	// 메뉴화면을 목록에 추가합니다.
	void AppendMenu(string menu_name, Menu* menu);
	// 메뉴화면을 목록에서 제거합니다.
	void RemoveMenu(string menu_name);

	// 현재 실행중인 메뉴화면을 반환합니다.
	Menu* GetCurrentMenu();

	// - recursive = true : 실행중인 메뉴화면의 함수 안에서 재귀적으로 다른 메뉴화면의 함수를 실행시킵니다.
	// 실행이 완료되면 이전에 저장된 텍스트를 불러와서 덮어쓰고 이전 상태로 돌아갑니다.
	// - recursive = false : 즉시 실행된 메뉴화면의 함수에서 벗어나 다른 메뉴화면의 함수를 실행시킵니다.
	void RunMenu(string menu_name, bool recursive = false);
};