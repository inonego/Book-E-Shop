#pragma once  

#include "DataComponent.h"

using namespace std;

class ShopManager
{
private: 
	vector<Product*> undeleted_product_list;

	vector<Product*> product_list;
	vector<Account*> account_list;
	vector<Invoice*> invoice_list;

	Account* admin = new Account("admin", "password");
	Account* current_user;
public:
	Account* Login(string id, string password);
	void Logout(); 

	Account* GetCurrentAccount() { return current_user; } 
	const Account* const GetAdminAccount() { return admin; } 

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

	void AddProduct(Product* product) { product_list.push_back(product); if (!product->deleted) undeleted_product_list.push_back(product); };
	void AddAccount(Account* account) { account_list.push_back(account); }
	void AddInvoice(Invoice* invoice) { invoice_list.push_back(invoice); }

	const vector<Product*>& GetProductList(bool all = false) { return all ? product_list : undeleted_product_list; }
	const vector<Account*>& GetAccountList() { return account_list; }
	const vector<Invoice*>& GetInvoiceList() { return invoice_list; }

	void RemoveProduct(int id) {
		Product* product = GetProduct(id); 

		if (product != nullptr) { 
			product->deleted = true;
			product->count = 0;
		}  

		auto& list = undeleted_product_list;
		list.erase(remove(list.begin(), list.end(), product), list.end()); 
	};
};