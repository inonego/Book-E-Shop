#include "DataComponent.h"

#include "Util.h"

string Product::GetKey()
{
	return to_string(id);
}

string Account::GetKey()
{
	return id;
}

string Invoice::GetKey()
{
	return to_string(id);
}

Product* Product::Parse(string* data)
{
	Product* product = new Product();

	/* parse from data array */

	return product;
}

string* Product::ToArray()
{
	return &(vector<string> {to_string(id), title, genre, to_string(price), to_string(count) })[0];
}

Account* Account::Parse(string* data)
{
	Account* account = new Account();

	/* parse from data array */

	return account;
}

string* Account::ToArray()
{  
	return &(vector<string> { name, id, password, phone_number, address, join<int>(invoice_id_list) })[0];
}


Invoice* Invoice::Parse(string* data)
{
	Invoice* invoice = new Invoice();

	/* parse from data array */

	return invoice;
}

string* Invoice::ToArray()
{  
	return &(vector<string> { to_string(id), buyer_id, recipient_phone_number, recipient_address, date, to_string(product_id), to_string(product_count) })[0];
}