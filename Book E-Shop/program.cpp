#include "program.h"
#include "Util.h"

#include <format>

DataManager Program::data_manager;
MenuManager Program::menu_manager;
ShopManager Program::shop_manager;

void Program::Run()
{
	LoadCSV();
	SetCommand();
	SetParser();
	SetMenu();
	SetPrevMenuCode();

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
	data_manager.AppendParser("test", (new Parser())
		->set_label("나이")
		->set_msg_info("나이를 입력하세요!")
	);

#pragma region *** account Parser ***
	data_manager.AppendParser("account_name", (new Parser())
		->set_label("이름")
		->set_regex(R"(^[ ]*[a-zA-Z가-힣]+[ a-zA-Z가-힣]*$)")
		->set_msg_error("이름은 공백, 영문자, 한글 허용한 공백을 제외한 길이가 1 이상인 문자열이어야 합니다!")
	);
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
	data_manager.AppendParser("account_phonenumber", (new Parser())
		->set_label("전화번호")
		->set_regex(R"(^010[0-9]{8}$)")
		->set_msg_error("전화번호는 숫자로 구성되어 010으로 시작하고, 길이가 11의 문자열이어야 합니다!")
	);
	data_manager.AppendParser("account_address", (new Parser())
		->set_label("주소")
	);
	data_manager.AppendParser("account_invoicelist", (new Parser())
		->set_label("주문 처리 정보 목록")
	);
#pragma endregion

#pragma region *** product Parser ***
	data_manager.AppendParser("product_id", (new Parser())
		->set_label("고유번호")
		->set_regex(R"(^[0-9]{6}$)")
		->set_msg_error("숫자로 구성된 길이가 6인 문자열이어야 합니다.")
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
		->set_msg_error("숫자로 구성된 길이가 1 이상의 문자열이어야 합니다.")
	);
	data_manager.AppendParser("product_count", (new Parser())
		->set_label("재고")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("숫자로 구성된 길이가 1 이상의 문자열이어야 합니다.")
	);
#pragma endregion

#pragma region *** invoice Parser ***
	data_manager.AppendParser("invoice_id", (new Parser())
		->set_label("고유번호")
		->set_regex(R"(^[0-9]{8}$)")
		->set_msg_error("숫자로 구성된 길이가 8인 문자열이어야 합니다")
	);
	data_manager.AppendParser("invoice_buyer_id", (new Parser())
		->set_label("구매자의 아이디")
		->set_msg_info("구매자의 아이디 입니다.")
	);
	data_manager.AppendParser("invoice_recipient_phonenumber", (new Parser())
		->set_label("수령인의 전화번호")
		->set_msg_info("수령인의 전화번호 입니다.")
	);
	data_manager.AppendParser("invoice_recipient_address", (new Parser())
		->set_label("수령인의 주소")
		->set_msg_info("수령인의 주소 입니다.")
	);
	data_manager.AppendParser("invoice_price", (new Parser())
		->set_label("구매 날짜")
		->set_regex(R"(YY.MM.DD)")
		->set_msg_error("구매 날짜는 (연도).(월).(일)로 구성된 문자열이어야 합니다")
	);
	data_manager.AppendParser("invoice_product_id", (new Parser())
		->set_label("상품 고유 번호")
		->set_msg_info("구매한 상품 고유 번호 입니다.")
	);
	data_manager.AppendParser("invoice_product_count", (new Parser())
		->set_label("상품 수량")
		->set_regex(R"(^[0-9]*$)")
		->set_msg_error("숫자로 구성된 길이가 1 이상의 문자열이어야 합니다")
	);
#pragma endregion


	data_manager.AppendParser("MENU_MENUSELECTION", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("메뉴에 표시된 번호 중 하나를 고르세요.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);

	data_manager.AppendParser("MENU_TABLE", (new Parser())
		->set_regex(R"(^[0-9a-zA-Z]$)")
		->set_msg_error("메뉴에 표시된 번호 또는 알파벳 중 하나를 고르세요.")
		->set_parse([&](string input) -> any { return input[0]; })
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
	{
		TemplateMenuSelection _template;
		_template.SubMenu("계정 로그인",   []() { menu_manager.RunMenu(MENU_LOGIN);  });	
		_template.SubMenu("계정 회원가입", []() { menu_manager.RunMenu(MENU_SIGNUP); });
		_template.Apply(MENU_START);
	}

	//테스트용
	menu_manager.AppendMenu(MENU_START, new Menu(
		[&](MenuIO& IO) {
			//menu_manager.RunMenu(MENU_A_PRODUCT_INFO,shop_manager.FindProduct(123456));
			//menu_manager.RunMenu(MENU_A_ACCOUNT_INFO, shop_manager.FindAccount("user1234"));
			//menu_manager.RunMenu(MENU_A_INVOICE_INFO, shop_manager.FindInvoice(12345678));
		}
	));

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
	{
		TemplateMenuSelection _template;
		_template.SubMenu("상품 관리 (등록/수정/제거, 재고관리)", []() { menu_manager.RunMenu(MENU_A_PRODUCT_LIST, shop_manager.GetProdcutList()); });
		_template.SubMenu("고객 관리 (계정, 주문)",				  []() { menu_manager.RunMenu(MENU_A_ACCOUNT_LIST, shop_manager.GetAccountList()); });
		_template.Apply(MENU_ADMIN);
	}

	#pragma region 상품 목록
	// 상품 목록 메뉴화면
	{
		TemplateTable<Product*> _template;
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<16}{4:<8}", "ID", "상품", "장르", "가격", "재고");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<16}{4:<8}", product->id, limit(product->title, 18), product->genre, product->price, product->count);
		}; 
		_template.SubMenu('p', MENU_A_PRODUCT_SEARCH, "상품 검색 및 장르 선택");
		_template.SubMenu('r', MENU_A_PRODUCT_REGISTER, "상품 신규 등록");

		_template.next_menu_code = MENU_A_PRODUCT_INFO;

		_template.Apply(MENU_A_PRODUCT_LIST);
	} 
	
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

			IO.print_line();

			IO.print("수정(M) 제거(R)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "M") {
					//IO.print("수정");
				}
				else if (input == "R") {
					//IO.print("제거");
				}
				else {
					IO.print("일치하는 명령어가 없음.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
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
	{
		TemplateTable<Account*> _template;
		_template.header_func = []() -> string {
			return format("{0:<16}{1:<8}{2:<16}", "아이디", "이름", "전화번호");
		};
		_template.show_func = [](Account* product) -> string {
			return format("{0:<16}{1:<8}{2:<16}", product->id, product->name, product->phone_number);
		};
		_template.SubMenu('p', MENU_A_PRODUCT_SEARCH, "고객 검색"); 

		_template.next_menu_code = MENU_A_ACCOUNT_INFO;

		_template.Apply(MENU_A_ACCOUNT_LIST);
	}


	// 고객 검색 메뉴화면
	menu_manager.AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	
	// 고객 계정 정보 메뉴화면
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			//all command allowed
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print("[고객 계정 정보]\n");

			IO.print(format("고유번호 : {0}\n", target->name));
			IO.print(format("제목 : {0}\n", target->id));
			IO.print(format("장르 : {0}\n", target->phone_number));
			IO.print(format("가격 : {0}\n", target->address));

			IO.print_line();

			IO.print("주문 처리 정보(O) 수정(M)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "O") {
					menu_manager.RunMenu(MENU_A_INVOICE_LIST, target->invoice_id_list);
				}
				else if (input == "M") {
					//IO.print("수정\n");
				}
				else {
					IO.print("일치하는 명령어가 없음.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}

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
	{
		TemplateTable<int> _template;
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<16}", "구매 날짜", "제목", "결제 금액");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager.GetInvoice(id);
			Product* product = shop_manager.GetProduct(invoice->product_id); 

			return format("{0:<12}{1:<20}{2:<16}", invoice->date, limit(product->title, 18), to_string(product->price * invoice->product_count) + "원");
		};

		_template.next_menu_code = MENU_A_INVOICE_INFO;

		_template.Apply(MENU_A_INVOICE_LIST);
	}

	// 주문 상세 정보 메뉴화면
	menu_manager.AppendMenu(MENU_A_INVOICE_INFO, new Menu<int>(
		[&](MenuIO& IO, int target_id) {
			//all command allowed
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print("[주문 상세 정보]\n");

			Invoice* target = shop_manager.GetInvoice(target_id);
			Product* product = shop_manager.GetProduct(target->product_id);
			Account* account = shop_manager.GetAccount(target->buyer_id);

			IO.print(format("구매 날짜 : {0}\n", target->date));

			IO.print("\n[상품 상세 정보]\n");
			IO.print(format("고유 번호 : {0}\n", product->id));
			IO.print(format("제목 : {0}\n", product->title));
			IO.print(format("장르 : {0}\n", product->genre));
			IO.print(format("가격 : {0}\n", product->price));

			IO.print("\n[주문자 정보]\n");
			IO.print(format("아이디 : {0}\n", account->id));
			IO.print(format("이름 : {0}\n", account->name));
			IO.print(format("전화번호 : {0}\n", target->recipient_phone_number));
			IO.print(format("주소 : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("상품 고유 번호 : {0}\n", target->product_id));
			IO.print(format("상품 수량 : {0}\n", target->product_count));
			IO.print(format("결제 금액 : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.print("아무 키나 입력하세요\n");

			auto checkpoint = IO.checkpoint();
			string input = IO.input();

		}
	));
	#pragma endregion

#pragma endregion

#pragma region 구매자 메뉴화면
	// 구매자 메뉴화면
	{
		TemplateMenuSelection _template;
		_template.SubMenu("상품 관리 (등록/수정/제거, 재고관리)", []() { MENU_B_PRODUCT_LIST, shop_manager.GetProdcutList(); });
		_template.SubMenu("고객 계정 정보", []() { menu_manager.RunMenu(MENU_B_ACCOUNT_INFO); });
		_template.Apply(MENU_BUYER);
	}
	// 상품 상세 정보 확인 메뉴화면
	menu_manager.AppendMenu(MENU_B_PRODUCT_INFO, new Menu<Product*>(
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

			IO.print_line();

			IO.print("구매(B)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "B") {
					//IO.print("구매");
				}
				else {
					IO.print("일치하는 명령어가 없음.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));

	// 고객 계정 정보 관리 메뉴화면
	menu_manager.AppendMenu(MENU_B_ACCOUNT_INFO, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			//all command allowed
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print("[고객 계정 정보]\n");

			IO.print(format("고유번호 : {0}\n", target->name));
			IO.print(format("제목 : {0}\n", target->id));
			IO.print(format("장르 : {0}\n", target->phone_number));
			IO.print(format("가격 : {0}\n", target->address));

			IO.print_line();

			IO.print("주문 처리 정보(O) 수정(M)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "O") {
					//IO.print("주문 처리 정보\n");
				}
				else if (input == "M") {
					//IO.print("수정\n");
				}
				else {
					IO.print("일치하는 명령어가 없음.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}

		}
	));

	// 주문 처리 정보 확인 메뉴화면
	menu_manager.AppendMenu(MENU_B_INVOICE_INFO, new Menu<Invoice*>(
		[&](MenuIO& IO, Invoice* target) {
			//all command allowed
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print("[주문 상세 정보]\n");

			Product* product = shop_manager.GetProduct(target->product_id);
			Account* account = shop_manager.GetAccount(target->buyer_id);

			IO.print(format("구매 날짜 : {0}\n", target->date));

			IO.print("\n[상품 상세 정보]\n");
			IO.print(format("고유 번호 : {0}\n", product->id));
			IO.print(format("제목 : {0}\n", product->title));
			IO.print(format("장르 : {0}\n", product->genre));
			IO.print(format("가격 : {0}\n", product->price));

			IO.print("\n[주문자 정보]\n");
			IO.print(format("아이디 : {0}\n", account->id));
			IO.print(format("이름 : {0}\n", account->name));
			IO.print(format("전화번호 : {0}\n", target->recipient_phone_number));
			IO.print(format("주소 : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("상품 고유 번호 : {0}\n", target->product_id));
			IO.print(format("상품 수량 : {0}\n", target->product_count));
			IO.print(format("결제 금액 : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.print("아무 키나 입력하세요\n");

			auto checkpoint = IO.checkpoint();
			string input = IO.input();

		}
	));
	{
		TemplateMenuSelection _template;

		_template.SubMenu("상품 관리 (등록/수정/제거, 재고관리)", []() { MENU_B_PRODUCT_LIST, shop_manager.GetProdcutList(); });
		_template.SubMenu("고객 계정 정보",						  []() { menu_manager.RunMenu(MENU_B_ACCOUNT_INFO); }); 
		_template.Apply(MENU_BUYER);
	}
#pragma endregion
}
#pragma endregion

