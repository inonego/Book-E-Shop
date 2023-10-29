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
		menu_manager.RunPreviousMenu();
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
	data_manager.AppendParser("test", (new Parser())
		->set_label("����")
		->set_msg_info("���̸� �Է��ϼ���!")
	);

#pragma region *** account Parser ***
	data_manager.AppendParser("account_name", (new Parser())
		->set_label("�̸�")
		->set_regex(R"(^[ ]*[a-zA-Z��-�R]+[a-zA-Z��-�R]*$)")
		->set_msg_error("�̸��� ����, ������, �ѱ� ����� ������ ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�!")
	);
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
	data_manager.AppendParser("account_phonenumber", (new Parser())
		->set_label("��ȭ��ȣ")
		->set_regex(R"(^010[0-9]{8}$)")
		->set_msg_error("��ȭ��ȣ�� ���ڷ� �����Ǿ� 010���� �����ϰ�, ���̰� 11�� ���ڿ��̾�� �մϴ�!")
	);
	data_manager.AppendParser("account_address", (new Parser())
		->set_label("�ּ�")
	);
	data_manager.AppendParser("account_invoicelist", (new Parser())
		->set_label("�ֹ� ó�� ���� ���")
	);
#pragma endregion

#pragma region *** product Parser ***
	data_manager.AppendParser("product_id", (new Parser())
		->set_label("������ȣ")
		->set_regex(R"(^[0-9]{6}$)")
		->set_msg_error("���ڷ� ������ ���̰� 6�� ���ڿ��̾�� �մϴ�.")
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
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.")
	);
	data_manager.AppendParser("product_count", (new Parser())
		->set_label("���")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.")
	);
#pragma endregion

#pragma region *** invoice Parser ***
	data_manager.AppendParser("invoice_id", (new Parser())
		->set_label("������ȣ")
		->set_regex(R"(^[0-9]{8}$)")
		->set_msg_error("���ڷ� ������ ���̰� 8�� ���ڿ��̾�� �մϴ�")
	);
	data_manager.AppendParser("invoice_buyer_id", (new Parser())
		->set_label("�������� ���̵�")
		->set_msg_info("�������� ���̵� �Դϴ�.")
	);
	data_manager.AppendParser("invoice_recipient_phonenumber", (new Parser())
		->set_label("�������� ��ȭ��ȣ")
		->set_msg_info("�������� ��ȭ��ȣ �Դϴ�.")
	);
	data_manager.AppendParser("invoice_recipient_address", (new Parser())
		->set_label("�������� �ּ�")
		->set_msg_info("�������� �ּ� �Դϴ�.")
	);
	data_manager.AppendParser("invoice_date", (new Parser())
		->set_label("���� ��¥")
		->set_regex(R"(YY.MM.DD)")
		->set_msg_error("���� ��¥�� (����).(��).(��)�� ������ ���ڿ��̾�� �մϴ�")
	);
	data_manager.AppendParser("invoice_product_id", (new Parser())
		->set_label("��ǰ ���� ��ȣ")
		->set_msg_info("������ ��ǰ ���� ��ȣ �Դϴ�.")
	);
	data_manager.AppendParser("invoice_product_count", (new Parser())
		->set_label("��ǰ ����")
		->set_regex(R"(^[1-9]*$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�")
	);
#pragma endregion


	data_manager.AppendParser("MENU_MENUSELECTION", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ��������.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);

	data_manager.AppendParser("MENU_TABLE", (new Parser())
		->set_regex(R"(^[0-9a-zA-Z]$)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �Ǵ� ���ĺ� �� �ϳ��� ��������.")
		->set_parse([&](string input) -> any { return input[0]; })
	);
}
#pragma endregion

#pragma region �޴�ȭ�� ����

void Program::SetMenu()
{	
#pragma region ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_QUIT, new Menu([&](MenuIO& IO) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ ���α׷� ���� ]");
		IO.print("���α׷��� �����Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit(0);
		}
		}));

	menu_manager.AppendMenu(MENU_LOGOUT, new Menu([&](MenuIO& IO) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ ���� �α׾ƿ� ]");
		IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			shop_manager.Logout();

			menu_manager.RunMenu(MENU_START);
		}
		}));
#pragma endregion


#pragma region ���� �޴�ȭ��
	// ���� �޴�ȭ��
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('q');
		_template.SetName("���� �޴�");
		_template.SubMenu("���� �α���",   []() { menu_manager.RunMenu(MENU_LOGIN);  });	
		_template.SubMenu("���� ȸ������", []() { menu_manager.RunMenu(MENU_SIGNUP); });
		_template.Apply(MENU_START);
	} 

	// ���� �α��� �޴�ȭ��
	menu_manager.AppendMenu(MENU_LOGIN, new Menu(
		[&](MenuIO& IO) { 
			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� �α��� ]");

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
					IO.print("���̵� �������� �ʰų� ��й�ȣ�� �ùٸ��� �ʽ��ϴ�.\n");
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
			IO.print_aligned_center("[ ���� ȸ������ ]");

			auto checkpoint = IO.checkpoint();
			vector<string> account;
			string input;
			vector<string> heads = { "�̸�","���̵�","��й�ȣ","��й�ȣ Ȯ��","��ȭ��ȣ","�ּ�" };
			vector<string> regex_t = { "account_name", "account_id","account_password","","account_phonenumber","account_address" };
			int t = 0;
			while (t < heads.size()) {				
				if (t != 3) {//��й�ȣ �˻�
					checkpoint = IO.checkpoint();
					input = IO.input("", heads[t]);
					if (data_manager.GetParser(regex_t[t])->Check(input)) {
						account.push_back(input);
						t++;
					}
					else {
						IO.print(data_manager.GetParser(regex_t[t])->msg_error);
						IO.pause();
						IO.rollback(checkpoint);
					}
				}
				else {
					checkpoint = IO.checkpoint();
					string match_p = IO.input("",heads[t]);
					if (match_p._Equal(input))
						t++;
					else 
					{
						IO.print("'��й�ȣ Ȯ��'�� '��й�ȣ'�� ��ġ���� �ʽ��ϴ�");
						IO.pause();
						IO.rollback(checkpoint);
					}

				}			
			}
			account.push_back("");
			shop_manager.AddAccount(new Account(account));
			IO.pause();
			menu_manager.RunMenu(MENU_START);
			//id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));
			//password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

		}
	));
#pragma endregion

#pragma region ������ �޴�ȭ��
	// ������ �޴�ȭ��
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('l', 'q'); 
		_template.SetName("������ �޴�ȭ��");
		_template.SubMenu("��ǰ ���� (���/����/����, �������)", []() { menu_manager.RunMenu(MENU_A_PRODUCT_LIST, shop_manager.GetProdcutList()); });
		_template.SubMenu("���� ���� (����, �ֹ�)",				  []() { menu_manager.RunMenu(MENU_A_ACCOUNT_LIST, shop_manager.GetAccountList()); });
		_template.Apply(MENU_ADMIN);
	}

	#pragma region ��ǰ ���
	// ��ǰ ��� �޴�ȭ��
	{
		TemplateTable<Product*> _template;
		_template.SetName("��ǰ ���");
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<12}{4:<8}", "ID", "��ǰ", "�帣", "����", "���");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<12}{4:<8}", product->id, limit(product->title, 18), product->genre, to_string(product->price) + "��", product->count);
		}; 
		_template.SubMenu('p', MENU_A_PRODUCT_SEARCH, "��ǰ �˻� �� �帣 ����");
		_template.SubMenu('r', MENU_A_PRODUCT_REGISTER, "��ǰ �ű� ���");

		_template.next_menu_code = MENU_A_PRODUCT_INFO;

		_template.Apply(MENU_A_PRODUCT_LIST);
	} 
	
	// ��ǰ �˻� �� �帣 ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ �˻� �� �帣 ���� ]");
		}
	));

	// ��ǰ �ű� ��� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_REGISTER, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ �ű� ��� ]");

			auto checkpoint = IO.checkpoint();
			vector<string> product;
			string input;
			vector<string> heads = { "����","�帣","����","���" };
			vector<string> regex_t = { "product_title","product_genre","product_price","product_count" };
			int t = 0;
			while (t < heads.size()) {
				checkpoint = IO.checkpoint();
				input = IO.input("", heads[t]);
				if (data_manager.GetParser(regex_t[t])->Check(input)) {
					product.push_back(input);
					t++;
				}
				else
				{
					IO.print(data_manager.GetParser(regex_t[t])->msg_error);
					IO.pause();
					IO.rollback(checkpoint);
				}
			}
			input = IO.input("��ǰ�� ����Ͻðڽ��ϱ�?(y/n)","");
			if (input == "y") {
				int id = 7 + shop_manager.GetProdcutList().size();//id ���� ��Ŀ����??
				product.insert(product.begin(), to_string(id));
				shop_manager.AddProduct(new Product(product));
				IO.pause();
			}
			menu_manager.RunMenu(MENU_A_PRODUCT_LIST,shop_manager.GetProdcutList());
			//id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));
			//password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));
		}
	));

	// ��ǰ ��� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) { 
			menu_manager.PrintCommand(); 
			IO.print_line(); 
			IO.print_aligned_center("[ ��ǰ ��� ���� ]");

			IO.print(format("������ȣ : {0}\n", target->id));
			IO.print(format("���� : {0}\n", target->title));
			IO.print(format("�帣 : {0}\n", target->genre));
			IO.print(format("���� : {0}\n", target->price));
			IO.print(format("��� : {0}\n", target->count));

			IO.print_line();

			IO.print("����(M) ����(R)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "M") {
					menu_manager.RunMenu(MENU_A_PRODUCT_INFO_M, target);
				}
				else if (input == "R") {
					//IO.print("����");
				}
				else {
					IO.print("��ġ�ϴ� ���ɾ ����.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));

	// ��ǰ ��� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_M, new Menu<Product*>(
		[&](MenuIO& IO,Product* target)  {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ��� ���� ���� ]");

			auto checkpoint = IO.checkpoint();
			vector<string> product;
			string input;
			vector<string> heads = { "����","�帣","����","���" };
			vector<string> regex_t = { "product_title","product_genre","product_price","product_count" };
			int t = 0;
			while (t < heads.size()) {
				checkpoint = IO.checkpoint();
				input = IO.input("", heads[t]);
				if (data_manager.GetParser(regex_t[t])->Check(input) || input._Equal("")) {
					product.push_back(input);
					t++;
				}
				else
				{
					IO.print(data_manager.GetParser(regex_t[t])->msg_error);
					IO.pause();
					IO.rollback(checkpoint);
				}
			}
			input = IO.input("��ǰ ��� ������ �����Ͻðڽ��ϱ�?(y/n)", "");
			if (input == "y") {
				target->title = product[0]._Equal("") ? target->title : product[0];
				target->genre = product[1]._Equal("") ? target->genre : product[1];
				target->price = product[2]._Equal("") ? target->price : stoi(product[2]);
				target->count = product[3]._Equal("") ? target->count : stoi(product[3]);
				IO.pause();
			}
			menu_manager.RunMenu(MENU_A_PRODUCT_INFO, target);
		}
	));

	// ��ǰ ��� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_R, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ��� ���� ���� ]");

		}
	));
	#pragma endregion 

	#pragma region ���� ���
	// ���� ��� �޴�ȭ�� 
	{
		TemplateTable<Account*> _template;
		_template.SetName("���� ���");
		_template.header_func = []() -> string {
			return format("{0:<16}{1:<8}{2:<16}", "���̵�", "�̸�", "��ȭ��ȣ");
		};
		_template.show_func = [](Account* product) -> string {
			return format("{0:<16}{1:<8}{2:<16}", product->id, product->name, product->phone_number);
		};
		_template.SubMenu('p', MENU_A_PRODUCT_SEARCH, "���� �˻�"); 

		_template.next_menu_code = MENU_A_ACCOUNT_INFO;

		_template.Apply(MENU_A_ACCOUNT_LIST);
	}


	// ���� �˻� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� �˻� ]");
		}
	));
	
	// ���� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� ���� ���� ]");

			IO.print(format("�̸� : {0}\n", target->name));
			IO.print(format("���̵� : {0}\n", target->id));
			IO.print(format("��ȭ��ȣ : {0}\n", target->phone_number));
			IO.print(format("�ּ� : {0}\n", target->address));

			IO.print_line();

			IO.print("�ֹ� ó�� ����(O) ����(M)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "O") {
					menu_manager.RunMenu(MENU_A_INVOICE_LIST, target->invoice_id_list);
				}
				else if (input == "M") {
					menu_manager.RunMenu(MENU_A_ACCOUNT_INFO_M, target);
				}
				else {
					IO.print("��ġ�ϴ� ���ɾ ����.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}

		}
	));

	// ���� ���� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO_M, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� ���� ���� ���� ]");

			auto checkpoint = IO.checkpoint();
			vector<string> account;
			string input, match_p;
			vector<string> heads = { "��й�ȣ","��й�ȣ Ȯ��","��ȭ��ȣ","�ּ�" };
			vector<string> regex_t = { "account_password","","account_phonenumber","account_address" };
			int t = 0;
			while (t < heads.size()) {
				if (t != 1) {
					checkpoint = IO.checkpoint();
					input = IO.input("", heads[t]);
					if (data_manager.GetParser(regex_t[t])->Check(input) || input._Equal("")) {
						account.push_back(input);
						t++;
					}
					else
					{
						IO.print(data_manager.GetParser(regex_t[t])->msg_error);
						IO.pause();
						IO.rollback(checkpoint);
					}
				}
				else{
					checkpoint = IO.checkpoint();
					match_p = IO.input("", heads[t]);
					if (input._Equal(match_p))
						t++;
					else
					{
						IO.print("'��й�ȣ Ȯ��'�� '��й�ȣ'�� ��ġ���� �ʽ��ϴ�");
						IO.pause();
						IO.rollback(checkpoint);
					}
				}
			}
			input = IO.input("���� ������ �����Ͻðڽ��ϱ�?(y/n)", "");
			if (input == "y") {
				target->password = account[0]._Equal("") ? target->password : account[0];
				target->phone_number = account[1]._Equal("") ? target->phone_number : account[1];
				target->address = account[2]._Equal("") ? target->address : account[2];
				IO.pause();
			}
			menu_manager.RunMenu(MENU_A_ACCOUNT_INFO, target);
		}
	));
	// ���� ���� �޴�ȭ��
	{
		TemplateTable<int> _template;
		_template.SetName("���� ����");
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<16}", "���� ��¥", "����", "���� �ݾ�");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager.GetInvoice(id);
			Product* product = shop_manager.GetProduct(invoice->product_id); 

			return format("{0:<12}{1:<20}{2:<16}", invoice->date, limit(product->title, 18), to_string(product->price * invoice->product_count) + "��");
		};

		_template.next_menu_code = MENU_A_INVOICE_INFO;

		_template.Apply(MENU_A_INVOICE_LIST);
	}

	// �ֹ� �� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_A_INVOICE_INFO, new Menu<int>(
		[&](MenuIO& IO, int target_id) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ �ֹ� �� ���� ]");

			Invoice* target = shop_manager.GetInvoice(target_id);
			Product* product = shop_manager.GetProduct(target->product_id);
			Account* account = shop_manager.GetAccount(target->buyer_id);

			IO.print(format("���� ��¥ : {0}\n", target->date));

			IO.print("\n[��ǰ �� ����]\n");
			IO.print(format("���� ��ȣ : {0}\n", product->id));
			IO.print(format("���� : {0}\n", product->title));
			IO.print(format("�帣 : {0}\n", product->genre));
			IO.print(format("���� : {0}\n", product->price));

			IO.print("\n[�ֹ��� ����]\n");
			IO.print(format("���̵� : {0}\n", account->id));
			IO.print(format("�̸� : {0}\n", account->name));
			IO.print(format("��ȭ��ȣ : {0}\n", target->recipient_phone_number));
			IO.print(format("�ּ� : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("��ǰ ���� ��ȣ : {0}\n", target->product_id));
			IO.print(format("��ǰ ���� : {0}\n", target->product_count));
			IO.print(format("���� �ݾ� : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.print("�ƹ� Ű�� �Է��ϼ���\n");

			auto checkpoint = IO.checkpoint();
			string input = IO.input();

		}
	));
	#pragma endregion

#pragma endregion

#pragma region ������ �޴�ȭ��
	// ������ �޴�ȭ��
	{
		TemplateMenuSelection _template;
		_template.SetName("������ �޴�ȭ��");
		_template.ToggleCommand('l', 'q');
		_template.SubMenu("��ǰ ���", []() { menu_manager.RunMenu(MENU_B_PRODUCT_LIST, shop_manager.GetProdcutList()); });
		_template.SubMenu("���� ���� ����", []() { menu_manager.RunMenu(MENU_B_ACCOUNT_INFO, shop_manager.GetUser()); });
		_template.Apply(MENU_BUYER);
	}

	// ��ǰ ��� �޴�ȭ��
	{
		TemplateTable<Product*> _template;
		_template.SetName("��ǰ ���");
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<16}{4:<8}", "ID", "��ǰ", "�帣", "����", "���");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<16}{4:<8}", product->id, limit(product->title, 18), product->genre, product->price, product->count);
		};
		_template.SubMenu('p', MENU_B_PRODUCT_SEARCH, "��ǰ �˻� �� �帣 ����");

		_template.next_menu_code = MENU_B_PRODUCT_INFO;

		_template.Apply(MENU_B_PRODUCT_LIST);
	}

	// ��ǰ �� ���� Ȯ�� �޴�ȭ��
	menu_manager.AppendMenu(MENU_B_PRODUCT_INFO, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ��� ���� ]");

			IO.print(format("������ȣ : {0}\n", target->id));
			IO.print(format("���� : {0}\n", target->title));
			IO.print(format("�帣 : {0}\n", target->genre));
			IO.print(format("���� : {0}\n", target->price));
			IO.print(format("��� : {0}\n", target->count));

			IO.print_line();

			IO.print("����(B)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "B") {
					menu_manager.RunMenu(MENU_B_PRODUCT_BUY, target);
				}
				else {
					IO.print("��ġ�ϴ� ���ɾ ����.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));
	// ��ǰ ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_B_PRODUCT_BUY, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ���� ]");

			string input;
			Account* user = shop_manager.GetUser();
			vector<string> invoice = { user->id,user->phone_number,user->address, "23.10.29", to_string(target->id)};
			IO.print_line();
			IO.print("[��ǰ ����]\n");
			IO.print("[��ǰ �� ����]\n");
			IO.print(format("������ȣ : {0}\n", target->id));
			IO.print(format("���� : {0}\n", target->title));
			IO.print(format("�帣 : {0}\n", target->genre));
			IO.print(format("���� : {0}\n", target->price));
			IO.print_line();
			auto checkpoint = IO.checkpoint();
			while (true) {
				input = IO.input("�ֹ� ������ �Է��ϼ���");
				if (data_manager.GetParser("invoice_product_count")->Check(input)) {
					if (target->count >= stoi(input))
					{
						invoice.push_back(input);
						break;
					}
				}
				IO.print(data_manager.GetParser("invoice_product_count")->msg_error);
				IO.pause();
				IO.rollback(checkpoint);
			}
			IO.print_line();
			IO.print("[�ֹ��� ����]\n");
			IO.print(format("�̸� : {0}\n", user->name));
			IO.print(format("��ȭ��ȣ : {0}\n", user->phone_number));
			IO.print(format("�ּ� : {0}\n", user->address));
			IO.print_line();
			IO.print("[�����ݾ�]\n");
			IO.print(format("{0}�� X {1}�� = {2}��\n", target->price, stoi(input), target->price * stoi(input)));
			IO.print_line();
			input = IO.input("��ǰ�� �ֹ��Ͻðڽ��ϱ�?(y/n)");
			if (input == "y") {
				int id = 7 + shop_manager.GetInvoiceList().size(); //id ���� ��Ŀ����??
				invoice.insert(invoice.begin(), to_string(id));
				shop_manager.AddInvoice(new Invoice(invoice));
				user->invoice_id_list.push_back(id);
				IO.pause();
				IO.print("��ǰ�� �����Ͽ����ϴ�.\n");
			}
			menu_manager.RunMenu(MENU_B_PRODUCT_INFO, target);
		}
	));


	// ���� ���� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_B_ACCOUNT_INFO, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� ���� ���� ]");

			IO.print(format("�̸� : {0}\n", target->name));
			IO.print(format("���̵� : {0}\n", target->id));
			IO.print(format("��ȭ��ȣ : {0}\n", target->phone_number));
			IO.print(format("�ּ� : {0}\n", target->address));

			IO.print_line();

			IO.print("�ֹ� ó�� ����(O) ����(M)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "O") {
					menu_manager.RunMenu(MENU_B_INVOICE_LIST, target->invoice_id_list);
				}
				else if (input == "M") {
					menu_manager.RunMenu(MENU_B_ACCOUNT_INFO_M, target);
				}
				else {
					IO.print("��ġ�ϴ� ���ɾ ����.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}

		}
	));
	// ���� ���� ���� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_B_ACCOUNT_INFO_M, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� ���� ���� ���� ]");

			auto checkpoint = IO.checkpoint();
			vector<string> account;
			string input, match_p;
			vector<string> heads = { "��й�ȣ","��й�ȣ Ȯ��","��ȭ��ȣ","�ּ�" };
			vector<string> regex_t = { "account_password","","account_phonenumber","account_address" };
			int t = 0;
			while (t < heads.size()) {
				if (t != 1) {
					checkpoint = IO.checkpoint();
					input = IO.input("", heads[t]);
					if (data_manager.GetParser(regex_t[t])->Check(input) || input._Equal("")) {
						account.push_back(input);
						t++;
					}
					else
					{
						IO.print(data_manager.GetParser(regex_t[t])->msg_error);
						IO.pause();
						IO.rollback(checkpoint);
					}
				}
				else {
					checkpoint = IO.checkpoint();
					match_p = IO.input("", heads[t]);
					if (input._Equal(match_p))
						t++;
					else
					{
						IO.print("'��й�ȣ Ȯ��'�� '��й�ȣ'�� ��ġ���� �ʽ��ϴ�");
						IO.pause();
						IO.rollback(checkpoint);
					}
				}
			}
			input = IO.input("���� ������ �����Ͻðڽ��ϱ�?(y/n)", "");
			if (input == "y") {
				target->password = account[0]._Equal("") ? target->password : account[0];
				target->phone_number = account[1]._Equal("") ? target->phone_number : account[1];
				target->address = account[2]._Equal("") ? target->address : account[2];
				IO.pause();
			}
			menu_manager.RunMenu(MENU_B_ACCOUNT_INFO, target);
		}
	));


	// ���� ���� �޴�ȭ��
	{
		TemplateTable<int> _template;
		_template.SetName("���� ����");
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<16}", "���� ��¥", "����", "���� �ݾ�");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager.GetInvoice(id);
			Product* product = shop_manager.GetProduct(invoice->product_id);

			return format("{0:<12}{1:<20}{2:<16}", invoice->date, limit(product->title, 18), to_string(product->price * invoice->product_count) + "��");
		};

		_template.next_menu_code = MENU_B_INVOICE_INFO;

		_template.Apply(MENU_B_INVOICE_LIST);
	}

	// �ֹ� �� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_B_INVOICE_INFO, new Menu<int>(
		[&](MenuIO& IO, int target_id) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ �ֹ� �� ���� ]");

			Invoice* target = shop_manager.GetInvoice(target_id);
			Product* product = shop_manager.GetProduct(target->product_id);
			Account* account = shop_manager.GetAccount(target->buyer_id);

			IO.print(format("���� ��¥ : {0}\n", target->date));

			IO.print("\n[��ǰ �� ����]\n");
			IO.print(format("���� ��ȣ : {0}\n", product->id));
			IO.print(format("���� : {0}\n", product->title));
			IO.print(format("�帣 : {0}\n", product->genre));
			IO.print(format("���� : {0}\n", product->price));

			IO.print("\n[�ֹ��� ����]\n");
			IO.print(format("���̵� : {0}\n", account->id));
			IO.print(format("�̸� : {0}\n", account->name));
			IO.print(format("��ȭ��ȣ : {0}\n", target->recipient_phone_number));
			IO.print(format("�ּ� : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("��ǰ ���� ��ȣ : {0}\n", target->product_id));
			IO.print(format("��ǰ ���� : {0}\n", target->product_count));
			IO.print(format("���� �ݾ� : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.pause();
			menu_manager.RunMenu(MENU_B_INVOICE_LIST,shop_manager.GetInvoiceList());
		}
	));

#pragma endregion
}
#pragma endregion
