#include "ShopManager.h"

Account* ShopManager::Login(string id, string password)
{
	if (id == admin->id && password == admin->password) {
		return current_user = admin;
	}

	if (account_list.find(id) != account_list.end()) {
		Account* user = account_list[id];

		if (user->password == password) {
			return current_user = user;
		}
	}

	return nullptr;
}
