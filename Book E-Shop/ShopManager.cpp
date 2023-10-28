#include "ShopManager.h"

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
