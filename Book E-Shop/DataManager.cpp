#include "DataManager.h"

void DataManager::AppendParser(string parser_name, Parser* parser)
{
	parser_list.insert({ parser_name, parser });
}

void DataManager::RemoveParser(string parser_name)
{
	parser_list.erase(parser_name);
}

Parser* DataManager::GetParser(string parser_name)
{
	return parser_list[parser_name];
}

vector<string*> DataManager::ParseCSV(string path)
{
	return vector<string*>();
}
 