#pragma once

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

class IMenu {
private:
	string name;

	MenuCode prev_menu_code;
public: 
	// 이전 메뉴의 코드를 반환합니다.
	MenuCode get_prev_menu_code() {
		return prev_menu_code;
	}

	// 이전 메뉴의 코드를 설정합니다.
	void set_prev_menu_code(MenuCode prev_menu_code) {
		this->prev_menu_code = prev_menu_code;
	}

	template<typename... TP>
	void Run(MenuIO& IO, TP... v);
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
inline void IMenu::Run(MenuIO& IO, TP ...v)
{
	using Menu = Menu<TP...>;

	Menu* menu = static_cast<Menu*>(this);
	 
	menu->run_func(IO, v...);
}
