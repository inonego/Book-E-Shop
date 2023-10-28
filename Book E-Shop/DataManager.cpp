#include "DataManager.h"
#pragma region for CSV parsing
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream> 
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

vector<vector<string>> DataManager::ParseCSV(string path)
{ 
    path = filesystem::absolute(path).string();

    vector<vector<string>> result;

    fstream stream(path, ios::in);

    if (stream.fail()) { 
        cerr << "���� : CSV ������ ã�� �� �����ϴ�" << '\n';
        
        exit(100);
    }

    string input, token;

    // ������ ��ҿ� �ش��ϴ� ���� ���ڿ��� �޾ƿɴϴ�.
    while (getline(stream, input)) {
        vector<string> data;
        
        stringstream sstream(input); 

        // �ϳ��� ������ ��ҿ� ���ԵǴ� ���� ������ ��Ҹ� �߰��մϴ�.
        while (!sstream.eof()) {
            getline(sstream, token, ',');
            
            data.push_back(token);
        }

        result.push_back(data);
    }

    stream.close();

    return result;
}
 