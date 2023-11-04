#include "util.h"

#include "program.h"

#include <set>
#include <format>

DataManager* const Program::data_manager = new DataManager();
MenuManager* const Program::menu_manager = new MenuManager();
ShopManager* const Program::shop_manager = new ShopManager();

void Program::CleanUp()
{
	delete data_manager;
	delete menu_manager;
	delete shop_manager;
}

void Program::Run()
{
	atexit(SaveCSV);

	LoadCSV();
	SetCommand();
	SetParser();
	SetMenu();

	menu_manager->Start(MENU_START);
}

void Program::LoadCSV()
{
	// Product CSV ÆÄÀÏÀ» ÆÄ½ÌÇÕ´Ï´Ù.
	auto product_raw = data_manager->ParseCSV("./data/product.csv");
	for (size_t i = 0; i < product_raw.size(); i++) {
		// »ý¼ºµÈ Product °´Ã¼¸¦ shop_manager¿¡ Ãß°¡ÇÕ´Ï´Ù.
		auto product = new Product(product_raw[i]);

		shop_manager->AddProduct(product);
		shop_manager->EnableProduct(product);
	}

	// Account CSV ÆÄÀÏÀ» ÆÄ½ÌÇÕ´Ï´Ù.
	auto account_raw = data_manager->ParseCSV("./data/account.csv");
	for (size_t i = 0; i < account_raw.size(); i++) {
		// »ý¼ºµÈ Account °´Ã¼¸¦ shop_manager¿¡ Ãß°¡ÇÕ´Ï´Ù.
		shop_manager->AddAccount(new Account(account_raw[i]));
	}

	// Invoice CSV ÆÄÀÏÀ» ÆÄ½ÌÇÕ´Ï´Ù.
	auto invoice_raw = data_manager->ParseCSV("./data/invoice.csv");
	
	for (size_t i = 0; i < invoice_raw.size(); i++) {
		// »ý¼ºµÈ Invoice °´Ã¼¸¦ shop_manager¿¡ Ãß°¡ÇÕ´Ï´Ù.
		shop_manager->AddInvoice(new Invoice(invoice_raw[i]));
	}
}

void Program::SaveCSV() {
	vector<vector<string>>raw_data;

	auto product_list = shop_manager->GetProductList(true);
	for (size_t i = 0; i < product_list.size(); i++) {
		raw_data.push_back(product_list[i]->ToArray());
	}
	data_manager->SaveCSV("./data/product.csv", raw_data);

	raw_data.clear();

	for (size_t i = 0; i < shop_manager->GetAccountList().size(); i++) {
		raw_data.push_back(shop_manager->GetAccountList()[i]->ToArray());
	}
	data_manager->SaveCSV("./data/account.csv", raw_data);

	raw_data.clear();

	for (size_t i = 0; i < shop_manager->GetInvoiceList().size(); i++) {
		raw_data.push_back(shop_manager->GetInvoiceList()[i]->ToArray());
	}
	data_manager->SaveCSV("./data/invoice.csv", raw_data);
}

void Program::SetCommand()
{
	menu_manager->AppendCommand('z', new MenuManager::Command("µÚ·Î°¡±â", [=]() {
		menu_manager->RunPreviousMenu();
	}));

	menu_manager->AppendCommand('l', new MenuManager::Command("·Î±×¾Æ¿ô", [=]() {
		menu_manager->RunRecursiveMenu(MENU_LOGOUT);
	}));

	menu_manager->AppendCommand('q', new MenuManager::Command("Á¾·á", [=]() {
		menu_manager->RunRecursiveMenu(MENU_QUIT);
	}));
}

#pragma region Parser ±¸Çö
void Program::SetParser()
{

#pragma region »óÇ° µî·Ï Á¤º¸ Parser

	// »óÇ° µî·Ï Á¤º¸ °íÀ¯ ¹øÈ£
	data_manager->AppendParser("product_id", (new Parser())
		->set_label("°íÀ¯¹øÈ£")
		->set_regex(LR"(^[0-9]{6}$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 6ÀÎ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// »óÇ° µî·Ï Á¤º¸ Á¦¸ñ
	data_manager->AppendParser("product_title", (new Parser())
		->set_label("Á¦¸ñ")
	);

	// »óÇ° µî·Ï Á¤º¸ Àå¸£
	data_manager->AppendParser("product_genre", (new Parser())
		->set_label("Àå¸£")
	);

	// »óÇ° µî·Ï Á¤º¸ Àå¸£
	data_manager->AppendParser("product_author", (new Parser())
		->set_label("ÀúÀÚ")
	);

	// »óÇ° µî·Ï Á¤º¸ °¡°Ý
	data_manager->AppendParser("product_price", (new Parser())
		->set_label("°¡°Ý")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// »óÇ° µî·Ï Á¤º¸ Àç°í
	data_manager->AppendParser("product_count", (new Parser())
		->set_label("Àç°í")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);
#pragma endregion

#pragma region °í°´ °èÁ¤ Á¤º¸ Parser

	// °í°´ °èÁ¤ Á¤º¸ ÀÌ¸§
	data_manager->AppendParser("account_name", (new Parser())
		->set_label("ÀÌ¸§")
		->set_regex(LR"(^[ ]*[a-zA-Z°¡-ÆR]+[ a-zA-Z°¡-ÆR]*$)")
		->set_msg_error("ÀÌ¸§Àº °ø¹é, ¿µ¹®ÀÚ, ÇÑ±Û Çã¿ëÇÑ °ø¹éÀ» Á¦¿ÜÇÑ ±æÀÌ°¡ 1 ÀÌ»óÀÎ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// °í°´ °èÁ¤ Á¤º¸ ¾ÆÀÌµð
	data_manager->AppendParser("account_id", (new Parser())
		->set_label("¾ÆÀÌµð")
		->set_regex(LR"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("¾ÆÀÌµð´Â ¼ýÀÚ, ¿µ¹®ÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 4 ÀÌ»ó 12 ÀÌÇÏÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// °í°´ °èÁ¤ Á¤º¸ ºñ¹Ð¹øÈ£
	data_manager->AppendParser("account_password", (new Parser())
		->set_label("ºñ¹Ð¹øÈ£")
		->set_regex(LR"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("ºñ¹Ð¹øÈ£´Â ¼ýÀÚ, ¿µ¹®ÀÚ, Æ¯¼ö ¹®ÀÚ(!@#$%^&*_)·Î ±¸¼ºµÈ ±æÀÌ°¡ 8 ÀÌ»ó 16 ÀÌÇÏÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// °í°´ °èÁ¤ Á¤º¸ ÀüÈ­¹øÈ£
	data_manager->AppendParser("account_phone_number", (new Parser())
		->set_label("ÀüÈ­¹øÈ£")
		->set_regex(LR"(^010[0-9]{8}$)")
		->set_msg_error("ÀüÈ­¹øÈ£´Â ¼ýÀÚ·Î ±¸¼ºµÇ¾î 010À¸·Î ½ÃÀÛÇÏ°í, ±æÀÌ°¡ 11ÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// °í°´ °èÁ¤ Á¤º¸ ÁÖ¼Ò
	data_manager->AppendParser("account_address", (new Parser())
		->set_label("ÁÖ¼Ò")
	);
	data_manager->AppendParser("account_coupon_count", (new Parser())
		->set_label("º¸À¯ ÄíÆù °³¼ö")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("º¸À¯ ÄíÆù °³¼ö´Â ¼ýÀÚ·Î ±¸¼ºµÈ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);
	data_manager->AppendParser("account_accumulated", (new Parser())
		->set_label("±¸¸ÅÈ®Á¤ ´©Àû ±Ý¾×")
		->set_regex(LR"(^[0-9]+$)")
		->set_msg_error("±¸¸ÅÈ®Á¤ ´©Àû ±Ý¾×Àº ¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);
#pragma endregion

#pragma region ÁÖ¹® Ã³¸® Á¤º¸ Parser
	// ÁÖ¹® Ã³¸® Á¤º¸ °íÀ¯¹øÈ£
	data_manager->AppendParser("invoice_id", (new Parser())
		->set_label("°íÀ¯¹øÈ£")
		->set_regex(LR"(^[0-9]{8}$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 8ÀÎ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);

	// ÁÖ¹® Ã³¸® Á¤º¸ ±¸¸ÅÀÚÀÇ ¾ÆÀÌµð
	data_manager->AppendParser("invoice_buyer_id", (new Parser())
		->set_label("±¸¸ÅÀÚÀÇ ¾ÆÀÌµð")
		->set_msg_info("±¸¸ÅÀÚÀÇ ¾ÆÀÌµð ÀÔ´Ï´Ù.\n")
	);

	// ÁÖ¹® Ã³¸® Á¤º¸ ±¸¸Å ³¯Â¥
	data_manager->AppendParser("invoice_date", (new Parser())
		->set_label("±¸¸Å ³¯Â¥")
	);

	// ÁÖ¹® Ã³¸® Á¤º¸ »óÇ° °íÀ¯ ¹øÈ£
	data_manager->AppendParser("invoice_product_id", (new Parser())
		->set_label("»óÇ° °íÀ¯ ¹øÈ£")
		->set_msg_info("±¸¸ÅÇÑ »óÇ° °íÀ¯ ¹øÈ£ ÀÔ´Ï´Ù.\n")
	);

	// ÁÖ¹® Ã³¸® Á¤º¸ »óÇ° ¼ö·®
	data_manager->AppendParser("invoice_product_count", (new Parser())
		->set_label("»óÇ° ¼ö·®")
		->set_regex(LR"(^[0-9]*[1-9]+[0-9]*$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.\n")
	);
#pragma endregion


	data_manager->AppendParser("MENU_MENUSELECTION", (new Parser())
		->set_regex(LR"(\d)")
		->set_msg_error("¸Þ´º¿¡ Ç¥½ÃµÈ ¹øÈ£ Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.\n")
	);

	data_manager->AppendParser("MENU_TABLE", (new Parser())
		->set_regex(LR"(^[0-9a-zA-Z]$)")
		->set_msg_error("¸Þ´º¿¡ Ç¥½ÃµÈ ¹øÈ£ ¶Ç´Â ¾ËÆÄºª Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.\n")
	);
}
#pragma endregion

#pragma region ¸Þ´ºÈ­¸é ±¸Çö

void Program::SetMenu()
{
	menu_manager->SetOnRefresh([=]() {
		auto invoice_list = shop_manager->GetInvoiceList();

		auto now = today();

		for (int i = 0; i < invoice_list.size(); i++) {
			int index = (int)invoice_list.size() - i - 1;

			Invoice* invoice = invoice_list[index];

			// ÁÖ¹® È®Á¤À» ¸¸³­ °æ¿ì ´õ ÀÌ»ó ÁøÇàÇÒ ÇÊ¿ä°¡ ¾ø½À´Ï´Ù.
			if (invoice->state != CONFIRMED) {
				// È¯ºÒÀÎ »óÅÂÀÎ °æ¿ì¸¦ ¹èÁ¦ÇÏ°í ±¸¸Å È®Á¤À¸·Î º¯°æÇÕ´Ï´Ù.
				if (invoice->state == PURCHASED) {
					// 7ÀÏÀÌ Áö³­ °æ¿ìÀÇ ÁÖ¹® Ã³¸® Á¤º¸ÀÎÁö È®ÀÎÇÕ´Ï´Ù. 
					if (day_diff(invoice->date, now) >= 7) {
						invoice->state = CONFIRMED;

						// ÄíÆù Ã³¸®
						Account* account = shop_manager->GetAccount(invoice->buyer_id);

						account->accumulated += invoice->final_price;

						while (account->accumulated >= 50000) {
							account->accumulated -= 50000;

							account->coupon_count += 1;
						}
					}
				}
			}
			else {
				break;
			}
		}
	});

	menu_manager->SetCommonHeader([=](MenuIO& IO) {
		IO.print_aligned_center("# µµ¼­ ÀÎÅÍ³Ý ¼îÇÎ¸ô #");
	});

#pragma region °øÅë ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_QUIT, new Menu([=](MenuIO& IO) {
		menu_manager->ToggleCommand();
		menu_manager->PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ ÇÁ·Î±×·¥ Á¾·á ]");
		IO.print("ÇÁ·Î±×·¥À» Á¾·áÇÏ½Ã°Ú½À´Ï±î? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit(0);
		}
		}));

	menu_manager->AppendMenu(MENU_LOGOUT, new Menu([=](MenuIO& IO) {
		menu_manager->ToggleCommand();
		menu_manager->PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ °èÁ¤ ·Î±×¾Æ¿ô ]");
		IO.print("°èÁ¤¿¡¼­ ·Î±×¾Æ¿ô ÇÏ½Ã°Ú½À´Ï±î? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			shop_manager->Logout();

			menu_manager->SetCommonHeader([=](MenuIO& IO) {
				IO.print_aligned_center("# µµ¼­ ÀÎÅÍ³Ý ¼îÇÎ¸ô #");
			});
			menu_manager->RunMenu(MENU_START);
		}
		}));
#pragma endregion


#pragma region ½ÃÀÛ ¸Þ´ºÈ­¸é
	// ½ÃÀÛ ¸Þ´ºÈ­¸é
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('q');
		_template.SetName("½ÃÀÛ ¸Þ´º");
		_template.SubMenu("°èÁ¤ ·Î±×ÀÎ",   [=]() { menu_manager->RunMenu(MENU_LOGIN);  });	
		_template.SubMenu("°èÁ¤ È¸¿ø°¡ÀÔ", [=]() { menu_manager->RunMenu(MENU_SIGNUP); });
		_template.Apply(MENU_START);
	} 

	// °èÁ¤ ·Î±×ÀÎ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_LOGIN, new Menu(
		[=](MenuIO& IO) { 
			menu_manager->ToggleCommand('z', 'q');
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °èÁ¤ ·Î±×ÀÎ ]");

			string id, password;

			auto checkpoint = IO.checkpoint();

			while (true) {
				// ¾ÆÀÌµð¸¦ ÀÔ·Â¹Þ½À´Ï´Ù.
				id = IO.input("", "¾ÆÀÌµð");
				// ºñ¹Ð¹øÈ£¸¦ ÀÔ·Â¹Þ½À´Ï´Ù.
				password = IO.input("", "ºñ¹Ð¹øÈ£");

				const Account* user;

				// ÀÏÄ¡ÇÏ´Â °èÁ¤ Á¤º¸°¡ ÀÖ´ÂÁö È®ÀÎÇÕ´Ï´Ù.
				if ((user = shop_manager->Login(id, password)) != nullptr) {
					if (user == shop_manager->GetAdminAccount()) {
						menu_manager->SetCommonHeader([=](MenuIO& IO) {
							IO.print_aligned_center("°ü¸®ÀÚ´ÔÀÌ Á¢¼ÓÇÏ¿´½À´Ï´Ù.");
						});
						menu_manager->RunMenu(MENU_ADMIN);
					}
					else {
						menu_manager->SetCommonHeader([=](MenuIO& IO) {
							IO.print_aligned_center(format("{}´ÔÀÌ Á¢¼ÓÇÏ¿´½À´Ï´Ù.", shop_manager->GetCurrentAccount()->name));
						});
						menu_manager->RunMenu(MENU_BUYER);
					}
				}
				else {
					IO.print("¾ÆÀÌµð°¡ Á¸ÀçÇÏÁö ¾Ê°Å³ª ºñ¹Ð¹øÈ£°¡ ¿Ã¹Ù¸£Áö ¾Ê½À´Ï´Ù.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));

	// °èÁ¤ È¸¿ø°¡ÀÔ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_SIGNUP, new Menu(
		[=](MenuIO& IO) {
			menu_manager->ToggleCommand('z', 'q');
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °èÁ¤ È¸¿ø°¡ÀÔ ]");

			vector<string> account;
			
			vector<string> parser_key = { "account_name", "account_id", "account_password", "", "account_phone_number", "account_address" };

			for (int i = 0; i < parser_key.size(); i++) {				
				if (i == 1) {
					// '¾ÆÀÌµð' ÀÔ·Â
					auto checkpoint = IO.checkpoint();

					while (true) {
						string input = IO.input(data_manager->GetParser(parser_key[i]));

						// ÀÔ·ÂµÈ ¾ÆÀÌµð°¡ °ü¸®ÀÚ °èÁ¤¿¡ ÇØ´çÇÏ´ÂÁö È®ÀÎÇÕ´Ï´Ù.
						if (input != shop_manager->GetAdminAccount()->id) {
							// ÀÔ·ÂµÈ ¾ÆÀÌµð°¡ °í°´ °èÁ¤ Á¤º¸°¡ ÀÖ´ÂÁö È®ÀÎÇÕ´Ï´Ù.
							if (shop_manager->GetAccount(input) == nullptr) {
								account.push_back(input);

								break;
							}
							else {
								IO.print("ÀÌ¹Ì Á¸ÀçÇÏ´Â ¾ÆÀÌµðÀÔ´Ï´Ù.\n");
								IO.pause();

								IO.rollback(checkpoint);
							}
						}
						else {
							IO.print(format("°ü¸®ÀÚ ¾ÆÀÌµð '{}'Àº ¾ÆÀÌµð·Î »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.\n", shop_manager->GetAdminAccount()->id));
							IO.pause();

							IO.rollback(checkpoint);
						}
					}
				}
				else if (i == 3) {
					// 'ºñ¹Ð¹øÈ£ È®ÀÎ' ÀÔ·Â
					auto checkpoint = IO.checkpoint();

					while (true) {
						string input = IO.input("", "ºñ¹Ð¹øÈ£ È®ÀÎ");

						if (input == account[2]) {
							break;
						}
						else
						{
							IO.print("'ºñ¹Ð¹øÈ£ È®ÀÎ'ÀÌ 'ºñ¹Ð¹øÈ£'¿Í ÀÏÄ¡ÇÏÁö ¾Ê½À´Ï´Ù.\n");
							IO.pause();

							IO.rollback(checkpoint);
						}
					}
				}
				else {
					// °¢°¢ÀÇ µ¥ÀÌÅÍ ¿ä¼Ò ÀÔ·Â¿¡ ´ëÇÑ Çü½ÄÀ» È®ÀÎÇÏ°í ÀúÀåÇÕ´Ï´Ù.
					account.push_back(IO.input(data_manager->GetParser(parser_key[i])));
				}
			}
			
			//ÁÖ¹® Ã³¸® Á¤º¸ ¸ñ·Ï
			account.push_back("");
			account.push_back("0");
			account.push_back("0"); 

			string input = IO.input("\n°èÁ¤À» »ý¼ºÇÏ½Ã°Ú½À´Ï±î? (y / n)");

			if (input == "y") {
				shop_manager->AddAccount(new Account(account));
			
				IO.print("\n°èÁ¤ÀÌ »ý¼ºµÇ¾ú½À´Ï´Ù.\n");
				IO.pause();
			}
			 
			menu_manager->RunMenu(MENU_START);
		}
	));
#pragma endregion

#pragma region °ü¸®ÀÚ ¸Þ´ºÈ­¸é
	// °ü¸®ÀÚ ¸Þ´ºÈ­¸é
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('l', 'q'); 
		_template.SetName("°ü¸®ÀÚ ¸Þ´ºÈ­¸é");
		_template.SubMenu("»óÇ° °ü¸® (µî·Ï/¼öÁ¤/Á¦°Å, Àç°í°ü¸®)", [=]() { menu_manager->RunMenu(MENU_A_PRODUCT_LIST, shop_manager->GetProductList()); });
		_template.SubMenu("°í°´ °ü¸® (°èÁ¤, ÁÖ¹®)",				  [=]() { menu_manager->RunMenu(MENU_A_ACCOUNT_LIST, shop_manager->GetAccountList()); });
		_template.Apply(MENU_ADMIN);
	}

	#pragma region »óÇ° ¸ñ·Ï
	// »óÇ° ¸ñ·Ï ¸Þ´ºÈ­¸é
	{
		TemplateTable<Product*> _template;
		_template.SetName("»óÇ° ¸ñ·Ï");

		// Å×ÀÌºí Ãâ·Â Çü½Ä ÁöÁ¤ 
		_template.header_func = []() -> string {
			return format("{0:<8}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", "ID", "»óÇ°", "Àå¸£", "ÀúÀÚ", "°¡°Ý", "Àç°í");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<8}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", format("{:06}", product->id), limit(product->title, 18), product->genre, product->author, to_string(product->price) + "¿ø", product->count);
		};

		// ¸Þ´º Ãß°¡ ¹× ±â´É
		_template.SubMenu('p', "»óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ", []() {
			set<string> genre_set;

			const vector<Product*>& product_list = shop_manager->GetProductList(); 

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				genre_set.insert(product->genre);
			}

			static vector<string> genre_list; genre_list = vector<string>(genre_set.begin(), genre_set.end());

			for (int i = 0; i < genre_list.size(); i += 10) {
				genre_list.insert(genre_list.begin() + i, "ÀüÃ¼");
			}

			menu_manager->RunMenu(MENU_A_PRODUCT_SEARCH, genre_list);
		});

		_template.SubMenu('r', "»óÇ° ½Å±Ô µî·Ï",         [=]() { menu_manager->RunMenu(MENU_A_PRODUCT_REGISTER);  });

		_template.next_menu_code = MENU_A_PRODUCT_INFO;

		_template.Apply(MENU_A_PRODUCT_LIST);
	}  

	// »óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ ¸Þ´ºÈ­¸é
	{
		TemplateTable<string> _template;
		_template.SetName("»óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ");
		_template.header_func = []() -> string {
			return "Àå¸£";
		};
		_template.show_func = [](string genre) -> string {
			return genre;
		};
		 
		_template.next_menu_code = MENU_A_PRODUCT_LIST; 

		_template.process_func = [=](MenuIO& IO, MenuCode next_menu_code, string genre) {
			string input = IO.input("", "Á¦¸ñ");

			const vector<Product*>& product_list = shop_manager->GetProductList();

			static vector<Product*> result; result.clear();

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				if ((input == "" || product->title == input) && (genre == "ÀüÃ¼" || product->genre == genre)) {
					result.push_back(product);
				}
			}
			 
			menu_manager->RunMenu(next_menu_code, result);
		};

		_template.Apply(MENU_A_PRODUCT_SEARCH);
	}

	// »óÇ° ½Å±Ô µî·Ï ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_A_PRODUCT_REGISTER, new Menu(
		[=](MenuIO& IO) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° ½Å±Ô µî·Ï ]");

			if (shop_manager->GetProductList().size() > 999999) {
				IO.print("´õ ÀÌ»ó »óÇ°À» µî·ÏÇÒ ¼ö ¾ø½À´Ï´Ù.\n");
				IO.pause();
				
				return;
			}

			vector<string> product;

			vector<string> parser_key = { "product_title", "product_genre", "product_author", "product_price", "product_count" };
			
			int id = (int)shop_manager->GetProductList().size();

			product.push_back(to_string(id));

			auto checkpoint = IO.checkpoint();

			for (int i = 0; i < parser_key.size(); i++) {
				product.push_back(IO.input(data_manager->GetParser(parser_key[i])));

				if (i == 2) {
					if (!shop_manager->IsProductRegisterable(-1, product[1], product[3])) {
						IO.print("°°Àº Á¦¸ñÀÇ µµ¼­³¢¸®´Â ÀúÀÚ±îÁö °°À» ¼ö ¾ø½À´Ï´Ù.\n");
						IO.pause();

						IO.rollback(checkpoint);

						product.clear();
						product.push_back(to_string(id));
						
						i = -1;	continue;
					}
				}
			}

			product.push_back("FALSE");

			string input = IO.input("\n»óÇ°À» µî·ÏÇÏ½Ã°Ú½À´Ï±î? (y / n)");

			if (input == "y") {
				Product* result = new Product(product);

				shop_manager->AddProduct(result);
				shop_manager->EnableProduct(result);

				IO.print(format("\n»óÇ°({})ÀÌ µî·ÏµÇ¾ú½À´Ï´Ù.\n", id));
				IO.pause();

				// »óÇ°À» µî·ÏÇÑ °æ¿ì ÀÌÀü ¸Þ´º·Î µ¹¾Æ°¡ ÀüÃ¼ »óÇ° µî·Ï Á¤º¸¸¦ º¸¿©ÁÝ´Ï´Ù.
				menu_manager->RunMenu(MENU_A_PRODUCT_LIST, shop_manager->GetProductList());
			}

			// »óÇ°À» µî·ÏÇÏÁö ¾ÊÀº °æ¿ì ÀÌÀü °Ë»ö »óÅÂ¸¦ À¯ÁöÇÏ¸é¼­ ÀÌÀü ¸Þ´º¸¦ º¸¿©ÁÝ´Ï´Ù.
		} 
	));

	// »óÇ° µî·Ï Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_A_PRODUCT_INFO, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) { 
			menu_manager->PrintCommand(); 
			IO.print_line(); 
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ ]");
			IO.print(target->ToString());
			IO.print(format("Àç°í : {0}\n", target->count));

			IO.print_line();

			IO.print_aligned_center("¼öÁ¤(M)     Á¦°Å(R)");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input.size() == 1) {
					char command = tolower(input[0]);

					if (command == 'm') {
						menu_manager->RunMenu(MENU_A_PRODUCT_INFO_M, target);
					}
					else if (command == 'r') { 
						menu_manager->RunMenu(MENU_A_PRODUCT_INFO_R, target);
					}
				} 

				IO.print("¸Þ´º¿¡ Ç¥½ÃµÈ ¾ËÆÄºª Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
		}
	));

	// »óÇ° µî·Ï Á¤º¸ ¼öÁ¤ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_A_PRODUCT_INFO_M, new Menu<Product*>(
		[=](MenuIO& IO, Product* target)  {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ ¼öÁ¤ ]");

			vector<string> product = target->ToArray();

			vector<string> parser_key = { "product_title", "product_genre", "product_author", "product_price", "product_count" };

			auto checkpoint_start = IO.checkpoint();

			for (int i = 0; i < parser_key.size(); i++) { 
				auto checkpoint = IO.checkpoint();

				while (true) { 
					Parser* parser = data_manager->GetParser(parser_key[i]);

					string input = IO.input("", parser->label);
					if (input == "" || parser->Check(input)) { 
						if (input != "") {
							product[i + 1] = input;
						}
						else {  
							IO.rollback(checkpoint);

							IO.print(format("{} : {}", parser->label, product[i + 1] + "\n"));
						}

						break;
					}
					else
					{
						IO.print(parser->msg_error);
						IO.pause();

						IO.rollback(checkpoint);
					}
				} 

				if (i == 2) {
					if (!shop_manager->IsProductRegisterable(target->id, product[1], product[3])) {
						IO.print("°°Àº Á¦¸ñÀÇ µµ¼­³¢¸®´Â ÀúÀÚ±îÁö °°À» ¼ö ¾ø½À´Ï´Ù.\n");
						IO.pause();

						IO.rollback(checkpoint_start);

						i = -1;	continue;
					}
				}
			}

			string input = IO.input("\n»óÇ° µî·Ï Á¤º¸¸¦ ¼öÁ¤ÇÏ½Ã°Ú½À´Ï±î? (y / n)");

			if (input == "y") {
				*target = Product(product);

				IO.print(format("»óÇ°({})ÀÇ µî·Ï Á¤º¸°¡ ¼öÁ¤µÇ¾ú½À´Ï´Ù.\n", target->id));

				IO.pause();
			}

			menu_manager->RunMenu(MENU_A_PRODUCT_INFO, target);
		}
	));

	// »óÇ° µî·Ï Á¤º¸ Á¦°Å ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_A_PRODUCT_INFO_R, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ Á¦°Å ]");

			string input = IO.input("»óÇ° µî·Ï Á¤º¸¸¦ Á¦°ÅÇÏ½Ã°Ú½À´Ï±î? (y / n)");

			if (input == "y") {
				shop_manager->DisableProduct(target);

				IO.print(format("\n»óÇ°({0})ÀÇ µî·Ï Á¤º¸°¡ Á¦°ÅµÇ¾ú½À´Ï´Ù.\n", target->id));
				IO.pause();

				menu_manager->RunMenu(MENU_A_PRODUCT_LIST, shop_manager->GetProductList());
			}
		}
	));
	#pragma endregion 

	#pragma region °í°´ ¸ñ·Ï
	// °í°´ ¸ñ·Ï ¸Þ´ºÈ­¸é 
	{
		TemplateTable<Account*> _template;
		_template.SetName("°í°´ ¸ñ·Ï");
		_template.header_func = []() -> string {
			return format("{0:<16}{1:<8}{2:<16}", "¾ÆÀÌµð", "ÀÌ¸§", "ÀüÈ­¹øÈ£");
		};
		_template.show_func = [](Account* product) -> string {
			return format("{0:<16}{1:<8}{2:<16}", product->id, product->name, phone_number(product->phone_number));
		};
		_template.SubMenu('p', "°í°´ °Ë»ö", [=]() { menu_manager->RunMenu(MENU_A_ACCOUNT_SEARCH);  });

		_template.next_menu_code = MENU_ACCOUNT_INFO;

		_template.Apply(MENU_A_ACCOUNT_LIST);
	}

	// °í°´ °Ë»ö ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[=](MenuIO& IO) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °Ë»ö ]");
			
			Account* target;

			auto checkpoint = IO.checkpoint();

			while (true) {

				string input = IO.input("¾ÆÀÌµð¸¦ ÀÔ·ÂÇÏ¼¼¿ä.");

				if (input == shop_manager->GetAdminAccount()->id) {
					IO.print("ÇØ´ç °èÁ¤Àº °ü¸®ÀÚ °èÁ¤ÀÔ´Ï´Ù.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
				else {
					if ((target = shop_manager->GetAccount(input)) != nullptr) {
						menu_manager->RunMenu(MENU_ACCOUNT_INFO, target);
					}
					else
					{
						IO.print("¾ÆÀÌµð°¡ Á¸ÀçÇÏÁö ¾Ê½À´Ï´Ù.\n");
						IO.pause();

						IO.rollback(checkpoint);
					}
				}
			}
		}
	));
	
	// °í°´ °èÁ¤ Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_ACCOUNT_INFO, new Menu<Account*>(
		[=](MenuIO& IO, Account* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °èÁ¤ Á¤º¸ ]");

			IO.print(format("ÀÌ¸§ : {0}\n", target->name));
			IO.print(format("¾ÆÀÌµð : {0}\n", target->id));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", phone_number(target->phone_number)));
			IO.print(format("ÁÖ¼Ò : {0}\n", target->address));

			IO.print("\n");

			IO.print(format("º¸À¯ÇÑ 3000¿ø ÄíÆù °³¼ö : {}\n", target->coupon_count));
			IO.print(format("´ÙÀ½ ÄíÆù±îÁö ³²Àº ±Ý¾× : {}¿ø\n", target->accumulated));

			IO.print_line();

			IO.print_aligned_center("ÁÖ¹® Ã³¸® Á¤º¸(O)     ¼öÁ¤(M)");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input.size() == 1) {
					char command = tolower(input[0]);

					if (command == 'o') {
						menu_manager->RunMenu(MENU_INVOICE_LIST, target->invoice_id_list);
					}
					else if (command == 'm') {
						menu_manager->RunMenu(MENU_ACCOUNT_INFO_M, target);
					}
				}

				IO.print("¸Þ´º¿¡ Ç¥½ÃµÈ ¾ËÆÄºª Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
		}
	));

	// °í°´ °èÁ¤ Á¤º¸ ¼öÁ¤ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_ACCOUNT_INFO_M, new Menu<Account*>(
		[=](MenuIO& IO, Account* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °èÁ¤ Á¤º¸ ¼öÁ¤ ]");

			vector<string> orignal = target->ToArray();

			vector<string> account;

			vector<string> parser_key = { "account_password","","account_phone_number","account_address" };

			account.push_back(orignal[0]);
			account.push_back(orignal[1]);

			for (int i = 0; i < parser_key.size(); i++) {
				if (i == 1) {
					// 'ºñ¹Ð¹øÈ£ È®ÀÎ' ÀÔ·Â
					auto checkpoint = IO.checkpoint();

					while (true) {
						string input = IO.input("", "ºñ¹Ð¹øÈ£ È®ÀÎ");

						if (input == account[2]) {
							break;
						}
						else
						{
							IO.print("'ºñ¹Ð¹øÈ£ È®ÀÎ'ÀÌ 'ºñ¹Ð¹øÈ£'¿Í ÀÏÄ¡ÇÏÁö ¾Ê½À´Ï´Ù.\n");
							IO.pause();

							IO.rollback(checkpoint);
						}
					}
				}
				else {
					// °¢°¢ÀÇ µ¥ÀÌÅÍ ¿ä¼Ò ÀÔ·Â¿¡ ´ëÇÑ Çü½ÄÀ» È®ÀÎÇÏ°í ÀúÀåÇÕ´Ï´Ù.
					account.push_back(IO.input(data_manager->GetParser(parser_key[i])));
				}
			}

			account.push_back(orignal[5]);
			account.push_back(orignal[6]);
			account.push_back(orignal[7]);

			string input = IO.input("\n°èÁ¤ Á¤º¸¸¦ ¼öÁ¤ÇÏ½Ã°Ú½À´Ï±î? (y / n)");
				
			if (input == "y") {
				*target = Account(account);

				IO.print("\n°èÁ¤ Á¤º¸°¡ ¼öÁ¤µÇ¾ú½À´Ï´Ù.\n");
				IO.pause();
			}

			menu_manager->RunMenu(MENU_ACCOUNT_INFO, target);	
		}
	));
	// ±¸¸Å ³»¿ª ¸Þ´ºÈ­¸é
	{
		TemplateTable<int> _template;
		_template.SetName("±¸¸Å ³»¿ª");
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<8}{3:<12}{4:<10}", "±¸¸Å ³¯Â¥", "Á¦¸ñ", "ÀúÀÚ", "°áÁ¦ ±Ý¾×", "»óÅÂ");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager->GetInvoice(id);
			Product* product = shop_manager->GetProduct(invoice->product_id);

			return format("{0:<12}{1:<20}{2:<8}{3:<12}{4:<10}", date_to_string(invoice->date), limit(product->title, 18), product->author,
															  to_string(product->price * invoice->product_count) + "¿ø", invoice->GetState());
		};

		_template.next_menu_code = MENU_INVOICE_INFO;

		_template.Apply(MENU_INVOICE_LIST);
	}

	// ÁÖ¹® »ó¼¼ Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_INVOICE_INFO, new Menu<int>(
		[=](MenuIO& IO, int target_id) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ÁÖ¹® »ó¼¼ Á¤º¸ ]");

			Invoice* target = shop_manager->GetInvoice(target_id);
			Product* product = shop_manager->GetProduct(target->product_id);
			Account* account = shop_manager->GetAccount(target->buyer_id);

			IO.print(format("±¸¸Å ³¯Â¥ : {0}\n", date_to_string(target->date)));

			IO.print("\n[ »óÇ° »ó¼¼ Á¤º¸ ]\n");
			IO.print(product->ToString());

			IO.print("\n[ ÁÖ¹®ÀÚ Á¤º¸ ]\n");
			IO.print(format("¾ÆÀÌµð : {0}\n", account->id));
			IO.print(format("ÀÌ¸§ : {0}\n", account->name));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", phone_number(target->recipient_phone_number)));
			IO.print(format("ÁÖ¼Ò : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("»óÇ° °íÀ¯ ¹øÈ£ : {0}\n", format("{:06}", target->product_id)));
			IO.print(format("»óÇ° ¼ö·® : {0}\n", target->product_count));
			IO.print(format("°áÁ¦ ±Ý¾× : {0}¿ø\n", target->price));
			IO.print(format("»ç¿ëÇÑ 3000¿ø ÄíÆù °³¼ö : {0}°³\n", target->coupon_count));
			IO.print(format("ÃÖÁ¾ °áÁ¦ ±Ý¾× : {0}¿ø\n", target->final_price));
			IO.print("\n");
			IO.print(format("»óÅÂ : {0}\n", target->GetState()));


			//¸Þ´º ºÐÇÒ
			if (shop_manager->IsAdmin() && target->state != PURCHASED) {
				IO.print_line();
				IO.pause();
			}
			else { 
				IO.print_line(false);
				IO.print_aligned_center("¹ÝÇ°(R)");
				IO.print_line();

				auto checkpoint = IO.checkpoint();
				string input = IO.input();
				if (input.size() == 1) {
					char command = tolower(input[0]);

					if (command == 'r') {
						menu_manager->RunMenu(MENU_B_REFUND, target);
					}
				}
				IO.print("¸Þ´º¿¡ Ç¥½ÃµÈ ¾ËÆÄºª Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.\n");
				IO.pause();

				IO.rollback(checkpoint);
			}
		}
	));
	#pragma endregion

#pragma endregion

#pragma region ±¸¸ÅÀÚ ¸Þ´ºÈ­¸é
	// ±¸¸ÅÀÚ ¸Þ´ºÈ­¸é
	{
		TemplateMenuSelection _template;
		_template.SetName("±¸¸ÅÀÚ ¸Þ´ºÈ­¸é");
		_template.ToggleCommand('l', 'q');
		_template.SubMenu("»óÇ° ¸ñ·Ï", [=]() { menu_manager->RunMenu(MENU_B_PRODUCT_LIST, shop_manager->GetProductList()); });
		_template.SubMenu("°í°´ °èÁ¤ Á¤º¸", [=]() { menu_manager->RunMenu(MENU_ACCOUNT_INFO, shop_manager->GetCurrentAccount()); });
		_template.Apply(MENU_BUYER);
	}
	
	#pragma region »óÇ° ¸ñ·Ï
	// »óÇ° ¸ñ·Ï ¸Þ´ºÈ­¸é
	{
		TemplateTable<Product*> _template;
		_template.SetName("»óÇ° ¸ñ·Ï");

		// Å×ÀÌºí Ãâ·Â Çü½Ä ÁöÁ¤ 
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", "ID", "»óÇ°", "Àå¸£", "ÀúÀÚ", "°¡°Ý", "Àç°í");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<8}{4:<12}{5:<8}", format("{:06}", product->id), limit(product->title, 18), product->genre, product->author, to_string(product->price) + "¿ø", product->count);
		}; 

		// ¸Þ´º Ãß°¡ ¹× ±â´É
		_template.SubMenu('p', "»óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ", []() {
			set<string> genre_set;

			const vector<Product*>& product_list = shop_manager->GetProductList(); 

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				genre_set.insert(product->genre);
			}

			static vector<string> genre_list; genre_list = vector<string>(genre_set.begin(), genre_set.end());

			for (int i = 0; i < genre_list.size(); i += 10) {
				genre_list.insert(genre_list.begin() + i, "ÀüÃ¼");
			}

			menu_manager->RunMenu(MENU_B_PRODUCT_SEARCH, genre_list);
		});

		_template.next_menu_code = MENU_B_PRODUCT_INFO;

		_template.Apply(MENU_B_PRODUCT_LIST);
	}  

	// »óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ ¸Þ´ºÈ­¸é
	{
		TemplateTable<string> _template;
		_template.SetName("»óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ");
		_template.header_func = []() -> string {
			return "Àå¸£";
		};
		_template.show_func = [](string genre) -> string {
			return genre;
		};
		 
		_template.next_menu_code = MENU_B_PRODUCT_LIST; 

		_template.process_func = [=](MenuIO& IO, MenuCode next_menu_code, string genre) {
			string input = IO.input("", "Á¦¸ñ");

			const vector<Product*>& product_list = shop_manager->GetProductList();

			static vector<Product*> result; result.clear();

			for (int i = 0; i < product_list.size(); i++) {
				Product* product = product_list[i];

				if ((input == "" || product->title == input) && (genre == "ÀüÃ¼" || product->genre == genre)) {
					result.push_back(product);
				}
			}
			 
			menu_manager->RunMenu(next_menu_code, result);
		};

		_template.Apply(MENU_B_PRODUCT_SEARCH);
	}
#pragma endregion

	// »óÇ° µî·Ï Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_B_PRODUCT_INFO, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° »ó¼¼ Á¤º¸ ]");
			IO.print(target->ToString());
			IO.print(format("Àç°í : {0}\n", target->count));

			IO.print_line(false);

			if (target->count > 0) {
				IO.print_aligned_center("±¸¸Å(B)");

				IO.print_line();

				auto checkpoint = IO.checkpoint();

				while (true) {
					string input = IO.input();

					if (input.size() == 1) {
						char command = tolower(input[0]);

						if (command == 'b') {
							menu_manager->RunMenu(MENU_B_PRODUCT_BUY, target);
						}
					}

					IO.print("¸Þ´º¿¡ Ç¥½ÃµÈ ¾ËÆÄºª Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
			else {
				IO.print_aligned_center("Ç°ÀýµÇ¾ú½À´Ï´Ù.");

				IO.print_line();
				IO.pause();
			}
		}
	));

	// »óÇ° ±¸¸Å ¸Þ´ºÈ­¸é
	menu_manager->AppendMenu(MENU_B_PRODUCT_BUY, new Menu<Product*>(
		[=](MenuIO& IO, Product* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° ±¸¸Å ]"); 

			if (shop_manager->GetInvoiceList().size() > 99999999) {
				IO.print("´õ ÀÌ»ó »óÇ°À» ±¸¸ÅÇÒ ¼ö ¾ø½À´Ï´Ù.\n");
				IO.pause();

				return;
			}

			int count, price, coupon_count, final_price;
			string recipient_phone_number, recipient_address;
			Account* user = shop_manager->GetCurrentAccount();

			vector<string> invoice = { user->id, "", "", "", to_string(target->id)};

			string input;

			IO.print_line();
			IO.print("[ »óÇ° »ó¼¼ Á¤º¸ ]\n");
			IO.print(target->ToString());
			IO.print(format("Àç°í : {0}\n", target->count));
			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				input = IO.input("ÁÖ¹® ¼ö·®À» ÀÔ·ÂÇÏ¼¼¿ä.");

				if (data_manager->GetParser("invoice_product_count")->Check(input)) {
					if (target->count >= stoi(input))
					{
						count = stoi(input);

						break;
					}
					else {
						IO.print("ÁÖ¹® ¼ö·®À» ÃÊ°úÇß½À´Ï´Ù.\n");
					}
				}
				else {
					IO.print(data_manager->GetParser("invoice_product_count")->msg_error);
				}

				IO.pause();
				IO.rollback(checkpoint);
			}
			IO.print_line();
			IO.print("[ ÁÖ¹®ÀÚ Á¤º¸ ]\n");
			IO.print(format("ÀÌ¸§ : {0}\n", user->name));
			 
			checkpoint = IO.checkpoint();
			while (true) {
				input = IO.input("", "ÀüÈ­¹øÈ£");

				if (data_manager->GetParser("account_phone_number")->Check(input) || input == "") { 
					recipient_phone_number = input == "" ? user->phone_number : input;

					IO.rollback(checkpoint); 
					IO.print(format("{} : {}\n", data_manager->GetParser("account_phone_number")->label, phone_number(recipient_phone_number)));

					break;
				}
				else {
					IO.print(data_manager->GetParser("account_phone_number")->msg_error);
				}

				IO.pause();
				IO.rollback(checkpoint);
			} 

			checkpoint = IO.checkpoint();
			
			recipient_address = IO.input(data_manager->GetParser("account_address"));

			recipient_address = recipient_address == "" ? user->address : recipient_address;

			IO.rollback(checkpoint); 
			IO.print(format("{} : {}\n", data_manager->GetParser("account_address")->label, recipient_address));
 
			IO.print_line(false);
			IO.print("[ °áÁ¦±Ý¾× ]\n");
			IO.print(format("{0}¿ø X {1}±Ç = {2}¿ø\n", target->price, count, price = target->price*count));
			IO.print_line();

			checkpoint = IO.checkpoint();

			while (true) {
				IO.print_aligned_center("»ç¿ëÇÏ½Ç ÄíÆù °³¼ö¸¦ ÀÔ·ÂÇÏ¼¼¿ä");
				IO.print_aligned_center(format("º¸À¯ ÄíÆù °³¼ö : {0}°³", user->coupon_count));

				string input = IO.input();
				if (data_manager->GetParser("account_coupon_count")->Check(input)) {
					coupon_count = stoi(input);
					if (user->coupon_count >= coupon_count)
					{				
						int max_coupon_count = (int)ceil(float(price) / 3000);
						if (max_coupon_count < coupon_count) //ÃÖ´ë »ç¿ë °¡´É ÄíÆùº¸´Ù ¸¹À¸¸é
						{
							IO.print(format("ÄíÆùÀ» {0}°³º¸´Ù ¸¹ÀÌ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù\n", max_coupon_count));
						}
						else {
							break;
						}					
					}
					else {
						IO.print("ÀÔ·ÂÇÑ ¼ö·®ÀÌ º¸À¯ÇÑ ÄíÆù °³¼ö¸¦ ÃÊ°úÇß½À´Ï´Ù.\n");
					}
				}
				else {
					IO.print(data_manager->GetParser("account_coupon_count")->msg_error);				
				}
				IO.pause();
				IO.rollback(checkpoint);
			}
			IO.print_line();

			final_price = max(price - 3000 * coupon_count, 0);
			
			IO.print(format("ÃÖÁ¾ °áÁ¦ ±Ý¾× : {0}\n", final_price));
			IO.print_line();

			input = IO.input("»óÇ°À» ÁÖ¹®ÇÏ½Ã°Ú½À´Ï±î? (y / n)");

			if (input == "y") {
				int id = (int)shop_manager->GetInvoiceList().size();
				target->count -= count; //Á¦Ç° °³¼ö ¾÷µ¥ÀÌÆ®
				user->coupon_count -= coupon_count;
				invoice.push_back(to_string(count));
				invoice.push_back(to_string(price));
				invoice.push_back(to_string(coupon_count));
				invoice.push_back(to_string(final_price));
				invoice.push_back("0");
				invoice.insert(invoice.begin(), to_string(id));	

				Invoice* result = new Invoice(invoice);

				result->date = today();
				result->recipient_phone_number = recipient_phone_number;
				result->recipient_address = recipient_address;

				// ÀüÃ¼ ¸ñ·Ï¿¡¼­´Â ¸¶Áö¸·¿¡ ÁÖ¹® Ã³¸® Á¤º¸ Ãß°¡
				shop_manager->AddInvoice(result);
				// °í°´¿¡°Ô´Â ³¯Â¥°¡ ³»¸²Â÷ ¼øÀ¸·Î º¸ÀÌµµ·Ï Ã¹¹øÂ° À§Ä¡¿¡ Ãß°¡
				user->invoice_id_list.insert(user->invoice_id_list.begin(), id);

				IO.print("\n»óÇ°À» ±¸¸ÅÇÏ¿´½À´Ï´Ù.\n");
				IO.pause();
			}
		}
	));

	//±¸¸ÅÀÚ ¹°°Ç ¹ÝÇ°
	menu_manager->AppendMenu(MENU_B_REFUND, new Menu<Invoice*>(
		[=](MenuIO& IO, Invoice* target) {
			menu_manager->PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ¹ÝÇ° ]");

			Product* product = shop_manager->GetProduct(target->product_id, true);
			Account* account = shop_manager->GetAccount(target->buyer_id);

			IO.print(format("°áÁ¦±Ý¾× : {0}¿ø\n", target->price));
			IO.print(format("»ç¿ëÇÑ 3000¿ø ÄíÆù °³¼ö : {0}°³\n", target->coupon_count));
			IO.print(format("ÃÖÁ¾ °áÁ¦±Ý¾× : {0}¿ø\n", target->final_price));

			IO.print_line();

			string input = IO.input("ÇØ´ç »óÇ°À» ¹ÝÇ°ÇÏ½Ã°Ú½À´Ï±î? (y / n)\n(»ç¿ëÇÑ ÄíÆùÀº µ¹·Á¹ÞÀ» ¼ö ¾ø°í, ÃÖÁ¾ °áÁ¦±Ý¾×¸¸ µ¹·Á¹Þ½À´Ï´Ù.)");

			if (input == "y") {
				if (product->deleted) {
					shop_manager->EnableProduct(product);
				} 
				product->count += target->product_count;

				target->state = REFUNDED;

				IO.print("\nÇØ´ç »óÇ°ÀÌ ¹ÝÇ°µÇ¾ú½À´Ï´Ù.\n");
				IO.pause();

			}
			menu_manager->RunMenu(MENU_INVOICE_LIST, account->invoice_id_list);
		}
	));
#pragma endregion
}
#pragma endregion

