#pragma once 

#include <regex>
#include <format>
#include <vector>

DataManager data_manager;
MenuManager menu_manager;

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

	data_manager.AppendParser("MENU_SELECT", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("메뉴에 표시된 번호 중 하나를 고르세요.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);
}

template <typename... TP, typename = pair<MenuCode, string>>
void MenuSelectionTemplate(MenuCode menu_code, TP... info) {
	
	menu_manager.AppendMenu(menu_code, new Menu([&, info...](MenuIO& IO, vector<any> args) {
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
	}));
}

void menu() {
	MenuSelectionTemplate(MENU_START, make_pair(MENU_LOGIN, "계정 로그인"), make_pair(MENU_SIGNUP, "계정 회원가입"));
	MenuSelectionTemplate(MENU_ADMIN, make_pair(MENU_A_PRODUCT_LIST, "상품 관리 (등록/수정/제거, 재고관리)"), make_pair(MENU_A_ACCOUNT_LIST, "고객 관리 (계정, 주문)"));
	MenuSelectionTemplate(MENU_BUYER, make_pair(MENU_B_PRODUCT_LIST, "상품 목록"), make_pair(MENU_B_ACCOUNT_INFO, "고객 계정 정보"));

	menu_manager.AppendMenu(MENU_LOGIN, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand('z', 'q');
		menu_manager.PrintCommand();
		IO.print_line();
		string admin = "admin";


		//IO.print(to_string(any_cast<int>(args[0])) + "\n");

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

	menu_manager.AppendMenu(MENU_QUIT, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print("프로그램을 종료하시겠습니까? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit();
		}
	}));

	menu_manager.AppendMenu(MENU_LOGOUT, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print("계정에서 로그아웃 하시겠습니까? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			menu_manager.RunMenu(MENU_START);
		}
	}));

}

// 엑셀로 관리됩니다.
void menu_value() {
	menu_manager[MENU_QUIT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGOUT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_START]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGIN]->set_prev_menu_code(MENU_START);
	//menu_manager[MENU_SIGNUP]->set_prev_menu_code(MENU_START);
	/*
	menu_manager[MENU_ADMIN]->set_prev_menu_code(MENU_START);
	menu_manager[MENU_A_PRODUCT_LIST]->set_prev_menu_code(MENU_ADMIN);
	menu_manager[MENU_A_PRODUCT_SEARCH]->set_prev_menu_code(MENU_A_PRODUCT_LIST);
	menu_manager[MENU_A_PRODUCT_REGISTER]->set_prev_menu_code(MENU_A_PRODUCT_LIST);
	menu_manager[MENU_A_PRODUCT_INFO]->set_prev_menu_code(MENU_A_PRODUCT_LIST);
	menu_manager[MENU_A_PRODUCT_INFO_M]->set_prev_menu_code(MENU_A_PRODUCT_INFO);
	menu_manager[MENU_A_PRODUCT_INFO_R]->set_prev_menu_code(MENU_A_PRODUCT_INFO);
	menu_manager[MENU_A_ACCOUNT_LIST]->set_prev_menu_code(MENU_ADMIN);
	menu_manager[MENU_A_ACCOUNT_SEARCH]->set_prev_menu_code(MENU_A_ACCOUNT_LIST);
	menu_manager[MENU_A_ACCOUNT_INFO]->set_prev_menu_code(MENU_A_ACCOUNT_LIST);
	menu_manager[MENU_A_ACCOUNT_INFO_M]->set_prev_menu_code(MENU_A_ACCOUNT_INFO);
	menu_manager[MENU_A_INVOICE_LIST]->set_prev_menu_code(MENU_A_ACCOUNT_INFO);
	menu_manager[MENU_A_INVOICE_INFO]->set_prev_menu_code(MENU_A_INVOICE_LIST);
	menu_manager[MENU_BUYER]->set_prev_menu_code(MENU_START);
	menu_manager[MENU_B_PRODUCT_LIST]->set_prev_menu_code(MENU_BUYER);
	menu_manager[MENU_B_PRODUCT_SEARCH]->set_prev_menu_code(MENU_B_PRODUCT_LIST);
	menu_manager[MENU_B_PRODUCT_INFO]->set_prev_menu_code(MENU_B_PRODUCT_LIST);
	menu_manager[MENU_B_PRODUCT_BUY]->set_prev_menu_code(MENU_B_PRODUCT_INFO);
	menu_manager[MENU_B_ACCOUNT_INFO]->set_prev_menu_code(MENU_BUYER);
	menu_manager[MENU_B_ACCOUNT_INFO_M]->set_prev_menu_code(MENU_B_ACCOUNT_INFO);
	menu_manager[MENU_B_INVOICE_LIST]->set_prev_menu_code(MENU_B_ACCOUNT_INFO);
	menu_manager[MENU_B_INVOICE_INFO]->set_prev_menu_code(MENU_B_INVOICE_LIST);
	*/
}