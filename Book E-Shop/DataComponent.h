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
	// 데이터 요소를 대표하는 키 값을 반환합니다.
	virtual string GetKey() = 0;

	// 데이터 요소를 배열로부터 파싱합니다.
	virtual DataComponent* Parse(string* data) = 0;

	// 데이터 요소를 배열로 변환합니다.
	virtual string* ToArray() = 0;
};

// 상품 등록 정보
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

// 고객 계정 정보
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

// 주문 처리 정보
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