#include <iostream>
 
#include "DataManager.h"
#include "MenuManager.h"
#include "Menu.h"

#include "implement.h" 

int main()
{
	menu_manager.IO.AppendCommand(Command::BACK, new MenuIO::Command("뒤로가기", []() {
		string prev_menu_name = menu_manager.GetCurrentMenu()->GetPrevMenuName();

		menu_manager.RunMenu(prev_menu_name, true);
	}));

	menu_manager.IO.AppendCommand(Command::LOGOUT, new MenuIO::Command("로그아웃", []() {
		menu_manager.RunMenu("logout", true);
	}));

	menu_manager.IO.AppendCommand(Command::QUIT, new MenuIO::Command("종료", []() {
		menu_manager.RunMenu("quit", true);
	}));
	 

	parser();

	menu();

	menu_value();
	
	menu_manager.Start("start");
}