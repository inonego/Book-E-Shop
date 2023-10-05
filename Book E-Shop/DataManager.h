#pragma once

#include <string>
#include <map>
#include <functional>

#include "DataComponent.h"
#include "Parser.h"

using namespace std;

class DataManager
{
private:
	map<string, Parser*> parser_list;
public:
	void AppendParser(string parser_name, Parser* parser);
	void RemoveParser(string parser_name);

	Parser* GetParser(string parser_name);

	vector<string*> ParseCSV(string path);
};

