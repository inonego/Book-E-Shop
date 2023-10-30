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
	// ȭ�鿡 ��µ� ��� �ؽ�Ʈ�� ����ϴ�.
	void clear();

	// ȭ�鿡 ��ü �ؽ�Ʈ�� �ٽ� ����մϴ�.
	void reprint();

	// ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
	virtual void print(string text);

	// ȭ�鿡�� �Է��� �޽��ϴ�.
	virtual string input();

	// üũ����Ʈ�� �����մϴ�.
	CheckPoint checkpoint();

	// ������ üũ����Ʈ�� �ǵ����ϴ�.
	void rollback(CheckPoint checkpoint);

	// �۾��� ���۸� �����մϴ�.
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

	// ȭ�鿡�� �Է��� �޽��ϴ�.
	string input() override;

	// �ʿ��� ������ ����ϰ�, ȭ�鿡�� �Է��� �޽��ϴ�.
	string input(string msg_info, string label = "�Է�");

	// Parser�� �̿��� �Է��� �ް�, �ùٸ� �����̸� �Ľ��Ͽ� ��ȯ�մϴ�.
	template <typename T = string> requires is_same_v<T, string> || is_same_v<T, char> || is_same_v<T, int>
	T input(Parser*);

	// "�ƹ�Ű�� �Է��ϼ���..."�� ����ϰ� ����մϴ�.
	void pause();

	// ȭ�鿡 ���м��� �׸��ϴ�.
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
					throw invalid_argument("�� ���ڿ��� ���ڷ� ��ȯ�� �� �����ϴ�!");
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
