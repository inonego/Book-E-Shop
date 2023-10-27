#pragma once

#include <map>

#include "DataComponent.h"

using namespace std;

class ShopManager
{
private:
	map<int, Product*> product_list;
	map<string, Account*> account_list;
	map<int, Invoice*> invoice_list;

	Account* admin = new Account("admin", "password");
	Account* current_user;
public:

	Account* Login(string id, string password);
	bool SignUp(Account* account);
	bool BuyProduct(Invoice* invoice);

	Account* GetAdminAccount() { return admin; }
	
	void AddProduct(Product* product) { product_list.insert({ product->id, product }); }
	void AddAccount(Account* account) { account_list.insert({ account->id, account }); }
	void AddInvoice(Invoice* invoice) { invoice_list.insert({ invoice->id, invoice }); }

	map<int, Product*> GetProdcutList() { return product_list; }
	map<string, Account*> GetAccountList() { return account_list; }
	map<int, Invoice*> GetInvoiceList() { return invoice_list; }

	Product* FindProduct(int id) { return product_list.find(id)->second; }
	Account* FindAccount(string id) { return account_list.find(id)->second; }
	Invoice* FindInvoice(int id) { return invoice_list.find(id)->second; }
};