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
	// 데이터 요소를 배열로부터 파싱합니다.
	DataComponent(string* data);

	// 데이터 요소를 대표하는 키 값을 반환합니다.
	virtual string GetKey() = 0;

	// 데이터 요소를 배열로 변환합니다.
	virtual vector<string> ToArray() = 0;
};

// 상품 등록 정보
class Product : DataComponent {
public:
	Product();
	Product(vector<string> data);

	int id;
	string title;
	string genre;
	int price;
	int count;

	virtual string GetKey() override;
	virtual vector<string> ToArray() override;
};



// 고객 계정 정보
class Account : DataComponent {
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

	virtual string GetKey() override; 
	virtual vector<string> ToArray() override;
};

// 주문 처리 정보
class Invoice : DataComponent {
public:	
	Invoice();
	Invoice(vector<string> data);

	int id;
	string buyer_id;
	string recipient_phone_number;
	string recipient_address;
	string date;
	int product_id;
	int product_count;

	virtual string GetKey() override; 
	virtual vector<string> ToArray() override;
};