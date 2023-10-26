#include "DataComponent.h"

#include "Util.h"


DataComponent::DataComponent() {}

DataComponent::DataComponent(string* data) {}

Product::Product() {}

string Product::GetKey()
{
	return to_string(id);
}

Account::Account() {}


Account::Account(string* data)
{

}


string Account::GetKey()
{
	return id;
}

Invoice::Invoice() {}

Invoice::Invoice(string* data)
{

}

string Invoice::GetKey()
{
	return to_string(id);
}

string* Product::ToArray()
{
	return &(vector<string> {to_string(id), title, genre, to_string(price), to_string(count) })[0];
}

string* Account::ToArray()
{  
	return &(vector<string> { name, id, password, phone_number, address, join(invoice_id_list) })[0];
}

string* Invoice::ToArray()
{  
	return &(vector<string> { to_string(id), buyer_id, recipient_phone_number, recipient_address, date, to_string(product_id), to_string(product_count) })[0];
}