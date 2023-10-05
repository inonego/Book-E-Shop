#pragma once

#include <string> 
#include <vector> 

using namespace std;
 
class DataComponent
{  
public: 
	// ������ ��Ҹ� ��ǥ�ϴ� Ű ���� ��ȯ�մϴ�.
	virtual string GetKey() = 0;

	// ������ ��Ҹ� �迭�κ��� �Ľ��մϴ�.
	virtual DataComponent* Parse(string* data) = 0;

	// ������ ��Ҹ� �迭�� ��ȯ�մϴ�.
	virtual string* ToArray() = 0;
};

// ��ǰ ��� ����
class Product : DataComponent {
public:
	int id;
	string title;
	string genre;
	int price;
	int count;

	virtual string GetKey() override;
	virtual Product* Parse(string* data) override;
	virtual string* ToArray() override;
};

// �� ���� ����
class Account : DataComponent {
public:
	string name;
	string id;
	string password;
	string phone_number;
	string address;
	vector<int> invoice_id_list;

	virtual string GetKey() override;
	virtual Account* Parse(string* data) override;
	virtual string* ToArray() override;
};

// �ֹ� ó�� ����
class Invoice : DataComponent {
public:
	int id;
	string buyer_id;
	string recipient_phone_number;
	string recipient_address;
	string date;
	int product_id;
	int product_count;

	virtual string GetKey() override;
	virtual Invoice* Parse(string* data) override;
	virtual string* ToArray() override;
};