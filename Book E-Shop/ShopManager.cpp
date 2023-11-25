#include "ShopManager.h"
#include "Util.h"

Account* ShopManager::Login(string id, string password)
{
	if (id == admin->id && password == admin->password) {
		return current_user = admin;
	}

	Account* user;

	if ((user = GetAccount(id)) != nullptr) {
		if (user->password == password) {
			return current_user = user;
		}
	}

	return nullptr;
}

void ShopManager::Logout()
{
	current_user = nullptr;
} 
 
void ShopManager::Confirm(chrono::system_clock::time_point now, Invoice* invoice)
{
	if (invoice->state != PURCHASED) return;
	
	invoice->state = CONFIRMED;

	bool couponed = false;
	int accumulated = 0;

	Account* account = GetAccount(invoice->buyer_id);

	for (int i = 0; i < account->invoice_id_list.size(); i++) {
		Invoice* invoice = invoice_list[account->invoice_id_list[i]];

		if (invoice->state == CONFIRMED) { 
			accumulated += invoice->final_price;
		}
	}

	// 쿠폰 발급
	while (accumulated >= 50000) {
		accumulated -= 50000;

		account->coupon_list.push_back(now); 

		couponed = true;
	}

	if (couponed) {
		for (int i = 0; i < account->invoice_id_list.size(); i++) {
			Invoice* invoice = invoice_list[account->invoice_id_list[i]];

			if (invoice->state == CONFIRMED) {
				invoice->state = CONFIRMED_V;
			}
		}
	}

	account->accumulated = accumulated;
}

void ShopManager::UpdateCouponList(chrono::system_clock::time_point now, vector<chrono::system_clock::time_point>& coupon_list)
{
	for (int i = 0; i < coupon_list.size(); i++) { 
		if (day_diff(coupon_list[i], now) >= COUPON_EXPIRE_DATE) {
			coupon_list.erase(coupon_list.begin() + i);
		}
		else {
			break;
		}
	}
}

void ShopManager::UpdateInvoiceList(chrono::system_clock::time_point now, vector<int>& invoice_id_list)
{  
	for (int i = 0; i < invoice_id_list.size(); i++) {
		int index = (int)invoice_id_list.size() - i - 1;

		Invoice* invoice = invoice_list[invoice_id_list[index]];

		// 구매 후 7일이 지난 경우 구매 확정으로 변경합니다.
		if (invoice->state == PURCHASED) {
			if (day_diff(invoice->date, now) >= CONFIRM_DATE) {
				Confirm(now, invoice);
			}
		}
		// '구매 확정' 후 7일이 지난 경우 '구매 확정(구매 확정 누적 금액 계산에서 제외)'으로 변경합니다.
		else if (invoice->state == CONFIRMED) {
			if (day_diff(invoice->confirm_date, now) >= ACCUMULATED_EXPIRE_DATE) {
				invoice->state = CONFIRMED_V;
			}
		}
	}
}

void ShopManager::UpdateAll(chrono::system_clock::time_point now)
{
	for (int i = 0; i < account_list.size(); i++) {
		Account* account = account_list[i]; 

		UpdateCouponList(now, account->coupon_list);
		UpdateInvoiceList(now, account->invoice_id_list);
	}
}

chrono::system_clock::time_point ShopManager::GetLastDate()
{
	chrono::system_clock::time_point result = string_to_date("00.01.01");

	for (int p = 0; p < account_list.size(); p++) {
		Account* account = account_list[p];

		// 마지막 쿠폰(가장 최근에 발급된 쿠폰)의 날짜를 불러옴 
		if (account_list.size() != 0) { 
			 result = max(result, account->coupon_list[account_list.size() - 1]);
		}

		for (int i = 0; i < account->invoice_id_list.size(); i++) {
			int index = (int)account->invoice_id_list.size() - i - 1;

			Invoice* invoice = invoice_list[account->invoice_id_list[index]];
			
			chrono::system_clock::time_point last_date = string_to_date("00.01.01");

			// 자동으로 구매 확정되는 7일 + 누적 금액
			if (day_diff(invoice->date, result) >= CONFIRM_DATE + ACCUMULATED_EXPIRE_DATE) {
				break;
			}
			 
			switch (invoice->state) {
			case PURCHASED:
				last_date = invoice->date;
				break;
			case CONFIRMED:
			case CONFIRMED_V:
				last_date = invoice->confirm_date;
				break;
			} 

			result = max(result, last_date);
		}
	} 

	return result;
}
