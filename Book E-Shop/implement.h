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
	vector<string> product_raw = data_manager.ParseCSV("C:/Users/ILISNN/Documents/products.csv");
	for (size_t i = 0; i < product_raw.size();i++) {
		// 생성된 Product 객체를 shop_manager에 추가
		shop_manager.Add_Product(new Product(product_raw[i]));
	}


#pragma endregion

}

void menu() { 
	menu_manager.AppendMenu(MENU_START, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();
		IO.print_line();
		string number;

		auto checkpoint = IO.checkpoint();

		Product* product = shop_manager.Get_Prodcut_list().find(123456)->second;

		while (true) {
			number = IO.input("아무 숫자나 입력해보세요"); 

			menu_manager.RunMenu(MENU_A_PRODUCT_INFO, product);
		}
	}));

	menu_manager.AppendMenu(MENU_LOGIN, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand('z', 'q');
		menu_manager.PrintCommand();
		IO.print_line();
		string admin = "admin";


		IO.print(to_string(any_cast<int>(args[0])) + "\n");

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

	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO, new Menu([&](MenuIO& IO, vector<any> args) {

		//all command allowed
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print("[상품등록정보]\n");

		Product* targetP = any_cast<Product*>(args[0]);
		
		IO.print("고유번호 : "); IO.print(to_string(any_cast<int>(targetP->id))); IO.print("\n");
		IO.print("제목 : "); IO.print(any_cast<string>(targetP->title)); IO.print("\n");
		IO.print("장르 : "); IO.print(any_cast<string>(targetP->genre)); IO.print("\n");
		IO.print("가격 : "); IO.print(to_string(any_cast<int>(targetP->price))); IO.print("\n");
		IO.print("재고 : "); IO.print(to_string(any_cast<int>(targetP->count))); IO.print("\n");

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