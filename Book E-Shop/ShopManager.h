#pragma once

#include <map>

#include "DataComponent.h"

using namespace std;

class ShopManager
{
private:
	map<int, Product*> product_list;
	map<string, Account> account_list;
	map<int, Invoice> invoice_list;

	Account current_user;
public:
	
	bool Login(string id, string password);
	bool SignUp(Account* account);
	bool BuyProduct(Invoice* invoice);

	//temporate method
	void Add_Product(Product* product) { product_list.insert({ product->id, product }); }
	map<int, Product*> Get_Prodcut_list() { return product_list; }
};