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
  
int ShopManager::GetAccumulated(Account* account)
{
	int result = 0;

	for (int i = 0; i < account->invoice_id_list.size(); i++) {
		Invoice* invoice = invoice_list[account->invoice_id_list[i]];

		if (invoice->state == CONFIRMED) {
			result += invoice->final_price;
		}
	}

	return result;
}

void ShopManager::Confirm(chrono::system_clock::time_point now, Invoice* invoice)
{
	if (invoice->state != PURCHASED) return;
	
	invoice->confirm_date = now;
	invoice->state = CONFIRMED;

	Account* account = GetAccount(invoice->buyer_id);

	bool couponed = false;
	int accumulated = GetAccumulated(account);

	// ���� �߱�
	while (accumulated >= 50000) {
		accumulated -= 50000;

		account->coupon_list.push_back(now); 

		couponed = true;
	}

	if (couponed) {
		accumulated = 0;

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

		// ���� �� 7���� ���� ��� ���� Ȯ������ �����մϴ�.
		if (invoice->state == PURCHASED) {
			if (day_diff(invoice->date, now) >= CONFIRM_DATE) {
				Confirm(invoice->date + std::chrono::days(CONFIRM_DATE), invoice);
			}
		}
	}

	for (int i = 0; i < invoice_id_list.size(); i++) {
		int index = (int)invoice_id_list.size() - i - 1;

		Invoice* invoice = invoice_list[invoice_id_list[index]];

		// '���� Ȯ��' �� 7���� ���� ��� '���� Ȯ��(���� Ȯ�� ���� �ݾ� ��꿡�� ����)'���� �����մϴ�.
		if (invoice->state == CONFIRMED) {
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

		UpdateInvoiceList(now, account->invoice_id_list);
		UpdateCouponList(now, account->coupon_list);
	}
}

chrono::system_clock::time_point ShopManager::GetLastDate()
{
	chrono::system_clock::time_point result = string_to_date("00.01.01");

	for (int p = 0; p < account_list.size(); p++) {
		Account* account = account_list[p];

		// ������ ����(���� �ֱٿ� �߱޵� ����)�� ��¥�� �ҷ��� 
		if (account->coupon_list.size() != 0) { 
			 result = max(result, account->coupon_list[account->coupon_list.size() - 1]);
		}

		for (int i = 0; i < account->invoice_id_list.size(); i++) {
			int index = (int)account->invoice_id_list.size() - i - 1;

			Invoice* invoice = invoice_list[account->invoice_id_list[index]];
			
			chrono::system_clock::time_point last_date = string_to_date("00.01.01");

			/*
			// �ڵ����� ���� Ȯ���Ǵ� 7�� + ���� �ݾ�
			if (day_diff(invoice->date, result) >= CONFIRM_DATE + ACCUMULATED_EXPIRE_DATE) {
				break;
			}
			 */

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
