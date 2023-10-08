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
	// ��������� �޴��� �����ϸ� �������� �޴� �Լ� �ȿ��� �ٸ� �޴��� �Լ��� ����˴ϴ�.
	// ������ �Ϸ�Ǹ� ���� �޴��� �����ϰ� ����� ����� ������մϴ�.
	if (recursive) {
		IO.freeze([&](auto rollback) {   
			menu_list[menu_name]->Execute(IO);

			rollback();
		});
	}
	//��������� �޴��� �������� ������ ��� ����� �޴� �Լ����� ��� �ٸ� �޴��� �Լ��� ����˴ϴ�.
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
	// �Է��� ��ɾ ��Ͽ� ������ ��ɾ �ش��ϴ� ��� �����մϴ�.
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

			// ��ɾ� ó��
			if (!input.empty() && input[0] == ':') {
				input.erase(0, 1);

				// �Է��� ��ɾ��� ���̰� 1�̶�� ��ɾ ó���մϴ�.
				if (input.length() == 1) {
					char command = tolower(input[0]);

					if (HasCommand(command)) {
						ProcessCommand(command);
					}
					else{
						// ��ɾ �������� ������ ���� �޽����� ����ϰ� �����
						this->print("��ɾ �������� �ʽ��ϴ�.\n");

						this->pause();
					}
				}
				else {
					// �Է� ���� ���� �� ���� �޽����� ����ϰ� �����
					this->print("��ɾ�� �� �տ� �����ݷ�(:)�� ���� �� �ڿ� ������ �ϳ��� �Է��ϼ���.\n");

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
				// �Է� ���� ���� �� ���� �޽����� ����ϰ� ���, �ѹ���.
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
	this->print("�ƹ�Ű�� �Է��ϼ���... \n");

	this->input_raw();
}