#include <iostream>
 
#include "DataManager.h"
#include "MenuManager.h"

#include "implement.h" 

int main()
{
	menu_manager.AppendCommand('z', new MenuManager::Command("뒤로가기", []() {
		//string prev_menu_name = menu_manager.GetCurrentMenu()->GetPrevMenuName();

		//menu_manager.RunMenu(prev_menu_name, true);
	}));

	menu_manager.AppendCommand('l', new MenuManager::Command("로그아웃", []() {
		menu_manager.RunRecursiveMenu(MenuCode::LOGOUT);
	}));

	menu_manager.AppendCommand('q', new MenuManager::Command("종료", []() {
		menu_manager.RunRecursiveMenu(MenuCode::QUIT);
	}));


	parser();

	menu();

	menu_value();
	
	menu_manager.Start(MenuCode::START);
}