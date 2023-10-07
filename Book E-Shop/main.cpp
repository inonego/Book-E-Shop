#include <iostream>

#include "DataManager.h"
#include "MenuManager.h"

int main()
{
	DataManager data_manager;

	// Parser 예시
	data_manager.AppendParser("account_id", (new Parser())
		->set_regex("")
		->set_msg_info("")
		->set_msg_error("")
		->set_parse(nullptr)
	);

	MenuManager menu_manager;

	menu_manager.AppendMenu("start_menu",
	new Menu([&](void) -> void {
			printf("예시");
	}));
}