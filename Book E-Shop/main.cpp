#include <iostream>
 
#include "DataManager.h"
#include "MenuManager.h"
#include "Menu.h"

#include <regex>
#include <format>

MenuManager menu_manager;
DataManager data_manager;


void parser() { 
	// Parser 예시

	data_manager.AppendParser("account_id", (new Parser())
		->set_pre("아이디")
		->set_regex(R"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("아이디는 숫자, 영문자로 구성된 길이가 4 이상 12 이하의 문자열이어야 합니다!")
	);

	data_manager.AppendParser("account_password", (new Parser())
		->set_pre("비밀번호")
		->set_regex(R"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("비밀번호는 숫자, 영문자, 특수 문자(!@#$%^&*_)로 구성된 길이가 8 이상 16 이하의 문자열이어야 합니다!")
	);
}

void menu() {
	menu_manager.AppendMenu("시작 메뉴화면", new Menu([&](MenuIO IO) {
		IO.print("Hello World!\n");

		string admin = "admin";

		string id;

		IO.input();

		IO.freeze([&](auto rollback) { 
			while (true) {
				id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));

				if (id == admin) {
					IO.print("아이디는 'admin'일 수 없습니다!\n");
					IO.pause();
					rollback();
				}
				else {
					break;
				}
			}
		});
	
		string password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

		IO.print(format("입력하신 아이디는 {0}, 비밀번호는 {1}입니다.\n", id, password));
		IO.pause();
		}));
}

int main()
{
	menu_manager.AppendCommandFunc('z', []() {

		});

	menu_manager.AppendCommandFunc('l', []() {

		});

	menu_manager.AppendCommandFunc('q', []() {

		});

	parser();

	menu();

	menu_manager.Start("시작 메뉴화면");
}