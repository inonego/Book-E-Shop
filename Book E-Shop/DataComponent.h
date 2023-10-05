#pragma once

#include <string> 
#include <vector> 

using namespace std;
 
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
	virtual Product* Parse(string* data) override;
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
	virtual Account* Parse(string* data) override;
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
	virtual Invoice* Parse(string* data) override;
	virtual string* ToArray() override;
};