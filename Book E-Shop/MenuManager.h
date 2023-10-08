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

		// 화면에 출력된 텍스트를 저장합니다.
		string saved = ""; 
	public: 
		// 화면에 출력된 텍스트를 저장하고 함수를 실행합니다.
		// 함수 안에서 rollback 함수를 실행하면, 이전에 저장된 화면 출력으로 롤백하여 재출력합니다.
		void freeze(function<void(function<void(void)>)> body);

		// 화면의 모든 텍스트를 지웁니다.
		void clear();

		// 화면에 텍스트를 출력합니다.
		void print(string text);

		// 이전에 저장된 화면 출력으로 재출력합니다.
		void reprint();

		// 입력을 받습니다.
		string input(); 
		string input(string msg_info, string pre = "입력"); 

		// Parser를 이용해 입력을 받고, 올바른 형식이면 파싱합니다.
		any input(Parser* parser);

		// "아무키나 입력하세요..."를 출력하고 대기합니다.
		void pause();
	};
private: 
	// 매뉴들의 목록입니다.
	map<string, Menu*> menu_list;

	Menu* next_menu;

	MenuIO IO;

	// 명령어에 해당하는 함수의 목록입니다.
	map<char, function<void()>> command_func_list;

	void CleanUp(); 
public: 

	MenuManager();
	~MenuManager();
	
	// 명령어에 해당하는 함수를 추가합니다.
	void AppendCommandFunc(char command, function<void()> command_func);
	// 명령어에 해당하는 함수를 제거합니다.
	void RemoveCommandFunc(char command);

	void Start(string start_menu_name);

	// 메뉴화면을 추가합니다.
	void AppendMenu(string menu_name, Menu* menu);
	// 메뉴화면을 제거합니다.
	void RemoveMenu(string menu_name);

	// 현재 메뉴화면을 설정합니다.
	void CallMenu(string menu_name);
	 
	// 메뉴화면 처리를 중지합니다.
	void Quit();
};

typedef MenuManager::MenuIO MenuIO;