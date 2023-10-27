#include "MenuManager.h"
#include "Util.h"

#include <format>
#include <iostream>

// ################### Menu Manager ################### 

IMenu* MenuManager::SetCurrentMenu(MenuCode menu_code)
{
	if (menu_list.find(menu_code) != menu_list.end()) { 
		return current_menu = menu_list[menu_code];
	}
	else {
		cerr << "오류 : 메뉴가 존재하지 않습니다." << '\n';

		exit(200);
	}
}

MenuManager::MenuManager()
	:
	IO(MenuIO([&](MenuIO& IO, string input) -> bool {
		// 명령어 처리
		if (!input.empty() && input[0] == ':') {
			input.erase(0, 1);

			// 입력한 명령어의 길이가 1이라면 명령어를 처리합니다.
			if (input.length() == 1) {
				char command = tolower(input[0]);

				if (HasCommand(command) && CanCommand(command)) {
					ProcessCommand(command);
				}
				else {
					// 명령어가 존재하지 않으면 오류 메시지를 출력하고 대기함
					IO.print("명령어가 존재하지 않습니다.\n");

					IO.pause();
				}
			}
			else {
				// 입력 형식 오류 시 오류 메시지를 출력하고 대기함
				IO.print("명령어는 맨 앞에 세미콜론(:)을 적고 그 뒤에 영문자 하나만 입력하세요.\n");

				IO.pause();
			}

			return false;
		}

		return true;
	}))
{}
 
MenuManager::~MenuManager()
{
	for (auto iter = menu_list.begin(); iter != menu_list.end(); ++iter) {
		delete iter->second;
	}

	menu_list.clear();
}

void MenuManager::Start(MenuCode menu_code)
{ 
	RunFunc run_func = [&, menu_code](MenuIO& IO) { SetCurrentMenu(menu_code)->Run(IO); };
	
	while (true) {
		IO::Buffer* buffer = new IO::Buffer();

		this->IO.set_buffer(buffer);

		ToggleCommand('z', 'l', 'q');

		try { 
			run_func(IO);

			current_menu = nullptr;
		}
		catch (RunFunc menu_func) { 
			run_func = menu_func;
		}

		delete buffer;

		if (current_menu == nullptr) return;
	}
}

void MenuManager::AppendMenu(MenuCode menu_code, IMenu* menu)
{
	menu_list.insert({ menu_code, menu });
}

void MenuManager::RemoveMenu(MenuCode menu_code)
{
	menu_list.erase(menu_code);
}

void MenuManager::AppendCommand(char character, Command* command)
{
	command_list.insert({ character, command });
}

void MenuManager::RemoveCommand(char character)
{
	command_list.erase(character);
}
 
bool MenuManager::HasCommand(char character)
{
	return command_list.find(character) != command_list.end();
}

bool MenuManager::CanCommand(char character)
{ 
	return command_availability.contains(character);
}

void MenuManager::ProcessCommand(char character)
{
	// 입력한 명령어가 목록에 있으면 명령어에 해당하는 기능 수행합니다.
	if (HasCommand(character) && CanCommand(character)) {
		command_list[character]->command_func();
	}
}
void MenuManager::PrintCommand()
{
	vector<string> printed;

	for (auto character = command_availability.begin(); character != command_availability.end(); character++) {
		if (true) {
			Command* command = command_list[*character];

			printed.push_back(format("{0}[{1}]", command->name, (char)toupper(*character)));
		}
	}

	IO.print(join(printed, "     ") + '\n');
}

IMenu* MenuManager::GetCurrentMenu()
{
	return current_menu;
}

IMenu* MenuManager::operator[](MenuCode menu_code)
{
	return menu_list[menu_code];
}