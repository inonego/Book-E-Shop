#include "program.h"
#include "Util.h"

#include <format>

DataManager Program::data_manager;
MenuManager Program::menu_manager;
ShopManager Program::shop_manager;

void Program::Run()
{
	LoadCSV();
	SetCommand();
	SetParser();
	SetMenu();

	menu_manager.Start(MENU_START);
}

void Program::LoadCSV()
{
	// Product CSV ÆÄÀÏÀ» ÆÄ½ÌÇÕ´Ï´Ù.
	auto product_raw = data_manager.ParseCSV("./data/product.csv");
	for (size_t i = 0; i < product_raw.size(); i++) {
		// »ý¼ºµÈ Product °´Ã¼¸¦ shop_manager¿¡ Ãß°¡ÇÕ´Ï´Ù.
		shop_manager.AddProduct(new Product(product_raw[i]));
	}

	// Account CSV ÆÄÀÏÀ» ÆÄ½ÌÇÕ´Ï´Ù.
	auto account_raw = data_manager.ParseCSV("./data/account.csv");
	for (size_t i = 0; i < account_raw.size(); i++) {
		// »ý¼ºµÈ Account °´Ã¼¸¦ shop_manager¿¡ Ãß°¡ÇÕ´Ï´Ù.
		shop_manager.AddAccount(new Account(account_raw[i]));
	}

	// Invoice CSV ÆÄÀÏÀ» ÆÄ½ÌÇÕ´Ï´Ù.
	auto invoice_raw = data_manager.ParseCSV("./data/invoice.csv");
	for (size_t i = 0; i < invoice_raw.size(); i++) {
		// »ý¼ºµÈ Invoice °´Ã¼¸¦ shop_manager¿¡ Ãß°¡ÇÕ´Ï´Ù.
		shop_manager.AddInvoice(new Invoice(invoice_raw[i]));
	}
}

void Program::SetCommand()
{
	menu_manager.AppendCommand('z', new MenuManager::Command("µÚ·Î°¡±â", [&]() {
		menu_manager.RunPreviousMenu();
	}));

	menu_manager.AppendCommand('l', new MenuManager::Command("·Î±×¾Æ¿ô", [&]() {
		menu_manager.RunRecursiveMenu(MENU_LOGOUT);
	}));

	menu_manager.AppendCommand('q', new MenuManager::Command("Á¾·á", [&]() {
		menu_manager.RunRecursiveMenu(MENU_QUIT);
	}));
}

#pragma region Parser ±¸Çö
void Program::SetParser()
{
	data_manager.AppendParser("test", (new Parser())
		->set_label("³ªÀÌ")
		->set_msg_info("³ªÀÌ¸¦ ÀÔ·ÂÇÏ¼¼¿ä!")
	);

#pragma region *** account Parser ***
	data_manager.AppendParser("account_name", (new Parser())
		->set_label("ÀÌ¸§")
		->set_regex(R"(^[ ]*[a-zA-Z°¡-ÆR]+[ a-zA-Z°¡-ÆR]*$)")
		->set_msg_error("ÀÌ¸§Àº °ø¹é, ¿µ¹®ÀÚ, ÇÑ±Û Çã¿ëÇÑ °ø¹éÀ» Á¦¿ÜÇÑ ±æÀÌ°¡ 1 ÀÌ»óÀÎ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù!")
	);
	data_manager.AppendParser("account_id", (new Parser())
		->set_label("¾ÆÀÌµð")
		->set_regex(R"(^[0-9a-zA-Z]{4,12}$)")
		->set_msg_error("¾ÆÀÌµð´Â ¼ýÀÚ, ¿µ¹®ÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 4 ÀÌ»ó 12 ÀÌÇÏÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù!")
	);

	data_manager.AppendParser("account_password", (new Parser())
		->set_label("ºñ¹Ð¹øÈ£")
		->set_regex(R"(^[0-9a-zA-Z!@#$%^&*_]{8,16}$)")
		->set_msg_error("ºñ¹Ð¹øÈ£´Â ¼ýÀÚ, ¿µ¹®ÀÚ, Æ¯¼ö ¹®ÀÚ(!@#$%^&*_)·Î ±¸¼ºµÈ ±æÀÌ°¡ 8 ÀÌ»ó 16 ÀÌÇÏÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù!")
	);
	data_manager.AppendParser("account_phonenumber", (new Parser())
		->set_label("ÀüÈ­¹øÈ£")
		->set_regex(R"(^010[0-9]{8}$)")
		->set_msg_error("ÀüÈ­¹øÈ£´Â ¼ýÀÚ·Î ±¸¼ºµÇ¾î 010À¸·Î ½ÃÀÛÇÏ°í, ±æÀÌ°¡ 11ÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù!")
	);
	data_manager.AppendParser("account_address", (new Parser())
		->set_label("ÁÖ¼Ò")
	);
	data_manager.AppendParser("account_invoicelist", (new Parser())
		->set_label("ÁÖ¹® Ã³¸® Á¤º¸ ¸ñ·Ï")
	);
#pragma endregion

#pragma region *** product Parser ***
	data_manager.AppendParser("product_id", (new Parser())
		->set_label("°íÀ¯¹øÈ£")
		->set_regex(R"(^[0-9]{6}$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 6ÀÎ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.")
	);
	data_manager.AppendParser("product_title", (new Parser())
		->set_label("Á¦¸ñ")
	);
	data_manager.AppendParser("product_genre", (new Parser())
		->set_label("Àå¸£")
	);
	data_manager.AppendParser("product_price", (new Parser())
		->set_label("°¡°Ý")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.")
	);
	data_manager.AppendParser("product_count", (new Parser())
		->set_label("Àç°í")
		->set_regex(R"(^[0-9]+$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù.")
	);
#pragma endregion

#pragma region *** invoice Parser ***
	data_manager.AppendParser("invoice_id", (new Parser())
		->set_label("°íÀ¯¹øÈ£")
		->set_regex(R"(^[0-9]{8}$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 8ÀÎ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù")
	);
	data_manager.AppendParser("invoice_buyer_id", (new Parser())
		->set_label("±¸¸ÅÀÚÀÇ ¾ÆÀÌµð")
		->set_msg_info("±¸¸ÅÀÚÀÇ ¾ÆÀÌµð ÀÔ´Ï´Ù.")
	);
	data_manager.AppendParser("invoice_recipient_phonenumber", (new Parser())
		->set_label("¼ö·ÉÀÎÀÇ ÀüÈ­¹øÈ£")
		->set_msg_info("¼ö·ÉÀÎÀÇ ÀüÈ­¹øÈ£ ÀÔ´Ï´Ù.")
	);
	data_manager.AppendParser("invoice_recipient_address", (new Parser())
		->set_label("¼ö·ÉÀÎÀÇ ÁÖ¼Ò")
		->set_msg_info("¼ö·ÉÀÎÀÇ ÁÖ¼Ò ÀÔ´Ï´Ù.")
	);
	data_manager.AppendParser("invoice_date", (new Parser())
		->set_label("±¸¸Å ³¯Â¥")
		->set_regex(R"(YY.MM.DD)")
		->set_msg_error("±¸¸Å ³¯Â¥´Â (¿¬µµ).(¿ù).(ÀÏ)·Î ±¸¼ºµÈ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù")
	);
	data_manager.AppendParser("invoice_product_id", (new Parser())
		->set_label("»óÇ° °íÀ¯ ¹øÈ£")
		->set_msg_info("±¸¸ÅÇÑ »óÇ° °íÀ¯ ¹øÈ£ ÀÔ´Ï´Ù.")
	);
	data_manager.AppendParser("invoice_product_count", (new Parser())
		->set_label("»óÇ° ¼ö·®")
		->set_regex(R"(^[1-9]*$)")
		->set_msg_error("¼ýÀÚ·Î ±¸¼ºµÈ ±æÀÌ°¡ 1 ÀÌ»óÀÇ ¹®ÀÚ¿­ÀÌ¾î¾ß ÇÕ´Ï´Ù")
	);
#pragma endregion


	data_manager.AppendParser("MENU_MENUSELECTION", (new Parser())
		->set_regex(R"(\d)")
		->set_msg_error("¸Þ´º¿¡ Ç¥½ÃµÈ ¹øÈ£ Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.")
		->set_parse([&](string input) -> any { return stoi(input); })
	);

	data_manager.AppendParser("MENU_TABLE", (new Parser())
		->set_regex(R"(^[0-9a-zA-Z]$)")
		->set_msg_error("¸Þ´º¿¡ Ç¥½ÃµÈ ¹øÈ£ ¶Ç´Â ¾ËÆÄºª Áß ÇÏ³ª¸¦ °í¸£¼¼¿ä.")
		->set_parse([&](string input) -> any { return input[0]; })
	);
}
#pragma endregion

#pragma region ¸Þ´ºÈ­¸é ±¸Çö

void Program::SetMenu()
{	
#pragma region °øÅë ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_QUIT, new Menu([&](MenuIO& IO) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ ÇÁ·Î±×·¥ Á¾·á ]");
		IO.print("ÇÁ·Î±×·¥À» Á¾·áÇÏ½Ã°Ú½À´Ï±î? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			exit(0);
		}
		}));

	menu_manager.AppendMenu(MENU_LOGOUT, new Menu([&](MenuIO& IO) {
		menu_manager.ToggleCommand();
		menu_manager.PrintCommand();

		IO.print_line();
		IO.print_aligned_center("[ °èÁ¤ ·Î±×¾Æ¿ô ]");
		IO.print("°èÁ¤¿¡¼­ ·Î±×¾Æ¿ô ÇÏ½Ã°Ú½À´Ï±î? (y / n)\n");
		string input = IO.input();

		if (input == "y") {
			shop_manager.Logout();

			menu_manager.RunMenu(MENU_START);
		}
		}));
#pragma endregion


#pragma region ½ÃÀÛ ¸Þ´ºÈ­¸é
	// ½ÃÀÛ ¸Þ´ºÈ­¸é
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('q');
		_template.SetName("½ÃÀÛ ¸Þ´º");
		_template.SubMenu("°èÁ¤ ·Î±×ÀÎ",   []() { menu_manager.RunMenu(MENU_LOGIN);  });	
		_template.SubMenu("°èÁ¤ È¸¿ø°¡ÀÔ", []() { menu_manager.RunMenu(MENU_SIGNUP); });
		_template.Apply(MENU_START);
	} 

	// °èÁ¤ ·Î±×ÀÎ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_LOGIN, new Menu(
		[&](MenuIO& IO) { 
			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
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
				if ((user = shop_manager.Login(id, password)) != nullptr) {
					if (user == shop_manager.GetAdminAccount()) {
						menu_manager.RunMenu(MENU_ADMIN);
					}
					else {
						menu_manager.RunMenu(MENU_BUYER);
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
	menu_manager.AppendMenu(MENU_SIGNUP, new Menu(
		[&](MenuIO& IO) {
			menu_manager.ToggleCommand('z', 'q');
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °èÁ¤ È¸¿ø°¡ÀÔ ]");

			auto checkpoint = IO.checkpoint();
			vector<string> account;
			string input;
			vector<string> heads = { "ÀÌ¸§","¾ÆÀÌµð","ºñ¹Ð¹øÈ£","ºñ¹Ð¹øÈ£ È®ÀÎ","ÀüÈ­¹øÈ£","ÁÖ¼Ò" };
			vector<string> regex_t = { "account_name", "account_id","account_password","","account_phonenumber","account_address" };
			int t = 0;
			while (t < heads.size()) {				
				if (t != 3) {//ºñ¹Ð¹øÈ£ °Ë»ç
					checkpoint = IO.checkpoint();
					input = IO.input("", heads[t]);
					if (data_manager.GetParser(regex_t[t])->Check(input)) {
						account.push_back(input);
						t++;
					}
					else
						IO.rollback(checkpoint);
				}
				else {
					checkpoint = IO.checkpoint();
					string match_p = IO.input("",heads[t]);
					if (match_p._Equal(input))
						t++;
					else 
						IO.rollback(checkpoint);

				}			
			}
			account.push_back("");
			shop_manager.AddAccount(new Account(account));
			IO.pause();
			menu_manager.RunMenu(MENU_START);
			//id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));
			//password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));

		}
	));
#pragma endregion

#pragma region °ü¸®ÀÚ ¸Þ´ºÈ­¸é
	// °ü¸®ÀÚ ¸Þ´ºÈ­¸é
	{
		TemplateMenuSelection _template;
		_template.ToggleCommand('l', 'q'); 
		_template.SetName("°ü¸®ÀÚ ¸Þ´ºÈ­¸é");
		_template.SubMenu("»óÇ° °ü¸® (µî·Ï/¼öÁ¤/Á¦°Å, Àç°í°ü¸®)", []() { menu_manager.RunMenu(MENU_A_PRODUCT_LIST, shop_manager.GetProdcutList()); });
		_template.SubMenu("°í°´ °ü¸® (°èÁ¤, ÁÖ¹®)",				  []() { menu_manager.RunMenu(MENU_A_ACCOUNT_LIST, shop_manager.GetAccountList()); });
		_template.Apply(MENU_ADMIN);
	}

	#pragma region »óÇ° ¸ñ·Ï
	// »óÇ° ¸ñ·Ï ¸Þ´ºÈ­¸é
	{
		TemplateTable<Product*> _template;
		_template.SetName("»óÇ° ¸ñ·Ï");
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<12}{4:<8}", "ID", "»óÇ°", "Àå¸£", "°¡°Ý", "Àç°í");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<12}{4:<8}", product->id, limit(product->title, 18), product->genre, to_string(product->price) + "¿ø", product->count);
		}; 
		_template.SubMenu('p', MENU_A_PRODUCT_SEARCH, "»óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ");
		_template.SubMenu('r', MENU_A_PRODUCT_REGISTER, "»óÇ° ½Å±Ô µî·Ï");

		_template.next_menu_code = MENU_A_PRODUCT_INFO;

		_template.Apply(MENU_A_PRODUCT_LIST);
	} 
	
	// »óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_PRODUCT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ ]");
		}
	));

	// »óÇ° ½Å±Ô µî·Ï ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_PRODUCT_REGISTER, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° ½Å±Ô µî·Ï ]");

			auto checkpoint = IO.checkpoint();
			vector<string> product;
			string input;
			vector<string> heads = { "Á¦¸ñ","Àå¸£","°¡°Ý","Àç°í" };
			vector<string> regex_t = { "product_title","product_genre","product_price","product_count" };
			int t = 0;
			while (t < heads.size()) {
				checkpoint = IO.checkpoint();
				input = IO.input("", heads[t]);
				if (data_manager.GetParser(regex_t[t])->Check(input)) {
					product.push_back(input);
					t++;
				}
				else
					IO.rollback(checkpoint);
			}
			input = IO.input("»óÇ°À» µî·ÏÇÏ½Ã°Ú½À´Ï±î?(y/n)","");
			if (input == "y") {
				int id = 7 + shop_manager.GetProdcutList().size();
				product.insert(product.begin(), to_string(id));
				shop_manager.AddProduct(new Product(product));
				IO.pause();
			}
			menu_manager.RunMenu(MENU_A_PRODUCT_LIST,shop_manager.GetProdcutList());
			//id = any_cast<string>(IO.input(data_manager.GetParser("account_id")));
			//password = any_cast<string>(IO.input(data_manager.GetParser("account_password")));
		}
	));

	// »óÇ° µî·Ï Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) { 
			menu_manager.PrintCommand(); 
			IO.print_line(); 
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ ]");

			IO.print(format("°íÀ¯¹øÈ£ : {0}\n", target->id));
			IO.print(format("Á¦¸ñ : {0}\n", target->title));
			IO.print(format("Àå¸£ : {0}\n", target->genre));
			IO.print(format("°¡°Ý : {0}\n", target->price));
			IO.print(format("Àç°í : {0}\n", target->count));

			IO.print_line();

			IO.print("¼öÁ¤(M) Á¦°Å(R)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "M") {
					menu_manager.RunMenu(MENU_A_PRODUCT_INFO_M, target);
				}
				else if (input == "R") {
					//IO.print("Á¦°Å");
				}
				else {
					IO.print("ÀÏÄ¡ÇÏ´Â ¸í·É¾î°¡ ¾øÀ½.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));

	// »óÇ° µî·Ï Á¤º¸ ¼öÁ¤ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_M, new Menu<Product*>(
		[&](MenuIO& IO,Product* target)  {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ ¼öÁ¤ ]");

			auto checkpoint = IO.checkpoint();
			vector<string> product;
			string input;
			vector<string> heads = { "Á¦¸ñ","Àå¸£","°¡°Ý","Àç°í" };
			vector<string> regex_t = { "product_title","product_genre","product_price","product_count" };
			int t = 0;
			while (t < heads.size()) {
				checkpoint = IO.checkpoint();
				input = IO.input("", heads[t]);
				if (data_manager.GetParser(regex_t[t])->Check(input) || input._Equal("")) {
					product.push_back(input);
					t++;
				}
				else
					IO.rollback(checkpoint);
			}
			input = IO.input("»óÇ° µî·Ï Á¤º¸¸¦ ¼öÁ¤ÇÏ½Ã°Ú½À´Ï±î?(y/n)", "");
			if (input == "y") {
				target->title = product[0]._Equal("") ? target->title : product[0];
				target->genre = product[1]._Equal("") ? target->genre : product[1];
				target->price = product[2]._Equal("") ? target->price : stoi(product[2]);
				target->count = product[3]._Equal("") ? target->count : stoi(product[3]);
				IO.pause();
			}
			menu_manager.RunMenu(MENU_A_PRODUCT_INFO, target);
		}
	));

	// »óÇ° µî·Ï Á¤º¸ Á¦°Å ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_PRODUCT_INFO_R, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ Á¦°Å ]");

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
			return format("{0:<16}{1:<8}{2:<16}", product->id, product->name, product->phone_number);
		};
		_template.SubMenu('p', MENU_A_PRODUCT_SEARCH, "°í°´ °Ë»ö"); 

		_template.next_menu_code = MENU_A_ACCOUNT_INFO;

		_template.Apply(MENU_A_ACCOUNT_LIST);
	}


	// °í°´ °Ë»ö ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_ACCOUNT_SEARCH, new Menu(
		[&](MenuIO& IO) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °Ë»ö ]");
		}
	));
	
	// °í°´ °èÁ¤ Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °èÁ¤ Á¤º¸ ]");

			IO.print(format("ÀÌ¸§ : {0}\n", target->name));
			IO.print(format("¾ÆÀÌµð : {0}\n", target->id));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", target->phone_number));
			IO.print(format("ÁÖ¼Ò : {0}\n", target->address));

			IO.print_line();

			IO.print("ÁÖ¹® Ã³¸® Á¤º¸(O) ¼öÁ¤(M)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "O") {
					menu_manager.RunMenu(MENU_A_INVOICE_LIST, target->invoice_id_list);
				}
				else if (input == "M") {
					menu_manager.RunMenu(MENU_A_ACCOUNT_INFO_M, target);
				}
				else {
					IO.print("ÀÏÄ¡ÇÏ´Â ¸í·É¾î°¡ ¾øÀ½.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}

		}
	));

	// °í°´ °èÁ¤ Á¤º¸ ¼öÁ¤ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_ACCOUNT_INFO_M, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °èÁ¤ Á¤º¸ ¼öÁ¤ ]");

			auto checkpoint = IO.checkpoint();
			vector<string> account;
			string input, match_p;
			vector<string> heads = { "ºñ¹Ð¹øÈ£","ºñ¹Ð¹øÈ£ È®ÀÎ","ÀüÈ­¹øÈ£","ÁÖ¼Ò" };
			vector<string> regex_t = { "account_password","","account_phonenumber","account_address" };
			int t = 0;
			while (t < heads.size()) {
				if (t != 1) {
					checkpoint = IO.checkpoint();
					input = IO.input("", heads[t]);
					if (data_manager.GetParser(regex_t[t])->Check(input) || input._Equal("")) {
						account.push_back(input);
						t++;
					}
					else
						IO.rollback(checkpoint);
				}
				else{
					checkpoint = IO.checkpoint();
					match_p = IO.input("", heads[t]);
					if (input._Equal(match_p))
						t++;
					else
						IO.rollback(checkpoint);
				}
			}
			input = IO.input("°èÁ¤ Á¤º¸¸¦ ¼öÁ¤ÇÏ½Ã°Ú½À´Ï±î?(y/n)", "");
			if (input == "y") {
				target->password = account[0]._Equal("") ? target->password : account[0];
				target->phone_number = account[1]._Equal("") ? target->phone_number : account[1];
				target->address = account[2]._Equal("") ? target->address : account[2];
				IO.pause();
			}
			menu_manager.RunMenu(MENU_A_ACCOUNT_INFO, target);
		}
	));
	// ±¸¸Å ³»¿ª ¸Þ´ºÈ­¸é
	{
		TemplateTable<int> _template;
		_template.SetName("±¸¸Å ³»¿ª");
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<16}", "±¸¸Å ³¯Â¥", "Á¦¸ñ", "°áÁ¦ ±Ý¾×");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager.GetInvoice(id);
			Product* product = shop_manager.GetProduct(invoice->product_id); 

			return format("{0:<12}{1:<20}{2:<16}", invoice->date, limit(product->title, 18), to_string(product->price * invoice->product_count) + "¿ø");
		};

		_template.next_menu_code = MENU_A_INVOICE_INFO;

		_template.Apply(MENU_A_INVOICE_LIST);
	}

	// ÁÖ¹® »ó¼¼ Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_A_INVOICE_INFO, new Menu<int>(
		[&](MenuIO& IO, int target_id) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ÁÖ¹® »ó¼¼ Á¤º¸ ]");

			Invoice* target = shop_manager.GetInvoice(target_id);
			Product* product = shop_manager.GetProduct(target->product_id);
			Account* account = shop_manager.GetAccount(target->buyer_id);

			IO.print(format("±¸¸Å ³¯Â¥ : {0}\n", target->date));

			IO.print("\n[»óÇ° »ó¼¼ Á¤º¸]\n");
			IO.print(format("°íÀ¯ ¹øÈ£ : {0}\n", product->id));
			IO.print(format("Á¦¸ñ : {0}\n", product->title));
			IO.print(format("Àå¸£ : {0}\n", product->genre));
			IO.print(format("°¡°Ý : {0}\n", product->price));

			IO.print("\n[ÁÖ¹®ÀÚ Á¤º¸]\n");
			IO.print(format("¾ÆÀÌµð : {0}\n", account->id));
			IO.print(format("ÀÌ¸§ : {0}\n", account->name));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", target->recipient_phone_number));
			IO.print(format("ÁÖ¼Ò : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("»óÇ° °íÀ¯ ¹øÈ£ : {0}\n", target->product_id));
			IO.print(format("»óÇ° ¼ö·® : {0}\n", target->product_count));
			IO.print(format("°áÁ¦ ±Ý¾× : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.print("¾Æ¹« Å°³ª ÀÔ·ÂÇÏ¼¼¿ä\n");

			auto checkpoint = IO.checkpoint();
			string input = IO.input();

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
		_template.SubMenu("»óÇ° ¸ñ·Ï", []() { menu_manager.RunMenu(MENU_B_PRODUCT_LIST, shop_manager.GetProdcutList()); });
		_template.SubMenu("°í°´ °èÁ¤ Á¤º¸", []() { menu_manager.RunMenu(MENU_B_ACCOUNT_INFO, shop_manager.GetUser()); });
		_template.Apply(MENU_BUYER);
	}

	// »óÇ° ¸ñ·Ï ¸Þ´ºÈ­¸é
	{
		TemplateTable<Product*> _template;
		_template.SetName("»óÇ° ¸ñ·Ï");
		_template.header_func = []() -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<16}{4:<8}", "ID", "»óÇ°", "Àå¸£", "°¡°Ý", "Àç°í");
		};
		_template.show_func = [](Product* product) -> string {
			return format("{0:<10}{1:<20}{2:<8}{3:<16}{4:<8}", product->id, limit(product->title, 18), product->genre, product->price, product->count);
		};
		_template.SubMenu('p', MENU_B_PRODUCT_SEARCH, "»óÇ° °Ë»ö ¹× Àå¸£ ¼±ÅÃ");

		_template.next_menu_code = MENU_B_PRODUCT_INFO;

		_template.Apply(MENU_B_PRODUCT_LIST);
	}

	// »óÇ° »ó¼¼ Á¤º¸ È®ÀÎ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_B_PRODUCT_INFO, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° µî·Ï Á¤º¸ ]");

			IO.print(format("°íÀ¯¹øÈ£ : {0}\n", target->id));
			IO.print(format("Á¦¸ñ : {0}\n", target->title));
			IO.print(format("Àå¸£ : {0}\n", target->genre));
			IO.print(format("°¡°Ý : {0}\n", target->price));
			IO.print(format("Àç°í : {0}\n", target->count));

			IO.print_line();

			IO.print("±¸¸Å(B)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "B") {
					menu_manager.RunMenu(MENU_B_PRODUCT_BUY, target);
				}
				else {
					IO.print("ÀÏÄ¡ÇÏ´Â ¸í·É¾î°¡ ¾øÀ½.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}
		}
	));
	// »óÇ° ±¸¸Å ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_B_PRODUCT_BUY, new Menu<Product*>(
		[&](MenuIO& IO, Product* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ »óÇ° ±¸¸Å ]");

			string input;
			Account* user = shop_manager.GetUser();
			vector<string> invoice = { user->id,user->phone_number,user->address, "23.10.29", to_string(target->id)};
			IO.print_line();
			IO.print("[»óÇ° ±¸¸Å]\n");
			IO.print("[»óÇ° »ó¼¼ Á¤º¸]\n");
			IO.print(format("°íÀ¯¹øÈ£ : {0}\n", target->id));
			IO.print(format("Á¦¸ñ : {0}\n", target->title));
			IO.print(format("Àå¸£ : {0}\n", target->genre));
			IO.print(format("°¡°Ý : {0}\n", target->price));
			IO.print_line();
			auto checkpoint = IO.checkpoint();
			while (true) {
				input = IO.input("ÁÖ¹® ¼ö·®À» ÀÔ·ÂÇÏ¼¼¿ä");
				if (data_manager.GetParser("invoice_product_count")->Check(input)) {
					if (target->count >= stoi(input))
					{
						invoice.push_back(input);
						break;
					}
				}
				IO.rollback(checkpoint);
			}
			IO.print_line();
			IO.print("[ÁÖ¹®ÀÚ Á¤º¸]\n");
			IO.print(format("ÀÌ¸§ : {0}\n", user->name));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", user->phone_number));
			IO.print(format("ÁÖ¼Ò : {0}\n", user->address));
			IO.print_line();
			IO.print("[°áÁ¦±Ý¾×]\n");
			IO.print(format("{0}¿ø X {1}±Ç = {2}¿ø\n", target->price, stoi(input), target->price * stoi(input)));
			IO.print_line();
			input = IO.input("»óÇ°À» ÁÖ¹®ÇÏ½Ã°Ú½À´Ï±î?(y/n)");
			if (input == "y") {
				int id = 7 + shop_manager.GetInvoiceList().size();
				invoice.insert(invoice.begin(), to_string(id));
				shop_manager.AddInvoice(new Invoice(invoice));
				user->invoice_id_list.push_back(id);
				IO.pause();
				IO.print("»óÇ°À» ±¸¸ÅÇÏ¿´½À´Ï´Ù.\n");
			}
			menu_manager.RunMenu(MENU_B_PRODUCT_INFO, target);
		}
	));


	// °í°´ °èÁ¤ Á¤º¸ °ü¸® ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_B_ACCOUNT_INFO, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °èÁ¤ Á¤º¸ ]");

			IO.print(format("ÀÌ¸§ : {0}\n", target->name));
			IO.print(format("¾ÆÀÌµð : {0}\n", target->id));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", target->phone_number));
			IO.print(format("ÁÖ¼Ò : {0}\n", target->address));

			IO.print_line();

			IO.print("ÁÖ¹® Ã³¸® Á¤º¸(O) ¼öÁ¤(M)\n");

			IO.print_line();

			auto checkpoint = IO.checkpoint();

			while (true) {
				string input = IO.input();

				if (input == "O") {
					menu_manager.RunMenu(MENU_B_INVOICE_LIST, target->invoice_id_list);
				}
				else if (input == "M") {
					menu_manager.RunMenu(MENU_B_ACCOUNT_INFO_M, target);
				}
				else {
					IO.print("ÀÏÄ¡ÇÏ´Â ¸í·É¾î°¡ ¾øÀ½.\n");
					IO.pause();

					IO.rollback(checkpoint);
				}
			}

		}
	));
	// °í°´ °èÁ¤ Á¤º¸ ¼öÁ¤ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_B_ACCOUNT_INFO_M, new Menu<Account*>(
		[&](MenuIO& IO, Account* target) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ °í°´ °èÁ¤ Á¤º¸ ¼öÁ¤ ]");

			auto checkpoint = IO.checkpoint();
			vector<string> account;
			string input, match_p;
			vector<string> heads = { "ºñ¹Ð¹øÈ£","ºñ¹Ð¹øÈ£ È®ÀÎ","ÀüÈ­¹øÈ£","ÁÖ¼Ò" };
			vector<string> regex_t = { "account_password","","account_phonenumber","account_address" };
			int t = 0;
			while (t < heads.size()) {
				if (t != 1) {
					checkpoint = IO.checkpoint();
					input = IO.input("", heads[t]);
					if (data_manager.GetParser(regex_t[t])->Check(input) || input._Equal("")) {
						account.push_back(input);
						t++;
					}
					else
						IO.rollback(checkpoint);
				}
				else {
					checkpoint = IO.checkpoint();
					match_p = IO.input("", heads[t]);
					if (input._Equal(match_p))
						t++;
					else
						IO.rollback(checkpoint);
				}
			}
			input = IO.input("°èÁ¤ Á¤º¸¸¦ ¼öÁ¤ÇÏ½Ã°Ú½À´Ï±î?(y/n)", "");
			if (input == "y") {
				target->password = account[0]._Equal("") ? target->password : account[0];
				target->phone_number = account[1]._Equal("") ? target->phone_number : account[1];
				target->address = account[2]._Equal("") ? target->address : account[2];
				IO.pause();
			}
			menu_manager.RunMenu(MENU_B_ACCOUNT_INFO, target);
		}
	));


	// ±¸¸Å ³»¿ª ¸Þ´ºÈ­¸é
	{
		TemplateTable<int> _template;
		_template.SetName("±¸¸Å ³»¿ª");
		_template.header_func = []() -> string {
			return format("{0:<12}{1:<20}{2:<16}", "±¸¸Å ³¯Â¥", "Á¦¸ñ", "°áÁ¦ ±Ý¾×");
		};
		_template.show_func = [](int id) -> string {
			Invoice* invoice = shop_manager.GetInvoice(id);
			Product* product = shop_manager.GetProduct(invoice->product_id);

			return format("{0:<12}{1:<20}{2:<16}", invoice->date, limit(product->title, 18), to_string(product->price * invoice->product_count) + "¿ø");
		};

		_template.next_menu_code = MENU_B_INVOICE_INFO;

		_template.Apply(MENU_B_INVOICE_LIST);
	}

	// ÁÖ¹® »ó¼¼ Á¤º¸ ¸Þ´ºÈ­¸é
	menu_manager.AppendMenu(MENU_B_INVOICE_INFO, new Menu<int>(
		[&](MenuIO& IO, int target_id) {
			menu_manager.PrintCommand();
			IO.print_line();
			IO.print_aligned_center("[ ÁÖ¹® »ó¼¼ Á¤º¸ ]");

			Invoice* target = shop_manager.GetInvoice(target_id);
			Product* product = shop_manager.GetProduct(target->product_id);
			Account* account = shop_manager.GetAccount(target->buyer_id);

			IO.print(format("±¸¸Å ³¯Â¥ : {0}\n", target->date));

			IO.print("\n[»óÇ° »ó¼¼ Á¤º¸]\n");
			IO.print(format("°íÀ¯ ¹øÈ£ : {0}\n", product->id));
			IO.print(format("Á¦¸ñ : {0}\n", product->title));
			IO.print(format("Àå¸£ : {0}\n", product->genre));
			IO.print(format("°¡°Ý : {0}\n", product->price));

			IO.print("\n[ÁÖ¹®ÀÚ Á¤º¸]\n");
			IO.print(format("¾ÆÀÌµð : {0}\n", account->id));
			IO.print(format("ÀÌ¸§ : {0}\n", account->name));
			IO.print(format("ÀüÈ­¹øÈ£ : {0}\n", target->recipient_phone_number));
			IO.print(format("ÁÖ¼Ò : {0}\n", target->recipient_address));

			IO.print("\n");
			IO.print(format("»óÇ° °íÀ¯ ¹øÈ£ : {0}\n", target->product_id));
			IO.print(format("»óÇ° ¼ö·® : {0}\n", target->product_count));
			IO.print(format("°áÁ¦ ±Ý¾× : {0}\n", product->price * target->product_count));
			IO.print_line();
			IO.pause();
			menu_manager.RunMenu(MENU_B_INVOICE_LIST,shop_manager.GetInvoiceList());
		}
	));

#pragma endregion
}
#pragma endregion

