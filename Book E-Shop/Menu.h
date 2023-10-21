#pragma once

#include <any> 
#include <string>
#include <functional>

enum MenuCode {
	QUIT, LOGOUT, START, LOGIN, SIGNUP,
	ADMIN,
	A_PRODUCT_LIST,
	A_PRODUCT_SEARCH,
	A_PRODUCT_REGISTER,
	A_PRODUCT_INFO,
	A_PRODUCT_INFO_M,
	A_PRODUCT_INFO_R,
	A_ACCOUNT_LIST,
	A_ACCOUNT_SEARCH,
	A_ACCOUNT_INFO,
	A_ACCOUNT_INFO_M,
	A_INVOICE_LIST,
	A_INVOICE_INFO,
	BUYER,
	B_PRODUCT_LIST,
	B_PRODUCT_SEARCH,
	B_PRODUCT_INFO,
	B_PRODUCT_BUY,
	B_ACCOUNT_INFO,
	B_ACCOUNT_INFO_M,
	B_INVOICE_LIST,
	B_INVOICE_INFO
};

using namespace std;

class Menu
{
private: 
	string name;

	function<void(MenuIO&, vector<any>)> run_func;
public:
	Menu(function<void(MenuIO&, vector<any>)> run_func) : run_func(run_func) {}

	template<typename... TP>
	void Run(MenuIO& IO, TP... args) {
		vector<any> v{ args... };

		run_func(IO, v);
	}
}; 