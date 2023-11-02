#pragma once  

#include "DataComponent.h"

using namespace std;

class ShopManager
{
private:
	vector<Product*> product_list;
	vector<Account*> account_list;
	vector<Invoice*> invoice_list;

	Account* admin = new Account("admin", "password");
	Account* current_user;
public:
	Account* Login(string id, string password);
	void Logout(); 

	Account* const GetCurrentAccount() { return current_user; }
	const Account* const GetAdminAccount() { return admin; }
	
	void AddProduct(Product* product) { product_list.push_back(product); }
	void AddAccount(Account* account) { account_list.push_back(account); }
	void AddInvoice(Invoice* invoice) { invoice_list.push_back(invoice); }

	Product* GetProduct(int id) {
		auto product = find_if(product_list.begin(), product_list.end(), [=](Product* element) -> bool { return element->id == id; });

		return (product != product_list.end()) ? (*product) : nullptr;
	};

	Account* GetAccount(string id) {
		auto account = find_if(account_list.begin(), account_list.end(), [=](Account* element) -> bool { return element->id == id; });

		return (account != account_list.end()) ? (*account) : nullptr;
	};

	Invoice* GetInvoice(int id) {
		auto invoice = find_if(invoice_list.begin(), invoice_list.end(), [=](Invoice* element) -> bool { return element->id == id; });

		return (invoice != invoice_list.end()) ? (*invoice) : nullptr;
	};

	void RemoveProduct(int id) {
		auto product = find_if(product_list.begin(), product_list.end(), [=](Product* element) -> bool { return element->id == id; });

		product_list.erase(product);
	};

	vector<Product*>& GetProductList() { return product_list; }
	vector<Account*>& GetAccountList() { return account_list; }
	vector<Invoice*>& GetInvoiceList() { return invoice_list; }

	Account* GetUser() { return current_user; }
};