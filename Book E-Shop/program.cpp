#include "program.h"
#include "Util.h"

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
		shop_manager->AddProduct(new Product(product_raw[i]));
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

	for (size_t i = 0; i < shop_manager->GetProductList().size(); i++) {
		raw_data.push_back(shop_manager->GetProductList()[i]->ToArray());
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
		->set_msg_error("���ڷ� ������ ���̰� 6�� ���ڿ��̾�� �մϴ�.")
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
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.")
	);

	// ��ǰ ��� ���� ���
	data_manager->AppendParser("product_count", (new Parser())
		->set_label("���")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�.")
	);
#pragma endregion

#pragma region �� ���� ���� Parser

	// �� ���� ���� �̸�
	data_manager->AppendParser("account_name", (new Parser())
		->set_label("�̸�")
		->set_regex(LR"(^[ ]*[a-zA-Z��-�R]+[ a-zA-Z��-�R]*$)")
		->set_msg_error("�̸��� ����, ������, �ѱ� ����� ������ ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�!")
	);

	// �� ���� ���� ���̵�
	data_manager->AppendParser("account_id", (new Parser())
		->set_label("���̵�")
		->set_regex(LR"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("���̵�� ����, �����ڷ� ������ ���̰� 4 �̻� 12 ������ ���ڿ��̾�� �մϴ�!")
	);

	// �� ���� ���� ��й�ȣ
	data_manager->AppendParser("account_password", (new Parser())
		->set_label("��й�ȣ")
		->set_regex(LR"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("��й�ȣ�� ����, ������, Ư�� ����(!@#$%^&*_)�� ������ ���̰� 8 �̻� 16 ������ ���ڿ��̾�� �մϴ�!")
	);

	// �� ���� ���� ��ȭ��ȣ
	data_manager->AppendParser("account_phonenumber", (new Parser())
		->set_label("��ȭ��ȣ")
		->set_regex(LR"(^010[0-9]{8}$)")
		->set_msg_error("��ȭ��ȣ�� ���ڷ� �����Ǿ� 010���� �����ϰ�, ���̰� 11�� ���ڿ��̾�� �մϴ�!")
	);

	// �� ���� ���� �ּ�
	data_manager->AppendParser("account_address", (new Parser())
		->set_label("�ּ�")
	);
#pragma endregion

#pragma region �ֹ� ó�� ���� Parser
	// �ֹ� ó�� ���� ������ȣ
	data_manager->AppendParser("invoice_id", (new Parser())
		->set_label("������ȣ")
		->set_regex(LR"(^[0-9]{8}$)")
		->set_msg_error("���ڷ� ������ ���̰� 8�� ���ڿ��̾�� �մϴ�")
	);

	// �ֹ� ó�� ���� �������� ���̵�
	data_manager->AppendParser("invoice_buyer_id", (new Parser())
		->set_label("�������� ���̵�")
		->set_msg_info("�������� ���̵� �Դϴ�.")
	);

	// �ֹ� ó�� ���� �������� ��ȭ��ȣ
		data_manager->AppendParser("invoice_recipient_phonenumber", (new Parser())
		->set_label("�������� ��ȭ��ȣ")
		->set_msg_info("�������� ��ȭ��ȣ �Դϴ�.")
	);

	// �ֹ� ó�� ���� �������� �ּ�
	data_manager->AppendParser("invoice_recipient_address", (new Parser())
		->set_label("�������� �ּ�")
		->set_msg_info("�������� �ּ� �Դϴ�.")
	);

	// �ֹ� ó�� ���� ���� ��¥
	data_manager->AppendParser("invoice_date", (new Parser())
		->set_label("���� ��¥")
	);

	// �ֹ� ó�� ���� ��ǰ ���� ��ȣ
	data_manager->AppendParser("invoice_product_id", (new Parser())
		->set_label("��ǰ ���� ��ȣ")
		->set_msg_info("������ ��ǰ ���� ��ȣ �Դϴ�.")
	);

	// �ֹ� ó�� ���� ��ǰ ����
	data_manager->AppendParser("invoice_product_count", (new Parser())
		->set_label("��ǰ ����")
		->set_regex(LR"(^[1-9][0-9]*$)")
		->set_msg_error("���ڷ� ������ ���̰� 1 �̻��� ���ڿ��̾�� �մϴ�,")
	);
#pragma endregion


	data_manager->AppendParser("MENU_MENUSELECTION", (new Parser())
		->set_regex(LR"(\d)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ������.")
	);

	data_manager->AppendParser("MENU_TABLE", (new Parser())
		->set_regex(LR"(^[0-9a-zA-Z]$)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �Ǵ� ���ĺ� �� �ϳ��� ������.")
	);
}
#pragma endregion

#pragma region �޴�ȭ�� ����

void Program::SetMenu()
{	
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
			SaveCSV();
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
			
			vector<string> parser_key = { "account_name", "account_id", "account_password", "", "account_phonenumber", "account_address" };

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
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", "ID", "��ǰ", "�帣", "����", "����", "���");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", product->id, limit(product->title, 18), product->genre, product->author, to_string(product->price) + "��", product->count);
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

			vector<string> product;

			vector<string> parser_key = { "product_title", "product_genre", "product_author", "product_price", "product_count" };
			
			int id = (int)shop_manager->GetProductList().size();

			product.push_back(to_string(id));

			for (int i = 0; i < parser_key.size(); i++) {
				product.push_back(IO.input(data_manager->GetParser(parser_key[i])));
			}

			product.push_back("FALSE");

			string input = IO.input("\n��ǰ�� ����Ͻðڽ��ϱ�? (y / n)");

			if (input == "y") {
				shop_manager->AddProduct(new Product(product));
				
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

			IO.print(format("������ȣ : {0}\n", target->id));
			IO.print(format("���� : {0}\n", target->title));
			IO.print(format("�帣 : {0}\n", target->genre));
			IO.print(format("���� : {0}\n", target->author));
			IO.print(format("���� : {0}\n", target->price));
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

			for (int i = 0; i < parser_key.size(); i++) { 
				auto checkpoint = IO.checkpoint();

				while (true) { 
					Parser* parser = data_manager->GetParser(parser_key[i]);

					string input = IO.input("", parser->label);
					if (input == "" || parser->Check(input)) { 
						if (input != "") {
							product[i + 1] = input;
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
			}

			string input = IO.input("��ǰ ��� ������ �����Ͻðڽ��ϱ�? (y / n)");

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
				shop_manager->RemoveProduct(target->id);

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

			vector<string> parser_key = { "account_password","","account_phonenumber","account_address" };

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

			string input = IO.input("\n���� ������ �����Ͻðڽ��ϱ�? (y / n)");
				
			if (input == "y") {
				*target = Account(account);

				IO.print("���� ������ �����Ǿ����ϴ�.\n");
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

			return format("{0:<12}{1:<20}{2:<8}{3:<12}{4:<10}", invoice->date, limit(product->title, 18), product->author,
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

			IO.print(format("���� ��¥ : {0}\n", target->date));

			IO.print("\n[ ��ǰ �� ���� ]\n");
			IO.print(format("���� ��ȣ : {0}\n", product->id));
			IO.print(format("���� : {0}\n", product->title));
			IO.print(format("�帣 : {0}\n", product->genre));
			IO.print(format("���� : {0}\n", product->price));

			IO.print("\n[ �ֹ��� ���� ]\n");
			IO.print(format("���̵� : {0}\n", account->id));
			IO.print(format("�̸� : {0}\n", account->name));
			IO.print(format("��ȭ��ȣ : {0}\n", phone_number(target->recipient_phone_number)));
			IO.print(format("�ּ� : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("��ǰ ���� ��ȣ : {0}\n", target->product_id));
			IO.print(format("��ǰ ���� : {0}\n", target->product_count));
			IO.print(format("���� �ݾ� : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.pause();
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
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", product->id, limit(product->title, 18), product->genre, product->author, to_string(product->price) + "��", product->count);
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
			IO.print_aligned_center("[ ��ǰ ��� ���� ]");

			IO.print(format("������ȣ : {0}\n", target->id));
			IO.print(format("���� : {0}\n", target->title));
			IO.print(format("�帣 : {0}\n", target->genre));
			IO.print(format("���� : {0}\n", target->price));
			IO.print(format("��� : {0}\n", target->count));

			IO.print_line();

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
	));

	// ��ǰ ���� �޴�ȭ��
	menu_manager->AppendMenu(MENU_B_PRODUCT_BUY, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ��ǰ ���� ]");
			int count;
			string input;
			Account* user = shop_manager->GetCurrentAccount();
			vector<string> invoice = { user->id,user->phone_number,user->address, today(), to_string(target->id)};
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
				input = IO.input("�ֹ� ������ �Է��ϼ���.");
				if (data_manager->GetParser("invoice_product_count")->Check(input)) {
					if (target->count >= stoi(input))
					{
						count = stoi(input);
						invoice.push_back(input);
						break;
					}
					else {
						IO.print("�ֹ� ������ �ʰ��߽��ϴ�.\n");
					}
				}
				IO.print(data_manager->GetParser("invoice_product_count")->msg_error);
				IO.pause();
				IO.rollback(checkpoint);
			}
			IO.print_line();
			IO.print("[�ֹ��� ����]\n");
			IO.print(format("�̸� : {0}\n", user->name));
			IO.print(format("��ȭ��ȣ : {0}\n", phone_number(user->phone_number)));
			IO.print(format("�ּ� : {0}\n", user->address));
			IO.print_line();
			IO.print("[�����ݾ�]\n");
			IO.print(format("{0}�� X {1}�� = {2}��\n", target->price, stoi(input), target->price * stoi(input)));
			IO.print_line();
			input = IO.input("��ǰ�� �ֹ��Ͻðڽ��ϱ�?(y / n)");
			if (input == "y") {
				int id = (int)shop_manager->GetInvoiceList().size();
				target->count -= count; //��ǰ ���� ������Ʈ
				invoice.insert(invoice.begin(), to_string(id));
				shop_manager->AddInvoice(new Invoice(invoice));
				user->invoice_id_list.push_back(id);
				IO.pause();
				IO.print("��ǰ�� �����Ͽ����ϴ�.\n");
			}
		}
	));

#pragma endregion
}
#pragma endregion

