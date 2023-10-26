#include "DataManager.h"
#pragma region for CSV parsing
#include <iostream>
#include <fstream>
#include <string.h>
#pragma endregion

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

vector<string> DataManager::ParseCSV(string path)
{
    vector<string> str_buf;
    string buf;
    fstream fs;

    fs.open(path, ios::in);

    while (!fs.eof()) {
        getline(fs, buf);
        str_buf.push_back(buf);
        //cout << buf << "\n"; 제대로 읽어옮
    }
    fs.close();
    return str_buf;
}
 