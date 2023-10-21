#pragma once 

#include <regex>
#include <format>
#include <vector>

DataManager data_manager;
MenuManager menu_manager;
/*
enum Command
{
	BACK = 'z',
	LOGOUT = 'l',
	QUIT = 'q'
};
*/
void init() {

}

void exit() {
	exit(0);
}

void parser() {

	// Parser 예시
	data_manager.AppendParser("account_id", (new Parser())
		->set_label("아이디")
		->set_regex(R"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("아이디는 숫자, 영문자로 구성된 길이가 4 이상 12 이하의 문자열이어야 합니다!")
	);

	data_manager.AppendParser("account_password", (new Parser())
		->set_label("비밀번호")
		->set_regex(R"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("비밀번호는 숫자, 영문자, 특수 문자(!@#$%^&*_)로 구성된 길이가 8 이상 16 이하의 문자열이어야 합니다!")
	);

	data_manager.AppendParser("test", (new Parser())
		->set_label("나이")
		->set_msg_info("나이를 입력하세요!")
	);
}

void menu() { 
	menu_manager.AppendMenu(MenuCode::START, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command(); 
		IO.print_line();
		string number;

		auto checkpoint = IO.checkpoint();

		while (true) {
			number = IO.input("1을 입력하세요");


			if (number == "1") {
				menu_manager.RunMenu(MenuCode::LOGIN);

				IO.pause();

				IO.rollback(checkpoint);
			}
			else {
				break;
			}
		}
	}));

	menu_manager.AppendMenu(MenuCode::LOGIN, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command(); 
		IO.print_line();
		string admin = "admin";

		string id;
		   
		auto checkpoint = IO.checkpoint();

		while (true) {
			id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));

			if (id == admin) {
				IO.print("관리자 아이디 ‘admin’은 아이디로 사용할 수 없습니다.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
			else {
				break;
			}
		}

		string password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

		IO.print(format("입력하신 아이디는 {0}, 비밀번호는 {1}입니다.\n", id, password));
		IO.pause();
	}));

	menu_manager.AppendMenu(MenuCode::QUIT, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command();

		IO.print_line();
		IO.print("프로그램을 종료하시겠습니까? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit();
		}
	}));

	menu_manager.AppendMenu(MenuCode::LOGOUT, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command();

		IO.print_line();
		IO.print("계정에서 로그아웃 하시겠습니까? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			menu_manager.RunMenu(MenuCode::START);
		}
	}));

}

// 엑셀로 관리됩니다.
void menu_value() {

}