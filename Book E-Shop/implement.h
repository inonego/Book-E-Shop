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

	// Parser ����
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
	//csv �Ľ�
	auto product_raw = data_manager.ParseCSV("products.csv");
	for (size_t i = 0; i < product_raw.size();i++) {
		// ������ Product ��ü�� shop_manager�� �߰�
		shop_manager.AddProduct(new Product(product_raw[i]));
	}


#pragma endregion


	data_manager.AppendParser("MENU_SELECT", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ������.")
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
					IO.print("�޴��� ǥ�õ� ��ȣ �� �ϳ��� ������.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));
}

void menu() {
	MenuSelectionTemplate(MENU_START, make_pair(MENU_LOGIN, "���� �α���"), make_pair(MENU_SIGNUP, "���� ȸ������"));
	MenuSelectionTemplate(MENU_ADMIN, make_pair(MENU_A_PRODUCT_LIST, "��ǰ ���� (���/����/����, ������)"), make_pair(MENU_A_ACCOUNT_LIST, "�� ���� (����, �ֹ�)"));
	MenuSelectionTemplate(MENU_BUYER, make_pair(MENU_B_PRODUCT_LIST, "��ǰ ���"), make_pair(MENU_B_ACCOUNT_INFO, "�� ���� ����"));

	// ���� �α��� �޴�ȭ��
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
					IO.print("������ ���̵� ��admin���� ���̵�� ����� �� �����ϴ�.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
				else {
					break;
				}
			}

			string password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

			IO.print(format("�Է��Ͻ� ���̵�� {0}, ��й�ȣ�� {1}�Դϴ�.\n", id, password));
			IO.pause();
		}
	));

	// ���α׷� ���� �޴�ȭ��
	menu_manager.AppendMenu(MENU_QUIT, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand();
			menu_manager.PrintCommand();

			IO.print_line();
			IO.print("���α׷��� �����Ͻðڽ��ϱ�? (y / n)\n");
			string input = IO.input();

			if (input == "y") {
				exit();
			}
		}
	));
	
	// ���� �α׾ƿ� �޴�ȭ��
	menu_manager.AppendMenu(MENU_LOGOUT, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand();
			menu_manager.PrintCommand();

			IO.print_line();
			IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
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

		
}

// ������ �����˴ϴ�.
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