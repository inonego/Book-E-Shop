// Book E-Shop.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "DataManager.h"
#include "MenuManager.h"

int main()
{
	DataManager data_manager;

	// Parser 예시
	data_manager.AppendParser("account_id", (new Parser())
		->set_regex("")
		->set_msg_info("")
		->set_msg_error("")
		->set_parse(nullptr)
	);
}