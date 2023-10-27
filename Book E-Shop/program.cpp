#include "program.h"

#include <format>

Program::Program()
{
	LoadCSV();
	SetCommand();
	SetParser();
	SetMenu();
	SetPrevMenuCode();
}

void Program::Run()
{
	menu_manager.Start(MENU_START);
}

void Program::LoadCSV()
{
	// Product CSV 파일을 파싱합니다.
	auto product_raw = data_manager.ParseCSV("./data/product.csv");
	for (size_t i = 0; i < product_raw.size(); i++) {
		// 생성된 Product 객체를 shop_manager에 추가합니다.
		shop_manager.AddProduct(new Product(product_raw[i]));
	}

	// Account CSV 파일을 파싱합니다.
	auto account_raw = data_manager.ParseCSV("./data/account.csv");
	for (size_t i = 0; i < account_raw.size(); i++) {
		// 생성된 Account 객체를 shop_manager에 추가합니다.
		shop_manager.AddAccount(new Account(account_raw[i]));
	}

	// Invoice CSV 파일을 파싱합니다.
	auto invoice_raw = data_manager.ParseCSV("./data/invoice.csv");
	for (size_t i = 0; i < invoice_raw.size(); i++) {
		// 생성된 Invoice 객체를 shop_manager에 추가합니다.
		shop_manager.AddInvoice(new Invoice(invoice_raw[i]));
	}
}

void Program::SetCommand()
{
	menu_manager.AppendCommand('z', new MenuManager::Command("뒤로가기", [&]() {
		auto prev_menu_code = menu_manager.GetCurrentMenu()->get_prev_menu_code();

		if (prev_menu_code != MENU_NONE) {
			menu_manager.RunMenu(prev_menu_code);
		}
	}));

	menu_manager.AppendCommand('l', new MenuManager::Command("로그아웃", [&]() {
		menu_manager.RunRecursiveMenu(MENU_LOGOUT);
	}));

	menu_manager.AppendCommand('q', new MenuManager::Command("종료", [&]() {
		menu_manager.RunRecursiveMenu(MENU_QUIT);
	}));
}

#pragma region Parser 구현
void Program::SetParser()
{
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



#pragma endregion


	data_manager.AppendParser("MENU_SELECT", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("메뉴에 표시된 번호 중 하나를 고르세요.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);
}
#pragma endregion

#pragma region 메뉴화면 구현
void Program::SetPrevMenuCode()
{
	// 각 메뉴에서 뒤로가기를 위한 이전 메뉴를 설정하는 것으로 엑셀로 관리됩니다.
	menu_manager[MENU_QUIT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGOUT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_START]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGIN]->set_prev_menu_code(MENU_START);
	menu_manager[MENU_SIGNUP]->set_prev_menu_code(MENU_START);
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
}

void Program::SetMenu()
{	
#pragma region 시작 메뉴화면
	// 시작 메뉴화면
	TemplateMenuSelection(MENU_START, make_pair(MENU_LOGIN, "계정 로그인"), make_pair(MENU_SIGNUP, "계정 회원가입"));

	// 계정 로그인 메뉴화면
	menu_manager.AppendMenu(MENU_LOGIN, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
			IO.print_line();

			string id, password;

			auto checkpoint = IO.checkpoint();

			while (true) {
				// 아이디를 입력받습니다.
				id = IO.input("", "아이디");
				// 비밀번호를 입력받습니다.
				password = IO.input("", "비밀번호");

				const Account* user;

				// 일치하는 계정 정보가 있는지 확인합니다.
				if ((user = shop_manager.Login(id, password)) != nullptr) {
					if (user == shop_manager.GetAdminAccount()) {
						menu_manager.RunMenu(MENU_ADMIN);
					}
					else {
						menu_manager.RunMenu(MENU_BUYER);
					}
				}
				else {
					IO.print("일치하는 계정 정보를 찾을 수 없습니다.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));

	// 계정 회원가입 메뉴화면
	menu_manager.AppendMenu(MENU_SIGNUP, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
			IO.print_line();

			//id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));
			//password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

		}
	));
#pragma endregion

#pragma region 관리자 메뉴화면
	// 관리자 메뉴화면
	TemplateMenuSelection(MENU_ADMIN, make_pair(MENU_A_PRODUCT_LIST, "상품 관리 (등록/수정/제거, 재고관리)"), make_pair(MENU_A_ACCOUNT_LIST, "고객 관리 (계정, 주문)"));
	
	#pragma region 상품 목록
	// 상품 목록 메뉴화면
	menu_manager.AppendMenu(MENU_A_PRODUCT_LIST, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	
	// 상품 검색 및 장르 선택 메뉴화면
	menu_manager.AppendMenu(MENU_A_PRODUCT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// 상품 등록 정보 메뉴화면
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

	// 상품 등록 정보 수정 메뉴화면
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_M, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// 상품 등록 정보 제거 메뉴화면
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_R, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	#pragma endregion 

	#pragma region 고객 목록
	// 고객 목록 메뉴화면
	menu_manager.AppendMenu(MENU_A_ACCOUNT_LIST, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// 고객 검색 메뉴화면
	menu_manager.AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	
	// 고객 계정 정보 메뉴화면
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// 고객 계정 정보 수정 메뉴화면
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO_M, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	// 구매 내역 메뉴화면
	menu_manager.AppendMenu(MENU_A_INVOICE_LIST, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	// 주문 상세 정보 메뉴화면
	menu_manager.AppendMenu(MENU_A_INVOICE_INFO, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	#pragma endregion

#pragma endregion

#pragma region 구매자 메뉴화면
	// 구매자 메뉴화면
	TemplateMenuSelection(MENU_BUYER, make_pair(MENU_B_PRODUCT_LIST, "상품 목록"), make_pair(MENU_B_ACCOUNT_INFO, "고객 계정 정보"));


#pragma endregion
}
#pragma endregion

