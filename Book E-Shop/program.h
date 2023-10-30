#pragma once

#include "DataManager.h"
#include "MenuManager.h"
#include "ShopManager.h" 

#include <format>

class Program
{
public:
	static DataManager data_manager;
	static MenuManager menu_manager;
	static ShopManager shop_manager; 

	static void Run();

	static void LoadCSV();
	static void SaveCSV();
	static void SetCommand();
	static void SetParser();
	static void SetMenu();

	class Template {
	protected:
		string name;

		bool command_toggled = false;
 
		unordered_set<char> command_availability;
	public:  
		// 명령어의 사용 가능 여부를 설정합니다.
		void ToggleCommand(unordered_set<char> command_availability) {
			this->command_availability = command_availability;

			command_toggled = true;
		}

		// 명령어의 사용 가능 여부를 설정합니다.
		template<typename... TP, typename = char>
		void ToggleCommand(TP... command) {
			ToggleCommand({ command... });
		}

		void SetName(string name) {
			this->name = name;
		}

		virtual void Apply(MenuCode menu_code) = 0;
	};

	class TemplateMenuSelection : public Template {
	private:
		vector<pair<string, action>> menu; 
	public:   
		void SubMenu(string text, action func) {
			menu.push_back(make_pair(text, func));
		}

		void Apply(MenuCode menu_code) override {
			auto func = [&, k = *this](MenuIO& IO) {
				TemplateMenuSelection setting = k;

				if (setting.command_toggled) menu_manager.ToggleCommand(setting.command_availability);
				menu_manager.PrintCommand();
				IO.print_line(); 
				IO.print_aligned_center(format("[ {} ]", setting.name));

				for (int i = 0; i < setting.menu.size(); i++) {
					IO.print_aligned_center(format("({0}) {1}", i + 1, setting.menu[i].first));
				}

				IO.print_line();

				int index;

				auto checkpoint = IO.checkpoint();

				while (true) {
					index = IO.input<int>(data_manager.GetParser("MENU_MENUSELECTION"));

					if (1 <= index && index <= setting.menu.size()) {
						(setting.menu[index - 1].second)();
					}
					else {
						IO.print("메뉴에 표시된 번호 중 하나를 고르세요.\n");
						IO.pause();

						IO.rollback(checkpoint);
					}
				}
			};

			menu_manager.AppendMenu(menu_code, new Menu(func));
		}
	};

	template <typename T>
	class TemplateTable : public Template {
	private:  
		unordered_map<char, pair<string, action>> menu; 
	public:  
		int max_count = 10;  

		MenuCode next_menu_code;

		function<string(void)> header_func;
		function<string(T& target)> show_func;

		function<void(MenuIO&, MenuCode, T)> process_func;
		 
		void SubMenu(char command, string text, action func) {
			menu.insert({ command, make_pair(text, func) });
		}

		void Apply(MenuCode menu_code) override {
			auto func = [&, k = *this](MenuIO& IO, vector<T>& v) {
				TemplateTable<T> setting = k;

				if (setting.command_toggled) menu_manager.ToggleCommand(setting.command_availability);
				menu_manager.PrintCommand();
				IO.print_line();
				IO.print_aligned_center(format("[ {} ]", setting.name));

				int page = 0;

				// 사용 가능한 서브 메뉴 목록을 출력합니다.
				for (auto iter = setting.menu.begin(); iter != setting.menu.end(); iter++) {
					pair<string, action>& info = iter->second;

					IO.print_aligned_center(format("({0}) {1}", (char)toupper(iter->first), info.first));
				}

				if (setting.menu.size() != 0) {
					IO.print_line(false);
				}

				// 표의 헤더를 표시합니다.
				IO.print_aligned_center(format("    {0}", setting.header_func()));

				auto checkpoint = IO.checkpoint();

				while (true) {
					for (int i = 0; i < setting.max_count; i++) {
						int index = page * setting.max_count + i;

						if (index < v.size()) {
							// 각 요소에 해당하는 열을 출력합니다.
							IO.print_aligned_center(format("({0}) {1}", i, setting.show_func(v[index])));
						}
						else{
							// 인덱스가 마지막을 넘어가는 경우 줄바꿈만 합니다.
							IO.print("\n");
						}
					}

					IO.print_line(false);

					int max_page = ((int)v.size() / setting.max_count) + 1; 

					IO.print_aligned_center(format("{0} / {1}\n", page + 1, max_page));
					IO.print(format("{0:<{2}}{1:>{2}}\n", "< (B) 이전 페이지", "(N)다음 페이지 >", IO.width / 2));

					IO.print_line();

					char input = IO.input<char>(data_manager.GetParser("MENU_TABLE"));
					
					input = tolower(input);

					// 페이지 설정
					if (input == 'b' || input == 'n') {
						if (input == 'b') {
							// 페이지를 이전 페이지로 바꿉니다.
							page = (page + max_page - 1) % max_page;
						}
						else { 
							// 페이지를 다음 페이지로 바꿉니다.
							page = (page + max_page + 1) % max_page;
						}

						IO.rollback(checkpoint);
					}
					else {
						// 데이터 요소 목록에서 선택
						 if ('0' <= input && input <= '9') {
							int n = page * setting.max_count + (input - '0');

							if (n < v.size()) {
								if (setting.process_func != nullptr) {
									// 처리하는 함수가 존재하면 선택한 요소를 처리합니다.
									setting.process_func(IO, setting.next_menu_code, v[n]);

									break;
								}
								else {
									// 처리하는 함수가 존재하지 않으면 선택한 요소를 다음 메뉴로 넘깁니다.
									menu_manager.RunMenu(setting.next_menu_code, v[n]);
								}
							}
						 } 

						 if (setting.menu.find(input) != setting.menu.end()) {
							 (setting.menu[input].second)();
						 }

						IO.print("메뉴에 표시된 번호 또는 알파벳 중 하나를 고르세요.\n");
						IO.pause();

						IO.rollback(checkpoint);
					} 
				}   
			};

			menu_manager.AppendMenu(menu_code, new Menu<vector<T>&>(func));
		}
	};
};