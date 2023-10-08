#include "MenuManager.h"
#include "Menu.h"

// ################### Menu Manager ###################

void MenuManager::CleanUp()
{ 
	for (auto iter = menu_list.begin(); iter != menu_list.end(); ++iter) {
		delete iter->second;
	}

	menu_list.clear(); 
}

void MenuManager::SetNextMenu(string menu_name)
{ 
	if (menu_list.find(menu_name) != menu_list.end()) {
		next_menu = menu_list[menu_name];
	}
}

MenuManager::MenuManager() : IO() {}
 
MenuManager::~MenuManager()
{
	CleanUp();
}

void MenuManager::Start(string start_menu_name)
{
	SetNextMenu(start_menu_name);

	while (next_menu) {
		try {
			next_menu->Execute(IO);

			next_menu = nullptr;
		}
		catch (Event e) {
			switch (e) {
				case Event::MENU:

				break;
			} 
		}
	} 
}

void MenuManager::AppendMenu(string menu_name, Menu* menu)
{
	if (menu != nullptr) {
		menu_list.insert({ menu_name, menu });
	}
}

void MenuManager::RemoveMenu(string menu_name)
{
	menu_list.erase(menu_name);
}

void MenuManager::RunMenu(string menu_name, bool recursive)
{
	// 재귀적으로 메뉴를 실행하면 실행중인 메뉴 함수 안에서 다른 메뉴의 함수가 실행됩니다.
	// 실행이 완료되면 이전 메뉴로 복귀하고 저장된 출력을 재출력합니다.
	if (recursive) {
		IO.freeze([&](auto rollback) {   
			menu_list[menu_name]->Execute(IO);

			rollback();
		});
	}
	//재귀적으로 메뉴를 실행하지 않으면 즉시 실행된 메뉴 함수에서 벗어나 다른 메뉴의 함수가 실행됩니다.
	else {
		SetNextMenu(menu_name);

		throw Event::MENU;
	} 
}
 
void MenuManager::Quit()
{
	throw;
}

// ################### MenuIO ###################

#include "Parser.h"

#include <iostream>  

string MenuIO::input_raw()
{
	string input;

	getline(cin, input);

	saved += input + '\n';

	return input;
}

void MenuIO::ProcessCommand(char command)
{
	// 입력한 명령어가 목록에 있으면 명령어에 해당하는 기능 수행합니다.
	if (HasCommand(command)) {
		command_func_list[command]();
	}
}

bool MenuIO::HasCommand(char command)
{
	return command_func_list.find(command) != command_func_list.end();
}


MenuIO::MenuIO() {}

MenuIO::~MenuIO() {}

void MenuIO::AppendCommandFunc(char command, function<void()> command_func)
{
	if (command_func != nullptr) {
		command_func_list.insert({ command, command_func });
	}
}

void MenuIO::RemoveCommandFunc(char command)
{
	command_func_list.erase(command); 
}

void MenuIO::freeze(function<void(function<void(void)>)> body)
{
	string freezed = saved;

	function<void(void)> rollback = [&]()
	{
		saved = freezed;
		this->reprint();
	};

	body(rollback);
}

void MenuIO::flush()
{
	saved = "";
	this->reprint();
}

void MenuIO::clear()
{
	system("cls");
}

void MenuIO::print(string text)
{
	saved += text;

	cout << text;
}

void MenuIO::reprint()
{
	this->clear();

	cout << saved;
}

string MenuIO::input()
{
	string input;

	this->freeze([&](auto rollback) {
		while (true) { 
			input = this->input_raw();

			// 명령어 처리
			if (!input.empty() && input[0] == ':') {
				input.erase(0, 1);

				// 입력한 명령어의 길이가 1이라면 명령어를 처리합니다.
				if (input.length() == 1) {
					char command = tolower(input[0]);

					if (HasCommand(command)) {
						ProcessCommand(command);
					}
					else{
						// 명령어가 존재하지 않으면 오류 메시지를 출력하고 대기함
						this->print("명령어가 존재하지 않습니다.\n");

						this->pause();
					}
				}
				else {
					// 입력 형식 오류 시 오류 메시지를 출력하고 대기함
					this->print("명령어는 맨 앞에 세미콜론(:)을 적고 그 뒤에 영문자 하나만 입력하세요.\n");

					this->pause();
				} 

				rollback();
			}
			else {
				break;
			}
		} 
	});

	return input;
}

string MenuIO::input(string msg_info, string pre)
{
	if(msg_info != "") { 
		this->print(msg_info + '\n');
	}

	this->print(pre + " : ");

	return this->input();
}
 
any MenuIO::input(Parser* parser)
{
	any result;

	this->freeze([&](auto rollback) {
		while (true) {
			string input = this->input(parser->msg_info, parser->pre);

			if (parser->Check(input)) {
				result = parser->Parse(input);

				break;
			}
			else {
				// 입력 형식 오류 시 오류 메시지를 출력하고 대기, 롤백함.
				this->print(parser->msg_error + '\n');

				this->pause(); 

				rollback();
			}
		} 
	});   

	return result;
}

void MenuIO::pause()
{
	this->print("아무키나 입력하세요... \n");

	this->input_raw();
}