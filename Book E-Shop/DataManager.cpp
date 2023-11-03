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
        cerr << "오류 : CSV 파일을 찾을 수 없습니다" << '\n';
        
        exit(100);
    }

    string input, token;

    // 데이터 요소에 해당하는 행의 문자열을 받아옵니다.
    while (getline(stream, input)) {
        if (input.size() <= 1) {
            return result;
        } 

        vector<string> data;
        
        stringstream sstream(input); 

        // 하나의 데이터 요소에 포함되는 하위 데이터 요소를 추가합니다.
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

        cout << "파일을 성공적으로 저장했습니다." << std::endl;
    }
    else {
        cerr << "파일을 열 수 없습니다." << std::endl;
    }

}
 