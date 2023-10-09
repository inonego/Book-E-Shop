#include "MenuManager.h"
#include "Menu.h"

#include "Util.h"

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
			(current_menu = next_menu)->Execute(IO);

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
void MenuManager::SetMenu(string title, string menu_name, string prev_menu_name, set<char> command_availability)
{
	if (menu_list.find(menu_name) != menu_list.end()) {
		menu_list[menu_name]->SetValue(title, prev_menu_name, command_availability);
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

Menu* MenuManager::GetCurrentMenu()
{
	return current_menu;
}

void MenuManager::RunMenu(string menu_name, bool recursive)
{
	// 재귀적으로 메뉴를 실행하면 실행중인 메뉴 함수 안에서 다른 메뉴의 함수가 실행됩니다.
	// 실행이 완료되면 이전 메뉴로 복귀하고 저장된 출력을 재출력합니다.
	if (recursive) {
		Menu* printed_menu = this->current_menu;

		IO.freeze([&](auto rollback) {   
			(current_menu = menu_list[menu_name])->Execute(IO);

			rollback();
		});

		this->current_menu = printed_menu;
	}
	//재귀적으로 메뉴를 실행하지 않으면 즉시 실행된 메뉴 함수에서 벗어나 다른 메뉴의 함수가 실행됩니다.
	else { 
		SetNextMenu(menu_name);

		throw Event::MENU;
	} 
}

// ################### MenuIO ###################

#include "Parser.h"

#include <iostream>  
#include <format>

string MenuIO::input_default()
{
	string input;

	getline(cin, input);

	printed += input + '\n';

	return input;
}

void MenuIO::ProcessCommand(char character)
{
	// 입력한 명령어가 목록에 있으면 명령어에 해당하는 기능 수행합니다.
	if (HasCommand(character) && CanCommand(character)) {
		command_list[character]->command_func();
	}
}

bool MenuIO::HasCommand(char character)
{
	return command_list.find(character) != command_list.end();
}

bool MenuIO::CanCommand(char character)
{
	return command_availability.contains(character);
}

void MenuIO::CleanUp()
{
	for (auto iter = command_list.begin(); iter != command_list.end(); ++iter) {
		delete iter->second;
	}

	command_list.clear();
}

MenuIO::MenuIO() {}

MenuIO::~MenuIO() {
	CleanUp();
}

void MenuIO::AppendCommand(char character, Command* command)
{
	if (command != nullptr) {
		command_list.insert({ character, command });
	}
} 

void MenuIO::RemoveCommand(char character)
{
	command_list.erase(character);

	command_availability.erase(character);
}

void MenuIO::ToggleCommand(set<char> command_availability)
{
	this->command_availability = command_availability;
}

void MenuIO::freeze(function<void(function<void(void)>)> body)
{
	set<char> availability = command_availability;

	string freezed = printed;

	function<void(void)> rollback = [&]()
	{
		command_availability = availability;

		printed = freezed;
		this->reprint();
	};

	body(rollback);
}

void MenuIO::flush()
{  
	printed = "";

	this->clear();
}

void MenuIO::clear()
{
	system("cls");
}

void MenuIO::print(string text)
{
	printed += text;

	cout << text;
}

void MenuIO::reprint()
{
	this->clear();

	cout << printed;
}

string MenuIO::input()
{
	string input;

	this->freeze([&](auto rollback) {
		while (true) { 
			input = this->input_default();

			// 명령어 처리
			if (!input.empty() && input[0] == ':') {
				input.erase(0, 1);

				// 입력한 명령어의 길이가 1이라면 명령어를 처리합니다.
				if (input.length() == 1) {
					char command = tolower(input[0]);

					if (HasCommand(command) && CanCommand(command)) {
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

	this->input_default();
}

void MenuIO::print_line(bool bold, int length)
{
	for (int i = 0; i < length; i++) {
		this->print(bold ? "=" : "-");
	} 

	this->print("\n");
} 

void MenuIO::print_available_command()
{
	vector<string> printed;

	for (auto character = command_availability.begin(); character != command_availability.end(); character++) {
		if (true) {
			Command* command = command_list[*character];

			printed.push_back(format("{0}[{1}]", command->name, (char)toupper(*character)));
		}
	}

	this->print(join(printed, "     ") + '\n');
}
