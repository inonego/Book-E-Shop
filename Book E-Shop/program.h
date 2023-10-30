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
		// ��ɾ��� ��� ���� ���θ� �����մϴ�.
		void ToggleCommand(unordered_set<char> command_availability) {
			this->command_availability = command_availability;

			command_toggled = true;
		}

		// ��ɾ��� ��� ���� ���θ� �����մϴ�.
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
						IO.print("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ������.\n");
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

				// ��� ������ ���� �޴� ����� ����մϴ�.
				for (auto iter = setting.menu.begin(); iter != setting.menu.end(); iter++) {
					pair<string, action>& info = iter->second;

					IO.print_aligned_center(format("({0}) {1}", (char)toupper(iter->first), info.first));
				}

				if (setting.menu.size() != 0) {
					IO.print_line(false);
				}

				// ǥ�� ����� ǥ���մϴ�.
				IO.print_aligned_center(format("    {0}", setting.header_func()));

				auto checkpoint = IO.checkpoint();

				while (true) {
					for (int i = 0; i < setting.max_count; i++) {
						int index = page * setting.max_count + i;

						if (index < v.size()) {
							// �� ��ҿ� �ش��ϴ� ���� ����մϴ�.
							IO.print_aligned_center(format("({0}) {1}", i, setting.show_func(v[index])));
						}
						else{
							// �ε����� �������� �Ѿ�� ��� �ٹٲ޸� �մϴ�.
							IO.print("\n");
						}
					}

					IO.print_line(false);

					int max_page = ((int)v.size() / setting.max_count) + 1; 

					IO.print_aligned_center(format("{0} / {1}\n", page + 1, max_page));
					IO.print(format("{0:<{2}}{1:>{2}}\n", "< (B) ���� ������", "(N)���� ������ >", IO.width / 2));

					IO.print_line();

					char input = IO.input<char>(data_manager.GetParser("MENU_TABLE"));
					
					input = tolower(input);

					// ������ ����
					if (input == 'b' || input == 'n') {
						if (input == 'b') {
							// �������� ���� �������� �ٲߴϴ�.
							page = (page + max_page - 1) % max_page;
						}
						else { 
							// �������� ���� �������� �ٲߴϴ�.
							page = (page + max_page + 1) % max_page;
						}

						IO.rollback(checkpoint);
					}
					else {
						// ������ ��� ��Ͽ��� ����
						 if ('0' <= input && input <= '9') {
							int n = page * setting.max_count + (input - '0');

							if (n < v.size()) {
								if (setting.process_func != nullptr) {
									// ó���ϴ� �Լ��� �����ϸ� ������ ��Ҹ� ó���մϴ�.
									setting.process_func(IO, setting.next_menu_code, v[n]);

									break;
								}
								else {
									// ó���ϴ� �Լ��� �������� ������ ������ ��Ҹ� ���� �޴��� �ѱ�ϴ�.
									menu_manager.RunMenu(setting.next_menu_code, v[n]);
								}
							}
						 } 

						 if (setting.menu.find(input) != setting.menu.end()) {
							 (setting.menu[input].second)();
						 }

						IO.print("�޴��� ǥ�õ� ��ȣ �Ǵ� ���ĺ� �� �ϳ��� ������.\n");
						IO.pause();

						IO.rollback(checkpoint);
					} 
				}   
			};

			menu_manager.AppendMenu(menu_code, new Menu<vector<T>&>(func));
		}
	};
};