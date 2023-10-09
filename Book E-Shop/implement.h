#pragma once 

#include <regex>
#include <format>
 
DataManager data_manager;
MenuManager menu_manager;

enum Command
{
	BACK = 'z',
	LOGOUT = 'l',
	QUIT = 'q'
};

void init() {

}

void exit() {
	exit(0);
}

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



	menu_manager.AppendMenu("start", new Menu([&](MenuIO IO) {
		IO.print_available_command(); 
		IO.print_line();

		string number;

		IO.freeze([&](auto rollback) {
			while (true) {
				number = IO.input();

				if (number == "1") {
					menu_manager.RunMenu("login");
					IO.pause();
					rollback();
				}
				else {
					break;
				}
			}
		});
	}));

	menu_manager.AppendMenu("login", new Menu([&](MenuIO IO) {
		IO.print_available_command(); 
		IO.print_line();
		string admin = "admin";

		string id;
		  
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

	menu_manager.AppendMenu("quit", new Menu([&](MenuIO IO) {
		IO.print_available_command();

		IO.print_line();
		IO.print("프로그램을 종료하시겠습니까? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit();
		}
	}));

	menu_manager.AppendMenu("logout", new Menu([&](MenuIO IO) {
		IO.print_available_command();

		IO.print_line();
		IO.print("계정에서 로그아웃 하시겠습니까? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			menu_manager.RunMenu("시작 메뉴화면");
		}
	}));

}

// 엑셀로 관리됩니다.
void menu_value() {
	menu_manager.SetMenu("프로그램 종료", "quit", "", {});
	menu_manager.SetMenu("계정 로그아웃", "logout", "", {});
	menu_manager.SetMenu("시작 메뉴화면", "start", "", { 'q' });
	menu_manager.SetMenu("계정 로그인", "login", "start", { 'z', 'q' });
	menu_manager.SetMenu("계정 회원가입", "signup", "start", { 'z', 'q' });
	menu_manager.SetMenu("관리자 메뉴화면", "admin", "start", { 'l', 'q' });
	menu_manager.SetMenu("상품 목록", "a_product_list", "admin", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 검색 및 장르 선택", "a_product_search", "a_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 신규 등록", "a_product_register", "a_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 등록 정보", "a_product_info", "a_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 등록 정보 수정", "a_product_info_m", "a_product_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 등록 정보 제거", "a_product_info_r", "a_product_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("고객 목록", "a_account_list", "admin", { 'z', 'l', 'q' });
	menu_manager.SetMenu("고객 검색", "a_account_search", "a_account_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("고객 계정 정보", "a_account_info", "a_account_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("고객 계정 정보 수정", "a_account_info_m", "a_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("구매 내역", "a_invoice_list", "a_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("주문 상세 정보", "a_invoice_info", "a_invoice_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("구매자 메뉴화면", "buyer", "start", { 'l', 'q', 'q' });
	menu_manager.SetMenu("상품 목록", "b_product_list", "buyer", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 검색 및 장르 선택", "b_product_search", "b_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 상세 정보", "b_product_info", "b_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("상품 구매", "b_product_buy", "b_product_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("고객 계정 정보", "b_account_info", "buyer", { 'z', 'l', 'q' });
	menu_manager.SetMenu("고객 계정 정보 수정", "b_account_info_m", "b_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("구매 내역", "b_invoice_list", "b_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("주문 상세 정보", "b_invoice_info", "b_invoice_list", { 'z', 'l', 'q' });
}