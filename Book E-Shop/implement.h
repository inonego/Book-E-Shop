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
	// Parser ����
	data_manager.AppendParser("account_id", (new Parser())
		->set_pre("���̵�")
		->set_regex(R"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("���̵�� ����, �����ڷ� ������ ���̰� 4 �̻� 12 ������ ���ڿ��̾�� �մϴ�!")
	);

	data_manager.AppendParser("account_password", (new Parser())
		->set_pre("��й�ȣ")
		->set_regex(R"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("��й�ȣ�� ����, ������, Ư�� ����(!@#$%^&*_)�� ������ ���̰� 8 �̻� 16 ������ ���ڿ��̾�� �մϴ�!")
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
					IO.print("���̵�� 'admin'�� �� �����ϴ�!\n");
					IO.pause();
					rollback();
				}
				else {
					break;
				}
			}
			});

		string password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

		IO.print(format("�Է��Ͻ� ���̵�� {0}, ��й�ȣ�� {1}�Դϴ�.\n", id, password));
		IO.pause();
	}));

	menu_manager.AppendMenu("quit", new Menu([&](MenuIO IO) {
		IO.print_available_command();

		IO.print_line();
		IO.print("���α׷��� �����Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit();
		}
	}));

	menu_manager.AppendMenu("logout", new Menu([&](MenuIO IO) {
		IO.print_available_command();

		IO.print_line();
		IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			menu_manager.RunMenu("���� �޴�ȭ��");
		}
	}));

}

// ������ �����˴ϴ�.
void menu_value() {
	menu_manager.SetMenu("���α׷� ����", "quit", "", {});
	menu_manager.SetMenu("���� �α׾ƿ�", "logout", "", {});
	menu_manager.SetMenu("���� �޴�ȭ��", "start", "", { 'q' });
	menu_manager.SetMenu("���� �α���", "login", "start", { 'z', 'q' });
	menu_manager.SetMenu("���� ȸ������", "signup", "start", { 'z', 'q' });
	menu_manager.SetMenu("������ �޴�ȭ��", "admin", "start", { 'l', 'q' });
	menu_manager.SetMenu("��ǰ ���", "a_product_list", "admin", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ �˻� �� �帣 ����", "a_product_search", "a_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ �ű� ���", "a_product_register", "a_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ ��� ����", "a_product_info", "a_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ ��� ���� ����", "a_product_info_m", "a_product_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ ��� ���� ����", "a_product_info_r", "a_product_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�� ���", "a_account_list", "admin", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�� �˻�", "a_account_search", "a_account_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�� ���� ����", "a_account_info", "a_account_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�� ���� ���� ����", "a_account_info_m", "a_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("���� ����", "a_invoice_list", "a_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�ֹ� �� ����", "a_invoice_info", "a_invoice_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("������ �޴�ȭ��", "buyer", "start", { 'l', 'q', 'q' });
	menu_manager.SetMenu("��ǰ ���", "b_product_list", "buyer", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ �˻� �� �帣 ����", "b_product_search", "b_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ �� ����", "b_product_info", "b_product_list", { 'z', 'l', 'q' });
	menu_manager.SetMenu("��ǰ ����", "b_product_buy", "b_product_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�� ���� ����", "b_account_info", "buyer", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�� ���� ���� ����", "b_account_info_m", "b_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("���� ����", "b_invoice_list", "b_account_info", { 'z', 'l', 'q' });
	menu_manager.SetMenu("�ֹ� �� ����", "b_invoice_info", "b_invoice_list", { 'z', 'l', 'q' });
}