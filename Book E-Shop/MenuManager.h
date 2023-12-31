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
		// 명령어 표시를 위한 이름입니다.
		string name;

		// 명령어가 작동될 때 실행되는 함수입니다.
		action command_func = nullptr;

		Command(string name, action command_func) : name(name), command_func(command_func) {}
	};
private:   
	// 매뉴화면들의 목록입니다.
	map<MenuCode, Menu*> menu_list;

	Menu* current_menu;

	// 명령어들의 목록입니다.
	unordered_map<char, Command*> command_list;
	unordered_set<char> command_availability;
public:   
	MenuManager();
	~MenuManager();

	// 화면의 입출력을 담당하는 메뉴 매니저 도구입니다.
	MenuIO IO;

	// 메뉴화면 처리를 시작합니다.
	void Start(MenuCode menu_code);

	// 메뉴화면을 목록에 추가합니다.
	void AppendMenu(MenuCode menu_code, Menu* menu);
	// 메뉴화면을 목록에서 제거합니다.
	void RemoveMenu(MenuCode menu_code);

	// 명령어를 목록에 추가합니다.
	void AppendCommand(char character, Command* command);
	// 명령어를 목록에서 제거합니다.
	void RemoveCommand(char character);

	// 명령어가 목록에 존재하는지를 반환합니다.
	bool HasCommand(char character);
	// 명령어가 현재 사용 가능한지를 반환합니다.
	bool CanCommand(char character);
	// 입력 받은 명령어를 작동시킵니다.
	void ProcessCommand(char character);
	// 명령어의 사용 가능 여부를 설정합니다.
	template<typename... TP, typename = char>
	void ToggleCommand(TP... command) {
		command_availability = unordered_set<char>{ command... };
	}

	// 현재 사용 가능한 명령어들을 출력합니다.
	void PrintCommand();

	Menu* GetCurrentMenu();

	// 실행중인 메뉴에서 벗어나 새로운 메뉴화면을 실행합니다.
	template<typename... TP> 
	void RunMenu(MenuCode menu_code, TP... args);

	// 실행중인 메뉴에서 재귀적으로 새로운 메뉴화면을 실행합니다.
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

	// 현재 상태 저장 및 다음 상태 설정
	auto command_availability = this->command_availability;

	auto current_menu = this->current_menu;
	 
	this->IO.set_buffer(next_buffer);

	ToggleCommand('z', 'l', 'q');

	// 메뉴 실행
	vector<any> v{ args... };

	menu_list[menu_code]->Run(IO, v);

	// 이전 상태로 복구
	this->command_availability = command_availability;

	this->current_menu = current_menu;

	this->IO.set_buffer(prev_buffer);

	delete next_buffer;
}
