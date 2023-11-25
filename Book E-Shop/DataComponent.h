#pragma once

#include <any>
#include <string> 
#include <vector> 
#include <chrono>
#include <iostream>
#include <sstream>
 
using namespace std;
using namespace chrono;

class DataComponent
{  
public: 
	DataComponent();
	// ������ ��Ҹ� �迭�κ��� �Ľ��մϴ�.
	DataComponent(string* data);

	// ������ ��Ҹ� ��ǥ�ϴ� Ű ���� ��ȯ�մϴ�.
	virtual string GetKey() = 0;

	// ������ ��Ҹ� �迭�� ��ȯ�մϴ�.
	virtual vector<string> ToArray() = 0;

	virtual string ToString() = 0;
};

// ��ǰ ��� ����
class Product : DataComponent {
public:
	Product();
	Product(vector<string> data);

	int id;
	string title;
	string genre;
	string author;
	int price;
	int count;
	bool deleted;

	virtual string GetKey() override;
	virtual vector<string> ToArray() override;
	virtual string ToString() override;
};



// �� ���� ����
class Account : DataComponent {
private:
	static const int MAX_RECENT_SIZE;
public: 
	Account();
	Account(string id, string password);
	Account(vector<string> data);

	string name;
	string id;
	string password;
	string phone_number;
	string address;
	vector<int> invoice_id_list;
	vector<chrono::system_clock::time_point> coupon_list; 
	vector<int> recent_product_id_list;

	int accumulated = 0;

	virtual string GetKey() override; 
	virtual vector<string> ToArray() override;
	virtual string ToString() override;  
	void AddRecentProduct(int id);
};

enum InvoiceState {
	PURCHASED = 0,
	REFUNDED  = 1,
	CONFIRMED = 2,
	CONFIRMED_V = 3
};

// �ֹ� ó�� ����
class Invoice : DataComponent {
private:

public:	
	Invoice();
	Invoice(vector<string> data);

	int id;
	string buyer_id;
	string recipient_phone_number;
	string recipient_address;
	chrono::system_clock::time_point date;
	chrono::system_clock::time_point confirm_date;
	int product_id;
	int product_count;
	int price;
	int coupon_count;
	int final_price;
	InvoiceState state;

	string GetState(); 

	virtual string GetKey() override; 
	virtual vector<string> ToArray() override;
	virtual string ToString() override;
};