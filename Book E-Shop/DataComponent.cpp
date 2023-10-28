#include "DataComponent.h"

#include "Util.h"

DataComponent::DataComponent() {}

DataComponent::DataComponent(string* data) {}

Product::Product() {}

Product::Product(vector<string> data)
{
	id = stoi(data[0]);
	title = data[1];
	genre = data[2];
	price = stoi(data[3]);
	count = stoi(data[4]);
};

string Product::GetKey()
{
	return to_string(id);
}

Account::Account() {}

Account::Account(string id, string password) : id(id), password(password) {}

Account::Account(vector<string> data)
{
	name = data[0];
	id = data[1];
	password = data[2];
	phone_number = data[3];
	address = data[4];
	for (size_t i = 0; i < data.size() - 5; i++)
		invoice_id_list.push_back(stoi(data[i+5]));
}


string Account::GetKey()
{
	return id;
}

Invoice::Invoice() {}

Invoice::Invoice(vector<string> data)
{
	id = stoi(data[0]);
	buyer_id = data[1];
	recipient_phone_number = data[2];
	recipient_address = data[3];
	date = data[4];
	product_id = stoi(data[5]);
	product_count = stoi(data[6]);
}

string Invoice::GetKey()
{
	return to_string(id);
}

string* Product::ToArray()
{
	return &(vector<string> {to_string(id), title, genre, to_string(price), to_string(count) })[0];
}

bool Product::operator==(const int& other) const
{
	return this->id == other;
}

string* Account::ToArray()
{  
	return &(vector<string> { name, id, password, phone_number, address, join(invoice_id_list) })[0];
}

bool Account::operator==(const string& other) const
{
	return this->id == other;
}

string* Invoice::ToArray()
{  
	return &(vector<string> { to_string(id), buyer_id, recipient_phone_number, recipient_address, date, to_string(product_id), to_string(product_count) })[0];
}

bool Invoice::operator==(const int& other) const
{
	return this->id == other;
}
