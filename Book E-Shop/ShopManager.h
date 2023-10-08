#pragma once

#include <map>

#include "DataComponent.h"

using namespace std;

class ShopManager
{
private:
	map<int, Product> product_list;
	map<string, Account> account_list;
	map<int, Invoice> invoice_list;

	Account current_user;
public:
	bool Login(string id, string password);
	bool SignUp(Account* account);
	bool BuyProduct(Invoice* invoice);
};

