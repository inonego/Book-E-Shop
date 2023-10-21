#pragma once 

#include <regex>
#include <format>
#include <vector>

DataManager data_manager;
MenuManager menu_manager;
/*
enum Command
{
	BACK = 'z',
	LOGOUT = 'l',
	QUIT = 'q'
};
*/
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
	menu_manager.AppendMenu(MenuCode::START, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command(); 
		IO.print_line();
		string number;

		auto checkpoint = IO.checkpoint();

		while (true) {
			number = IO.input("1�� �Է��ϼ���");


			if (number == "1") {
				menu_manager.RunMenu(MenuCode::LOGIN);

				IO.pause();

				IO.rollback(checkpoint);
			}
			else {
				break;
			}
		}
	}));

	menu_manager.AppendMenu(MenuCode::LOGIN, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command(); 
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
	}));

	menu_manager.AppendMenu(MenuCode::QUIT, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command();

		IO.print_line();
		IO.print("���α׷��� �����Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit();
		}
	}));

	menu_manager.AppendMenu(MenuCode::LOGOUT, new Menu([&](MenuIO& IO, vector<any> args) {
		//IO.print_available_command();

		IO.print_line();
		IO.print("�������� �α׾ƿ� �Ͻðڽ��ϱ�? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			menu_manager.RunMenu(MenuCode::START);
		}
	}));

}

// ������ �����˴ϴ�.
void menu_value() {

}