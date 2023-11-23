#include "util.h"
#include "DataComponent.h"

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
	deleted = data[6]._Equal("TRUE");
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
		vector<int> product_id_list;
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

	updateList(stoi(data[8]));
}


void Account::updateList(int productId)
{
	auto it = find(product_id_list.begin(), product_id_list.end(), productId);
	if (it != product_id_list.end()) {
		product_id_list.erase(it);
	}

	product_id_list.insert(product_id_list.begin(), productId);

	if (product_id_list.size() > MAX_SIZE) {
		product_id_list.pop_back();
	}
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
	 
	date = string_to_date((data[4] == "") ? "00.01.01" : data[4]);

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
	return (vector<string> { format("{0:06}", id), title, genre, author, to_string(price), to_string(count), deleted ? "TRUE" : "FALSE" });
}

string Product::ToString()
{
	string result = "";
	result += format("제품 고유번호 : {0}\n", format("{:06}", this->id));
	result += format("제목 : {0}\n", limit(this->title,75));
	result += format("장르 : {0}\n", limit(this->genre,75));
	result += format("저자 : {0}\n", limit(this->author,75));
	result += format("가격 : {0}\n", this->price);

	return result;
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

string Account::ToString()
{
	return string();
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

	return (vector<string> { format("{0:08}", id), buyer_id, recipient_phone_number, recipient_address, date_to_string(date),
							 to_string(product_id), to_string(product_count), to_string(price), to_string(coupon_count), to_string(final_price), to_string(state)  });
}

string Invoice::ToString()
{
	return string();
}
