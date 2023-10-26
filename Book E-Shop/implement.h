#pragma once 

#include <regex>
#include <format>
#include <vector>
#include "ShopManager.h" 

DataManager data_manager;
MenuManager menu_manager;
ShopManager shop_manager;

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
#pragma region *** product Parser ***
	data_manager.AppendParser("product_id", (new Parser())
		->set_label("고유번호")
		->set_regex(R"(^[0-9]{6}$)")
		->set_msg_error("숫자로 구성된 길이가 6인 문자열이어야 합니다")
	);
	data_manager.AppendParser("product_title", (new Parser())
		->set_label("제목")
	);
	data_manager.AppendParser("product_genre", (new Parser())
		->set_label("장르")
	);
	data_manager.AppendParser("product_price", (new Parser())
		->set_label("가격")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("숫자로 구성된 길이가 1 이상의 문자열이어야 합니다")
	);
	data_manager.AppendParser("product_count", (new Parser())
		->set_label("재고")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("숫자로 구성된 길이가 1 이상의 문자열이어야 합니다")
	);
	//csv 파싱
	auto product_raw = data_manager.ParseCSV("products.csv");
	for (size_t i = 0; i < product_raw.size();i++) {
		// 생성된 Product 객체를 shop_manager에 추가
		shop_manager.AddProduct(new Product(product_raw[i]));
	}


#pragma endregion


	data_manager.AppendParser("MENU_SELECT", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("메뉴에 표시된 번호 중 하나를 고르세요.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);
}

template <typename... TP, typename = pair<MenuCode, string>>
void MenuSelectionTemplate(MenuCode menu_code, TP... info) {
	
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

void menu() {
	MenuSelectionTemplate(MENU_START, make_pair(MENU_LOGIN, "계정 로그인"), make_pair(MENU_SIGNUP, "계정 회원가입"));
	MenuSelectionTemplate(MENU_ADMIN, make_pair(MENU_A_PRODUCT_LIST, "상품 관리 (등록/수정/제거, 재고관리)"), make_pair(MENU_A_ACCOUNT_LIST, "고객 관리 (계정, 주문)"));
	MenuSelectionTemplate(MENU_BUYER, make_pair(MENU_B_PRODUCT_LIST, "상품 목록"), make_pair(MENU_B_ACCOUNT_INFO, "고객 계정 정보"));

	// 계정 로그인 메뉴화면
	menu_manager.AppendMenu(MENU_LOGIN, new Menu(
		[&](MenuIO& IO) {
			//test
			menu_manager.RunMenu(MENU_A_PRODUCT_INFO, shop_manager.GetProdcutList()[123123]);

			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
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
		}
	));

	// 프로그램 종료 메뉴화면
	menu_manager.AppendMenu(MENU_QUIT, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand();
			menu_manager.PrintCommand();

			IO.print_line();
			IO.print("프로그램을 종료하시겠습니까? (y / n)\n");
			string input = IO.input();

			if (input == "y") {
				exit();
			}
		}
	));
	
	// 계정 로그아웃 메뉴화면
	menu_manager.AppendMenu(MENU_LOGOUT, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand();
			menu_manager.PrintCommand();

			IO.print_line();
			IO.print("계정에서 로그아웃 하시겠습니까? (y / n)\n");
			string input = IO.input();

			if (input == "y") {
				menu_manager.RunMenu(MENU_START);
			}
		}
	));

	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) {
			//all command allowed
			menu_manager.PrintCommand();

			IO.print_line();
			IO.print("[상품등록정보]\n"); 
		
			IO.print(format("고유번호 : {0}\n", target->id));
			IO.print(format("제목 : {0}\n", target->title));
			IO.print(format("장르 : {0}\n", target->genre));
			IO.print(format("가격 : {0}\n", target->price));
			IO.print(format("재고 : {0}\n", target->count));
		 
			IO.print("계정에서 로그아웃 하시겠습니까? (y / n)\n");
			string input = IO.input();

			if (input == "y") {
				menu_manager.RunMenu(MENU_START);
			}
		}
	));

		
}

// 엑셀로 관리됩니다.
void menu_value() {
	menu_manager[MENU_QUIT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGOUT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_START]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGIN]->set_prev_menu_code(MENU_START);

	menu_manager[MENU_A_PRODUCT_INFO]->set_prev_menu_code(MENU_A_PRODUCT_LIST);

	//menu_manager[MENU_SIGNUP]->set_prev_menu_code(MENU_START);
	/*
	menu_manager[MENU_ADMIN]->set_prev_menu_code(MENU_START);
	menu_manager[MENU_A_PRODUCT_LIST]->set_prev_menu_code(MENU_ADMIN);
	menu_manager[MENU_A_PRODUCT_SEARCH]->set_prev_menu_code(MENU_A_PRODUCT_LIST);
	menu_manager[MENU_A_PRODUCT_REGISTER]->set_prev_menu_code(MENU_A_PRODUCT_LIST);
	
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