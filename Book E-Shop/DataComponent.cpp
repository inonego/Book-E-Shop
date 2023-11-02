#include "DataComponent.h"

#include "Util.h"

DataComponent::DataComponent() {}

DataComponent::DataComponent(string* data) {}

Product::Product() {}

Product::Product(vector<string> data)
{
/*
	int id;
	string title;
	string genre;
	int price;
	int count;
	string author;
	bool deleted;
*/
	id = stoi(data[0]);
	title = data[1];
	genre = data[2];
	author = data[3];
	price = stoi(data[4]);
	count = stoi(data[5]);
	deleted = data[6] == "TRUE";
};

string Product::GetKey()
{
	return to_string(id);
}

Account::Account() {}

Account::Account(string id, string password) : id(id), password(password) {}

Account::Account(vector<string> data)
{
	/*
		string name;
		string id;
		string password;
		string phone_number;
		string address;
		vector<int> invoice_id_list;
		int coupon_count;
		int accumulated;
	*/

	name = data[0];
	id = data[1];
	password = data[2];
	phone_number = data[3];
	address = data[4];
	 
	stringstream sstream(data[5]);

	string input;

	while (getline(sstream, input, '/')) {
		invoice_id_list.push_back(stoi(input));
	} 
	
	coupon_count = stoi(data[6]);
	accumulated = stoi(data[7]);
}


string Account::GetKey()
{
	return id;
}

Invoice::Invoice() {}

Invoice::Invoice(vector<string> data)
{
	/*
		int id;
		string buyer_id;
		string recipient_phone_number;
		string recipient_address;
		string date;
		int product_id;
		int product_count;
		int price;
		int coupon_count;
		int final_price;
		InvoiceState state;
	*/

	id = stoi(data[0]);
	buyer_id = data[1];
	recipient_phone_number = data[2];
	recipient_address = data[3];
	date = data[4];
	product_id = stoi(data[5]);
	product_count = stoi(data[6]);
	price = stoi(data[7]);
	coupon_count = stoi(data[8]);
	final_price = stoi(data[9]); 
	state = (InvoiceState)stoi(data[10]);
}

string Invoice::GetState()
{
	switch (this->state) {
	case PURCHASED:
		return "구매 확정 이전";
	case CONFIRMED:
		return "구매 확정";
	case REFUNDED:
		return "반품";
	}

	return "";
}

string Invoice::GetKey()
{
	return to_string(id);
}

vector<string> Product::ToArray()
{
/*
	int id;
	string title;
	string genre;
	int price;
	int count;
	string author;
	bool deleted;
*/
	return (vector<string> { to_string(id), title, genre, author, to_string(price), to_string(count), deleted ? "TRUE" : "FALSE" });
}

vector<string> Account::ToArray()
{  
	/*
		string name;
		string id;
		string password;
		string phone_number;
		string address;
		vector<int> invoice_id_list;
		int coupon_count;
		int accumulated;
	*/

	return (vector<string> { name, id, password, phone_number, address, join(invoice_id_list,"/"), to_string(coupon_count), to_string(accumulated) });
}

vector<string> Invoice::ToArray()
{  	
	/*
		int id;
		string buyer_id;
		string recipient_phone_number;
		string recipient_address;
		string date;
		int product_id;
		int product_count;
		int price;
		int coupon_count;
		int final_price;
		InvoiceState state;
	*/

	return (vector<string> { to_string(id), buyer_id, recipient_phone_number, recipient_address, date,
							 to_string(product_id), to_string(product_count), to_string(price), to_string(coupon_count), to_string(final_price), to_string(state)  });
}