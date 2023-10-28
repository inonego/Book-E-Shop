#pragma once

#include "DataManager.h"
#include "MenuManager.h"
#include "ShopManager.h" 

class Program
{
private:
	DataManager data_manager;
	MenuManager menu_manager;
	ShopManager shop_manager;
public:
	Program();

	void Run();

	void LoadCSV();
	void SetCommand();
	void SetParser();
	void SetPrevMenuCode();
	void SetMenu();

	template <typename... TP, typename = pair<MenuCode, string>>
	void TemplateMenuSelection(MenuCode menu_code, TP ...info) {
		menu_manager.AppendMenu(menu_code, new Menu(
			[&, info...](MenuIO& IO) {
				menu_manager.PrintCommand();
				IO.print_line();

				vector<pair<MenuCode, string>> v{ info... };

				for (int i = 0; i < v.size(); i++) {
					IO.print(format("({0}) {1}\n", i + 1, v[i].second));
				}

				IO.print_line();

				int index;

				auto checkpoint = IO.checkpoint();

				while (true) {
					index = any_cast<int>(IO.input(data_manager.GetParser("MENU_SELECT")));

					if (1 <= index && index <= v.size()) {
						menu_manager.RunMenu(v[index - 1].first);
					}
					else {
						IO.print("메뉴에 표시된 번호 중 하나를 고르세요.\n");
						IO.pause();

						IO.rollback(checkpoint);
					}
				}
			}
		));
	}
};