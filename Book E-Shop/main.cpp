#include <iostream>
 
#include "DataManager.h"
#include "MenuManager.h"

#include "implement.h" 

int main()
{
	menu_manager.AppendCommand('z', new MenuManager::Command("뒤로가기", []() {
		auto prev_menu_code = menu_manager.GetCurrentMenu()->get_prev_menu_code();

		if (prev_menu_code != MENU_NONE) { 
			menu_manager.RunMenu(prev_menu_code);
		}
	}));

	menu_manager.AppendCommand('l', new MenuManager::Command("로그아웃", []() {
		menu_manager.RunRecursiveMenu(MENU_LOGOUT);
	}));

	menu_manager.AppendCommand('q', new MenuManager::Command("종료", []() {
		menu_manager.RunRecursiveMenu(MENU_QUIT);
	}));


	parser();

	menu();

	menu_value();
	
	menu_manager.Start(MENU_START);
}