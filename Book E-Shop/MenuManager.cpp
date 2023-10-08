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

			// ��ɾ� ó��
			if (!input.empty() && input[0] == ':') {
				input.erase(0, 1);

				// �Է��� ��ɾ��� ���̰� 1�̶�� ��ɾ ó���մϴ�.
				if (input.length() == 1) {
					char command = tolower(input[0]);
					/*
					// �Է��� ��ɾ ��Ͽ� ������ ��ɾ �ش��ϴ� ��� �����մϴ�.
					if (command_func_list.find(command) != command_func_list.end()) {
						command_func_list[command]();
					}
					*/
					return;
				}
				else {
					this->print("��ɾ�� �� �տ� �����ݷ�(:)�� ���� �� �ڿ� ������ �ϳ��� �Է��ϼ���.\n");

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
	this->print("�ƹ�Ű�� �Է��ϼ���... \n");
	this->input();
}