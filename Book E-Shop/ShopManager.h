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
	bool SignUp(Account* account);
	bool BuyProduct(Invoice* invoice);

	Account* GetAdminAccount() { return admin; }
	
	void AddProduct(Product* product) { product_list.push_back(product); }
	void AddAccount(Account* account) { account_list.push_back(account); }
	void AddInvoice(Invoice* invoice) { invoice_list.push_back(invoice); }

	Product* GetProduct(int id) { 
		//auto product = find(product_list.begin(), product_list.end(), id);

		//return (product != product_list.end()) ? (*product) : nullptr;
		
	};
	Account* GetAccount(string id) { 
		//auto account = find(account_list.begin(), account_list.end(), id);

		//return (account != account_list.end()) ? (*account) : nullptr;

		return nullptr;
	};
	Invoice* GetInvoice(int id) { 
		//auto invoice = find(invoice_list.begin(), invoice_list.end(), id);

		//return (invoice != invoice_list.end()) ? (*invoice) : nullptr;
	};

	vector<Product*>& GetProdcutList() { return product_list; }
	vector<Account*>& GetAccountList() { return account_list; }
	vector<Invoice*>& GetInvoiceList() { return invoice_list; }
};