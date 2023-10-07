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
public:
	
};

