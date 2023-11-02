#pragma once

#include <string>
#include <iostream>
#include <functional>

enum MenuCode {
	MENU_NONE = 0,
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
	MENU_BUYER,
	MENU_B_PRODUCT_LIST,
	MENU_B_PRODUCT_SEARCH,
	MENU_B_PRODUCT_INFO,
	MENU_B_PRODUCT_BUY,
	MENU_ACCOUNT_INFO,
	MENU_ACCOUNT_INFO_M,
	MENU_INVOICE_LIST,
	MENU_INVOICE_INFO,
	MENU_B_REFUND
};

using namespace std;   

using namespace std;
class IMenu {
private:
	string name;

	function<void(MenuIO&)> run_func;
public:
	IMenu() {}

	template<typename... TP>
	IMenu* SetArgs(TP&&... args);

	void Run(MenuIO& IO) {
		run_func(IO);
	}
};

template <typename... TP>
class Menu : public IMenu
{
private:
	friend class IMenu;

	function<void(MenuIO&, TP...)> run_func;
public:
	template <typename T> requires invocable<T, MenuIO&, TP...>
	Menu(T run_func) : run_func(run_func) {}
};

template<typename ...TP>
inline IMenu* IMenu::SetArgs(TP&& ...args)
{
	using Menu = Menu<TP...>;

	Menu* menu = static_cast<Menu*>(this); 

	if (menu == nullptr) {
		cerr << "오류 : 메뉴의 인자 형식이 올바르지 않습니다." << '\n';

		exit(300);
	}

	//this->run_func = [&, menu](MenuIO& IO) { menu->run_func(IO, forward<TP>(args)...); };
	this->run_func = [...captured = forward<TP>(args), menu](MenuIO& IO) mutable { menu->run_func(IO, forward<TP>(captured)...); };   

	return this;
}
