#pragma once

#include <any> 
#include <string>
#include <functional>

enum MenuCode {
	MENU_NONE = -1,
	MENU_QUIT,
	MENU_LOGOUT,
	MENU_START,
	MENU_LOGIN,
	MENU_SIGNUP,
	MENU_ADMIN,
	MENU_A_PRODUCT_LIST,
	MENU_A_PRODUCT_SEARCH,
	MENU_A_PRODUCT_REGISTER,
	MENU_A_PRODUCT_INFO,
	MENU_A_PRODUCT_INFO_M,
	MENU_A_PRODUCT_INFO_R,
	MENU_A_ACCOUNT_LIST,
	MENU_A_ACCOUNT_SEARCH,
	MENU_A_ACCOUNT_INFO,
	MENU_A_ACCOUNT_INFO_M,
	MENU_A_INVOICE_LIST,
	MENU_A_INVOICE_INFO,
	MENU_BUYER,
	MENU_B_PRODUCT_LIST,
	MENU_B_PRODUCT_SEARCH,
	MENU_B_PRODUCT_INFO,
	MENU_B_PRODUCT_BUY,
	MENU_B_ACCOUNT_INFO,
	MENU_B_ACCOUNT_INFO_M,
	MENU_B_INVOICE_LIST,
	MENU_B_INVOICE_INFO
};

using namespace std;

class Menu
{
private:
	string name;

	MenuCode prev_menu_code;

	function<void(MenuIO&, vector<any>)> run_func;
public:
	Menu(function<void(MenuIO&, vector<any>)> run_func) : run_func(run_func) {}
	
	// 이전 메뉴의 코드를 반환합니다.
	MenuCode get_prev_menu_code() {
		return prev_menu_code;
	}
	  
	// 이전 메뉴의 코드를 설정합니다.
	void set_prev_menu_code(MenuCode prev_menu_code) {
		this->prev_menu_code = prev_menu_code;
	}

	template<typename... TP>
	void Run(MenuIO& IO, vector<any> v) { 
		run_func(IO, v);
	}
}; 