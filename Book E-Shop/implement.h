#pragma once 

#include <regex>
#include <format>
#include <vector>

DataManager data_manager;
MenuManager menu_manager;

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
}

void menu() { 
	menu_manager.AppendMenu(MENU_START, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();
		IO.print_line();
		string number;

		auto checkpoint = IO.checkpoint();

		while (true) {
			number = IO.input("�ƹ� ���ڳ� �Է��غ�����"); 

			menu_manager.RunMenu(MENU_LOGIN, stoi(number));
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
	}));

	menu_manager.AppendMenu(MENU_QUIT, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print("���α׷��� �����Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit();
		}
	}));

	menu_manager.AppendMenu(MENU_LOGOUT, new Menu([&](MenuIO& IO, vector<any> args) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			menu_manager.RunMenu(MENU_START);
		}
	}));

}

// ������ �����˴ϴ�.
void menu_value() {
	menu_manager[MENU_QUIT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGOUT]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_START]->set_prev_menu_code(MENU_NONE);
	menu_manager[MENU_LOGIN]->set_prev_menu_code(MENU_START);
	//menu_manager[MENU_SIGNUP]->set_prev_menu_code(MENU_START);
	/*
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
	*/
}