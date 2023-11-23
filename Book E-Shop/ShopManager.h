#pragma once  

#include "DataComponent.h"

using namespace std;

class ShopManager
{
private:
	vector<Product*> live_product_list;

	vector<Product*> product_list;
	vector<Account*> account_list;
	vector<Invoice*> invoice_list;

	Account* admin = new Account("admin", "password");
	Account* current_user;
public:
	const int COUPON_EXPIRE_DATE = 7;
	const int ACCUMULATED_EXPIRE_DATE = 7;
	const int CONFIRM_DATE = 7;

	Account* Login(string id, string password);
	void Logout();

	const bool IsAdmin() { return current_user->id == admin->id; }
	Account* GetCurrentAccount() { return current_user; }
	const Account* const GetAdminAccount() { return admin; }

	const bool IsProductRegisterable(int id, string title, string author) {
		for (Product* product : product_list) {
			if (product->id != id && product->title == title && product->author == author) {
				return false;
			}
		}

		return true;
	}

	Product* GetProduct(int id, bool all = false) {
		auto& list = all ? product_list : live_product_list;

		auto product = find_if(list.begin(), list.end(), [=](Product* element) -> bool { return element->id == id; });

		return (product != list.end()) ? (*product) : nullptr;
	};

	Account* GetAccount(string id) {
		auto account = find_if(account_list.begin(), account_list.end(), [=](Account* element) -> bool { return element->id == id; });

		return (account != account_list.end()) ? (*account) : nullptr;
	};

	Invoice* GetInvoice(int id) {
		auto invoice = find_if(invoice_list.begin(), invoice_list.end(), [=](Invoice* element) -> bool { return element->id == id; });

		return (invoice != invoice_list.end()) ? (*invoice) : nullptr;
	};


	void EnableProduct(Product* product) {
		if (product != nullptr) {
			product->deleted = false;

			if (find(live_product_list.begin(), live_product_list.end(), product) == live_product_list.end()) live_product_list.push_back(product);
		}
	};
	void DisableProduct(Product* product) {
		if (product != nullptr) {
			product->deleted = true;
			product->count = 0;

			live_product_list.erase(remove(live_product_list.begin(), live_product_list.end(), product), live_product_list.end());
		}
	}

	void AddProduct(Product* product) { product_list.push_back(product); };
	void AddAccount(Account* account) { account_list.push_back(account); }
	void AddInvoice(Invoice* invoice) { invoice_list.push_back(invoice); }

	const vector<Product*>& GetProductList(bool all = false) { return all ? product_list : live_product_list; }
	const vector<Account*>& GetAccountList() { return account_list; }
	const vector<Invoice*>& GetInvoiceList() { return invoice_list; }

	void Confirm(chrono::system_clock::time_point now, Invoice* invoice);
	void UpdateCouponList(chrono::system_clock::time_point now, vector<chrono::system_clock::time_point>& coupon_list);
	void UpdateInvoiceList(chrono::system_clock::time_point now, vector<int>& invoice_id_list);
	void UpdateAll(chrono::system_clock::time_point now);
	chrono::system_clock::time_point GetLastDate();
};