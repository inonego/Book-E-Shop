#include "MenuManager.h"
#include "Menu.h"

// ################### Menu Manager ###################

void MenuManager::CleanUp()
{ 
	for (auto iter = menu_list.begin(); iter != menu_list.end(); ++iter) {
		delete iter->second;
	}

	menu_list.clear();

	command_func_list.clear();
}

MenuManager::MenuManager() : IO() {}
 
MenuManager::~MenuManager()
{
	CleanUp();
}

void MenuManager::Start(string start_menu_name)
{  
	try {
		CallMenu(start_menu_name);
	}
	catch (int e) {

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

void MenuManager::AppendCommandFunc(char command, function<void()> command_func)
{
	if (command_func != nullptr) {
		command_func_list.insert({ command, command_func });
	}
}

void MenuManager::RemoveCommandFunc(char command)
{
	command_func_list.erase(command);
}

void MenuManager::CallMenu(string menu_name)
{
	if (menu_list.find(menu_name) != menu_list.end()) {
		menu_list[menu_name]->Execute(IO);
	}
	else {
		throw;
	}
}
 
void MenuManager::Quit()
{
	throw;
}

// ################### MenuIO ###################

#include "Parser.h"

#include <iostream>  

void MenuManager::MenuIO::freeze(function<void(function<void(void)>)> body)
{
	string freezed = saved;

	function<void(void)> rollback = [&]()
	{
		saved = freezed;
		this->reprint();
	};

	body(rollback);
}

void MenuManager::MenuIO::clear()
{
	system("cls");
}

void MenuManager::MenuIO::print(string text)
{
	saved += text;

	cout << text;
}

void MenuManager::MenuIO::reprint()
{
	this->clear();

	cout << saved;
}

string MenuManager::MenuIO::input()
{
	string input;

	this->freeze([&](auto rollback) {
		while (true) { 
			getline(cin, input);

			saved += input + '\n';

			// 명령어 처리
			if (!input.empty() && input[0] == ':') {
				input.erase(0, 1);

				// 입력한 명령어의 길이가 1이라면 명령어를 처리합니다.
				if (input.length() == 1) {
					char command = tolower(input[0]);
					/*
					// 입력한 명령어가 목록에 있으면 명령어에 해당하는 기능 수행합니다.
					if (command_func_list.find(command) != command_func_list.end()) {
						command_func_list[command]();
					}
					*/
					return;
				}
				else {
					this->print("명령어는 맨 앞에 세미콜론(:)을 적고 그 뒤에 영문자 하나만 입력하세요.\n");

					this->pause();

					rollback();
				}
			}
			else {
				break;
			}
		} 
	});

	return input;
}

string MenuManager::MenuIO::input(string msg_info, string pre)
{
	if(msg_info != "") { 
		this->print(msg_info + '\n');
	}

	this->print(pre + " : ");

	return this->input();
}
 
any MenuManager::MenuIO::input(Parser* parser)
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
				this->print(parser->msg_error + '\n');

				this->pause(); 

				rollback();
			}
		} 
	});   

	return result;
}

void MenuManager::MenuIO::pause()
{
	this->print("아무키나 입력하세요... \n");
	this->input();
}