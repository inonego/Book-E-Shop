#include "MenuManager.h"

#include "Util.h"

// ################### Menu Manager ################### 

MenuManager::MenuManager() 
	:
	IO(MenuIO([&](MenuIO& IO, string input) -> bool {
		// ��ɾ� ó��
		if (!input.empty() && input[0] == ':') {
			input.erase(0, 1);

			// �Է��� ��ɾ��� ���̰� 1�̶�� ��ɾ ó���մϴ�.
			if (input.length() == 1) {
				char command = tolower(input[0]);

				if (HasCommand(command) && CanCommand(command)) {
					ProcessCommand(command);
				}
				else {
					// ��ɾ �������� ������ ���� �޽����� ����ϰ� �����
					IO.print("��ɾ �������� �ʽ��ϴ�.\n");

					IO.pause();
				}
			}
			else {
				// �Է� ���� ���� �� ���� �޽����� ����ϰ� �����
				IO.print("��ɾ�� �� �տ� �����ݷ�(:)�� ���� �� �ڿ� ������ �ϳ��� �Է��ϼ���.\n");

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
	Menu* current_menu = menu_list[menu_code];

	while (true) {
		IO::Buffer* buffer = new IO::Buffer();

		this->IO.set_buffer(buffer);

		try { 
			current_menu->Run(IO);

			current_menu = nullptr;
		}
		catch (MenuCode next_menu_code) {
			current_menu = menu_list[next_menu_code];
		}

		delete buffer;

		if (current_menu == nullptr) return;
	}
}

void MenuManager::AppendMenu(MenuCode menu_code, Menu* menu)
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
	return true;
	//return command_availability.contains(character);
}

void MenuManager::ProcessCommand(char character)
{
	// �Է��� ��ɾ ��Ͽ� ������ ��ɾ �ش��ϴ� ��� �����մϴ�.
	if (HasCommand(character) && CanCommand(character)) {
		command_list[character]->command_func();
	}
} 