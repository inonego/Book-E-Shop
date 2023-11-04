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
        if (input.size() <= 1) {
            return result;
        } 

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

void DataManager::SaveCSV(string path, vector<vector<string>> data) {
    ofstream file(path);

    if (file.is_open()) {
        for (const std::vector<std::string>& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];

                if (i < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }

        file.close();

        cout << "������ ���������� �����߽��ϴ�." << std::endl;
    }
    else {
        cerr << "������ �� �� �����ϴ�." << std::endl;
    }

}
 