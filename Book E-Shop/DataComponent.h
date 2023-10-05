#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

using namespace std;

template <typename T>
class Parser {
private:
	function<T*(string)> parse = nullptr;

public:	
	string regex = ".*";
	string msg_info = "";
	string msg_error = "";
	
	Parser* set_regex(string regex) { this->regex = regex; return this; }
	Parser* set_msg_info(string msg_info) { this->msg_info = msg_info; return this; }
	Parser* set_msg_error(string msg_error) { this->msg_error = msg_error; return this; }

	Parser* set_parse(function<T* (string)> parse) { this->parse = parse; return this; }

	T* Parse(string input) {
		return parse != nullptr ? parse(input) : nullptr;
	}
};

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
	virtual Product* Parse(string* data);
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
	virtual Account* Parse(string* data);
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
	virtual Invoice* Parse(string* data);
	virtual string* ToArray() override;
};