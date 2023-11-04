#include "util.h"

#include "program.h"

#include <set>
#include <format>

DataManager* const Program::data_manager = new DataManager();
MenuManager* const Program::menu_manager = new MenuManager();
ShopManager* const Program::shop_manager = new ShopManager();

void Program::CleanUp()
{
	delete data_manager;
	delete menu_manager;
	delete shop_manager;
}

void Program::Run()
{
	atexit(SaveCSV);

	LoadCSV();
	SetCommand();
	SetParser();
	SetMenu();

	menu_manager->Start(MENU_START);
}

void Program::LoadCSV()
{
	// Product CSV ������ �Ľ��մϴ�.
	auto product_raw = data_manager->ParseCSV("./data/product.csv");
	for (size_t i = 0; i < product_raw.size(); i++) {
		// ������ Product ��ü�� shop_manager�� �߰��մϴ�.
		auto product = new Product(product_raw[i]);

		shop_manager->AddProduct(product);
		shop_manager->EnableProduct(product);
	}

	// Account CSV ������ �Ľ��մϴ�.
	auto account_raw = data_manager->ParseCSV("./data/account.csv");
	for (size_t i = 0; i < account_raw.size(); i++) {
		// ������ Account ��ü�� shop_manager�� �߰��մϴ�.
		shop_manager->AddAccount(new Account(account_raw[i]));
	}

	// Invoice CSV ������ �Ľ��մϴ�.
	auto invoice_raw = data_manager->ParseCSV("./data/invoice.csv");
	
	for (size_t i = 0; i < invoice_raw.size(); i++) {
		// ������ Invoice ��ü�� shop_manager�� �߰��մϴ�.
		shop_manager->AddInvoice(new Invoice(invoice_raw[i]));
	}
}

void Program::SaveCSV() {
	vector<vector<string>>raw_data;

	auto product_list = shop_manager->GetProductList(true);
	for (size_t i = 0; i < product_list.size(); i++) {
		raw_data.push_back(product_list[i]->ToArray());
	}
	data_manager->SaveCSV("./data/product.csv", raw_data);

	raw_data.clear();

	for (size_t i = 0; i < shop_manager->GetAccountList().size(); i++) {
		raw_data.push_back(shop_manager->GetAccountList()[i]->ToArray());
	}
	data_manager->SaveCSV("./data/account.csv", raw_data);

	raw_data.clear();

	for (size_t i = 0; i < shop_manager->GetInvoiceList().size(); i++) {
		raw_data.push_back(shop_manager->GetInvoiceList()[i]->ToArray());
	}
	data_manager->SaveCSV("./data/invoice.csv", raw_data);
}

void Program::SetCommand()
{
	menu_manager->AppendCommand('z', new MenuManager::Command("�ڷΰ���", [=]() {
		menu_manager->RunPreviousMenu();
	}));

	menu_manager->AppendCommand('l', new MenuManager::Command("�α׾ƿ�", [=]() {
		menu_manager->RunRecursiveMenu(MENU_LOGOUT);
	}));

	menu_manager->AppendCommand('q', new MenuManager::Command("����", [=]() {
		menu_manager->RunRecursiveMenu(MENU_QUIT);
	}));
}

#pragma region Parser ����
void Program::SetParser()
{

#pragma region ��ǰ ��� ���� Parser

	// ��ǰ ��� ���� ���� ��ȣ
	data_manager->AppendParser("product_id", (new Parser())
		->set_label("������ȣ")
		->set_regex(LR"(^[0-9]{6}$)")
		->set_msg_error("���ڷ� ������ ���̰� 6�� ���ڿ��̾�� �մϴ�.\n")
	);

	// ��ǰ ��� ���� ����
	data_manager->AppendParser("product_title", (new Parser())
		->set_label("����")
	);

	// ��ǰ ��� ���� �帣
	data_manager->AppendParser("product_genre", (new Parser())
		->set_label("�帣")
	);

	// ��ǰ ��� ���� �帣
	data_manager->AppendParser("product_author", (new Parser())
		->set_label("����")
	);

	// ��ǰ ��� ���� ����
	data_manager->AppendParser("product_price", (new Parser())
		->set_label("����")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.\n")
	);

	// ��ǰ ��� ���� ���
	data_manager->AppendParser("product_count", (new Parser())
		->set_label("���")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.\n")
	);
#pragma endregion

#pragma region �� ���� ���� Parser

	// �� ���� ���� �̸�
	data_manager->AppendParser("account_name", (new Parser())
		->set_label("�̸�")
		->set_regex(LR"(^[ ]*[a-zA-Z��-�R]+[ a-zA-Z��-�R]*$)")
		->set_msg_error("�̸��� ����, ������, �ѱ� ����� ������ ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.\n")
	);

	// �� ���� ���� ���̵�
	data_manager->AppendParser("account_id", (new Parser())
		->set_label("���̵�")
		->set_regex(LR"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("���̵�� ����, �����ڷ� ������ ���̰� 4 �̻� 12 ������ ���ڿ��̾�� �մϴ�.\n")
	);

	// �� ���� ���� ��й�ȣ
	data_manager->AppendParser("account_password", (new Parser())
		->set_label("��й�ȣ")
		->set_regex(LR"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("��й�ȣ�� ����, ������, Ư�� ����(!@#$%^&*_)�� ������ ���̰� 8 �̻� 16 ������ ���ڿ��̾�� �մϴ�.\n")
	);

	// �� ���� ���� ��ȭ��ȣ
	data_manager->AppendParser("account_phone_number", (new Parser())
		->set_label("��ȭ��ȣ")
		->set_regex(LR"(^010[0-9]{8}$)")
		->set_msg_error("��ȭ��ȣ�� ���ڷ� �����Ǿ� 010���� �����ϰ�, ���̰� 11�� ���ڿ��̾�� �մϴ�.\n")
	);

	// �� ���� ���� �ּ�
	data_manager->AppendParser("account_address", (new Parser())
		->set_label("�ּ�")
	);
	data_manager->AppendParser("account_coupon_count", (new Parser())
		->set_label("���� ���� ����")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("���� ���� ������ ���ڷ� ������ ���ڿ��̾�� �մϴ�.\n")
	);
	data_manager->AppendParser("account_accumulated", (new Parser())
		->set_label("����Ȯ�� ���� �ݾ�")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("����Ȯ�� ���� �ݾ��� ���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.\n")
	);
#pragma endregion

#pragma region �ֹ� ó�� ���� Parser
	// �ֹ� ó�� ���� ������ȣ
	data_manager->AppendParser("invoice_id", (new Parser())
		->set_label("������ȣ")
		->set_regex(LR"(^[0-9]{8}$)")
		->set_msg_error("���ڷ� ������ ���̰� 8�� ���ڿ��̾�� �մϴ�.\n")
	);

	// �ֹ� ó�� ���� �������� ���̵�
	data_manager->AppendParser("invoice_buyer_id", (new Parser())
		->set_label("�������� ���̵�")
		->set_msg_info("�������� ���̵� �Դϴ�.\n")
	);

	// �ֹ� ó�� ���� ���� ��¥
	data_manager->AppendParser("invoice_date", (new Parser())
		->set_label("���� ��¥")
	);

	// �ֹ� ó�� ���� ��ǰ ���� ��ȣ
	data_manager->AppendParser("invoice_product_id", (new Parser())
		->set_label("��ǰ ���� ��ȣ")
		->set_msg_info("������ ��ǰ ���� ��ȣ �Դϴ�.\n")
	);

	// �ֹ� ó�� ���� ��ǰ ����
	data_manager->AppendParser("invoice_product_count", (new Parser())
		->set_label("��ǰ ����")
		->set_regex(LR"(^[0-9]*[1-9]+[0-9]*$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.\n")
	);
#pragma endregion


	data_manager->AppendParser("MENU_MENUSELECTION", (new Parser())
		->set_regex(LR"(\d)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ������.\n")
	);

	data_manager->AppendParser("MENU_TABLE", (new Parser())
		->set_regex(LR"(^[0-9a-zA-Z]$)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �Ǵ� ���ĺ� �� �ϳ��� ������.\n")
	);
}
#pragma endregion

#pragma region �޴�ȭ�� ����

void Program::SetMenu()
{
	menu_manager->SetOnRefresh([=]() {
		auto invoice_list = shop_manager->GetInvoiceList();

		auto now = today();

		for (int i = 0; i < invoice_list.size(); i++) {
			int index = (int)invoice_list.size() - i - 1;

			Invoice* invoice = invoice_list[index];

			// �ֹ� Ȯ���� ���� ��� �� �̻� ������ �ʿ䰡 �����ϴ�.
			if (invoice->state != CONFIRMED) {
				// ȯ���� ������ ��츦 �����ϰ� ���� Ȯ������ �����մϴ�.
				if (invoice->state == PURCHASED) {
					// 7���� ���� ����� �ֹ� ó�� �������� Ȯ���մϴ�. 
					if (day_diff(invoice->date, now) >= 7) {
						invoice->state = CONFIRMED;

						// ���� ó��
						Account* account = shop_manager->GetAccount(invoice->buyer_id);

						account->accumulated += invoice->final_price;

						while (account->accumulated >= 50000) {
							account->accumulated -= 50000;

							account->coupon_count += 1;
						}
					}
				}
			}
			else {
				break;
			}
		}
	});

	menu_manager->SetCommonHeader([=](MenuIO& IO) {
		IO.print_aligned_center("# ���� ���ͳ� ���θ� #");
	});

#pragma region ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_QUIT, new Menu([=](MenuIO& IO) {
		menu_manager->ToggleCommand();
		menu_manager->PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ ���α׷� ���� ]");
		IO.print("���α׷��� �����Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit(0);
		}
		}));

	menu_manager->AppendMenu(MENU_LOGOUT, new Menu([=](MenuIO& IO) {
		menu_manager->ToggleCommand();
		menu_manager->PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ ���� �α׾ƿ� ]");
		IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			shop_manager->Logout();

			menu_manager->SetCommonHeader([=](MenuIO& IO) {
				IO.print_aligned_center("# ���� ���ͳ� ���θ� #");
			});
			menu_manager->RunMenu(MENU_START);
		}
		}));
#pragma endregion


#pragma region ���� �޴�ȭ��
	// ���� �޴�ȭ��
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('q');
		_template.SetName("���� �޴�");
		_template.SubMenu("���� �α���",   [=]() { menu_manager->RunMenu(MENU_LOGIN);  });	
		_template.SubMenu("���� ȸ������", [=]() { menu_manager->RunMenu(MENU_SIGNUP); });
		_template.Apply(MENU_START);
	} 

	// ���� �α��� �޴�ȭ��
	menu_manager->AppendMenu(MENU_LOGIN, new Menu(
		[=](MenuIO& IO) { 
			menu_manager->ToggleCommand('z', 'q');
			menu_manager->PrintCommand();
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
				if ((user = shop_manager->Login(id, password)) != nullptr) {
					if (user == shop_manager->GetAdminAccount()) {
						menu_manager->SetCommonHeader([=](MenuIO& IO) {
							IO.print_aligned_center("�����ڴ��� �����Ͽ����ϴ�.");
						});
						menu_manager->RunMenu(MENU_ADMIN);
					}
					else {
						menu_manager->SetCommonHeader([=](MenuIO& IO) {
							IO.print_aligned_center(format("{}���� �����Ͽ����ϴ�.", shop_manager->GetCurrentAccount()->name));
						});
						menu_manager->RunMenu(MENU_BUYER);
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
	menu_manager->AppendMenu(MENU_SIGNUP, new Menu(
		[=](MenuIO& IO) {
			menu_manager->ToggleCommand('z', 'q');
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ���� ȸ������ ]");

			vector<string> account;
			
			vector<string> parser_key = { "account_name", "account_id", "account_password", "", "account_phone_number", "account_address" };

			for (int i = 0; i < parser_key.size(); i++) {				
				if (i == 1) {
					// '���̵�' �Է�
					auto checkpoint = IO.checkpoint();

					while (true) {
						string input = IO.input(data_manager->GetParser(parser_key[i]));

						// �Էµ� ���̵� ������ ������ �ش��ϴ��� Ȯ���մϴ�.
						if (input != shop_manager->GetAdminAccount()->id) {
							// �Էµ� ���̵� �� ���� ������ �ִ��� Ȯ���մϴ�.
							if (shop_manager->GetAccount(input) == nullptr) {
								account.push_back(input);

								break;
							}
							else {
								IO.print("�̹� �����ϴ� ���̵��Դϴ�.\n");
								IO.pause();

								IO.rollback(checkpoint);
							}
						}
						else {
							IO.print(format("������ ���̵� '{}'�� ���̵�� ����� �� �����ϴ�.\n", shop_manager->GetAdminAccount()->id));
							IO.pause();

							IO.rollback(checkpoint);
						}
					}
				}
				else if (i == 3) {
					// '��й�ȣ Ȯ��' �Է�
					auto checkpoint = IO.checkpoint();

					while (true) {
						string input = IO.input("", "��й�ȣ Ȯ��");

						if (input == account[2]) {
							break;
						}
						else
						{
							IO.print("'��й�ȣ Ȯ��'�� '��й�ȣ'�� ��ġ���� �ʽ��ϴ�.\n");
							IO.pause();

							IO.rollback(checkpoint);
						}
					}
				}
				else {
					// ������ ������ ��� �Է¿� ���� ������ Ȯ���ϰ� �����մϴ�.
					account.push_back(IO.input(data_manager->GetParser(parser_key[i])));
				}
			}
			
			//�ֹ� ó�� ���� ���
			account.push_back("");
			account.push_back("0");
			account.push_back("0"); 

			string input = IO.input("\n������ �����Ͻðڽ��ϱ�? (y / n)");

			if (input == "y") {
				shop_manager->AddAccount(new Account(account));
			
				IO.print("\n������ �����Ǿ����ϴ�.\n");
				IO.pause();
			}
			 
			menu_manager->RunMenu(MENU_START);
		}
	));
#pragma endregion

#pragma region ������ �޴�ȭ��
	// ������ �޴�ȭ��
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('l', 'q'); 
		_template.SetName("������ �޴�ȭ��");
		_template.SubMenu("��ǰ ���� (���/����/����, ������)", [=]() { menu_manager->RunMenu(MENU_A_PRODUCT_LIST, shop_manager->GetProductList()); });
		_template.SubMenu("�� ���� (����, �ֹ�)",				  [=]() { menu_manager->RunMenu(MENU_A_ACCOUNT_LIST, shop_manager->GetAccountList()); });
		_template.Apply(MENU_ADMIN);
	}

	#pragma region ��ǰ ���
	// ��ǰ ��� �޴�ȭ��
	{
		TemplateTable<Product*> _template;
		_template.SetName("��ǰ ���");

		// ���̺� ��� ���� ���� 
		_template.header_func = []() -> string {
			return format("{0:<8}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", "ID", "��ǰ", "�帣", "����", "����", "���");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<8}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", format("{:06}", product->id), limit(product->title, 18), product->genre, product->author, to_string(product->price) + "��", product->count);
		};

		// �޴� �߰� �� ���
		_template.SubMenu('p', "��ǰ �˻� �� �帣 ����", []() {
			set<string> genre_set;

			const vector<Product*>& product_list = shop_manager->GetProductList(); 

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				genre_set.insert(product->genre);
			}

			static vector<string> genre_list; genre_list = vector<string>(genre_set.begin(), genre_set.end());

			for (int i = 0; i < genre_list.size(); i += 10) {
				genre_list.insert(genre_list.begin() + i, "��ü");
			}

			menu_manager->RunMenu(MENU_A_PRODUCT_SEARCH, genre_list);
		});

		_template.SubMenu('r', "��ǰ �ű� ���",         [=]() { menu_manager->RunMenu(MENU_A_PRODUCT_REGISTER);  });

		_template.next_menu_code = MENU_A_PRODUCT_INFO;

		_template.Apply(MENU_A_PRODUCT_LIST);
	}  

	// ��ǰ �˻� �� �帣 ���� �޴�ȭ��
	{
		TemplateTable<string> _template;
		_template.SetName("��ǰ �˻� �� �帣 ����");
		_template.header_func = []() -> string {
			return "�帣";
		};
		_template.show_func = [](string genre) -> string {
			return genre;
		};
		 
		_template.next_menu_code = MENU_A_PRODUCT_LIST; 

		_template.process_func = [=](MenuIO& IO, MenuCode next_menu_code, string genre) {
			string input = IO.input("", "����");

			const vector<Product*>& product_list = shop_manager->GetProductList();

			static vector<Product*> result; result.clear();

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				if ((input == "" || product->title == input) && (genre == "��ü" || product->genre == genre)) {
					result.push_back(product);
				}
			}
			 
			menu_manager->RunMenu(next_menu_code, result);
		};

		_template.Apply(MENU_A_PRODUCT_SEARCH);
	}

	// ��ǰ �ű� ��� �޴�ȭ��
	menu_manager->AppendMenu(MENU_A_PRODUCT_REGISTER, new Menu(
		[=](MenuIO& IO) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ �ű� ��� ]");

			if (shop_manager->GetProductList().size() > 999999) {
				IO.print("�� �̻� ��ǰ�� ����� �� �����ϴ�.\n");
				IO.pause();
				
				return;
			}

			vector<string> product;

			vector<string> parser_key = { "product_title", "product_genre", "product_author", "product_price", "product_count" };
			
			int id = (int)shop_manager->GetProductList().size();

			product.push_back(to_string(id));

			auto checkpoint = IO.checkpoint();

			for (int i = 0; i < parser_key.size(); i++) {
				product.push_back(IO.input(data_manager->GetParser(parser_key[i])));

				if (i == 2) {
					if (!shop_manager->IsProductRegisterable(-1, product[1], product[3])) {
						IO.print("���� ������ ���������� ���ڱ��� ���� �� �����ϴ�.\n");
						IO.pause();

						IO.rollback(checkpoint);

						product.clear();
						product.push_back(to_string(id));
						
						i = -1;	continue;
					}
				}
			}

			product.push_back("FALSE");

			string input = IO.input("\n��ǰ�� ����Ͻðڽ��ϱ�? (y / n)");

			if (input == "y") {
				Product* result = new Product(product);

				shop_manager->AddProduct(result);
				shop_manager->EnableProduct(result);

				IO.print(format("\n��ǰ({})�� ��ϵǾ����ϴ�.\n", id));
				IO.pause();

				// ��ǰ�� ����� ��� ���� �޴��� ���ư� ��ü ��ǰ ��� ������ �����ݴϴ�.
				menu_manager->RunMenu(MENU_A_PRODUCT_LIST, shop_manager->GetProductList());
			}

			// ��ǰ�� ������� ���� ��� ���� �˻� ���¸� �����ϸ鼭 ���� �޴��� �����ݴϴ�.
		} 
	));

	// ��ǰ ��� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_A_PRODUCT_INFO, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) { 
			menu_manager->PrintCommand(); 
			IO.print_line(); 
			IO.print_aligned_center("[ ��ǰ ��� ���� ]");
			IO.print(target->ToString());
			IO.print(format("��� : {0}\n", target->count));

			IO.print_line();

			IO.print_aligned_center("����(M)     ����(R)");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input.size() == 1) {
					char command = tolower(input[0]);

					if (command == 'm') {
						menu_manager->RunMenu(MENU_A_PRODUCT_INFO_M, target);
					}
					else if (command == 'r') { 
						menu_manager->RunMenu(MENU_A_PRODUCT_INFO_R, target);
					}
				} 

				IO.print("�޴��� ǥ�õ� ���ĺ� �� �ϳ��� ������.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
		}
	));

	// ��ǰ ��� ���� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_A_PRODUCT_INFO_M, new Menu<Product*>(
		[=](MenuIO& IO, Product* target)  {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ��� ���� ���� ]");

			vector<string> product = target->ToArray();

			vector<string> parser_key = { "product_title", "product_genre", "product_author", "product_price", "product_count" };

			auto checkpoint_start = IO.checkpoint();

			for (int i = 0; i < parser_key.size(); i++) { 
				auto checkpoint = IO.checkpoint();

				while (true) { 
					Parser* parser = data_manager->GetParser(parser_key[i]);

					string input = IO.input("", parser->label);
					if (input == "" || parser->Check(input)) { 
						if (input != "") {
							product[i + 1] = input;
						}
						else {  
							IO.rollback(checkpoint);

							IO.print(format("{} : {}", parser->label, product[i + 1] + "\n"));
						}

						break;
					}
					else
					{
						IO.print(parser->msg_error);
						IO.pause();

						IO.rollback(checkpoint);
					}
				} 

				if (i == 2) {
					if (!shop_manager->IsProductRegisterable(target->id, product[1], product[3])) {
						IO.print("���� ������ ���������� ���ڱ��� ���� �� �����ϴ�.\n");
						IO.pause();

						IO.rollback(checkpoint_start);

						i = -1;	continue;
					}
				}
			}

			string input = IO.input("\n��ǰ ��� ������ �����Ͻðڽ��ϱ�? (y / n)");

			if (input == "y") {
				*target = Product(product);

				IO.print(format("��ǰ({})�� ��� ������ �����Ǿ����ϴ�.\n", target->id));

				IO.pause();
			}

			menu_manager->RunMenu(MENU_A_PRODUCT_INFO, target);
		}
	));

	// ��ǰ ��� ���� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_A_PRODUCT_INFO_R, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ��� ���� ���� ]");

			string input = IO.input("��ǰ ��� ������ �����Ͻðڽ��ϱ�? (y / n)");

			if (input == "y") {
				shop_manager->DisableProduct(target);

				IO.print(format("\n��ǰ({0})�� ��� ������ ���ŵǾ����ϴ�.\n", target->id));
				IO.pause();

				menu_manager->RunMenu(MENU_A_PRODUCT_LIST, shop_manager->GetProductList());
			}
		}
	));
	#pragma endregion 

	#pragma region �� ���
	// �� ��� �޴�ȭ�� 
	{
		TemplateTable<Account*> _template;
		_template.SetName("�� ���");
		_template.header_func = []() -> string {
			return format("{0:<16}{1:<8}{2:<16}", "���̵�", "�̸�", "��ȭ��ȣ");
		};
		_template.show_func = [](Account* product) -> string {
			return format("{0:<16}{1:<8}{2:<16}", product->id, product->name, phone_number(product->phone_number));
		};
		_template.SubMenu('p', "�� �˻�", [=]() { menu_manager->RunMenu(MENU_A_ACCOUNT_SEARCH);  });

		_template.next_menu_code = MENU_ACCOUNT_INFO;

		_template.Apply(MENU_A_ACCOUNT_LIST);
	}

	// �� �˻� �޴�ȭ��
	menu_manager->AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[=](MenuIO& IO) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ �� �˻� ]");
			
			Account* target;

			auto checkpoint = IO.checkpoint();

			while (true) {

				string input = IO.input("���̵� �Է��ϼ���.");

				if (input == shop_manager->GetAdminAccount()->id) {
					IO.print("�ش� ������ ������ �����Դϴ�.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
				else {
					if ((target = shop_manager->GetAccount(input)) != nullptr) {
						menu_manager->RunMenu(MENU_ACCOUNT_INFO, target);
					}
					else
					{
						IO.print("���̵� �������� �ʽ��ϴ�.\n");
						IO.pause();

						IO.rollback(checkpoint);
					}
				}
			}
		}
	));
	
	// �� ���� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_ACCOUNT_INFO, new Menu<Account*>(
		[=](MenuIO& IO, Account* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ �� ���� ���� ]");

			IO.print(format("�̸� : {0}\n", target->name));
			IO.print(format("���̵� : {0}\n", target->id));
			IO.print(format("��ȭ��ȣ : {0}\n", phone_number(target->phone_number)));
			IO.print(format("�ּ� : {0}\n", target->address));

			IO.print("\n");

			IO.print(format("������ 3000�� ���� ���� : {}\n", target->coupon_count));
			IO.print(format("���� �������� ���� �ݾ� : {}��\n", target->accumulated));

			IO.print_line();

			IO.print_aligned_center("�ֹ� ó�� ����(O)     ����(M)");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input.size() == 1) {
					char command = tolower(input[0]);

					if (command == 'o') {
						menu_manager->RunMenu(MENU_INVOICE_LIST, target->invoice_id_list);
					}
					else if (command == 'm') {
						menu_manager->RunMenu(MENU_ACCOUNT_INFO_M, target);
					}
				}

				IO.print("�޴��� ǥ�õ� ���ĺ� �� �ϳ��� ������.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
		}
	));

	// �� ���� ���� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_ACCOUNT_INFO_M, new Menu<Account*>(
		[=](MenuIO& IO, Account* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ �� ���� ���� ���� ]");

			vector<string> orignal = target->ToArray();

			vector<string> account;

			vector<string> parser_key = { "account_password","","account_phone_number","account_address" };

			account.push_back(orignal[0]);
			account.push_back(orignal[1]);

			for (int i = 0; i < parser_key.size(); i++) {
				if (i == 1) {
					// '��й�ȣ Ȯ��' �Է�
					auto checkpoint = IO.checkpoint();

					while (true) {
						string input = IO.input("", "��й�ȣ Ȯ��");

						if (input == account[2]) {
							break;
						}
						else
						{
							IO.print("'��й�ȣ Ȯ��'�� '��й�ȣ'�� ��ġ���� �ʽ��ϴ�.\n");
							IO.pause();

							IO.rollback(checkpoint);
						}
					}
				}
				else {
					// ������ ������ ��� �Է¿� ���� ������ Ȯ���ϰ� �����մϴ�.
					account.push_back(IO.input(data_manager->GetParser(parser_key[i])));
				}
			}

			account.push_back(orignal[5]);
			account.push_back(orignal[6]);
			account.push_back(orignal[7]);

			string input = IO.input("\n���� ������ �����Ͻðڽ��ϱ�? (y / n)");
				
			if (input == "y") {
				*target = Account(account);

				IO.print("\n���� ������ �����Ǿ����ϴ�.\n");
				IO.pause();
			}

			menu_manager->RunMenu(MENU_ACCOUNT_INFO, target);	
		}
	));
	// ���� ���� �޴�ȭ��
	{
		TemplateTable<int> _template;
		_template.SetName("���� ����");
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<8}{3:<12}{4:<10}", "���� ��¥", "����", "����", "���� �ݾ�", "����");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager->GetInvoice(id);
			Product* product = shop_manager->GetProduct(invoice->product_id);

			return format("{0:<12}{1:<20}{2:<8}{3:<12}{4:<10}", date_to_string(invoice->date), limit(product->title, 18), product->author,
															  to_string(product->price * invoice->product_count) + "��", invoice->GetState());
		};

		_template.next_menu_code = MENU_INVOICE_INFO;

		_template.Apply(MENU_INVOICE_LIST);
	}

	// �ֹ� �� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_INVOICE_INFO, new Menu<int>(
		[=](MenuIO& IO, int target_id) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ �ֹ� �� ���� ]");

			Invoice* target = shop_manager->GetInvoice(target_id);
			Product* product = shop_manager->GetProduct(target->product_id);
			Account* account = shop_manager->GetAccount(target->buyer_id);

			IO.print(format("���� ��¥ : {0}\n", date_to_string(target->date)));

			IO.print("\n[ ��ǰ �� ���� ]\n");
			IO.print(product->ToString());

			IO.print("\n[ �ֹ��� ���� ]\n");
			IO.print(format("���̵� : {0}\n", account->id));
			IO.print(format("�̸� : {0}\n", account->name));
			IO.print(format("��ȭ��ȣ : {0}\n", phone_number(target->recipient_phone_number)));
			IO.print(format("�ּ� : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("��ǰ ���� ��ȣ : {0}\n", format("{:06}", target->product_id)));
			IO.print(format("��ǰ ���� : {0}\n", target->product_count));
			IO.print(format("���� �ݾ� : {0}��\n", target->price));
			IO.print(format("����� 3000�� ���� ���� : {0}��\n", target->coupon_count));
			IO.print(format("���� ���� �ݾ� : {0}��\n", target->final_price));
			IO.print("\n");
			IO.print(format("���� : {0}\n", target->GetState()));


			//�޴� ����
			if (shop_manager->IsAdmin() && target->state != PURCHASED) {
				IO.print_line();
				IO.pause();
			}
			else { 
				IO.print_line(false);
				IO.print_aligned_center("��ǰ(R)");
				IO.print_line();

				auto checkpoint = IO.checkpoint();
				string input = IO.input();
				if (input.size() == 1) {
					char command = tolower(input[0]);

					if (command == 'r') {
						menu_manager->RunMenu(MENU_B_REFUND, target);
					}
				}
				IO.print("�޴��� ǥ�õ� ���ĺ� �� �ϳ��� ������.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
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
		_template.SubMenu("��ǰ ���", [=]() { menu_manager->RunMenu(MENU_B_PRODUCT_LIST, shop_manager->GetProductList()); });
		_template.SubMenu("�� ���� ����", [=]() { menu_manager->RunMenu(MENU_ACCOUNT_INFO, shop_manager->GetCurrentAccount()); });
		_template.Apply(MENU_BUYER);
	}
	
	#pragma region ��ǰ ���
	// ��ǰ ��� �޴�ȭ��
	{
		TemplateTable<Product*> _template;
		_template.SetName("��ǰ ���");

		// ���̺� ��� ���� ���� 
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", "ID", "��ǰ", "�帣", "����", "����", "���");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", format("{:06}", product->id), limit(product->title, 18), product->genre, product->author, to_string(product->price) + "��", product->count);
		}; 

		// �޴� �߰� �� ���
		_template.SubMenu('p', "��ǰ �˻� �� �帣 ����", []() {
			set<string> genre_set;

			const vector<Product*>& product_list = shop_manager->GetProductList(); 

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				genre_set.insert(product->genre);
			}

			static vector<string> genre_list; genre_list = vector<string>(genre_set.begin(), genre_set.end());

			for (int i = 0; i < genre_list.size(); i += 10) {
				genre_list.insert(genre_list.begin() + i, "��ü");
			}

			menu_manager->RunMenu(MENU_B_PRODUCT_SEARCH, genre_list);
		});

		_template.next_menu_code = MENU_B_PRODUCT_INFO;

		_template.Apply(MENU_B_PRODUCT_LIST);
	}  

	// ��ǰ �˻� �� �帣 ���� �޴�ȭ��
	{
		TemplateTable<string> _template;
		_template.SetName("��ǰ �˻� �� �帣 ����");
		_template.header_func = []() -> string {
			return "�帣";
		};
		_template.show_func = [](string genre) -> string {
			return genre;
		};
		 
		_template.next_menu_code = MENU_B_PRODUCT_LIST; 

		_template.process_func = [=](MenuIO& IO, MenuCode next_menu_code, string genre) {
			string input = IO.input("", "����");

			const vector<Product*>& product_list = shop_manager->GetProductList();

			static vector<Product*> result; result.clear();

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				if ((input == "" || product->title == input) && (genre == "��ü" || product->genre == genre)) {
					result.push_back(product);
				}
			}
			 
			menu_manager->RunMenu(next_menu_code, result);
		};

		_template.Apply(MENU_B_PRODUCT_SEARCH);
	}
#pragma endregion

	// ��ǰ ��� ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_B_PRODUCT_INFO, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ �� ���� ]");
			IO.print(target->ToString());
			IO.print(format("��� : {0}\n", target->count));

			IO.print_line(false);

			if (target->count > 0) {
				IO.print_aligned_center("����(B)");

				IO.print_line();

				auto checkpoint = IO.checkpoint();

				while (true) {
					string input = IO.input();

					if (input.size() == 1) {
						char command = tolower(input[0]);

						if (command == 'b') {
							menu_manager->RunMenu(MENU_B_PRODUCT_BUY, target);
						}
					}

					IO.print("�޴��� ǥ�õ� ���ĺ� �� �ϳ��� ������.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
			else {
				IO.print_aligned_center("ǰ���Ǿ����ϴ�.");

				IO.print_line();
				IO.pause();
			}
		}
	));

	// ��ǰ ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_B_PRODUCT_BUY, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ���� ]"); 

			if (shop_manager->GetInvoiceList().size() > 99999999) {
				IO.print("�� �̻� ��ǰ�� ������ �� �����ϴ�.\n");
				IO.pause();

				return;
			}

			int count, price, coupon_count, final_price;
			string recipient_phone_number, recipient_address;
			Account* user = shop_manager->GetCurrentAccount();

			vector<string> invoice = { user->id, "", "", "", to_string(target->id)};

			string input;

			IO.print_line();
			IO.print("[ ��ǰ �� ���� ]\n");
			IO.print(target->ToString());
			IO.print(format("��� : {0}\n", target->count));
			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				input = IO.input("�ֹ� ������ �Է��ϼ���.");

				if (data_manager->GetParser("invoice_product_count")->Check(input)) {
					if (target->count >= stoi(input))
					{
						count = stoi(input);

						break;
					}
					else {
						IO.print("�ֹ� ������ �ʰ��߽��ϴ�.\n");
					}
				}
				else {
					IO.print(data_manager->GetParser("invoice_product_count")->msg_error);
				}

				IO.pause();
				IO.rollback(checkpoint);
			}
			IO.print_line();
			IO.print("[ �ֹ��� ���� ]\n");
			IO.print(format("�̸� : {0}\n", user->name));
			 
			checkpoint = IO.checkpoint();
			while (true) {
				input = IO.input("", "��ȭ��ȣ");

				if (data_manager->GetParser("account_phone_number")->Check(input) || input == "") { 
					recipient_phone_number = input == "" ? user->phone_number : input;

					IO.rollback(checkpoint); 
					IO.print(format("{} : {}\n", data_manager->GetParser("account_phone_number")->label, phone_number(recipient_phone_number)));

					break;
				}
				else {
					IO.print(data_manager->GetParser("account_phone_number")->msg_error);
				}

				IO.pause();
				IO.rollback(checkpoint);
			} 

			checkpoint = IO.checkpoint();
			
			recipient_address = IO.input(data_manager->GetParser("account_address"));

			recipient_address = recipient_address == "" ? user->address : recipient_address;

			IO.rollback(checkpoint); 
			IO.print(format("{} : {}\n", data_manager->GetParser("account_address")->label, recipient_address));
 
			IO.print_line(false);
			IO.print("[ �����ݾ� ]\n");
			IO.print(format("{0}�� X {1}�� = {2}��\n", target->price, count, price = target->price*count));
			IO.print_line();

			checkpoint = IO.checkpoint();

			while (true) {
				IO.print_aligned_center("����Ͻ� ���� ������ �Է��ϼ���");
				IO.print_aligned_center(format("���� ���� ���� : {0}��", user->coupon_count));

				string input = IO.input();
				if (data_manager->GetParser("account_coupon_count")->Check(input)) {
					coupon_count = stoi(input);
					if (user->coupon_count >= coupon_count)
					{				
						int max_coupon_count = (int)ceil(float(price) / 3000);
						if (max_coupon_count < coupon_count) //�ִ� ��� ���� �������� ������
						{
							IO.print(format("������ {0}������ ���� ����� �� �����ϴ�\n", max_coupon_count));
						}
						else {
							break;
						}					
					}
					else {
						IO.print("�Է��� ������ ������ ���� ������ �ʰ��߽��ϴ�.\n");
					}
				}
				else {
					IO.print(data_manager->GetParser("account_coupon_count")->msg_error);				
				}
				IO.pause();
				IO.rollback(checkpoint);
			}
			IO.print_line();

			final_price = max(price - 3000 * coupon_count, 0);
			
			IO.print(format("���� ���� �ݾ� : {0}\n", final_price));
			IO.print_line();

			input = IO.input("��ǰ�� �ֹ��Ͻðڽ��ϱ�? (y / n)");

			if (input == "y") {
				int id = (int)shop_manager->GetInvoiceList().size();
				target->count -= count; //��ǰ ���� ������Ʈ
				user->coupon_count -= coupon_count;
				invoice.push_back(to_string(count));
				invoice.push_back(to_string(price));
				invoice.push_back(to_string(coupon_count));
				invoice.push_back(to_string(final_price));
				invoice.push_back("0");
				invoice.insert(invoice.begin(), to_string(id));	

				Invoice* result = new Invoice(invoice);

				result->date = today();
				result->recipient_phone_number = recipient_phone_number;
				result->recipient_address = recipient_address;

				// ��ü ��Ͽ����� �������� �ֹ� ó�� ���� �߰�
				shop_manager->AddInvoice(result);
				// �����Դ� ��¥�� ������ ������ ���̵��� ù��° ��ġ�� �߰�
				user->invoice_id_list.insert(user->invoice_id_list.begin(), id);

				IO.print("\n��ǰ�� �����Ͽ����ϴ�.\n");
				IO.pause();
			}
		}
	));

	//������ ���� ��ǰ
	menu_manager->AppendMenu(MENU_B_REFUND, new Menu<Invoice*>(
		[=](MenuIO& IO, Invoice* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ]");

			Product* product = shop_manager->GetProduct(target->product_id, true);
			Account* account = shop_manager->GetAccount(target->buyer_id);

			IO.print(format("�����ݾ� : {0}��\n", target->price));
			IO.print(format("����� 3000�� ���� ���� : {0}��\n", target->coupon_count));
			IO.print(format("���� �����ݾ� : {0}��\n", target->final_price));

			IO.print_line();

			string input = IO.input("�ش� ��ǰ�� ��ǰ�Ͻðڽ��ϱ�? (y / n)\n(����� ������ �������� �� ����, ���� �����ݾ׸� �����޽��ϴ�.)");

			if (input == "y") {
				if (product->deleted) {
					shop_manager->EnableProduct(product);
				} 
				product->count += target->product_count;

				target->state = REFUNDED;

				IO.print("\n�ش� ��ǰ�� ��ǰ�Ǿ����ϴ�.\n");
				IO.pause();

			}
			menu_manager->RunMenu(MENU_INVOICE_LIST, account->invoice_id_list);
		}
	));
#pragma endregion
}
#pragma endregion

