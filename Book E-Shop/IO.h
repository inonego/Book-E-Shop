#pragma once

#include <any>
#include <vector>
#include <string>
#include <functional>
#include <exception>

#include "Parser.h"

using namespace std; 

class IO {
public:  
	class Buffer {
	public:
		struct CheckPoint { friend class Buffer; private: int index; public: CheckPoint(int index) : index(index) {} };
	private:
		vector<string> buffer;
	public:
		CheckPoint checkpoint();
		void rollback(CheckPoint checkpoint);
		void clear();
		void write(string text);
		string text();
	}; 

	using CheckPoint = Buffer::CheckPoint;
private:    
	Buffer* buffer = nullptr;
public:
	// 화면에 출력된 모든 텍스트를 지웁니다.
	void clear();

	// 화면에 전체 텍스트를 다시 출력합니다.
	void reprint();

	// 화면에 텍스트를 출력합니다.
	virtual void print(string text);

	// 화면에서 입력을 받습니다.
	virtual string input();

	// 체크포인트를 설정합니다.
	CheckPoint checkpoint();

	// 지정된 체크포인트로 되돌립니다.
	void rollback(CheckPoint checkpoint);

	// 작업할 버퍼를 설정합니다.
	void set_buffer(Buffer* buffer);
	Buffer* get_buffer();
};


class MenuIO : public IO {
private:
	function<bool(MenuIO&, string)> input_func;
public:
	MenuIO(function<bool(MenuIO&, string)> input_func) : input_func(input_func) {}

	int width = 60;

	void print_aligned_center(string text);

	// 화면에서 입력을 받습니다.
	string input() override;

	// 필요한 정보를 출력하고, 화면에서 입력을 받습니다.
	string input(string msg_info, string label = "입력");

	// Parser를 이용해 입력을 받고, 올바른 형식이면 파싱하여 반환합니다.
	template <typename T = string> requires is_same_v<T, string> || is_same_v<T, char> || is_same_v<T, int>
	T input(Parser*);

	// "아무키나 입력하세요..."를 출력하고 대기합니다.
	void pause();

	// 화면에 구분선을 그립니다.
	void print_line(bool bold = true);
};

template <typename T> requires is_same_v<T, string> || is_same_v<T, char> || is_same_v<T, int>
inline T MenuIO::input(Parser* parser)
{
	auto checkpoint = this->checkpoint();

	while (true) {
		string input = this->input(parser->msg_info, parser->label);

		if (parser->Check(input)) {
			if constexpr (is_same_v<T, string>) {
				return input;
			}
			else if constexpr (is_same_v<T, char>) {
				if (input.size() > 0) {
					return input[0];
				} 
				else {
					throw invalid_argument("빈 문자열은 문자로 변환할 수 없습니다!");
				}
			}
			else if constexpr (is_same_v<T, int>) {
				return stoi(input);
			}
		}
		else {
			this->print(parser->msg_error + '\n'); this->pause();

			this->rollback(checkpoint);
		}
	}
}
