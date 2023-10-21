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
		// 명령어 표시를 위한 이름입니다.
		string name;

		// 명령어가 작동될 때 실행되는 함수입니다.
		action command_func = nullptr;

		Command(string name, action command_func) : name(name), command_func(command_func) {}
	};
private:   
	// 매뉴화면들의 목록입니다.
	map<MenuCode, Menu*> menu_list;

	// 명령어들의 목록입니다.
	unordered_map<char, Command*> command_list;
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

	// 실행중인 메뉴에서 벗어나 새로운 메뉴화면을 실행합니다.
	template<typename... TP> 
	void RunMenu(MenuCode menu_code, TP... args);

	// 실행중인 메뉴에서 재귀적으로 새로운 메뉴화면을 실행합니다.
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
