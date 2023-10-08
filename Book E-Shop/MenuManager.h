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

		// 화면에 출력된 텍스트를 저장합니다.
		string saved = "";

		// 명령어에 해당하는 함수의 목록입니다.
		map<char, function<void()>> command_func_list;		  

		// 입력을 받습니다.
		string input_raw(); 

		// 명령어에 해당하는 함수를 실행합니다.
		void ProcessCommand(char command);
		// 명령어가 존재하는지 반환합니다.
		bool HasCommand(char command);
	public:   
		MenuIO();
		~MenuIO();

		// 명령어에 해당하는 함수를 추가합니다.
		void AppendCommandFunc(char command, function<void()> command_func);
		// 명령어에 해당하는 함수를 제거합니다.
		void RemoveCommandFunc(char command);

		// 화면에 출력된 텍스트를 저장하고 함수를 실행합니다.
		// 함수 안에서 rollback 함수를 실행하면, 이전에 저장된 화면 출력으로 롤백하여 재출력합니다.
		void freeze(function<void(function<void(void)>)> body);

		void flush();

		// 화면의 모든 텍스트를 지웁니다.
		void clear();

		// 화면에 텍스트를 출력합니다.
		void print(string text);

		// 이전에 저장된 화면 출력으로 재출력합니다.
		void reprint();

		// 입력을 받고 명령어 처리를 합니다.
		string input();

		// 필요한 정보를 출력하고 입력을 받습니다.
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

	// 종료 전 메모리를 해제합니다.
	void CleanUp();

	void SetNextMenu(string menu_name);
public:   
	MenuManager();
	~MenuManager();

	MenuIO IO;

	void Start(string start_menu_name);

	// 메뉴화면을 추가합니다.
	void AppendMenu(string menu_name, Menu* menu);
	// 메뉴화면을 제거합니다.
	void RemoveMenu(string menu_name);

	// - 재귀적으로 메뉴를 실행하면 실행중인 메뉴 함수 안에서 다른 메뉴의 함수가 실행됩니다.
	// 실행이 완료되면 이전 메뉴로 복귀하고 저장된 출력을 재출력합니다.
	// - 재귀적으로 메뉴를 실행하지 않으면 즉시 실행된 메뉴 함수에서 벗어나 다른 메뉴의 함수가 실행됩니다.
	void RunMenu(string menu_name, bool recursive = false);

	// 메뉴화면 처리를 중지합니다.
	void Quit();
};

typedef MenuManager::MenuIO MenuIO;