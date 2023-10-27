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
	// Product CSV ������ �Ľ��մϴ�.
	auto product_raw = data_manager.ParseCSV("./data/product.csv");
	for (size_t i = 0; i < product_raw.size(); i++) {
		// ������ Product ��ü�� shop_manager�� �߰��մϴ�.
		shop_manager.AddProduct(new Product(product_raw[i]));
	}

	// Account CSV ������ �Ľ��մϴ�.
	auto account_raw = data_manager.ParseCSV("./data/account.csv");
	for (size_t i = 0; i < account_raw.size(); i++) {
		// ������ Account ��ü�� shop_manager�� �߰��մϴ�.
		shop_manager.AddAccount(new Account(account_raw[i]));
	}

	// Invoice CSV ������ �Ľ��մϴ�.
	auto invoice_raw = data_manager.ParseCSV("./data/invoice.csv");
	for (size_t i = 0; i < invoice_raw.size(); i++) {
		// ������ Invoice ��ü�� shop_manager�� �߰��մϴ�.
		shop_manager.AddInvoice(new Invoice(invoice_raw[i]));
	}
}

void Program::SetCommand()
{
	menu_manager.AppendCommand('z', new MenuManager::Command("�ڷΰ���", [&]() {
		auto prev_menu_code = menu_manager.GetCurrentMenu()->get_prev_menu_code();

		if (prev_menu_code != MENU_NONE) {
			menu_manager.RunMenu(prev_menu_code);
		}
	}));

	menu_manager.AppendCommand('l', new MenuManager::Command("�α׾ƿ�", [&]() {
		menu_manager.RunRecursiveMenu(MENU_LOGOUT);
	}));

	menu_manager.AppendCommand('q', new MenuManager::Command("����", [&]() {
		menu_manager.RunRecursiveMenu(MENU_QUIT);
	}));
}

#pragma region Parser ����
void Program::SetParser()
{
	data_manager.AppendParser("account_id", (new Parser())
		->set_label("���̵�")
		->set_regex(R"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("���̵�� ����, �����ڷ� ������ ���̰� 4 �̻� 12 ������ ���ڿ��̾�� �մϴ�!")
	);

	data_manager.AppendParser("account_password", (new Parser())
		->set_label("��й�ȣ")
		->set_regex(R"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("��й�ȣ�� ����, ������, Ư�� ����(!@#$%^&*_)�� ������ ���̰� 8 �̻� 16 ������ ���ڿ��̾�� �մϴ�!")
	);

	data_manager.AppendParser("test", (new Parser())
		->set_label("����")
		->set_msg_info("���̸� �Է��ϼ���!")
	);
#pragma region *** product Parser ***
	data_manager.AppendParser("product_id", (new Parser())
		->set_label("������ȣ")
		->set_regex(R"(^[0-9]{6}$)")
		->set_msg_error("���ڷ� ������ ���̰� 6�� ���ڿ��̾�� �մϴ�")
	);
	data_manager.AppendParser("product_title", (new Parser())
		->set_label("����")
	);
	data_manager.AppendParser("product_genre", (new Parser())
		->set_label("�帣")
	);
	data_manager.AppendParser("product_price", (new Parser())
		->set_label("����")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�")
	);
	data_manager.AppendParser("product_count", (new Parser())
		->set_label("���")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�")
	);



#pragma endregion


	data_manager.AppendParser("MENU_SELECT", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ������.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);
}
#pragma endregion

#pragma region �޴�ȭ�� ����
void Program::SetPrevMenuCode()
{
	// �� �޴����� �ڷΰ��⸦ ���� ���� �޴��� �����ϴ� ������ ������ �����˴ϴ�.
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
#pragma region ���� �޴�ȭ��
	// ���� �޴�ȭ��
	TemplateMenuSelection(MENU_START, make_pair(MENU_LOGIN, "���� �α���"), make_pair(MENU_SIGNUP, "���� ȸ������"));

	// ���� �α��� �޴�ȭ��
	menu_manager.AppendMenu(MENU_LOGIN, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
			IO.print_line();

			string id, password;

			auto checkpoint = IO.checkpoint();

			while (true) {
				// ���̵� �Է¹޽��ϴ�.
				id = IO.input("", "���̵�");
				// ��й�ȣ�� �Է¹޽��ϴ�.
				password = IO.input("", "��й�ȣ");

				const Account* user;

				// ��ġ�ϴ� ���� ������ �ִ��� Ȯ���մϴ�.
				if ((user = shop_manager.Login(id, password)) != nullptr) {
					if (user == shop_manager.GetAdminAccount()) {
						menu_manager.RunMenu(MENU_ADMIN);
					}
					else {
						menu_manager.RunMenu(MENU_BUYER);
					}
				}
				else {
					IO.print("��ġ�ϴ� ���� ������ ã�� �� �����ϴ�.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));

	// ���� ȸ������ �޴�ȭ��
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

#pragma region ������ �޴�ȭ��
	// ������ �޴�ȭ��
	TemplateMenuSelection(MENU_ADMIN, make_pair(MENU_A_PRODUCT_LIST, "��ǰ ���� (���/����/����, ������)"), make_pair(MENU_A_ACCOUNT_LIST, "�� ���� (����, �ֹ�)"));
	
	#pragma region ��ǰ ���
	// ��ǰ ��� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_LIST, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	
	// ��ǰ �˻� �� �帣 ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// ��ǰ ��� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) {
			//all command allowed
			menu_manager.PrintCommand();

			IO.print_line();
			IO.print("[��ǰ�������]\n");

			IO.print(format("������ȣ : {0}\n", target->id));
			IO.print(format("���� : {0}\n", target->title));
			IO.print(format("�帣 : {0}\n", target->genre));
			IO.print(format("���� : {0}\n", target->price));
			IO.print(format("��� : {0}\n", target->count));

			IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
			string input = IO.input();

			if (input == "y") {
				menu_manager.RunMenu(MENU_START);
			}
		}
	));

	// ��ǰ ��� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_M, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// ��ǰ ��� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_R, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	#pragma endregion 

	#pragma region �� ���
	// �� ��� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_LIST, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// �� �˻� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	
	// �� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));

	// �� ���� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO_M, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	// ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_INVOICE_LIST, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	// �ֹ� �� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_INVOICE_INFO, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();

		}
	));
	#pragma endregion

#pragma endregion

#pragma region ������ �޴�ȭ��
	// ������ �޴�ȭ��
	TemplateMenuSelection(MENU_BUYER, make_pair(MENU_B_PRODUCT_LIST, "��ǰ ���"), make_pair(MENU_B_ACCOUNT_INFO, "�� ���� ����"));


#pragma endregion
}
#pragma endregion

