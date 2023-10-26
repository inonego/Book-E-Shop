#pragma once

#include<any>
#include <string> 
#include <vector> 

#include <iostream>
#include <sstream>

using namespace std;
 
class DataComponent
{  
public: 
	DataComponent();
	// ������ ��Ҹ� �迭�κ��� �Ľ��մϴ�.
	DataComponent(string* data);

	// ������ ��Ҹ� ��ǥ�ϴ� Ű ���� ��ȯ�մϴ�.
	virtual string GetKey() = 0;

	// ������ ��Ҹ� �迭�� ��ȯ�մϴ�.
	virtual string* ToArray() = 0;
};

// ��ǰ ��� ����
class Product : DataComponent {
public:
	Product();
	Product(string datas)
	{
		//cout << datas << endl;
		istringstream ss(datas);
		string buf;
		vector<string> data;

		while (getline(ss, buf, ',')) {
			data.push_back(buf);
			//cout << buf << endl;
		}
		
		id = stoi(data[0]);
		title = data[1];
		genre = data[2];
		price = stoi(data[3]);
		count = stoi(data[4]);
	};

	int id;
	string title;
	string genre;
	int price;
	int count;

	virtual string GetKey() override;
	virtual string* ToArray() override;
};



// �� ���� ����
class Account : DataComponent {
public:	
	Account();
	Account(string* data);

	string name;
	string id;
	string password;
	string phone_number;
	string address;
	vector<int> invoice_id_list;

	virtual string GetKey() override; 
	virtual string* ToArray() override;
};

// �ֹ� ó�� ����
class Invoice : DataComponent {
public:	
	Invoice();
	Invoice(string* data);

	int id;
	string buyer_id;
	string recipient_phone_number;
	string recipient_address;
	string date;
	int product_id;
	int product_count;

	virtual string GetKey() override; 
	virtual string* ToArray() override;
};